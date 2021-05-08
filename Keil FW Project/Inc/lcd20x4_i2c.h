/*
 * lcd20x4_i2c.h
 *
 *  Created on: Mar 28, 2020
 *      Author: Mohamed Yaqoob
 */

//ifndef LCD20x4_I2C_H_
//#define LCD20x4_I2C_H_

#include "stm32f1xx_hal.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

/*
#include <stdint.h>
#include <stdbool.h>
#include "stm32f1xx_hal.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
*/

/* LCD Commands */
#define LCD_CLEARDISPLAY    0x01
#define LCD_RETURNHOME      0x02
#define LCD_ENTRYMODESET    0x04
#define LCD_DISPLAYCONTROL  0x08
#define LCD_CURSORSHIFT     0x10
#define LCD_FUNCTIONSET     0x20
#define LCD_SETCGRAMADDR    0x40
#define LCD_SETDDRAMADDR    0x80

/* Commands bitfields */
//1) Entry mode Bitfields
#define LCD_ENTRY_SH      0x01
#define LCD_ENTRY_ID      0x02
//2) Display control
#define LCD_DISPLAY_B     0x01
#define LCD_DISPLAY_C     0x02
#define LCD_DISPLAY_D     0x04
//3) Shift control
#define LCD_SHIFT_RL      0x04
#define LCD_SHIFT_SC      0x08
//4) Function set control
#define LCD_FUNCTION_F    0x04
#define LCD_FUNCTION_N    0x08
#define LCD_FUNCTION_DL   0x10

/* I2C Control bits */
#define LCD_RS        (1 << 0)
#define LCD_RW        (1 << 1)
#define LCD_EN        (1 << 2)
#define LCD_BK_LIGHT  (1 << 3)

/* Library variables */
static I2C_HandleTypeDef* lcd20x4_i2cHandle;
static uint8_t LCD_I2C_SLAVE_ADDRESS=0;
#define LCD_I2C_SLAVE_ADDRESS_0  0x4E
#define LCD_I2C_SLAVE_ADDRESS_1  0x7E

/* Private functions */
static void lcd20x4_i2c_sendCommand(uint8_t command);

static void lcd20x4_i2c_sendData(uint8_t data);

/**
 * @brief Initialise LCD20x4
 * @param[in] *pI2cHandle - pointer to HAL I2C handle
 */
bool lcd20x4_i2c_init(I2C_HandleTypeDef *pI2cHandle);

/**
 * @brief Set cursor position
 * @param[in] row - 0 - 3 (4 row LCD)
 * @param[in] col - 0 - 19 (20 column LCD)
 */
void lcd20x4_i2c_setCursor(uint8_t row, uint8_t col);

/**
 * @brief Move to beginning of 1st line
 */
void lcd20x4_i2c_1stLine(void);

/**
 * @brief Move to beginning of 2nd line
 */
void lcd20x4_i2c_2ndLine(void);

/**
 * @brief Move to beginning of 3rd line
 */
void lcd20x4_i2c_3rdLine(void);

/**
 * @brief Move to beginning of 4th line
 */
void lcd20x4_i2c_4thLine(void);

/**
 * @brief Select LCD Number of lines mode
 */
void lcd20x4_i2c_TwoLines(void);

/**
 * @brief Select LCD Number of lines mode
 */
void lcd20x4_i2c_OneLine(void);

/**
 * @brief Cursor ON/OFF
 */
void lcd20x4_i2c_cursorShow(bool state);

/**
 * @brief Display clear
 */
void lcd20x4_i2c_clear(void);

/**
 * @brief Display ON/OFF, to hide all characters, but not clear
 */
void lcd20x4_i2c_display(bool state);

/**
 * @brief Shift content to right
 */
void lcd20x4_i2c_shiftRight(uint8_t offset);

/**
 * @brief Shift content to left
 */
void lcd20x4_i2c_shiftLeft(uint8_t offset);

/**
 * @brief Print to display
 */
void lcd20x4_i2c_printf(const char* str, ...);


void lcd20x4_i2c_display(bool state);

/**
 * @brief Shift content to right
 */
void lcd20x4_i2c_shiftRight(uint8_t offset);

/**
 * @brief Shift content to left
 */
void lcd20x4_i2c_shiftLeft(uint8_t offset);

/**
 * @brief Print to display
 */
void lcd20x4_i2c_printf(const char* str, ...);

//#endif /* LCD20x4_I2C_H_ */
