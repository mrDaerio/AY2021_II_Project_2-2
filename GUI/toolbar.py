from kivy.properties import NumericProperty, ObjectProperty, StringProperty
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.popup import Popup
from communication import KivySerial
import os

class Toolbar(BoxLayout):
    """
    @brief Lateral toolbar widget.

    The toolbar widget is used to show some
    settings related to the app.
    """

    """
    @brief String used to send messages to bottom bar.
    """
    message_string = StringProperty("")

    def __init__(self, **kwargs):
        super(Toolbar, self).__init__(**kwargs)
        self.board = KivySerial()

    def sample_rate_dialog(self):
        """
        @brief Open popup for wave selection.
        """
        self.message_string = "Sample Rate Dialog"
        popup = SampleRateDialog()
        popup.open()

    def full_scale_range_dialog(self):
        """
        @brief Open popup for wave selection.
        """
        self.message_string = "Range Selection Dialog"
        popup = FullScaleRangeDialog()
        popup.open()

    def output_file_dialog(self):
        if self.board.signal.x_data != []:
            popup = SaveDialog()
            popup.open()
        #else print 'no data collected'?

class SaveDialog(Popup):
    save = ObjectProperty(None)
    text_input = ObjectProperty(None)
    cancel = ObjectProperty(None)

    def __init__(self, **kwargs):
        self.board = KivySerial()
        super(SaveDialog, self).__init__(**kwargs)

    def save(self, path, filename):
        if filename[-4:].lower() != '.csv':
            filename += '.csv'
        with open(os.path.join(path, filename), 'w') as csv_file:
            #save whole raw data of the session in an unique csv file
            csv_file.write('x,y,z\n')
            for i in range(len(self.board.signal.x_data)):
                csv_file.write(str(self.board.signal.x_data[i])+','+
                               str(self.board.signal.y_data[i])+','+
                               str(self.board.signal.z_data[i])+'\n')
        self.dismiss()

    def cancel(self):
        self.dismiss()

class SampleRateDialog(Popup):
    """
    @brief Popup to allow wave selection 
    """
    sample_rate_spinner = ObjectProperty(None)

    def __init__(self, **kwargs):
        self.board = KivySerial()
        super(SampleRateDialog, self).__init__(**kwargs)

    def on_sample_rate_spinner(self, instance, value):
        self.sample_rate_spinner.text = f'{self.board.sample_rate} Hz'

    ##
    #   @brief          Callback called when update button is pressed.
    #
    #   If the board is connected, update the sample rate.
    #
    def update_pressed(self):
        
        if (self.board.is_connected()):
           self.board.update_sample_rate_on_board(self.sample_rate_spinner.text)
        self.dismiss()


class FullScaleRangeDialog(Popup):
    """
    @brief Popup to allow range selection 
    """
    fsr_select_spinner = ObjectProperty(None)

    def __init__(self, **kwargs):
        super(FullScaleRangeDialog, self).__init__(**kwargs)
        self.board = KivySerial()

    def on_sample_rate_spinner(self, instance, value):
        self.fsr_select_spinner.text = f'{self.board.full_scale_range}'

    ##
    #   @brief          Callback called when update button is pressed.
    #
    #   If the board is connected, update the full scale range.
    #
    def update_pressed(self):
        """
        @brief Callback called when update button is pressed.

        If the board is connected, update the range selection.
        """
        if (self.board.is_connected()):
            self.board.update_fsr_on_board(self.fsr_select_spinner.text)
        self.dismiss()