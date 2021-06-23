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

Double click on the file "Workspace01.cywrk", PSoC Creator will automatically open. Compile the project and upload it on the board.

## GUI

Check the readme in the "GUI" folder and install the needed packages to make the interface work. You also need to connect the Bluetooth module to your PC, you can follow the same procedure you would do when pairing some wireless headphones. The module will be called "HC-05", but the name may differ.

Now you can run the "main.py" file from a Python virtual environment and you're ready to go! 


### How is the GUI structured?

[Map of the GUI](https://miro.com/app/board/o9J_lDBEVHY=/)

### Students

* Dario Comini

* Stefano Lucchesini

* Giovanni Merighi
