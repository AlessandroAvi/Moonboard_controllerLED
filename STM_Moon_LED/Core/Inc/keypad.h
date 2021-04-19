// This file contains the functions declarations for the library that controls the keypad
#include "main.h"

#define 		no_bit		0x00
#define 		bit0 		0x01
#define 		bit1 		0x02
#define 		bit2 		0x04
#define 		bit3 		0x08





uint8_t decode_key(uint8_t line, uint8_t col);

uint8_t read_key();

uint32_t keypad_getNumber();


void write_GPIO(uint8_t riga);

uint8_t read_GPIO();

uint8_t decode_key_v2(uint8_t line, uint8_t col);

uint8_t read_key_v2();

uint32_t keypad_getNumber_v2();

