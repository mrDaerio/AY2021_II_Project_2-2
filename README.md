# Project 2 - Heart Rate Monitor
This repo contains all the necessary files to create a heart rate monitor using the CY8CKIT-059, a HC-05 Bluetooth module and a LIS3DH accelerometer.

### User Guide

You can 3D print the case of the accelerometer, look in the "Case" folder for further information.

###### Setup of the system

![](/Photos/setup.jpg)

Connect the pins of the components following the table:

![](/Photos/pins.PNG)

###### Programming the firmware of the microcontroller

Double click on the file "Workspace01.cywrk", PSoC Creator will automatically open. Compile the project and upload it on the board.

###### Running the GUI

Check the readme in the "GUI" folder and install the needed packages to make the interface work. You also need to connect the Bluetooth module to your PC, you can follow the same procedure you would do when pairing some wireless headphones. The module will be called "HC-05", but the name may differ.

Now you can run the "main.py" file from a Python virtual environment and you're ready to go! 

###### How to wear the sensor?

You can use a rubber band to tie the sensor close to the body like so:

![](/Photos/band.jpg)

![](/Photos/3DModel.gif)

### How is the GUI structured?

[Map of the GUI](https://miro.com/app/board/o9J_lDBEVHY=/)

### Students

* Dario Comini

* Stefano Lucchesini

* Giovanni Merighi
