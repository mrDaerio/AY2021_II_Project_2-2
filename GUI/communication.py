##
# @package communication
from datetime import datetime
from kivy.event import EventDispatcher
from kivy.properties import NumericProperty, StringProperty
from numpy import matrixlib  # pylint: disable=no-name-in-module
import serial
import serial.tools.list_ports as list_ports
import struct
import threading
import time
from scipy.signal import butter, lfilter, hilbert, find_peaks
import numpy as np
from sklearn.decomposition import PCA
import pandas as pd

##
#   @brief          Data packet header.
#
DATA_PACKET_HEADER = 0xA0

##
#   @brief          Data packet tail.
#
DATA_PACKET_TAIL = 0xC0

##
#   @brief          Command to start connection with board.
#
CONNECTION_CMD = 'v'

##
#   @brief          Command to start streaming from the board.
#
START_STREAMING_CMD = 'b'

##
#   @brief          Command to stop streaming from the board.
#
STOP_STREAMING_CMD = 's'

##
#   @brief          Disconnected port state.
#
CONNECTION_STATE_DISCONNECTED = 0

##
#   @brief          Port found state.
#
CONNECTION_STATE_FOUND = 1

##
#   @brief          Connected state.
#
CONNECTION_STATE_CONNECTED = 2


##
#   @brief          Class used for Singleton pattern.
#
#   This class allows to implement the Singleton pattern.
#   This pattern restricts the instantiation of a class
#   to one single instance.
#   [Link for documentation](https://en.wikipedia.org/wiki/Singleton_pattern)
#


class Singleton(type):
    _instances = {}

    def __call__(cls, *args, **kwargs):
        if cls not in cls._instances:
            cls._instances[cls] = super(
                Singleton, cls).__call__(*args, **kwargs)
        return cls._instances[cls]


##
#   @brief          Main class used for serial communication.
#
#   This is the main class used to communicate with the serial port.
#   It has \ref Singleton as a metaclass, so only one instance of this
#   class exists throughout the application.
#   Automatic port discovery is implemented: it is not required to
#   specify the serial port, as it is automatically detected by
#   scanning all the available ports, and sending a known command
#   to the port. If the expected response is detected, then
#   a connection with the serial port is carried out.


