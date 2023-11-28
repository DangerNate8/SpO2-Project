#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Application.h>

#include <HAL.h>
#include <Graphics.h>
#include <Timer.h>
#include <Wire.h>
#include <SPI.h>


Adafruit_SSD1306 display(OLED_RESET);
//Adafruit_SSD1306 display(LCD_WIDTH, LCD_HEIGHT, &Wire, -1);

Application app;

// put function declarations here:

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  app = Application_construct();

  InitializePins();

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
      
  }

  displayTitleScreen(&display);
  delay(3000);
  displayData(&display, app.heartrate, app.spo2);

  tone(LED_RED_PIN, LED_FREQ);

  
}

void loop() {
  // put your main code here, to run repeatedly:
  Application_loop(&app);
}



void Application_loop(Application *app_p){
  
  if( SWTimer_expired(&app_p->ac_sample_timer)){
    app_p->red_ac_samples[app_p->ac_index] = analogRead(AC_RED_IN);
    app_p->time_stamps[app_p->ac_index] = millis();
    app_p->ir_ac_samples[app_p->ac_index] = analogRead(AC_IR_IN);
    app_p->ac_index += 1;
    SWTimer_start(&app_p->ac_sample_timer);
  }

  if(SWTimer_expired(&app_p->dc_sample_timer)){
    app_p->red_dc = analogRead(DC_RED_IN);
    app_p->ir_dc = analogRead(DC_IR_IN);
    SWTimer_start(&app_p->dc_sample_timer);
  }

  if(SWTimer_expired(&app_p->display_update_timer)){
    updateData(&display, app_p->heartrate, app_p->spo2);
  }

  if(app_p->ac_index == NUM_HIST_SAMP){
    CalcStuff(app_p);
  }

  
}

Application Application_construct(){
  Application app; 

  app.ac_sample_timer = SWTimer_construct(AC_WAIT);
  app.dc_sample_timer = SWTimer_construct(DC_WAIT);
  app.display_update_timer = SWTimer_construct(REFRESH_RATE);

  app.heartrate = 0;
  app.ir_max = 0;
  app.red_max = 0;
  app.ir_min = 0;
  app.red_min = 0;
  app.sample_index = 0;
  app.spo2 = 0;
  app.red_dc = 0;
  app.ir_dc = 0;
  app.ir_ac_rms = 0;
  app.red_ac_rms = 0;
  app.ac_index = 0;
  
  memset(app.red_ac_samples, 0, NUM_HIST_SAMP * sizeof(byte));
  memset(app.ir_ac_samples, 0, NUM_HIST_SAMP * sizeof(byte));

  return app;

}

void CalcStuff(Application *app_p){
  int i;

  //loop finds voltage min and max for both ir and red signals
  for(i = 0; i< app_p->ac_index; i++){
    if(app_p->ir_max < app_p->ir_ac_samples[i]){
      app_p->ir_max = app_p->ir_ac_samples[i];
    }
    if(app_p->red_max < app_p->red_ac_samples[i]){
      app_p->red_max = app_p->red_ac_samples[i];
    }
    if(app_p->ir_min > app_p->ir_ac_samples[i]){
      app_p->ir_min = app_p->ir_ac_samples[i];
    }
    if(app_p->red_min > app_p->red_ac_samples[i]){
      app_p->red_min = app_p->red_ac_samples[i];
    }
  }

  byte marker =  app_p->red_ac_samples[5];
  for(i = 0; i < app_p->ac_index; i++){
    


  }



  //calculate RMS using forumula
  uint32_t sum_red = 0;
  uint32_t sum_ir = 0;
  for (int i = 0; i < NUM_HIST_SAMP; i++)
  {
    sum_red += (uint32_t)pow((uint32_t)app_p->red_ac_samples[i], 2);
    sum_ir += (uint32_t)pow((uint32_t)app_p->ir_ac_samples[i], 2);
  }
  app_p->red_ac_rms = sqrt(sum_red / NUM_HIST_SAMP);
  app_p->ir_ac_rms = sqrt(sum_ir / NUM_HIST_SAMP);

  double R = (app_p->red_ac_rms / app_p->red_dc) / (app_p->ir_ac_rms / app_p->ir_dc);

  app_p->spo2 = 110.0 - 25.0 * R;
  if (app_p->spo2 > 100)
  {
    app_p->spo2 = 100;
  }


}