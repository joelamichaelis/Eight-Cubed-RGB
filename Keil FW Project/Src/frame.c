/*
 * frame.c
 *
 *  Created on: Dec 6, 2020
 *      Author: Joel Michaelis
 */
 
 #include "frame.h"
 
 /**
 * @brief sets a single led in frame, as indexed by lyrNum & ledNum, to the color specified by colorVal
 * @param[in] frame - a struct containing all eight lyrframes which index the entire cube
 * @param[in] lyrNum - a number between 0 (bottom layer) and 7 (top layer)
 * @param[in] colorVal - an RGB color in the format 0x00000RRR0GGG0BBB
 * @param[in] ledNum - a value between 0 & 63
 */
void frame_set_single_led_color(Frame_TypeDef *framePtr, uint8_t lyrNum, uint64_t colorVal, uint8_t ledNum)
{
	if(lyrNum == 0) lyr_frame_set_single_led_color((*framePtr).lyr0, colorVal, ledNum);
	else if(lyrNum == 1) lyr_frame_set_single_led_color((*framePtr).lyr0, colorVal, ledNum);
	else if(lyrNum == 2) lyr_frame_set_single_led_color((*framePtr).lyr0, colorVal, ledNum);
	else if(lyrNum == 3) lyr_frame_set_single_led_color((*framePtr).lyr0, colorVal, ledNum);
	else if(lyrNum == 4) lyr_frame_set_single_led_color((*framePtr).lyr0, colorVal, ledNum);
	else if(lyrNum == 5) lyr_frame_set_single_led_color((*framePtr).lyr0, colorVal, ledNum);
	else if(lyrNum == 6) lyr_frame_set_single_led_color((*framePtr).lyr0, colorVal, ledNum);
	else if(lyrNum == 7) lyr_frame_set_single_led_color((*framePtr).lyr0, colorVal, ledNum);
}
 

/**
 * @brief sets a single led in frame, as indexed by lyrNum & ledNum, to the color specified by colorVal
 * @param[in] frame - a struct containing all eight lyrframes which index the entire cube
 * @param[in] lyrNum - a number between 0 (bottom layer) and 7 (top layer)
 * @param[in] colorVal - an RGB color in the format 0x00000RRR0GGG0BBB
 * @param[in] maskPtr - points to the first element of the boolMask that specifies which LEDs to set
 */
void frame_set_mask_color(Frame_TypeDef *framePtr, uint8_t lyrNum, uint64_t colorVal, bool *maskPtr)
{
	if(lyrNum == 0) lyr_frame_set_mask_color((*framePtr).lyr0,colorVal,maskPtr);
	if(lyrNum == 1) lyr_frame_set_mask_color((*framePtr).lyr1,colorVal,maskPtr);
	if(lyrNum == 2) lyr_frame_set_mask_color((*framePtr).lyr2,colorVal,maskPtr);
	if(lyrNum == 3) lyr_frame_set_mask_color((*framePtr).lyr3,colorVal,maskPtr);
	if(lyrNum == 4) lyr_frame_set_mask_color((*framePtr).lyr4,colorVal,maskPtr);
	if(lyrNum == 5) lyr_frame_set_mask_color((*framePtr).lyr5,colorVal,maskPtr);
	if(lyrNum == 6) lyr_frame_set_mask_color((*framePtr).lyr6,colorVal,maskPtr);
	if(lyrNum == 7) lyr_frame_set_mask_color((*framePtr).lyr7,colorVal,maskPtr);
}
