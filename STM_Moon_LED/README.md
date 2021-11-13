# EXPLANATION OF THE CODE



## LED EXPLANATION

model: WS2811 25 cm spacing

AliExpress link: [link](https://it.aliexpress.com/wholesale?catId=0&initiative_id=SB_20210505001530&SearchText=ws2811+25cm)

video tutorial that I used: [link](https://www.youtube.com/watch?v=-3VKkTSAytM&list=PLKbve1xdC21o1C_YVfbdDPc5UOA68WjFB&index=3&t=0s)

The code for the LED is inside LED.c.

The serie of LED WS2811 can be easily controlled with just 3 lines connected to the STM. Two of these lines are GND and VDD (in my case 5 Volt but on Aliexpress you can find also the 12 V version), the third is the actual data line control. From the [data sheet](https://www.alldatasheet.com/datasheet-pdf/pdf/1132633/WORLDSEMI/WS2811.html) of the LED is quite easy to understand how they work. 

Each single led is able to reppresent 255 levels of red, green and blue. The information about the color cannel should be given to each LEDs in order (of the cable connection), starting from the first encountered. The signal on the data line needs to be sent in bit (high voltage and low voltage at a specific rate), so basically every single led requires 24 bit for the specification of the 3 color channels (8 bit for red, 8 bit for green, 8 bit for blue). Since there is just 1 line for the control, the LEDs require that the bit signal is sent at a specific frequency, in this case the model WS2811 can work at two frequencies, 800 KHz ad 400 KHz (I tested only the 800 KHz). 

In order to distinguish 1 bits from 0 bits the LED requires the usage of a PWM signal, where the ducty cycle value is the variable that should be controlled. From the data sheet we get: 

 ![name-of-you-image](https://github.com/AlessandroAvi/Moonboard_LED_DIY/blob/main/Img/PWM.jpg) 

So a 0 bit signal should stay high for 0.5 us and low for 2 us, while a 1 bit should stay high for 1.2 us​ and low for 1.3 us. Note that the total time of the signal is 2.5 us, which corresponds to 400 KHz. To have a general definition of the duty cycle just define the in term of percentage:

- 0 bit -> duty cycle = 20%
- 1 bit -> duty cycle = 48%

Additionally there is also the RESET signal, which should be of at least 50 us. The purpose of this signal is to tell to the LEDs that the control signal is going to start or finish, in fact it is necessary to put it at the beginning and at the end of the PWM in order to make it work properly. Which basically means that for the first and last 50 us the PWM should have a duty cycle of 0% (in the case of 800 KHz 50 us corresponds to 40 cycles).   

In the code I developed I use the DMA to have better performance and easily send the signal through the PWM. The code basically generates a matrix 198x3 (there are 198 holds on the moonboard, 3 are the color values for each one), fills it with 0 values to have all LED black. After fetching the boulder problem and translating the couple letter-number into a number that reppresents the i-th LED position (all the 198 LED are connected in series, so it's necessary to transform the 2 D reppresentation of the hold position into a number), the code sets to white/green/red only the holds used by the problem (color depends on if it's start, top, or middle). 

After this a simple routine fills an array called `pwmData` with the values of the duty cycles (40 cycles reset + cycles for the LED + 40 cycles for reset). Note that each single position of the `pwmData` array reppresent one bit of the numbers reppresenting the color for each LED. So if the matrix containing the colors was a 198x3 matrix (total 596), this new array `pwmData` has a length of (198x3x8)+100, where 8 is the dimension of each color, 198 the number of LEDs, 3 the number of colors for each LED, 80 is the number of cycles that have to be put to 0 for the reset command. Finally this array is sent to the PWM peripheral which will generate a signal depending on the duty cycles specified. 

Important: I had a very hard time making the DMA work. My problem was that I didn't set coherently the dimensions of the DMA transmission from cubeMX and in the code. If you want to develop similar code for the LED make sure to set correctly those, I put "Word" in the cubeMX DMA transmission length and used uint32_t as the dimension of the array `pwmData`, even if it has to hold numbers up to 100. 

## KEYPAD EXPLANATION

model: keypad for arduino 4x4

amazon link: [link](https://www.amazon.com/Matrix-Membrane-Switch-Keyboard-Arduino/dp/B07THCLGCZ/ref=sr_1_3?dchild=1&keywords=arduino+keypad&qid=1620202495&sr=8-3)

webiste tutorial I used: [link](https://deepbluembedded.com/stm32-keypad-interfacing-library/)

The code for the keypad is inside keypad.c.

This type of keypad is quite easy to use, there are 8 total connections where 4 are inputs (rows) and 4 are outputs (columns). 
The kaypad is made in the following way:

 ![name-of-you-image](https://github.com/AlessandroAvi/Moonboard_LED_DIY/blob/main/Img/Keypad_circuit.jpg)

The rows and cols are connected through a switch on each button. By pressing one button the switch closes and two lines (one from the rows and one from the cols) are shorted together. 

The idea for obtaining a signal from this device is to use a polling mode in which 3 rows are set on high level and 1 row on low level (and iteratively change the row that is brought to low level). Each time this is performed I read also the 4 lines coming from the columns, in this way is possible to understand if a button press has occurred, this because a short circuit is generated on a button press  (important to set with a pull-up all the column lines, so they are high by default and the short circuit is able to bring it down only when necessary). 

By knowing the last row that has been set to low and also by knowing the column that has been short circuited to low, it's immediate to find out which button has been pressed. 



## LCD EXPLANATION

model: display LCD ICQUANZX 16x2

amazon link: [link](https://www.amazon.com/ICQUANZX-Interface-Backlight-Ar-duino-MEGA2560/dp/B08XQMKXW1/ref=sr_1_2?dchild=1&keywords=display+LCD+ICQUANZX+16x2&qid=1620202398&sr=8-2)

library used: 

In this developed code just a couple functions have been used for making the LCD work. In particular the init function, the write function and the reset (for deleting the last thing written). 



## BLUETOOTH MODULE EXPLANATION

model: hc-05

amazon link: [link](https://www.amazon.com/HiLetgo-Wireless-Bluetooth-Transceiver-Arduino/dp/B071YJG8DR/ref=sr_1_2_sspa?dchild=1&keywords=hc+05&qid=1635588155&sr=8-2-spons&psc=1&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUEzTjQ3NVAwUVZWUE5RJmVuY3J5cHRlZElkPUEwNDU0MzQxM08xNE04NVJNTUlNUSZlbmNyeXB0ZWRBZElkPUEwNjgxMDUxMjlXTU9IMEJZTzMyUyZ3aWRnZXROYW1lPXNwX2F0ZiZhY3Rpb249Y2xpY2tSZWRpcmVjdCZkb05vdExvZ0NsaWNrPXRydWU=)

video tutorial used: [link](https://www.youtube.com/watch?v=xE6GVt7XuJI)

The code developed for this module is entirely inside the main.c and in the function MessageHandler inside `LED.c`

The code is very short and quite easy to implement/understand. After connecting correctly the device to the microcontroller (gnd, vdd, uart connection rx and tx and reset button) the code is only inside the interrupt call at the bottom of the main. At the beginning of the code the pin connected to the Reset line of the device is raised high in order to enable the bluetooth receiver. After that the device is already ready to be used.

The only lines of code that are requried for the bluetooth communication are the function `HAL_UART_Receive_IT(&huart6, rx, 50)`, which is at the beginning of the `USER CODE 2`, and the other important function is the interrupt callback function, `HAL_UART_RxCpltCallback()` which is positioned at the bottom of the code in the secion `USER CODE 4`. 

The idea is that the anroid device sends multiple strings long 50 chars. Each single string contains the informations related to the problem (name, grade, number of holds, letter and digit tha reppresent the hold, type of hold). This methos uses srtings that are always long 50 chars, this because is easier to use UART interrupts with constant lengths of messages. 

Each time a new string is read from the UART, also its type is recorded. The first char of the string reppresents the information that is stored inside, and depending on the first character the string in copieed in different buffers. These info are then manipulated and put insied the struct that contains the info of the current boulder problem.
