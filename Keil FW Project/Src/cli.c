/*
 * cli.c
 *
 *  Initial creation: Apr 24, 2021
 *      Author: Joel Michaelis
 */
 
 /*
 FW Revisioning Convention:
 v0.0.0.X daily: versions of library. It's OK if these FW versions are buggy or uncompilable.
 v0.0.X.0 prerelease: versions of library when functionality has been updated but yet to be fully validated. These builds must be compilable and ideally minimally tested.
 v0.X.0.0 subrelease: stable builds that contain new functionality
 vX.0.0.0 release: (also project indicator) 0 means initial development. Will be incremented once initial development is finalized.
 
 examples:
 v1.1.0.0 - release 1.1 
 v1.1.0.1 - daily build (perhaps a bug fix) after release 1.1
 v1.1.1.0 - prerelease build post release 1.1 and prior to release 1.2 
 v2.0.0.0 - new "separate" codebase from v1.x.x.x due to major updates outside scope of release 1
			release 1 & release 2 may exist in parallel while release 1.1 supercedes release 1.0
 */

#include "cli.h"

/**
 * @brief parses the rawTextInput from the cli and updates the cliData struct accordingly
 * @param[in] cliDataPtr: points to the cliData structure
 */
void cli_parse_data(cliData_TypeDef *cliDataPtr)
{
	uint8_t tempIndex = 0;
	uint8_t strIndex = 0;
	uint8_t argCount = 0;
	
	if((*cliDataPtr).rawTextInput[0] == 0x00) return;
	
	while(tempIndex<RAW_CLI_DATA_STR_LEN && ((*cliDataPtr).rawTextInput[tempIndex] != '\r') && ((*cliDataPtr).rawTextInput[tempIndex] != '\n') && ((*cliDataPtr).rawTextInput[tempIndex] != 0x00))
	{				
		if((*cliDataPtr).rawTextInput[tempIndex] == ' ')
		{
			tempIndex++;
			argCount++;
			strIndex = 0;
		}
		
		if (argCount == 0) (*cliDataPtr).command[strIndex] = (*cliDataPtr).rawTextInput[tempIndex];		
		else if (argCount == 1) (*cliDataPtr).arg1[strIndex] = (*cliDataPtr).rawTextInput[tempIndex];
		else if (argCount == 2) (*cliDataPtr).arg2[strIndex] = (*cliDataPtr).rawTextInput[tempIndex];
		else if (argCount == 3) (*cliDataPtr).arg3[strIndex] = (*cliDataPtr).rawTextInput[tempIndex];
		else if (argCount == 4) (*cliDataPtr).arg4[strIndex] = (*cliDataPtr).rawTextInput[tempIndex];
		else if (argCount == 5) (*cliDataPtr).arg5[strIndex] = (*cliDataPtr).rawTextInput[tempIndex];
		else if (argCount == 6) (*cliDataPtr).arg6[strIndex] = (*cliDataPtr).rawTextInput[tempIndex];
		else if (argCount == 7) (*cliDataPtr).arg7[strIndex] = (*cliDataPtr).rawTextInput[tempIndex];
		else if (argCount == 8) (*cliDataPtr).arg8[strIndex] = (*cliDataPtr).rawTextInput[tempIndex];
		else if (argCount == 9) (*cliDataPtr).arg9[strIndex] = (*cliDataPtr).rawTextInput[tempIndex];
		else if (argCount == 10) (*cliDataPtr).arg10[strIndex] = (*cliDataPtr).rawTextInput[tempIndex];
		tempIndex++;
		strIndex++;
	}
	
	(*cliDataPtr).numberOfArgs = argCount;
}

/**
 * @brief clears the cliData struct pointed to by cliDataPtr so it's ready to use for the next command
 * @param[in] cliDataPtr: points to the cliData structure
 */
