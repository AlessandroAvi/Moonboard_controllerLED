# IDEA FOR MOONBOARD LED PROJECT

This repository contains the code used for simulating the moonboard LED kit. The hardware used is:

- Microcontroller [STM32 F401RE](https://www.amazon.com/NUCLEO-F401RE-Nucleo-64-Development-STM32F401RE-connectivity/dp/B07JYBPWN4) 
- arduino [keypad ](https://www.amazon.com/Matrix-Membrane-Switch-Keyboard-Arduino/dp/B07THCLGCZ/ref=sr_1_3?dchild=1&keywords=arduino+keypad&qid=1620202495&sr=8-3) 4x4
- [LCD screen](https://www.amazon.com/ICQUANZX-Interface-Backlight-Ar-duino-MEGA2560/dp/B08XQMKXW1/ref=sr_1_2?dchild=1&keywords=display+LCD+ICQUANZX+16x2&qid=1620202398&sr=8-2) 
- [LED WS2811](https://it.aliexpress.com/wholesale?catId=0&initiative_id=SB_20210505001530&SearchText=ws2811+25cm) (25 cm of spacing between each LED is required because of the holds distance)
- Bluetooth module [HC-05](https://www.amazon.com/HiLetgo-Wireless-Bluetooth-Transceiver-Arduino/dp/B071YJG8DR/ref=sr_1_1_sspa?dchild=1&keywords=bluetooth+module+hc+05&qid=1634827066&sr=8-1-spons&psc=1&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUEySjRXTkNPVFVPQUY3JmVuY3J5cHRlZElkPUEwNDY1MDg2MjgwQ0ROQ1JSV0VMUSZlbmNyeXB0ZWRBZElkPUEwNjgxMDUxMjlXTU9IMEJZTzMyUyZ3aWRnZXROYW1lPXNwX2F0ZiZhY3Rpb249Y2xpY2tSZWRpcmVjdCZkb05vdExvZ0NsaWNrPXRydWU=)

 ![name-of-you-image](https://github.com/AlessandroAvi/Moonboard_LED_DIY/blob/main/Img/material.png) 

This project is based on a custom made android app used for filtering and visualizing boulder problems. 

**IDEA**: Use the custom made app on the phone in order to filter and select the boulder problem. Once the problem has been selected the app shows an ID that will be used on the keypad in order to light the correct LED for the holds. The purpose of the LCD is just for understanding if the problem selected on the STM is correct (name and grade).



## GENERAL IDEA OF  THE STM CODE

- The user **searches on the app** the boulder problem that he want to do. The app will display the problem holds together with the ID
- The user presses the **blue button**, this activates the microcontroller to listen for the keypad numbers
- **`keypad_GetNumber`** number gets saved as I insert the digits (press A when finished, press D to delete last digit)
  - polling mode, wait for a key to be pressed, store the digits separately in an array, transform the array in a number, return ID number
- **`problem_fetch`** STM searches the ID just obtained from the keypad in a very big "switch case" that has been filled a priori as a database for the problems
  - the function `switch case` is copy pasted from a script previously run by a console app on the laptop
  - the function `problem_fetch` saves in a struct the information of the problem (ID, name, letters and number of the holds, start and top holds, grade)
- **`problem_genArray`** generate a matrix (198x4, row 1=number of the led, row2 = red color, row3 = green color, row4 = blue color) and initializes it to 0 everywhere, then save in the corresponding position the correct colors for the holds used by the problem (holds position is obtained from the struct)
- **`WS2811_light`** sends a PWM signal and lights the leds
  - in a double `for` cycle, generate an array of values of duty cycles depending on the 0 and 1 bits (from the color array defined previously), send the correct duty cycle data to the PWM peripheral (using DMA)

More detailed explanation of the code developed is in the `README` in the `STM_Moon_LED` directory 

 

## TO DO

- python script that generates a new JSON file for all the boulder problems (wip - already working, need to be refined)
- feature on the app search by name
- bluetooth connection between phone-STM in order to avoid to use keypad and avoid storing all the problems in the STM (done - have to debug)

## PHOTOS OF THE CONTROLLER

![name-of-you-image](https://github.com/AlessandroAvi/Moonboard_LED_DIY/blob/main/Img/SCHEMATIC.png) 

## PHOTOS OF THE BOARD

![name-of-you-image](https://github.com/AlessandroAvi/Moonboard_LED_DIY/blob/main/Img/boardAll.jpg) 
