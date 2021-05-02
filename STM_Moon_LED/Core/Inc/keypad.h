// This file contains the functions declarations for the library that controls the keypad
#include "main.h"



// reads all 4 GPIO that are selected as input columns
// returns a value if it detects a line low (button pressed)
uint8_t read_GPIO();


// enables high 3 lines and low 1 line depending on the input "riga"
void write_GPIO(uint8_t riga);


// depending on the current active row and the current active column
// returns the correct key value
uint8_t decode_key(uint8_t line, uint8_t col);


// perform a polling on 4 rows, each time enables a row and reads columns
// returns the value of the key read
uint8_t read_key();


// receives teh value of the key just read, perform different actions
// returns the ID composed of the digits just selected
uint32_t keypad_getNumber();