void cli_data_cleanse(cliData_TypeDef *cliDataPtr)
{
	uint8_t tempIndex = 0;
	
	while(tempIndex<RAW_CLI_DATA_STR_LEN)
	{
		(*cliDataPtr).rawTextInput[tempIndex] = 0x00;
		
		if(tempIndex<DEF_STR_LEN) (*cliDataPtr).responseStr[tempIndex] = 0x00;
		
		if(tempIndex<MAX_CLI_CMD_LEN) (*cliDataPtr).command[tempIndex] = 0x00;
		
		if(tempIndex<MAX_CLI_ARG_LEN)
		{
			(*cliDataPtr).arg1[tempIndex] = 0x00;
			(*cliDataPtr).arg2[tempIndex] = 0x00;
			(*cliDataPtr).arg3[tempIndex] = 0x00;
			(*cliDataPtr).arg4[tempIndex] = 0x00;
			(*cliDataPtr).arg5[tempIndex] = 0x00;
			(*cliDataPtr).arg6[tempIndex] = 0x00;
			(*cliDataPtr).arg7[tempIndex] = 0x00;
			(*cliDataPtr).arg8[tempIndex] = 0x00;
			(*cliDataPtr).arg9[tempIndex] = 0x00;
			(*cliDataPtr).arg10[tempIndex] = 0x00;
		}
		tempIndex++;
	}
	
	(*cliDataPtr).numberOfArgs = 0x00;
	(*cliDataPtr).responseEnum = Null;
	(*cliDataPtr).responseLen = 0;
	
}

/**
 * @brief checks if strVar is the same as strConst
 * @param[in] strVar: the variable string
 * @param[in] strConst: the constant string to compare the variable one against
 * @return true means the strings are equal, false means the opposite 
 */
bool cli_strcmp(char strVar[DEF_STR_LEN], char strConst[DEF_STR_LEN])
{
	char *strVarPtr = &strVar[0];
	char *strConstPtr = &strConst[0];
	bool areStringsEqual = 0;
	uint8_t strCmpIndex = 0;
	
	while((*(strVarPtr + strCmpIndex) != '\0') && (*(strConstPtr + strCmpIndex) != '\0'))
	{
		if(*(strVarPtr + strCmpIndex) == *(strConstPtr + strCmpIndex)) strCmpIndex++;
		else return areStringsEqual;
	}
	
	if((*(strVarPtr + strCmpIndex) == '\0') && (*(strConstPtr + strCmpIndex) == '\0')) areStringsEqual = true;
		
	return areStringsEqual;
}


/**
 * @brief converts the input string into an integer and returns it.
					currently only positive values supported
 * @param[in] tempStr: the input string to be converted 
 * @return the resulting integer  
 */
uint16_t cli_str2int(char tempStr[DEF_STR_LEN])
{
	char *tempStrPtr = &tempStr[0];
	uint8_t tempIndex = 0;
	uint16_t tempInt = 0;
	
	while(*(tempStrPtr + tempIndex) != 0x00)
	{
		tempInt = tempInt*10 + *(tempStrPtr + tempIndex) - '0';
		tempIndex++;
	}
	return tempInt;
}


/**
 * @brief converts the input individual RGB values to the format 0x00000RRR0GGG0BBB
 * @param[in] redVal - applies eqaully for grn and blu. Should be a value between 0 & 4095.
 * @return the resulting formatted color
 */
uint64_t cli_rgb2color(uint64_t redVal,uint64_t grnVal,uint64_t bluVal)
{
	uint64_t tempColor;
	tempColor |= redVal;
	tempColor = tempColor << 16;
	tempColor |= bluVal;
	tempColor = tempColor << 16;
	tempColor |= grnVal;
	return tempColor;
}

void cli_color_definition(char colorName[DEF_STR_LEN], uint64_t colorVal)
{
	// write colorName and associated colorVal to defined_colors.txt
	// yet to be implemented
}

/**
 * @brief copies responseMsg to derecferenced responsePtr
 * @param[in] responsePtr - points to the first element of the response message
 * @param[in] responseMsg - the message
 */
void cli_strcpy(uint8_t *responsePtr,char responseMsg[DEF_STR_LEN])
{
	uint8_t responseIndex = 0;
	while(responseIndex<DEF_STR_LEN && responseMsg[responseIndex] != 0x00)
	{
		*(responsePtr + responseIndex) = responseMsg[responseIndex];
		responseIndex++;
	}
}

/**
 * @brief formulates a response message to and populates the cliData.responseStr accordingly
 * @param[in] cliDataPtr - points to cliData struct which contains received command line interface commands, etc.
 */
