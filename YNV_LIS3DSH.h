/*
 * YNV_LIS3DSH.h
 *
 *  Created on: Jan 12, 2021
 *      Author: moise
 */

#ifndef INC_YNV_LIS3DSH_H_
#define INC_YNV_LIS3DSH_H_

#include <stm32f4xx_hal.h>


/* Device address define */
#define STATUS				0x27
#define OUT_X_L				0x28
#define OUT_X_H				0x29
#define OUT_Y_L				0x2A
#define OUT_Y_H				0x2B

/* Registers address */
#define CTRL_REG4 			0x20
#define CTRL_REG5			0x24

/* Configuration register REG4 */
#define LIS3DSH_OFF			((uint8_t) 0x00)
#define LIS3DSH_ON			((uint8_t) 0x60)

#define LIS3DSH_XY			((uint8_t) 0x03)
#define LIS3DSH_X			((uint8_t) 0x01)
#define LIS3DSH_Y			((uint8_t) 0x02)

/* Configuration register REG5 */
#define LIS3DSH_SCALE_2G	((uint8_t) 0x00)
#define LIS3DSH_SCALE_4G	((uint8_t) 0x08)
#define LIS3DSH_SCALE_6G	((uint8_t) 0x10)
#define LIS3DSH_SCALE_8G	((uint8_t) 0x18)
#define LIS3DSH_SCALE_16G	((uint8_t) 0x20)

#define CS_LOW			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET)
#define CS_HIGH			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET)


/* Return status enum */
typedef enum {
	LIS3DSH_OK,
	LIS3DSH_ERROR,
}LIS3DSH_Status;

/* Init struct */
typedef struct{
	uint8_t power;
	uint8_t axe;
	uint8_t scale;
}LIS3DSH_Init;

/* Stockage result LIS3DSH */
typedef struct{
	int16_t resultX;
	int16_t resultY;
}LIS3DSH_Result;

/* Write register */
LIS3DSH_Status LIS3DSH_Write_reg(SPI_HandleTypeDef *hspi,
		uint8_t reg_addr,
		uint8_t *dataW,
		uint8_t size);

/* Read register */
LIS3DSH_Status LIS3DSH_Read_reg(SPI_HandleTypeDef *hspi,
		uint8_t reg_addr,
		uint8_t *dataR,
		uint8_t size);

/* Init LIS3DSH */
LIS3DSH_Status LIS3DSH_Init_t(SPI_HandleTypeDef *hspi,
		LIS3DSH_Init *posInitDef);

/* Get position */
LIS3DSH_Status LIS3DSH_Get_Pos(SPI_HandleTypeDef *hspi,
		LIS3DSH_Result* structResult);



#endif /* INC_YNV_LIS3DSH_H_ */