class KivySerial(EventDispatcher, metaclass=Singleton):
    ##
    #   @brief          Connection status.
    #
    #   Possible values are:
    #       - \ref CONNECTION_STATE_DISCONNECTED: board is disconnected
    #       - \ref CONNECTION_STATE_FOUND: board is found among serial ports
    #       - \ref CONNECTION_STATE_CONNECTED: board is connected
    #
    connected = NumericProperty(0)

    ##
    #   @brief          Debug message string.
    #
    #   This is a debug message string that can be bound to other
    #   widgets properties/functions to show debug messages from this
    #   module.
    message_string = StringProperty('')

    ##
    #   @brief          Sample rate set on the board.
    #
    #   This is the true sample rate value which is set on the board.
    #   It can be used to compare the sample rate of retrieved values
    #   to the theoretical one.
    #
    sample_rate = NumericProperty(1)

    ##
    #   @brief          Full Scale Range set on the board.
    #
    #   This is the true full scale range value which is set on the board.
    #   It can be used to compare the full scale range of retrieved values
    #   to the theoretical one.
    #
    full_scale_range = NumericProperty(2)

    ##
    #  @brief           Initialize the class.
    #
    #  @param[in]       baudrate: the desired baudrate for serial communication.
    #
    def __init__(self, baudrate=115200):

        self.port_name = ""  # port name, set later when port is found
        self.baudrate = baudrate  # baudrate for serial communication
        self.is_streaming = False  # streaming status
        self.connected = 0  # connection status
        self.read_state = 0  # read state for data parser
        self.callbacks = []  # list of callbacks to be called when new data are available
        self.samples_counter = 0  # counter for samples received
        self.initial_time = 0  # time of first sample received
        self.timeout = 1
        self.signal = Signal()
        # Start thread for automatic port discovery
        find_port_thread = threading.Thread(target=self.find_port, daemon=True)
        find_port_thread.start()

    ##
    #  @brief           Add callback to be called upon packet reception.
    #
    #  @param[in]       callback: the callback function to be called.
    #
    def add_callback(self, callback):
        if (callback not in self.callbacks):
            self.callbacks.append(callback)

    ##
    #   @brief          Automatic serial port discovery.
    #   
    #   This function scans all the available COM ports to
    #   check if one of them is the proper one. It does it
    #   by sending a \ref CONNECTION_CMD and checking that
    #   the expected string is received.
    def find_port(self):
        port_found = False
        time.sleep(2)
        while (not port_found):
            ports = list_ports.comports()
            if (len(ports) == 0):
                self.message_string = 'No ports found.. Check your connections'
                time.sleep(2)
            for port in ports:
                port_found = self.check_lis3dh_port(port.device)
                if (port_found):
                    self.port_name = port.device
                    if (self.connect() == 0):
                        break

    ##
    #   @brief              Check if the port is the desired one.
    #
    #   This function sends a \ref CONNECTION_CMD to the port,
    #   and checks if three $$$ are found in the response from
    #   the port.
    #
    #   @param[in]          port_name: the name of the port to be checked
    #   @return             True if check was successfull, False otherwise.
    #
    def check_lis3dh_port(self, port_name):
        self.message_string = 'Checking: {}'.format(port_name)
        try:
            port = serial.Serial(
                port=port_name, baudrate=self.baudrate, write_timeout=0, timeout=5)
            if (port.is_open):
                port.write(CONNECTION_CMD.encode('utf-8'))
                time.sleep(2)
                received_string = ''
                while (port.in_waiting > 0):
                    received_string += port.read().decode('utf-8', errors='replace')
                if ('$$$' in received_string and 'LIS' in received_string):
                    self.message_string = 'Device found on port: {}'.format(
                        port_name)
                    self.connected = CONNECTION_STATE_FOUND
                    port.close()
                    time.sleep(2)
                    return True
        except serial.SerialException:
            return False
        except ValueError:
            return False
        return False

    ##
    #   @brief          Connect to the serial port that was found.
    #
    #   @return         0 if connection was successful, -1 otherwise
    def connect(self):
        try:
            self.port = serial.Serial(
                port=self.port_name, baudrate=self.baudrate, timeout=self.timeout)
        except serial.SerialException:
            self.message_string = f'Error when opening port'
            return -1
        if (self.port.is_open):
            self.message_string = f'Device connected at {self.port_name}'
            self.update_sample_rate_on_board('1 Hz')
            self.connected = CONNECTION_STATE_CONNECTED
            return 0
        return -1

    ##
    #   @brief          Start streaming data from the device.
    #
    #   This function sends the proper command to start data 
    #   streaming, and initiates a thread to collec data
    #   received from the serial port.
    #
    def start_streaming(self):
        if (self.connected == CONNECTION_STATE_CONNECTED):
            if (not (self.is_streaming)):
                self.message_string = 'Starting data streaming'
                self.port.write(START_STREAMING_CMD.encode('utf-8'))
                self.is_streaming = True
                self.read_state = 0
                self.skipped_bytes = 0
                self.samples_counter = 0
                read_thread = threading.Thread(target=self.collect_data)
                read_thread.daemon = True
                read_thread.start()
        else:
            self.message_string = 'Device is not connected.'

    ##
    #   @brief          Target function for thread collecting data.
    #
    #   This function receives packets from the serial port and
    #   streams them to all the callbacks that were added. It also
    #   updates the computed sample rate.
    def collect_data(self):
        while (self.is_streaming):
            packet = self.read_serial_binary()
            if (packet):
                for callback in self.callbacks:
                    callback(packet, self.signal)
                self.update_sample_rate()

    ##
    #   @brief          Compute new sample rate value upon reception of a packet.
    #
    def update_sample_rate(self):
        if (self.samples_counter == 0):
            self.initial_time = datetime.now()
        else:
            diff = (datetime.now() - self.initial_time).total_seconds()
            if (diff != 0):
                self.current_sample_rate = (self.samples_counter + 1) / diff
                bpm_to_print = int(self.signal.meanbpm) if self.signal.meanbpm == self.signal.meanbpm else 0 
                self.message_string = f'Heart rate: {bpm_to_print:3d} | Sample Rate: {self.current_sample_rate:5.2f} Hz'
        self.samples_counter += 32

    ##
    #   @brief          Serial data parser.
    #
    #   State machine to parse incoming data packet into a \ref LIS3DHDataPacket
    #   The structure of the incoming packet is as follows:
    #       - Header byte: 0xA0
    #       - X Axis data: 64 bytes
    #       - Y Axis data: 64 bytes
    #       - Z Axis data: 64 bytes
    #       - Tail byte: 0xC0
    #
    #   @param[in]      max_bytes_to_skip: optional number of bytes to skip when looking for header byte
    #   @return         \ref LIS3DHDataPacket packet with accelerometer data
    #
    def read_serial_binary(self, max_bytes_to_skip=3000):
        for rep in range(max_bytes_to_skip):
            if (not self.is_streaming):
                break
            if (self.read_state == 0):
                # State 0, looking for header byte
                b = self.port.read(1)
                if (len(b) > 0):
                    b = struct.unpack('B', b)[0]
                    if (b == DATA_PACKET_HEADER):
                        if (rep > 0):
                            print(f'Skipped {rep} bytes')
                        rep = 0
                        self.read_state = 1
            elif (self.read_state == 1):
                # Get six bytes*32 of acceleration data
                data = self.port.read(6 * 32)
                if (len(data) == 6 * 32):
                    data = struct.unpack('192B', data)
                    x_data = self.convert_acc_data(data[0:64])
                    y_data = self.convert_acc_data(data[64:128])
                    z_data = self.convert_acc_data(data[128:192])
                    self.read_state = 2
                else:
                    self.read_state = 0
            elif (self.read_state == 2):
                tail_byte = self.port.read(1)
                if (len(tail_byte) == 1):
                    # We have a valid byte
                    tail_byte = struct.unpack('B', tail_byte)[0]
                    if (tail_byte == DATA_PACKET_TAIL):
                        packet = LIS3DHDataPacket(x_data, y_data, z_data)
                        self.signal.append_data(x_data, y_data, z_data)
                        self.read_state = 0
                        return packet
                else:
                    # Reset state machine
                    self.read_state = 0

    ##
    #   @brief          Convert acceleration data in float format.
    #
    #   This function converts raw bytes into a float value representing
    #   acceleration data. Check is done on negative/positive data based
    #   on 2's complement notation. Conversion is based on normal mode,
    #   +/- 2g settings.
    #   @param[in]      data: two bytes representing acceleration
    #   @return         float formatted acceleration value
    #
    def convert_acc_data(self, data):
        try:
            temp_data = [0 for i in range(len(data) // 2)]
            for i in range(len(data) // 2):
                temp_data[i] = data[i * 2] << 8 | data[2 * i + 1]
                if temp_data[i] & 0x8000:
                    # We have a negative number
                    temp_data[i] = 0xFFFF - temp_data[i]
                    temp_data[i] = temp_data[i] + 1
                    temp_data[i] = - temp_data[i]
        except Exception as e:
            print("errore", e)
            return data
        # temp_data = temp_data >> 6  # 6-byte shift since we are in normal mode
        # temp_data = temp_data * 4   # Sensitivity of 4 mg/digit in normal mode, +/-2g
        return [i / 1000 for i in temp_data]

    ##
    #   @brief          Stop data streaming.
    #
    #   Stop data streaming and show statistics on collected data.
    def stop_streaming(self):
        self.is_streaming = False
        if (self.samples_counter == 0):
            self.message_string = f'Stopped streaming data'
        else:
            self.message_string = f'Stopped streaming data. Collected {self.samples_counter:d} samples with {self.current_sample_rate:.2f} Hz sample rate.'
        self.port.write(STOP_STREAMING_CMD.encode('utf-8'))

    ##
    #   @brief          Update sample rate on board
    #
    #   Update the accelerometer sample rate based on selected value.
    #   @param[in]      value: the desired sample rate to be set.
    def update_sample_rate_on_board(self, value):
        sample_rate_dict = {
            '1 Hz': '0',
            '10 Hz': '1',
            '25 Hz': '2',
            '50 Hz': '3',
            '100 Hz': '4',
            '200 Hz': '5',
            '400 Hz': '6',
            '1344 Hz': '7'
        }
        if (self.port.is_open):
            try:
                self.port.write(sample_rate_dict[value].encode('utf-8'))
                self.message_string = f'Updated sample rate to {value}'
                self.sample_rate = int(value.split(' ')[0])
            except:
                self.message_string = "Could not update sample rate"

    ##
    #   @brief          Update full scale range on board
    #
    #   Update the accelerometer sample rate based on selected value.
    #   @param[in]      value: the desired sample rate to be set.
    def update_fsr_on_board(self, value):
        fsr_dict = {
            '+/- 2 g': 'w',
            '+/- 4 g': 'x',
            '+/- 8 g': 'j',
            '+/- 16 g': 'k'

        }
        if (self.port.is_open):
            # try:
            self.port.write(fsr_dict[value].encode('utf-8'))
            self.message_string = f'Updated full scale range to {value}'
            self.full_scale_range = int(value.split(' ')[1])
            # except:
            # self.message_string = "Could not update full scale range"

    ##
    #   @brief          Get if serial port is connected.
    #   @return         True if connected, False otherwise
    def is_connected(self):
        if (self.connected == CONNECTION_STATE_CONNECTED):
            return True
        else:
            return False


##
#   @brief          Class holding a packet of accelerometer data.
class LIS3DHDataPacket():

    ##
    #   @brief          Initialization function.
    #   @param[in]      x_data: x axis acceleration
    #   @param[in]      y_data: y axis acceleration
    #   @param[in]      z_data: z axis acceleration
    def __init__(self, x_data, y_data, z_data):
        self.x_data = x_data
        self.y_data = y_data
        self.z_data = z_data

    ##
    #   @brief          Get x axis acceleration.
    def get_x_data(self):
        return self.x_data

    ##
    #   @brief          Get y axis acceleration.
    def get_y_data(self):
        return self.y_data

    ##
    #   @brief          Get z axis acceleration.
    def get_z_data(self):
        return self.z_data


class Signal():

    def __init__(self):
        self.x_data = []
        self.y_data = []
        self.z_data = []
        self.window_start_pos = 0
        self.stride = 32
        self.window_length = 2080
        self.filtered_sum = []
        self.flag_first_filter = False
        self.diff = 0
        self.peaks = 0
        self.meanbpm = 0

    ##
    #   @brief          Get x axis acceleration.
    def get_x_data(self):
        return self.x_data

    ##
    #   @brief          Get y axis acceleration.
    def get_y_data(self):
        return self.y_data

    ##
    #   @brief          Get z axis acceleration.
    def get_z_data(self):
        return self.z_data

    def append_data(self, x, y, z):
        self.x_data += x
        self.y_data += y
        self.z_data += z
        if (len(self.x_data)>=self.window_length):
            self.filter()
            self.flag_first_filter = True

    def filter(self):
        #calculate filter coefficients
        nyq = 0.5 * 200
        low = 20 / nyq
        high = 50 / nyq
        b, a = butter(4, [low, high], btype='band')
        x_windowed = self.x_data[self.window_start_pos:self.window_start_pos+self.window_length]
        y_windowed = self.y_data[self.window_start_pos:self.window_start_pos+self.window_length]
        z_windowed =self.z_data[self.window_start_pos:self.window_start_pos+self.window_length]

        """
            PCA
        """
        pca = PCA(n_components = 1)
        #create dataframe
        matrix = pd.DataFrame({'x' : x_windowed,
                               'y' : y_windowed,
                               'z' : z_windowed})
        #remove mean
        matrix = matrix-matrix.mean()
        #print(matrix)
        #find principal component
        principal_component = pca.fit_transform(matrix)
        principal_component = [i[0] for i in principal_component]
        #print(principal_component)

        self.filtered_sum = lfilter(b, a, principal_component)

        self.filtered_sum = np.abs(hilbert(self.filtered_sum))
        #if self.window_start_pos % 640 == 0:
        self.peaks = find_peaks(self.filtered_sum[-640:], distance=80, prominence=0.015)
        self.peaks = self.peaks[0]
        self.diff = [t - s for s, t in zip(self.peaks, self.peaks[1:])]
        
        board = KivySerial()
        self.diff = [i/board.current_sample_rate if board.current_sample_rate != 0 else 0 for i in self.diff]


        self.meanbpm = 60/np.mean(self.diff)
        print(self.meanbpm)

        #if(self.flag_first_filter):
        self.filtered_sum = self.filtered_sum[1000-16:1000+16]

        self.window_start_pos += self.stride

    def get_filtered_data(self):
        logic_peak = [1 if i in self.peaks else 0 for i in range(640)]
        print(self.peaks)
        return logic_peak[-32:]
        #return self.filtered_sum[-32:]
