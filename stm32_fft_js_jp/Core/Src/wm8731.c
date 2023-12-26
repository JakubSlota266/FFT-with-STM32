/*
 * wm8731.c
 *
 *  Created on: Nov 9, 2023
 *      Author: jakub
 */
#include "wm8731.h"

WM8731_STATUS WM8731_SendControl(uint8_t regAdress, uint16_t data)
{
//	uint8_t dataToSend;
//	dataToSend = data & 0x00FF;
//
//	regAdress = regAdress << 1;
//	regAdress = regAdress | ((data>>8) & 1);
//
//	if(HAL_I2C_Mem_Write(&hi2c1, WM8731_ADDRESS, regAdress, 1U, (uint8_t*)&dataToSend, 1U, HAL_MAX_DELAY) != HAL_OK)
//		return WM8731_ERROR;
//
	uint8_t dataToSend[2] = {0U};
	dataToSend[1] = (uint8_t)(data & 0x00FF);

	regAdress = regAdress << 1;
	regAdress = regAdress | ((data>>8) & 1);

	dataToSend[0] = regAdress;

	if(HAL_I2C_Master_Transmit(&hi2c1, WM8731_ADDRESS, dataToSend, 2, HAL_MAX_DELAY) != HAL_OK)
		return WM8731_OK;

	return WM8731_OK;
}

WM8731_STATUS WM8731_Init(I2C_HandleTypeDef* hi2c)
{
	if(WM8731_SendControl(WM8731_REG_LLINE_IN, _WM8731_left_lineIn) != WM8731_OK)
		return WM8731_ERROR;
	if(WM8731_SendControl(WM8731_REG_RLINE_IN, _WM8731_Right_lineIn)!= WM8731_OK)
		return WM8731_ERROR;
	if(WM8731_SendControl(WM8731_REG_LHPHONE_OUT, _WM8731_Left_hp) != WM8731_OK)
		return WM8731_ERROR;
	if(WM8731_SendControl(WM8731_REG_RHPHONE_OUT, _WM8731_Right_hp) != WM8731_OK)
		return WM8731_ERROR;
	if(WM8731_SendControl(WM8731_REG_ANALOG_PATH, _WM8731_AnalogAudio) != WM8731_OK)
		return WM8731_ERROR;
	if(WM8731_SendControl(WM8731_REG_DIGITAL_PATH, _WM8731_DigitalAudio) != WM8731_OK)
		return WM8731_ERROR;
	if(WM8731_SendControl(WM8731_REG_PDOWN_CTRL, _WM8731_power) != WM8731_OK)
		return WM8731_ERROR;
	if(WM8731_SendControl(WM8731_REG_DIGITAL_IF, _WM8731_DAIF) != WM8731_OK)
		return WM8731_ERROR;
	if(WM8731_SendControl(WM8731_REG_SAMPLING_CTRL, _WM8731_Sampling) != WM8731_OK)
		return WM8731_ERROR;

	return WM8731_OK;
}

WM8731_STATUS WM8731_Start(void)
{
	WM8731_STATUS retval = WM8731_OK;
	if(WM8731_SendControl(WM8731_REG_ACTIVE_CTRL, _WM8731_Activate) != WM8731_OK)
		retval = WM8731_ERROR;
	return retval;
}


WM8731_STATUS WM8731_Stop(void)
{
	WM8731_STATUS retval = WM8731_OK;
	if(WM8731_SendControl(WM8731_REG_RESET, _WM8731_Reset) != WM8731_OK)
		retval = WM8731_ERROR;
	return retval;
}

WM8731_STATUS WM8731_RecieveData(uint16_t* buffer)
{
	 if(HAL_I2S_Receive(&hi2s1, buffer, 1, HAL_MAX_DELAY) != HAL_OK)
	 {
		  return WM8731_ERROR;
	 }

	 return WM8731_OK;
}
