# Brushed-DC-Motor-Controller
Motor Control Schematics, PCB and Code

This repository contains a collection of files that detail the creation of a brushed DC motor contoller.

This project utilizes an IC_DRV8251DDAR brushed motor controller from Texas Instruments. This sensor utilizes PWM signals to drive a brushed dc motor forward and backwards via command line controls. Particularly, this motor controller is designed to work with the PSOC6 microprocessor. This schematic and PCB was created in Altium Designer.

![alt text](https://github.com/MozillaMudkip/Brushed-DC-Motor-Controller/blob/main/DCMotorSchematic.png?raw=true)

This particular motor controller was created as an "introductory demo" for my Engineering Capstone Design project. Two of these motor controllers will be utilized in my teams project, a fully automatic sorting machine, to move the sensor module and base platform with sorted containers back and forth on an X and Y grid. 

![alt text](https://github.com/MozillaMudkip/Brushed-DC-Motor-Controller/blob/main/DCMotorPCB.png?raw=true)

This motor controller takes in two power sources, a 3.3V input for reference and a 12V input via a barrel jack to provide the DC motor with sufficient power. The DC motor itself connects to the board via the two channel TB006-508-02BE connector. Also, amongst test points for 12V, 3.3V and GND, there are test points and status LEDs on both PWM signal lanes to make testing, debugging and monitoring via oscilloscope easy. For more specifics on the specifications of the IC_DRV8251DDAR brushed motor controller itself, please refer to the below datasheet.

https://www.ti.com/lit/ds/symlink/drv8251.pdf?ts=1741592249631&ref_url=https%253A%252F%252Fwww.mouser.de%252F

