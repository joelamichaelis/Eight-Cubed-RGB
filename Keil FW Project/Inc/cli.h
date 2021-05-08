/*
 * cli.h
 *
 *  Initial creation: Apr 24, 2021
 *      Author: Joel Michaelis
 */
 
 #ifndef LCD20x4_I2C_H_
#define LCD20x4_I2C_H_

#define CLI_FW_VERS 0.0.0.1

#define MAX_BRIGHTNESS 4095
#define MAX_LYR_NUM 7
#define MAX_LED_NUM 63
#define MAX_COLOR_NUM 0x00000fff0fff0fff

#define MAX_CLI_ARG_LEN 10
#define MAX_CLI_CMD_LEN 20

#define DEF_STR_LEN 32 // default string length
#define RAW_CLI_DATA_STR_LEN 100 // default string length

#include "frame.h"
#include "bool_mask.h"

enum response{Null, OK, InvalidCommand, Error};  

typedef struct __cliData_TypeDef
{	
	uint8_t rawTextInput[RAW_CLI_DATA_STR_LEN];
	uint8_t numberOfArgs;
	enum response responseEnum;
	uint8_t responseStr[DEF_STR_LEN];
	uint8_t responseLen;
	char command[MAX_CLI_CMD_LEN];
	char arg1[MAX_CLI_ARG_LEN];
	char arg2[MAX_CLI_ARG_LEN];
	char arg3[MAX_CLI_ARG_LEN];
	char arg4[MAX_CLI_ARG_LEN];
	char arg5[MAX_CLI_ARG_LEN];
	char arg6[MAX_CLI_ARG_LEN];
	char arg7[MAX_CLI_ARG_LEN];
	char arg8[MAX_CLI_ARG_LEN];
	char arg9[MAX_CLI_ARG_LEN];
	char arg10[MAX_CLI_ARG_LEN];
} cliData_TypeDef;

//------------------------public functions begin---------------------------------------//
/**
 * @brief parses the rawTextInput from the cli and updates the cliData struct accordingly
 * @param[in] cliDataPtr: points to the cliData structure
 */
void cli_parse_data(cliData_TypeDef *cliDataPtr);

/**
 * @brief interprets the received cli command contained in cliData and modifies frame accordingly
 * @param[in] cliDataPtr - points to cliData struct which contains received command line interface commands, etc.
 * @param[in] frame - the struct which represets the LED cube and it's corresponding animation "frame" brightness values
 */
void cli_interpret_data(cliData_TypeDef *cliDataPtr, Frame_TypeDef *framePtr);

/**
 * @brief formulates a response message to and populates the cliData.responseStr accordingly
 * @param[in] cliDataPtr - points to cliData struct which contains received command line interface commands, etc.
 */
void cli_formulate_response(cliData_TypeDef *cliDataPtr);

/**
 * @brief clears the cliData struct pointed to by cliDataPtr so it's ready to use for the next command
 * @param[in] cliDataPtr: points to the cliData structure
 */
void cli_data_cleanse(cliData_TypeDef *cliDataPtr);
//------------------------public functions end-----------------------------------------//

#endif /* LCD20x4_I2C_H_ */
