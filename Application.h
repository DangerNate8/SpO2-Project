/**
 * @file Application.h
 * @author Bradley Frye 
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
#include <Graphics.h>
#include <Timer.h>

//Macros 

#ifndef APPLICATION_H_
#define APPLICATION_H_

#define SAMPLE_RATE 32   // samples per second
#define NUM_HIST_SAMP 64

#define US_IN_SECONDS 1000000 // microseconds in a second

#define AC_WAIT US_IN_SECONDS / SAMPLE_RATE // time between samples (in us)
#define DC_WAIT 50                         // time after a state change to take a DC sample (in us)
#define LED_FREQ 1000                       // Frequency that the LED circuit switches polarity (Hz)
#define REFRESH_RATE 1000000                // Frequency that new information is sent to display (us)


struct _Application
{
    // Holds the variables used in main.cpp
    // =========================================================================

    // timers
     SWTimer ac_sample_timer;
     SWTimer dc_sample_timer;
     SWTimer display_update_timer;
     SWTimer watchdog_timer;

    uint32_t sample_index; // keeps track of the current sample's index in the sample arrays

    // ac component of the signal
    double red_ac_rms;
    double ir_ac_rms;
    byte red_max;
    byte ir_max;
    byte red_min;
    byte ir_min;

    int ac_index;
    int dc_index;
    // dc component of the signal
    double red_dc;
    double ir_dc;

    // final values for BPM and SpO2
    word heartrate;
    double spo2;

    // sample arrays
    byte red_ac_samples[NUM_HIST_SAMP]; // Historical red AC samples
    byte ir_ac_samples[NUM_HIST_SAMP]; // Historical inf AC samples
    unsigned long time_stamps[NUM_HIST_SAMP];
};
typedef struct _Application Application;

/** Constructs the application construct */
Application Application_construct();

void CalcStuff(Application *app_p);
/** Primary loop */
void Application_loop(Application *app_p);

void ISR_rising();
void ISR_falling();

#endif