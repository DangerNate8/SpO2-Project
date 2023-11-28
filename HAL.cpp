/**
 * @file HAL.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-20
 * 
 * @copyright Copyright (c) 2023
 * 
 */

/* Arduino Includes */
#include <Arduino.h>

/* HAL Includes */
#include <HAL.h>

void InitializePins(){
    pinMode(CONTROL_A_PIN, OUTPUT);
    pinMode(CONTROL_B_PIN,OUTPUT);
    pinMode(LED_IR_PIN, OUTPUT);
    pinMode(LED_RED_PIN, OUTPUT);

    pinMode(INTR_PIN_1, INPUT);
    pinMode(INTR_PIN_2, INPUT);

    pinMode(AC_IR_IN, INPUT);
    pinMode(DC_IR_IN, INPUT);
    pinMode(AC_RED_IN, INPUT);
    pinMode(DC_RED_IN, INPUT);

    attachInterrupt(digitalPinToInterrupt(2),ISR_falling, FALLING);
    attachInterrupt(digitalPinToInterrupt(3),ISR_rising, RISING);

    
}

void ISR_falling(){
    digitalWrite(CONTROL_A_PIN, HIGH);
    digitalWrite(CONTROL_B_PIN, LOW);
    digitalWrite(LED_IR_PIN, HIGH);
}

void ISR_rising(){
    digitalWrite(CONTROL_A_PIN, LOW);
    digitalWrite(CONTROL_B_PIN, HIGH);
    digitalWrite(LED_IR_PIN,LOW);
}