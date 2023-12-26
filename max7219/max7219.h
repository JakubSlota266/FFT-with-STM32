/*
 * max7219.h
 *
 *  Created on: Oct 24, 2023
 *      Author: jakub
 */
#ifndef INC_MAX7219_H_
#define INC_MAX7219_H_

#include "stdint.h"
#include "gpio.h"
#include "spi.h"

// Registers //

#define MAX7219_NO_OP_REG  					0x00
#define MAX7219_DIGIT0_REG 					0x01
#define MAX7219_DIGIT1_REG					0x02
#define MAX7219_DIGIT2_REG					0x03
#define MAX7219_DIGIT3_REG					0x04
#define MAX7219_DIGIT4_REG					0x05
#define MAX7219_DIGIT5_REG					0x06
#define MAX7219_DIGIT6_REG					0x07
#define MAX7219_DIGIT7_REG					0x08
#define MAX7219_DECODE_MODE_REG 			0x09
#define MAX7219_INTENSITY_REG  				0x0A
#define MAX7219_SCAN_LIMIT_REG 				0x0B
#define MAX7219_SHUTDOWN_REG 				0x0C
#define MAX7219_DISPLAY_TEST_REG	 		0x0F

// Decode mode values //
#define NO_DECODE_FOR_DIGITS_7_TO_0			0x00
#define DECODE_FOR_DIGIT_0					0x01
#define DECODE_FOR_DIGIT_3_TO_0				0x0F
#define DECODE_FOR_DIGIT_7_TO_0				0xFF

// Led Matrix //

#define MAX7219_PIXELS_PER_ROW_COLUMN 		8

typedef enum
{
	MAX7219_OK 								= 0,
	MAX7219_ERROR							= 1,
}MAX7219_STATUS;

typedef enum
{
	NoDecodeForDigits7_to_0 				= 0,
	DecodeForDigit0							= 1,
	DecodeForDigits_3_to_0					= 2,
	DecodeFor7Digits_7_to_0					= 3
}MAX7219_DecodeMode;

typedef enum{
	Shutdown_mode							= 0,
	Operational_mode 						= 1
}MAX7219_OperationalMode;

typedef enum
{
	ScanDigit0		= 0,
	ScanDigit0_1	= 1,
	ScanDigit0_2	= 2,
	ScanDigit0_3	= 3,
	ScanDigit0_4	= 4,
	ScanDigit0_5	= 5,
	ScanDigit0_6	= 6,
	ScanDigit0_7	= 7,
}MAX7219_ScanLimit;

typedef enum
{
	TestOff									= 0,
	TestMode								= 1
}MAX7219_TestMode;

typedef enum
{
	MAX7219_OFF = 0,
	MAX7219_ON = 1,
	MAX7219_INVERSE = 2,
}MAX7219_digit_state;

MAX7219_STATUS MAX7219_SetIntensity(uint8_t intensivity);

MAX7219_STATUS MAX7219_SetDecodeMode(MAX7219_DecodeMode decode_mode);

MAX7219_STATUS MAX7219_SendData(uint8_t addr, uint8_t data);

MAX7219_STATUS MAX7219_SetOperationalMode(MAX7219_OperationalMode operational_mode);

MAX7219_STATUS MAX7219_SetTestMode(MAX7219_TestMode test_mode);

MAX7219_STATUS MAX7219_Init(SPI_HandleTypeDef *hspi);

MAX7219_STATUS MAX7219_SetPixel(int x, int y, MAX7219_digit_state state);

MAX7219_STATUS MAX7219_SetAllDigits(MAX7219_digit_state state);

MAX7219_STATUS MAX7219_Display(void);

MAX7219_STATUS MAX7219_SetSpectrum(int* spectrumTab, int numOfRanges);

uint8_t acctualDecodeModeVal(MAX7219_DecodeMode decode_mode);

#endif /* INC_MAX7219_H_ */
