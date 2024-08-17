/*
 * frame.h
 *
 *  Created on: Dec 6, 2020
 *      Author: Joel Michaelis
 */

#ifndef FRAME
#define FRAME

#include "lyrframe.h"

typedef struct __Frame_TypeDef
{
	LyrFrame_TypeDef lyr0;
	LyrFrame_TypeDef lyr1;
	LyrFrame_TypeDef lyr2;
	LyrFrame_TypeDef lyr3;
	LyrFrame_TypeDef lyr4;
	LyrFrame_TypeDef lyr5;
	LyrFrame_TypeDef lyr6;
	LyrFrame_TypeDef lyr7;
	LyrFrame_TypeDef *lyr0Ptr;
	LyrFrame_TypeDef *lyr1Ptr;
	LyrFrame_TypeDef *lyr2Ptr;
	LyrFrame_TypeDef *lyr3Ptr;
	LyrFrame_TypeDef *lyr4Ptr;
	LyrFrame_TypeDef *lyr5Ptr;
	LyrFrame_TypeDef *lyr6Ptr;
	LyrFrame_TypeDef *lyr7Ptr;
} Frame_TypeDef;


/**
 * @brief sets a single led in frame, as indexed by lyrNum & ledNum, to the color specified by colorVal
 * @param[in] frame - a struct containing all eight lyrframes which index the entire cube
 * @param[in] lyrNum - a number between 0 (bottom layer) and 7 (top layer)
 * @param[in] colorVal - an RGB color in the format 0x00000RRR0GGG0BBB
 * @param[in] ledNum - a value between 0 & 63
 */
void frame_set_single_led_color(Frame_TypeDef *framePtr, uint8_t lyrNum, uint64_t colorVal, uint8_t ledNum);


/**
 * @brief sets a single led in frame, as indexed by lyrNum & ledNum, to the color specified by colorVal
 * @param[in] frame - a struct containing all eight lyrframes which index the entire cube
 * @param[in] lyrNum - a number between 0 (bottom layer) and 7 (top layer)
 * @param[in] colorVal - an RGB color in the format 0x00000RRR0GGG0BBB
 * @param[in] maskPtr - points to the first element of the boolMask that specifies which LEDs to set
 */
void frame_set_mask_color(Frame_TypeDef *framePtr, uint8_t lyrNum, uint64_t colorVal, bool *maskPtr);


/*
//rotate functions wrap around unlike shifts
void frame_rotate_right(FrameTypeDef frame,uint8_t repetition);
void frame_rotate_left(FrameTypeDef frame,uint8_t repetition);
void frame_rotate_forward(FrameTypeDef frame,uint8_t repetition);
void frame_rotate_back(FrameTypeDef frame,uint8_t repetition);
void frame_rotate_up(FrameTypeDef frame,uint8_t repetition);
void frame_rotate_down(FrameTypeDef frame,uint8_t repetition);

//shift functions don't wrap around unlike rotates
void frame_shift_right(FrameTypeDef frame,uint8_t repetition);
void frame_shift_left(FrameTypeDef frame,uint8_t repetition);
void frame_shift_forward(FrameTypeDef frame,uint8_t repetition);
void frame_shift_back(FrameTypeDef frame,uint8_t repetition);
void frame_shift_up(FrameTypeDef frame,uint8_t repetition);
void frame_shift_down(FrameTypeDef frame,uint8_t repetition);


void frame_brighten_add(uint16_t brightnessDelta);
void frame_brighten_shift(uint16_t shiftAmount);
void frame_brighten_rotate(uint16_t rotateAmount);

void frame_dim_subtract(uint16_t brightnessDelta);
void frame_dim_shift(uint16_t shiftAmount);
void frame_dim_rotate(uint16_t rotateAmount);

void frame_set_red(uint16_t brightness, bool mask[64]);
void frame_set_grn(uint16_t brightness, bool mask[64]);
void frame_set_blu(uint16_t brightness, bool mask[64]);
*/

#endif
