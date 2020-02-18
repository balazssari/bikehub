# bikehub
This is the code for my ebike controller with TFT touchscreen.

What is done:
- TFT Display communication fully working
    Library used: MCUFRIEND tft library
    Needed to modify macros to IO-s used on the Bikehub board

- Touchscreen working
    Resistive touch panel with 4 wire output.
    Basic working:
    Imagine the touch panel as 4 resistors connected in star topology: 
                    y1
                    |
                   | |
                   | |
           ____     |     ____
    x1----|____|----|----|____|----x2
                    |
                   | |
                   | |
                    |
                    y2
    For reading out y axis value we need to measure with ADC on either x1 or x2 point.
    If we chose to connect ADC to x2 we need to set x1 to INPUT or high impedance with the MCU.
    Furthermore, we need to set y1 to HIGH level and y2 to LOW voltage level or vice versa.
    Now we can read with the ADC on x2. Do the same on x axis.
    This is done in the readTouch() function.

- A simple numeric pad for secure login


What needs to be done:
- Let the user to set up own password
- Communication over UART to VESC
- Write basic values sent by VESC to the screen
- Acces all values over UART
- Modify values over UART
- Create UI for value editing
- Get analog inputs working
- 
