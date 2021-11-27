# IDEA FOR MOONBOARD LED PROJECT

This repository contains the code that I developed for creating a LED moonboard controller. The project is applied on a STM32 microcontroller and it work together with other repositories that I developed. These are:

- [Android app](https://github.com/AlessandroAvi/Moonboard_Android_App) that can be used to filter the moonboard problems (dataset created from this repo), visualize them on the screen and later send the most relevant information via bluetooth to the STM32 microcontroller
- [Python code](https://github.com/AlessandroAvi/Moonboard_Dataset) that I developed for generating the moonboard boulder problems dataset. This uses some computer vision and automatic scrolling throught the problems (on an android simulator installed on the pc) in order to create a json that contains all the info of the boulder problems (because the original dataset is private)

 ![name-of-you-image](https://github.com/AlessandroAvi/Moonboard_LED_DIY/blob/main/Img/Intro.png) 

# BOM

The necessary hardware for this part of the prohject applied on the STM32 microcontroller is:

- Microcontroller STM32 F401RE: [Amazon link](https://www.amazon.com/NUCLEO-F401RE-Nucleo-64-Development-STM32F401RE-connectivity/dp/B07JYBPWN4)
- arduino keypad 4x4: [Amazon link](https://www.amazon.com/Matrix-Membrane-Switch-Keyboard-Arduino/dp/B07THCLGCZ/ref=sr_1_3?dchild=1&keywords=arduino+keypad&qid=1620202495&sr=8-3)
- LCD screen: [Amazon link](https://www.amazon.com/ICQUANZX-Interface-Backlight-Ar-duino-MEGA2560/dp/B08XQMKXW1/ref=sr_1_2?dchild=1&keywords=display+LCD+ICQUANZX+16x2&qid=1620202398&sr=8-2) 
- LED WS2811 (25 cm of spacing between each LED is required because of the holds distance: [Aliexpress link](https://it.aliexpress.com/wholesale?catId=0&initiative_id=SB_20210505001530&SearchText=ws2811+25cm)
- Bluetooth module HC-05: [Amazon link](https://www.amazon.com/HiLetgo-Wireless-Bluetooth-Transceiver-Arduino/dp/B071YJG8DR/ref=sr_1_1_sspa?dchild=1&keywords=bluetooth+module+hc+05&qid=1634827066&sr=8-1-spons&psc=1&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUEySjRXTkNPVFVPQUY3JmVuY3J5cHRlZElkPUEwNDY1MDg2MjgwQ0ROQ1JSV0VMUSZlbmNyeXB0ZWRBZElkPUEwNjgxMDUxMjlXTU9IMEJZTzMyUyZ3aWRnZXROYW1lPXNwX2F0ZiZhY3Rpb249Y2xpY2tSZWRpcmVjdCZkb05vdExvZ0NsaWNrPXRydWU=)

 ![name-of-you-image](https://github.com/AlessandroAvi/Moonboard_LED_DIY/blob/main/Img/material.png) 

**IDEA**: Use the custom made app on the phone in order to filter and select the boulder problem. Once the problem has been selected the app shows an ID that will be used on the keypad in order to light the correct LED for the holds. The purpose of the LCD is just for understanding if the problem selected on the STM is correct (name and grade). Another option is to use the "bluetooth send" button i  the app, that will send all the relevant information to the STM32 via bluetooth, wich will then control the LEDs and light up the boulder problem.

# GENERAL IDEA OF  THE STM CODE

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

- feature on the app search by name
- bluetooth connection between phone-STM in order to avoid to use keypad and avoid storing all the problems in the STM (done - have to debug)

# PHOTOS OF THE CONTROLLER

![name-of-you-image](https://github.com/AlessandroAvi/Moonboard_LED_DIY/blob/main/Img/SCHEMATIC.png) 

![name-of-you-image](https://github.com/AlessandroAvi/Moonboard_LED_DIY/blob/main/Img/Assembly.png) 

| DEVICE PIN        | STM32 PIN | DEVICE PIN       | STM32 PIN       |
| ----------------- | --------- | ---------------- | --------------- |
| Keypad - row 1    | C7        | LED line 1 - VDD | Wall plug - 5V  |
| Keypad - row 2    | A9        | LED line 1 - GND | Wall plug - GND |
| Keypad - row 3    | A8        | LED line 1 - PWM | A6              |
| Keypad - row 4    | B10       | LED line 2 - VDD | Wall plug - 5V  |
| Keypad - column 1 | B4        | LED line 2 - GND | Wall plug - GND |
| Keypad - column 2 | B5        | LED line 2 - PWM | A0              |
| Keypad - column 3 | B3        | LCD - VDD        | 5V              |
| Keypad - column 4 | A10       | LCD - GND        | GND             |
| HC-05 - VDD       | 5V        | LCD - SDA        | B7              |
| HC-05 - GND       | GND       | LCD - SCL        | B6              |
| HC-05 - TDX       | C6        |                  |                 |
| HC-05 - RDX       | A12       |                  |                 |
| HC-05 - Reset     | B9        |                  |                 |

The case for the STM32 with the LCD screen, keypad and bluetooth module has been 3d printed. The STL file is contained in the directory 'STL'.

# PHOTOS OF THE BOARD

![name-of-you-image](https://github.com/AlessandroAvi/Moonboard_LED_DIY/blob/main/Img/boardAll.png) 
