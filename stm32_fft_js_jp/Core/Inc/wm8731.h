/*
 * wm8731.h
 *
 *  Created on: Nov 9, 2023
 *      Author: jakub
 */

#ifndef INC_WM8731_H_
#define INC_WM8731_H_

#include "main.h"
#include "spi.h"
#include "i2c.h"
#include "stdlib.h"
#include "i2s.h"

#define  WM8731_ADDRESS            	0x34       // WM8731 chip address on I2C bus
#define  WM8731_REG_LLINE_IN      	0x00       // Left Channel Line Input Volume Control
#define  WM8731_REG_RLINE_IN        0x01       // Right Channel Line Input Volume Control
#define  WM8731_REG_LHPHONE_OUT     0x02       // Left Channel Headphone Output Volume Control
#define  WM8731_REG_RHPHONE_OUT     0x03       // Right Channel Headphone Output Volume Control
#define  WM8731_REG_ANALOG_PATH     0x04       // Analog Audio Path Control
#define  WM8731_REG_DIGITAL_PATH    0x05       // Digital Audio Path Control
#define  WM8731_REG_PDOWN_CTRL      0x06       // Power Down Control Register
#define  WM8731_REG_DIGITAL_IF      0x07       // Digital Audio Interface Format
#define  WM8731_REG_SAMPLING_CTRL   0x08       // Sampling Control Register
#define  WM8731_REG_ACTIVE_CTRL     0x09       // Active Control
#define  WM8731_REG_RESET           0x0F       // Reset register

#define _WM8731_left_lineIn        0b010010111  // LIN settings: Mute left line
#define _WM8731_Right_lineIn       0b010010111  // RIN settings: Mute right line
#define _WM8731_Left_hp            0b001111001  // Headphone settings : -9dB output
#define _WM8731_Right_hp           0b001111001  // Headphone settings : -9dB output
#define _WM8731_AnalogAudio        0b000100100  // Unmute microphone, enable side tone
#define _WM8731_DigitalAudio       0b000001110	// Default digital path settings
#define _WM8731_power              0b000000000  // Turn on all modules
//#define _WM8731_DAIF               0x4A    		// Enable Master Mode and select I2S interface 24 bit
#define _WM8731_DAIF               0b000000010  // Enable Master Mode and select I2S interface 16 bit
#define _WM8731_Sampling           0b000011100  // Sampling 96kHz
//#define _WM8731_Sampling           0b000000100  // Sampling 48kHz
#define _WM8731_Activate           0x01     	// Module is ON
#define _WM8731_Deactivate         0x00     	// Module is OFF
#define _WM8731_Reset              0x00      	// Reset value

typedef enum WM8731_STATUS{
	WM8731_OK,
	WM8731_ERROR
}WM8731_STATUS;

WM8731_STATUS WM8731_Init(I2C_HandleTypeDef* hi2c);
WM8731_STATUS WM8731_SendControl(uint8_t regAdress, uint16_t data);
WM8731_STATUS WM8731_Start(void);
WM8731_STATUS WM8731_Stop(void);
WM8731_STATUS WM8731_Init(I2C_HandleTypeDef* hi2c);
WM8731_STATUS WM8731_SendControl(uint8_t regAdress, uint16_t data);
WM8731_STATUS WM8731_Start(void);
WM8731_STATUS WM8731_Stop(void);
WM8731_STATUS WM8731_RecieveData(uint16_t* buffer);

#endif /* INC_WM8731_H_ */
