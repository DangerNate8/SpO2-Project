/**
 * @file Graphics.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-20
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <Graphics.h>

// Function to display title screen
void displayTitleScreen(Adafruit_SSD1306 *oled) {
  oled->clearDisplay();
  oled->setTextSize(0.5);
  oled->setTextColor(WHITE);

  // First line for "SpO2 Sensor Project" with underline
  oled->setCursor(7, 0);
  oled->print("SpO2 Sensor Project");
  oled->setCursor(0, 3);
  oled->print("_____________________");

  // Second line for "Bradley Frye & Nathan Skirvin"
  oled->setCursor(15, 12);
  oled->print("Bradley Frye &");
  oled->setCursor(15, 20);
  oled->print("Nathan Skirvin");

  oled->display();
}

void displayData(Adafruit_SSD1306 *oled, double bpm, int Sp02){
    oled->clearDisplay();
    oled->setTextSize(1);
    oled->setTextColor(WHITE);

    oled->setCursor(0, 9);
    oled->print("BPM: ");
    oled->print(bpm);

    oled->setCursor(0,17);
    oled->print("Sp02");
    oled->print(Sp02);
    oled->print(" %");

    oled->display();
}

void updateData(Adafruit_SSD1306 *oled, double bpm, int Sp02){
    oled->fillRect(9, 9, 9, 9, BLACK);
    oled->fillRect(17, 17, 17,17, BLACK);
    oled->display();

    oled->setCursor(12, 9);
    oled->print(bpm);

    oled->setCursor(13,17);
    oled->print(Sp02);
    
    oled->display();

}