void cli_formulate_response(cliData_TypeDef *cliDataPtr)
{
	if((*cliDataPtr).responseEnum == Null) return;
	else if((*cliDataPtr).responseEnum == OK) 
	{
		cli_strcpy(&(*cliDataPtr).responseStr[0],"OK\r\n");
		(*cliDataPtr).responseLen = 4;
	}
	
	else if ((*cliDataPtr).responseEnum == ArgError) 
	{
		cli_strcpy(&(*cliDataPtr).responseStr[0],"Argument Error\r\n");
		(*cliDataPtr).responseLen = 16;
	}
	
	else if ((*cliDataPtr).responseEnum == InvalidCommand) 
	{
		cli_strcpy(&(*cliDataPtr).responseStr[0],"Invalid Command\r\n");
		(*cliDataPtr).responseLen = 17;
	}
}


/**
 * @brief interprets the received cli command contained in cliData and modifies frame accordingly
 * @param[in] cliDataPtr - points to cliData struct which contains received command line interface commands, etc.
 * @param[in] frame - the struct which represets the LED cube and it's corresponding animation "frame" brightness values
 * set_single_led_rgb lednum lyrnum redval grnval blueval
 */
void cli_interpret_data(cliData_TypeDef *cliDataPtr, Frame_TypeDef *framePtr)
{	
	if(cli_strcmp((*cliDataPtr).command,"set_single_led_rgb") == true)
	{
		if((*cliDataPtr).numberOfArgs != 5)
		{
			(*cliDataPtr).responseEnum = ArgError;
			return;
		}	
		uint8_t ledNum = cli_str2int((*cliDataPtr).arg1);
		uint8_t lyrNum = cli_str2int((*cliDataPtr).arg2);			
		uint16_t redVal = cli_str2int((*cliDataPtr).arg3);
		uint16_t grnVal = cli_str2int((*cliDataPtr).arg4);
		uint16_t bluVal = cli_str2int((*cliDataPtr).arg5);
		//cli_value_check(MAX_LED_NUM,MAX_LYR_NUM,MAX_BRIGHTNESS,MAX_BRIGHTNESS,MAX_BRIGHTNESS); //if values are exceeded, reports an arg value error.
		uint64_t colorVal = cli_rgb2color(redVal,grnVal,bluVal);
		frame_set_single_led_color(framePtr,lyrNum,colorVal,ledNum);
		UPDATE_FRAME = true;
		
		(*cliDataPtr).responseEnum = OK;
	}
	
	else if(cli_strcmp((*cliDataPtr).command,"set_single_led_color") == true)
	{
		if((*cliDataPtr).numberOfArgs != 3)
		{
			(*cliDataPtr).responseEnum = ArgError;
			return;
		}	
		uint8_t ledNum = cli_str2int((*cliDataPtr).arg1);
		uint8_t lyrNum = cli_str2int((*cliDataPtr).arg2);
		uint64_t colorVal = cli_str2int((*cliDataPtr).arg3); //this is definitely wrong
		//char colorName[DEF_STR_LEN] = (*cliDataPtr).arg3;
		//cli_value_check(MAX_LED_NUM,MAX_LYR_NUM,MAX_COLOR_NUM);
		frame_set_single_led_color(framePtr,lyrNum,colorVal,ledNum);
		(*cliDataPtr).responseEnum = OK;
	}
		
	else if(cli_strcmp((*cliDataPtr).command,"color_definition") == true)
	{
		//color_definition colorname redval grnval bluval
		if((*cliDataPtr).numberOfArgs != 4)
		{
			(*cliDataPtr).responseEnum = ArgError;
			return;
		}	
		uint8_t redVal = cli_str2int((*cliDataPtr).arg2);
		uint8_t grnVal = cli_str2int((*cliDataPtr).arg3);
		uint8_t bluVal = cli_str2int((*cliDataPtr).arg4);
		uint64_t colorVal = cli_rgb2color(redVal,grnVal,bluVal);
		cli_color_definition((*cliDataPtr).arg1,colorVal); // yet to be implemented
		(*cliDataPtr).responseEnum = OK;
	}
		
	else if(cli_strcmp((*cliDataPtr).command,"color_definition?") == true)
	{
		//color_definition?
		if((*cliDataPtr).numberOfArgs != 0)
		{
			(*cliDataPtr).responseEnum = ArgError;
			return;
		}	
		//list color definitions
		(*cliDataPtr).responseEnum = OK;
	}	
		
	else if(cli_strcmp((*cliDataPtr).command,"set_lyr_color") == true)
	{
		//set_lyr_color lyrnum color boolmaskenc
		
		if((*cliDataPtr).numberOfArgs != 3)
		{
			(*cliDataPtr).responseEnum = ArgError;
			return;
		}	
		uint8_t lyrNum = cli_str2int((*cliDataPtr).arg1);
		uint64_t color = cli_str2int((*cliDataPtr).arg2);
		uint64_t boolMaskEnc = cli_str2int((*cliDataPtr).arg3);
		
		bool maskTemp[64];
		bool_mask_decode(&maskTemp[0],boolMaskEnc);
		
		frame_set_mask_color(framePtr,lyrNum,color,maskTemp);
		(*cliDataPtr).responseEnum = OK;
	}
			
	else if(cli_strcmp((*cliDataPtr).command,"set_lyr_rgb") == true)
	{
		//set_lyr_rgb lyrnum boolmask redval grnval bluval
		if((*cliDataPtr).numberOfArgs != 5)
		{
			(*cliDataPtr).responseEnum = ArgError;
			return;
		}	
		uint8_t lyrNum = cli_str2int((*cliDataPtr).arg1);
		uint64_t boolMask = cli_str2int((*cliDataPtr).arg2);
		//uint8_t redVal;
		//what am I doing here?
		(*cliDataPtr).responseEnum = OK;
	}
	
	else if(cli_strcmp((*cliDataPtr).command,"") != true)
	{
		(*cliDataPtr).responseEnum = InvalidCommand;
	}
	
}


