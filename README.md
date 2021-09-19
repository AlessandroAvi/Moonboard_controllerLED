# IDEA FOR MOONBOARD LED PROJECT

This repository contains the code used for the simulation of the moonboard LED kit. The project uses a STM32 nucleo F401RE, an arduino keypad 4x4, an LCD screen and LED lights WS2811. This project is based on a custom made android app used for filtering and visualizing boulder problems. 

The idea is to use the custom made app on the phone for filtering and visualizing the boulder problems together with its unique ID. The ID (which has been added to each single problem in order to make them easy to search) is then used by the user as an input for the keypad mounted on the STM mirocontroller. Thanks to this ID, the mircocontroller is able to find the correct boulder problem and later light correctly the holds used by it. The purpose of the LCD is just for understanding if the problem selected on the STM is correct. For now it displays the grade and the name of the problem. 

The components used are a microcontroller [STM32 F401RE](https://www.amazon.com/NUCLEO-F401RE-Nucleo-64-Development-STM32F401RE-connectivity/dp/B07JYBPWN4) [LCD screen](https://www.amazon.com/ICQUANZX-Interface-Backlight-Ar-duino-MEGA2560/dp/B08XQMKXW1/ref=sr_1_2?dchild=1&keywords=display+LCD+ICQUANZX+16x2&qid=1620202398&sr=8-2) [keypad](https://www.amazon.com/Matrix-Membrane-Switch-Keyboard-Arduino/dp/B07THCLGCZ/ref=sr_1_3?dchild=1&keywords=arduino+keypad&qid=1620202495&sr=8-3) [LED WS2811](https://it.aliexpress.com/wholesale?catId=0&initiative_id=SB_20210505001530&SearchText=ws2811+25cm) (leds need to have 25 cm of spacing in between LEDs because of the distance between moonboard holds).

 ![name-of-you-image](https://github.com/AlessandroAvi/Moonboard_LED_DIY/blob/main/Img/material.png) 

## GENERAL IDEA OF THE STM CODE

- **search on the app** the boulder problem that I want to do, display in on the phone screen together with the ID
- **Blue button interrupt** I press the blue button and activate the function for catching the keypad number
- **`keypad_GetNumber`** number gets saved as I insert the digits (press A when finished, press D to delete last digit)
  - polling mode, wait for a key to be pressed, store digit in an array, transform array in number, return ID number
- **`problem_fetch`** STM searches the boulder assigned to that ID in a "switch case" function already written 
  - the fnunction "switch case" is copy pasted from a script previously run by a console app on the lapton, 
  - the function `problem_fetch` saves in a struct the information of the problem (ID, name, letters and number of the holds, start and top holds, grade)
- **`problem_genArray`** generate a matrix (198x4, row 1=number of the led, row2 = red color, row3 = green color, row4 = blue color) and initializes it to 0 everywhere, then save in the corresponding position the correct colors for the holds used by the problem (hols position is obtained from the struct)
- **`WS2811_light`**, sends a PWM signal and lights the leds
  - in a double for cycle, generate an array of values of duty cycles depending on the 0 and 1 bits (from the color array defined previously), send the correct duty cycle data to the PWM peripheral (using DMA)

 

## TO DO

- script that download and updates list of problem in JSON format
- feature on the app that saves the boulder already sent
- feature on the app search by name
- possible wi fi/bluetooth connection between phone and STM



## LED EXPLANATION

model: WS2811 25 cm spacing

AliExpress link: [link](https://it.aliexpress.com/wholesale?catId=0&initiative_id=SB_20210505001530&SearchText=ws2811+25cm)

video tutorial that I used: [link](https://www.youtube.com/watch?v=-3VKkTSAytM&list=PLKbve1xdC21o1C_YVfbdDPc5UOA68WjFB&index=3&t=0s)

The code for the LED is inside LED.c.

The serie of LED WS2811 can be easily controlled with just 3 lines connected to the STM. Two of these lines are GND and VDD (in my case 5 Volt but on the marker there exist also the 12 V version), the third is the actual data line control. From the [data sheet](https://www.alldatasheet.com/datasheet-pdf/pdf/1132633/WORLDSEMI/WS2811.html) of the LED is quite easy to understand how they work. 

Each single leed is able to reppresent 255 levels of red, green and blue. The information about the color cannel should be given to the LEDs in order, starting from the first encountered. The signal on teh data line needs to be sent in bit, so basically every single led requires 24 bit for a specification of 3 color channels (8 bt for red, 8 bit for green, 8 bit for blue). Since there is just 1 line for the control, the LEDs require that the bit signal is sent at a specific frequency, in this case the model WS2811 can work at two frequencies, 800 KHz ad 400 KHz (I tested only the 800 KHz). 

In order to distinguish 1 bits from 0 bits the LED requires the usage of a PWM signal, where the ducty cycle value is the variable that should be controlled. From the data sheet we get:

So a 0 bit signal should stay highfor 0.5 us and low for 2  us, while a 1 bit should stay high for 1.2 us​ and low for 1.3 us; note that the total time of the signal is 2.5 us, which corresponds to 400 KHz. To have a general definition of the duty cycle just define the in term of percentage:

- 0 bit -> duty cycle = 20%
- 1 bit -> duty cycle = 48%

Additionally there is also the RESET signal, which should be of at least 50 us. The purpose of this signal is to tell to the LEDs that the control signal is going to start or finish, in fact it's necessary to put it at the beginning and at the end of the PWM. Which basically means that for the first and last 50 us the PWM should have a duty cycle of 0% (in the case of 800 KHz 50 us corresponds to 40 cycles).   

In the code I developed I use the DMA to have better performance and easily send the signal through the PWM. The code basically generates a matrix 198x3 (there are 198 holds on the moonboard, 3 are the color values for each one), fills it with 0 values to have all LED black. After fetching the boulder problem and translating the couple letter-number into a number that reppresents the i-th LED position (all the 198 LED are connected in series, so it's necessary to transform the 2 D reppresentation of the hold position into a number), the code sets to white/green/red only the holds used by the problem (color depends on if it's start, top, or middle). 

After this a simple routine fills an array called `pwmData` with the values of the duty cycles (40 cycles reset + cycles for the LED + 40 cycles for reset). Note that each single position of the `pwmData` array reppresent one bit of the numbers reppresenting the color for each LED. So if the matrix containing the colors was a 198x3 matrix (total 596), this new array `pwmData` has a length of (198x3x8)+100, where 8 is the dimension of each color, 198 the number of LEDs, 3 the number of colors for each LED, 80 is the number of cycles that have to be put to 0 for the reset command. Finally this array is sent to the PWM peripheral which will generate a signal depending on the duty cycles specified. 

Important: I hava very hard time making the DMA work. My problem was that I didn't set coherently the dimensions of the DMA transmission from cubeMX and in the code. If you want to develop similar code for the LED make sure to set correctly those, I put "Word" in the cubeMX DMA transmission length and used uint32_t as the dimension of the array `pwmData`, even if it has to hold numbers up to 100. 

## KEYPAD EXPLANATION

model: keypad for arduino 4x4

amazon link: [link](https://www.amazon.com/Matrix-Membrane-Switch-Keyboard-Arduino/dp/B07THCLGCZ/ref=sr_1_3?dchild=1&keywords=arduino+keypad&qid=1620202495&sr=8-3)

webiste tutorial I used: [link](https://deepbluembedded.com/stm32-keypad-interfacing-library/)

The code for the keypad is inside keypad.c.

This type of keypad is quite easy to use, there are 8 total connections where 4 are inputs (rows) and 4 are outputs (columns). 
The kaypad is made in the following way:

 ![name-of-you-image](https://github.com/AlessandroAvi/Moonboard_LED_DIY/blob/main/Img/Keypad_circuit.jpg)

The rows and cols are connected through a switch on each button. By pressing one button the switch closes and two lines (one from the rows and one from the cols) are shorted together. 

The idea for obtaining a signal from this device is to use a polling mode in which 3 rows are set on high level and 1 row on low level (and iteritevly change the row that is brought to low level). Each time this is performed I read also the 4 lines coming from the columns, in this way is possible to understand if a button press has occurred, this because a short circuit is generated on a button press  (important to set with a pull-up all the column lines, so they are high by default and the short circuit is able to bring it down only when necessary). 

By knowing the last row that has been set to low and also by knowing the column that has been short circuited to low, it's immediate to find out which button has been pressed. 



## LCD EXPLANATION

model: display LCD ICQUANZX 16x2

amazon link: [link](https://www.amazon.com/ICQUANZX-Interface-Backlight-Ar-duino-MEGA2560/dp/B08XQMKXW1/ref=sr_1_2?dchild=1&keywords=display+LCD+ICQUANZX+16x2&qid=1620202398&sr=8-2)

library used: 

In this developed code just a couple functions have been used for making the LCD work. In particular the init function, the write function and the reset (for deleting the last thing written). 



## PHOTOS OF THE BOARD

![name-of-you-image](https://github.com/AlessandroAvi/Moonboard_LED_DIY/blob/main/Img/boardEmpty.jpg) ![name-of-you-image](https://github.com/AlessandroAvi/Moonboard_LED_DIY/blob/main/Img/boardFull.jpg) ![name-of-you-image](https://github.com/AlessandroAvi/Moonboard_LED_DIY/blob/main/Img/boardLed.jpg) 

