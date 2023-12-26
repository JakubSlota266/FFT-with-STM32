/*
 * max7219.c
 *
 *  Created on: Oct 24, 2023
 *      Author: jakub
 */

#include "max7219.h"
#include "main.h"
#include "string.h"

#define CS_SET() 	HAL_GPIO_WritePin(IOEXP_CS_GPIO_Port, IOEXP_CS_Pin, GPIO_PIN_RESET)
#define CS_RESET() 	HAL_GPIO_WritePin(IOEXP_CS_GPIO_Port, IOEXP_CS_Pin, GPIO_PIN_SET)

uint8_t Max7219PixelsBuffer[8];
uint8_t Max7219SpiBuffer[2];

MAX7219_STATUS MAX7219_SendData(uint8_t addr, uint8_t data){

	Max7219SpiBuffer[0] = addr;
	Max7219SpiBuffer[1] = data;

	CS_SET();
	if(HAL_OK != HAL_SPI_Transmit(&hspi2, Max7219SpiBuffer, 2, HAL_MAX_DELAY))
		return MAX7219_ERROR;
	CS_RESET();

	return MAX7219_OK;
}

uint8_t acctualDecodeModeVal(MAX7219_DecodeMode decode_mode){
switch(decode_mode)
{
	case NoDecodeForDigits7_to_0:
	{
		return NO_DECODE_FOR_DIGITS_7_TO_0;
	}
	case DecodeForDigit0:
	{
		return DECODE_FOR_DIGIT_0;
	}
	case DecodeForDigits_3_to_0:
	{
		return DECODE_FOR_DIGIT_3_TO_0;
	}
	case DecodeFor7Digits_7_to_0:
	{
		return DECODE_FOR_DIGIT_7_TO_0;
	}
}
return NO_DECODE_FOR_DIGITS_7_TO_0;
}

MAX7219_STATUS MAX7219_SetDecodeMode(MAX7219_DecodeMode decode_mode){

	uint8_t mode = acctualDecodeModeVal(decode_mode);

	if(MAX7219_OK != MAX7219_SendData(MAX7219_DECODE_MODE_REG, mode))
	{
		return MAX7219_ERROR;
	}
	return MAX7219_OK;
}

MAX7219_STATUS MAX7219_SetOperationalMode(MAX7219_OperationalMode operational_mode){

	if(MAX7219_OK != MAX7219_SendData(MAX7219_SHUTDOWN_REG, operational_mode))
	{
		return MAX7219_ERROR;
	}
	return MAX7219_OK;
}

MAX7219_STATUS MAX7219_SetTestMode(MAX7219_TestMode test_mode){

	if(MAX7219_OK != MAX7219_SendData(MAX7219_DISPLAY_TEST_REG, test_mode))
	{
		return MAX7219_ERROR;
	}
	return MAX7219_OK;
}

MAX7219_STATUS MAX7219_SetIntensity(uint8_t intensivity){
	//  Intensity range 0x00 -> 0xXF
	if(MAX7219_OK != MAX7219_SendData(MAX7219_INTENSITY_REG, intensivity))
	{
			return MAX7219_ERROR;
	}
	return MAX7219_OK;
}

MAX7219_STATUS MAX7219_SetScanLimit(MAX7219_ScanLimit scanLimit){

	if(MAX7219_OK != MAX7219_SendData(MAX7219_SCAN_LIMIT_REG, scanLimit))
	{
			return MAX7219_ERROR;
	}
	return MAX7219_OK;
}

MAX7219_STATUS MAX7219_Init(SPI_HandleTypeDef *hspi)
{

	if(MAX7219_OK != MAX7219_SetDecodeMode(NoDecodeForDigits7_to_0))	return MAX7219_ERROR;
	if(MAX7219_OK != MAX7219_SetIntensity(1)) 							return MAX7219_ERROR;
	if(MAX7219_OK != MAX7219_SetScanLimit(ScanDigit0_7)) 				return MAX7219_ERROR;
	if(MAX7219_OK != MAX7219_SetTestMode(TestOff)) 						return MAX7219_ERROR;
	if(MAX7219_OK != MAX7219_SetOperationalMode(Operational_mode)) 		return MAX7219_ERROR;
	if(MAX7219_OK != MAX7219_SetAllDigits(MAX7219_OFF))					return MAX7219_ERROR;

	return MAX7219_OK;
}

MAX7219_STATUS MAX7219_SetPixel(int x, int y, MAX7219_digit_state state){

	if ((x < 0) || (x >= 8) || (y < 0) || (y >= 8))
			 return MAX7219_ERROR;
	switch(state)
	{
		case MAX7219_ON:   		Max7219PixelsBuffer[(MAX7219_PIXELS_PER_ROW_COLUMN - 1) - (y % 8)] |=  (1 << (x & 7)); break;
		case MAX7219_OFF:   	Max7219PixelsBuffer[(MAX7219_PIXELS_PER_ROW_COLUMN - 1) - (y % 8)] &= ~(1 << (x & 7)); break;
		case MAX7219_INVERSE: 	Max7219PixelsBuffer[(MAX7219_PIXELS_PER_ROW_COLUMN - 1) - (y % 8)] ^=  (1 << (x & 7)); break;


	default: return MAX7219_ERROR;
	}
	return MAX7219_OK;
}

MAX7219_STATUS MAX7219_SetAllDigits(MAX7219_digit_state state)
{
	switch (state)
	{
		case MAX7219_ON:
			memset(Max7219PixelsBuffer, 0xFF, MAX7219_PIXELS_PER_ROW_COLUMN);
			break;
		case MAX7219_OFF:
			memset(Max7219PixelsBuffer, 0x00, MAX7219_PIXELS_PER_ROW_COLUMN);
			break;
		default:
			return MAX7219_ERROR;
	}
	return MAX7219_OK;
}

MAX7219_STATUS MAX7219_Display(void)
{
	for(int i = 0; i < 8; i++)
	{
		if(MAX7219_SendData(MAX7219_DIGIT7_REG - i, Max7219PixelsBuffer[i]) != MAX7219_OK)
			return MAX7219_ERROR;
	}
	return MAX7219_OK;
}

MAX7219_STATUS MAX7219_SetSpectrum(int* spectrumTab, int numOfRanges)
{
	for(int i = 0; i < numOfRanges; i++)
	{
		for(int j = 0; j < spectrumTab[i]; j++)
		{
			if(MAX7219_SetPixel(i, j, MAX7219_ON) != MAX7219_OK)
				return MAX7219_ERROR;
		}
	}
	return MAX7219_OK;
}
