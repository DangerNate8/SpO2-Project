/////////////////////////////////////////////////////////////////////////////////
// Authors: Bradley Fyre & Nathaniel Skirvin
// Revision 5: November 14, 2023
// Description: Reads Red and IR signals from circuit and calculates
//              the heart rate and SpO2 %. Then, display our results to
//              the OLED screen.
/////////////////////////////////////////////////////////////////////////////////
#include <Adafruit_SSD1306.h>

#define OLED_RESET 1
#define SCREEN_ADDRESS 0x3C
#define CONTROL_PINA 9
#define CONTROL_PINB 8
#define INHIBIT 10
#define RED 7
#define INFRARED 6
#define ACRed A1
#define ACIR A2
#define DCRed 3
#define DCIR 2

Adafruit_SSD1306 display(OLED_RESET);

int count = 0;

unsigned long lastPeakTime = 0;

void setup() {
  pinMode(INHIBIT, OUTPUT);
  pinMode(CONTROL_PINA, OUTPUT);
  pinMode(CONTROL_PINB, OUTPUT);
  pinMode(INFRARED, OUTPUT);
  pinMode(RED, OUTPUT);

  Serial.begin(115200);  // Initialize serial communication, this value for speed and reliability

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }

  display.clearDisplay();  // Clears OLED screen
  displayTitleScreen();
  delay(5000);
}

void loop() {
  // Need to write function for MUX
  int valueRed = analogRead(ACRed);  // Reads A1
  int valueIR = analogRead(ACIR);    // Reads A2
  int valuered = digitalRead(DCRed);
  int valueir = digitalRead(DCIR);

  if (count % 2 == 0) {
    digitalWrite(INHIBIT, LOW);
    digitalWrite(CONTROL_PINA, LOW);
    digitalWrite(CONTROL_PINB, HIGH);
    digitalWrite(RED, HIGH);
    digitalWrite(INFRARED, LOW);
  } 
  else {
    digitalWrite(CONTROL_PINA, HIGH);
    digitalWrite(CONTROL_PINB, LOW);
    digitalWrite(RED, LOW);
    digitalWrite(INFRARED, HIGH);
  }

  unsigned long currentCrossingTime = micros();

  if (currentCrossingTime > 0) {
    int comRed = analogRead(A1);
    int maxRed = 0;
    int minRed = 100000;
    int comIR = analogRead(A2);
    int maxIR = 0;
    int minIR = 1000000;

    if (comRed > maxRed)
      maxRed = comRed;
    if (comRed < minRed)
      minRed = comRed;
    if (comIR > maxIR)
      maxIR = comIR;
    if (comIR < minIR)
      minIR = comIR;

    float heartRate = calculateHeartRate(&lastPeakTime);
    float redRMS = calculateRMS(maxRed, minRed);
    float IRRMS = calculateRMS(maxIR, minIR);
    int SpO2 = calculateSpO2(redRMS, IRRMS, valuered, valueir);

    displayFrequencyAndBPM(heartRate, SpO2);
  }

  count++;
  delay(2);
}
// Function to display title screen
void displayTitleScreen() {
  display.clearDisplay();
  display.setTextSize(0.5);
  display.setTextColor(WHITE);

  // First line for "SpO2 Sensor Project" with underline
  display.setCursor(7, 0);
  display.print("SpO2 Sensor Project");
  display.setCursor(0, 3);
  display.print("_____________________");

  // Second line for "Bradley Frye & Nathan Skirvin"
  display.setCursor(15, 12);
  display.print("Bradley Frye &");
  display.setCursor(15, 20);
  display.print("Nathan Skirvin");

  display.display();
}

// Function to display the frequency and BPM values on the OLED screen
void displayFrequencyAndBPM(float bpm, int SpO2) {
  display.clearDisplay();       // Clear the display
  display.setTextSize(1);       // Set text size
  display.setTextColor(WHITE);  // Set text color

  // Display Title
  display.setCursor(23, 0);
  display.print("Measurements");
  display.setCursor(0, 2);
  display.print("____________________");

  // Display BPM
  display.setCursor(0, 9);  // Position the cursor
  display.print("BPM: ");
  display.print(bpm);

  //Dislay SpO2 %
  display.setCursor(0, 17);
  display.print("SpO2: ");
  display.print(SpO2);
  display.println(" %");

  display.display();  // Refresh the OLED screen to show the updated values
}

float calculateHeartRate(unsigned long* lastPeakTime) {
  unsigned long currentTime = millis();
  if (*lastPeakTime == 0) {
    *lastPeakTime = currentTime;  // Initialize lastPeakTime on the first call
    return 0;
  }

  unsigned long period = currentTime - *lastPeakTime;

  if (period == 0)
    return 0;

  float frequency = 1000000.0 / period;  // Calculate frequency from the period
  float heartRate = frequency * 60;      // Convert the frequency to BPM (Beats Per Minute)
  return heartRate;
}

float calculateRMS(int maxVal, int minVal) {
  double avg = (maxVal - minVal) / 2.0;
  return avg / sqrt(2);
}

int calculateSpO2(float redRMS, float IRRMS, int dcred, int dcir) {
  float R = (IRRMS / DCIR) / (redRMS / DCRed);
  int SpO2 = 110 - 25 * R;
  return SpO2;
}