/*
//command list
set_single_led_rgb lednum lyrnum redval grnval blueval
set_single_led_color lednum lyrnum color

color_definition colorname redval grnval bluval
color_definition?

boolmask_definition maskname value
boolmask_definition?

set_lyr_color lyrnum boolmask color
set_lyr_rgb lyrnum boolmask redval grnval bluval
*/

/*
command: set_single_led_rgb
syntax: set_single_led_rgb lednum lyrnum redval grnval blueval
lednum: a value between 0 & 63
lyrnum: a value between 0 & 7
redval: a value between 0 & 4095
grnval: a value between 0 & 4095
bluval: a value between 0 & 4095
ex: set_single_led_rgb 63 0 4095 4095 4095 // sets led 63 on lyr 0 to white
ex: set_single_led_rgb 0 7 0 4095 0 // sets led 0 on lyr 7 to green full brightness
ex: set_single_led_rgb 45 3 0 0 2047 // sets led 45 on lyr 3 to blue half brightness
*/

/*
command: set_single_led_color
syntax: set_single_led_color lednum lyrnum color
ex: set_single_led_color 17 2 white
*/

/*
command: color_definition
syntax: color_definition colorname redval grnval bluval
colorname: a color's name
redval: a value between 0 & 4095
grnval: a value between 0 & 4095
bluval: a value between 0 & 4095
*/

/*
command: color_definition?
purpose: lists defined colors
syntax: color_definition?
*/

/*
command: boolmask_definition
syntax: boolmask_definition maskname value
maskname: the mask's name
value: the boolmask's 64-bit mask 
ex: boolmask_definition LetterJ 0xFF0404040484CC78
*/

/* 
command: boolmask_definition?
purpose: lists defined boolmasks
syntax: boolmask_definition?
*/

/*
command: set_lyr_color
syntax: set_lyr_color lyrnum boolmask color
lyrnum: value between 0 & 7
boolmask: either a 64-bit value or predefined boolmask
color: a predefined color 
ex: set_lyr_color 0 LetterJ orange
ex: set_lyr_color 6 0xffffffffffffffff purple
*/

/*
command: set_lyr_rgb
syntax: set_lyr_rgb lyrnum boolmask redval grnval bluval
boolmask: either a 64-bit value or predefined boolmask
lyrnum: value between 0 & 7
redval: either a value between 0 & 4095 or "NA"
grnval: either a value between 0 & 4095 or "NA"
bluval: either a value between 0 & 4095 or "NA"
ex: set_lyr_rgb 0 LetterJ 4095 0000 4095
ex: set_lyr_rgb 6 0xffffffffffffffff purple
*/


/*
//future commands: shifts, rotates, copy, paste, ....
brightness_saturation 2047 //sets max brightness to half of HW maximum. any values above this will saturate
brightness_saturation? //queries max brightness
*/
