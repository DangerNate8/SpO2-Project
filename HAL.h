/**
 * @file HAL.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-20
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <Arduino.h>

#ifndef HAL_H_
#define HAL_H_

//Pins for LED switching
#define LED_RED_PIN     7
#define LED_IR_PIN      6
#define INTR_PIN_1      2
#define INTR_PIN_2      3

//OLED display pins
#define SCREEN_ADDRESS 0x3C
#define OLED_RESET      1
#define LED_PIN 13

//MUX pins
#define CONTROL_A_PIN   9
#define CONTROL_B_PIN   8

//Input pins 
#define DC_RED_IN       A2
#define DC_IR_IN        A3
#define AC_RED_IN       A0
#define AC_IR_IN        A3

#define LED_FREQ        1000

void InitializePins();

void ISR_rising();
void ISR_falling();

#endif