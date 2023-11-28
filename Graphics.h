/**
 * @file Graphics.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-20
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <Arduino.h>
#include <Adafruit_SSD1306.h>

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#define OLED_RESET 1
#define SCREEN_ADDRESS 0x3C

#define LCD_WIDTH 128         // width of screen
#define LCD_HEIGHT 64         // height of screen

void displayTitleScreen(Adafruit_SSD1306 *oled);

void displayData(Adafruit_SSD1306 *oled, double bpm, int Sp02);

void updateData(Adafruit_SSD1306 *oled, double bpm, int Sp02);

#endif