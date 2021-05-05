# IDEA FOR MOONBOARD LED PROJECT

This project contains the code used for the simulation of the moonboard LED kit. The project uses a STM32 nucleo F401RE, a simple keypad, a LCD screen and LED lights WS2811. The usage of this project is based also on a custom made application for android that simulates the moonboard app for filtering and selecting boulder problems. 

The idea is to use the custom made app on the phone, this allows the user to filter through problems and see graphically the layout of the board with higlighted holds depending on the selected problem. The app also outputs an ID number, which is then used as an input to the SMT mictocontroller (trought a keypad) for specifiyng the desired problem. Thanks by the selection of the ID the STM microcontroller will then switch on specific LEDS.

The components used are a microcontroller [STM32 F401RE](https://www.amazon.com/Matrix-Membrane-Switch-Keyboard-Arduino/dp/B07THCLGCZ/ref=sr_1_3?dchild=1&keywords=keypad+arduino&qid=1620036372&sr=8-3) [LCD screen]() [keypad]() [LED WS2811]()

![name-of-you-image](https://github.com/AlessandroAvi/Moonboard_LED_DIY/blob/main/Img/STMF401RE.jpg)





## GENERAL IDEA OF THE CODE

- **Blue burron interrupt** I press the blue button and activate the function for catching the keypad number
- **`keypad_GetNumber`** number gets saved
  - polling mode, wait for a key to be pressed, store digit in an array, transform array in number, return ID number
- **`problem_fetch`** STM searches the boulder assigned to that ID in a "switch case" function already written 
  - the fnunction "switch case" is copy pasted from a script previously run by a console app on the lapton, 
  - the function `problem_fetch` saves in a struct the information of the problem (ID, name, letters and number of the holds, start and top holds, grade)
- **`problem_genArray`** generate a matrix (198x4, row 1=number of the led, row2 = red color, row3 = green color, row4 = blue color) and initializes it to 0 everywhere, then save in the corresponding position the correct colors for the holds used by the problem (hols position is obtained from the struct)
- **`WS2811_light`**, sends a PWM signal and lights the leds
  - in a double for cycle, generate an array of values of duty cycles depending on the 0 and 1 bits (from the color array defined previously), send the correct duty cycle data to the PWM peripheral (using DMA)

 



## TO DO

- script that download and updates list of problem in JSON format

## LED EXPLANATION

model: WS2811
AliExpress link: 

The serie of LED WS2811 can be easily controlled with just 3 lines connected to the microcontrolloer. Two of these lines are GND and VDD (in my case 5 Volt), the third is the actual data line control. From the [data sheet](https://www.alldatasheet.com/datasheet-pdf/pdf/1132633/WORLDSEMI/WS2811.html) of the LED is quite easy to get how they work. 

The data line is used for senting to the LED the infomrations related to the color that they should light up. Each LED is can create 255 levels of color for the channels Red, Green, Blue. Sicen the data line is just one it requires a signal specifically generated. This specific model of LED requires a signal

Each single LED behaves as a repetitor, where it catches the signal from the data line, removes some information that is dedicated for that specific LED and repeats the signal. The control of the LED requires the use of a PWM signal, where the duty cycle defines the information sent. A duty cycle of 48% reppresents a bit 1, a dytu cycle of 18% reppresents a bit 0. 

## KEYPAD EXPLANATION

## LCD EXPLANATION



## PHOTOS OF THE BOARD



