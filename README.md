# Project 2 - Heart Rate Monitor Accelerometer
This repository contains all the files used to build a heart rate monitoring device, as requested by the final project of the course. The hardware used for this project consists in the CY8CKIT-059, a HC-05 Bluetooth module, a LIS3DH accelerometer and a breadbord which allows to connect all the components.

In this file are listed all the steps to reproduce and program the device, with also explanatory photos and images to help the user in the process.

## Device Building and Placement
In this first section is explained in detail how to build the device and where to place it on the body in order to obtain the best signal acquisition.

### Material:

As illustrated in the picture below, in order to reproduce the device is needed a microcontroller, a bluetooth module, an accelerometer (with the 3D printed case) and an elastic band to easily place it on the torax. The powerbank is not mandatory, but it is useful if the subject wants to have the possiblity to be physically detached from the computer.

![](/Photos/setup.jpg)

### Connection:

Follow the table below to properly connect the pins of the components to the microcontroller:

![](/Photos/pins.PNG)

### 3D Case:

In order to have a better adeherence of the accelerometer to the body, and a better support for the elastic band, a special case has been developed, and it is possible to find all the relative information in the folder named "Case". 
The relative README file in the folder explains how to attach the case to the accelerometer.

### How to wear the sensor:

The rubber band needs to be placed behind the case, like showed in the picture below. 

![](/Photos/band.jpg)

A 3D animation shows how the accelerometer needs to be placed on the body, in order to obtain good acquisitions.

![](/Photos/3DModel.gif)


## Programming the Microcontroller

Once the repository has been downloaded on the computer, it's possible to program the kit through PSoC creator. To do that, simply double click on the file "Workspace01.cywrk",  and PSoC Creator will automatically open. Compile the project and upload it on the board. In the Top Design file is also possible to find other information and visual representation on how to connect the components to the board, if not clear.
Mind that a specific library has been used, called AnnotationLibrary. You might have to add it in order to correctly build the project and program the device.

If all the connection are correct, at the end of the programming phase you should have a red led blinking on the bluetooth module and the blue led of the microcontroller lighted up. If the blue led is blinking, it means that the accelerometer is not connected properly.

## GUI

The GUI has been developed with kivy, and allows to display the data retrieved by the microcontroller and elaborate them to extract the heart rate of the subject. Through the interface is possible also to have a graphical representation of the signal rielaborated, in order to have a clear representation of the beating, and also allows to change internal parameter of the microcontroller, such as the sample rate and the full scale range. Another feature is the data export of the three raw channels of the accelerometers in a .csv file, which can be used for further studies or further data elaboration on other platforms.
The microcontroller is also able to save the last parameter that the user has set, and retrieve them for the next session.

All the packets that need to be installed in the virtual environment to make the interface work are listed in the folder named "GUI". The bluetooth module connection with the computer can be established in the bluetooth settings, as any other bluetooth device. The module is called "HC-05", but the name may differ.

### First Run

Run the "main.py" file from a Python virtual environment to start the GUI. This window should appear:

![](/Photos/start.png)

### Connection

Once the connection with the bluetooth module has been established, the graphical interface should look like this:

![](/Photos/connection.png)

As it is possible to notice, the selection buttons can now be pressed to change the parameters of the microcontroller, and start button on the top right corner enables the data streaming and the graphical representation of the data.

The GUI should visualize the signal in this way:

![](/Photos/signal.png)

To interrput the connection, simply press the stop button. The data export button allows to save the current session locally in the computer.

Note that you have to wait about 10 seconds before seeing any plot on the GUI because vectors need to be filled and data needs to be processed.

The exported data saved in the csv file will contain all the data of the last acquisition (time between start and stop commands).

### How is the GUI structured?

Since the GUI code can be complicated at first glance, this graph tries to simplify and clarify every connection inside the code.
[Map of the GUI](https://miro.com/app/board/o9J_lDBEVHY=/)

## Students Authors of the Project

* Dario Comini

* Stefano Lucchesini

* Giovanni Merighi
