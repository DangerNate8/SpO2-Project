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
#define ACRed A0
#define ACIR A1
#define DCRed A2
#define DCIR A3
#define INTR_PIN_1 2
#define INTR_PIN_2 3
Adafruit_SSD1306 display(OLED_RESET);



int maxRed = 0;
int minRed = 100000;
int maxIR = 0;
int minIR = 1000000;
int periodCount = 0;
int comIR, comRed, SpO2;
int periodMarker;
unsigned long periodHolder = 0.0;

unsigned long previousMillis = 0.0; 
bool firstRun;
double redRMS, IRRMS;
double redDC = 1.0;
double IRDC = 1.0;
double periodTotal;
double heartRate = 0.0;

unsigned long periodTimes[10];


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

float getAverage(double runningTotal, double newVal) {
  return (runningTotal + newVal) / 2;
}

double calculateRMS(double maxVal, double minVal) {
  double avg = (maxVal - minVal) / 2.0;
  return avg / sqrt(2);
}

int calculateSpO2(int redRMS, int IRRMS, int dcred, int dcir) {
  int R = (IRRMS / DCIR) / (redRMS / DCRed);
  int SpO2 = 110 - 25 * R;
  return SpO2;
}


void setup() {
  pinMode(INHIBIT, OUTPUT);
  pinMode(CONTROL_PINA, OUTPUT);
  pinMode(CONTROL_PINB, OUTPUT);
  pinMode(INFRARED, OUTPUT);
  pinMode(RED, OUTPUT);  
  pinMode(ACRed,INPUT );
  pinMode(ACIR, INPUT);
  pinMode(INTR_PIN_1, INPUT_PULLUP);
  pinMode(INTR_PIN_2, INPUT_PULLUP);
  Serial.begin(115200);  // Initialize serial communication, this value for speed and reliability
  attachInterrupt(digitalPinToInterrupt(2),ISR_falling, FALLING);
  attachInterrupt(digitalPinToInterrupt(3),ISR_rising, RISING);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }

  display.clearDisplay();  // Clears OLED screen
  displayTitleScreen();
  delay(5000);

  digitalWrite(INHIBIT, LOW);
  firstRun = true;

}

void loop() {
  // Need to write function for MUX
  unsigned long currentMillis = millis();
  //turns on RED led and signal path

    tone(RED, 500);

    int redDCtemp = analogRead(DCRed);  // Reads A1
    int comRed = analogRead(ACRed);
    redDC = getAverage(redDC, redDCtemp);
    if (comRed > maxRed)
      maxRed = comRed;
    if (comRed < minRed)
      minRed = comRed;
    if (comRed == periodMarker){
      periodTimes[periodCount] = millis();
    }

    int IRDCtemp = analogRead(DCIR);
    int comIR = analogRead(ACIR);
    IRDC = getAverage(IRDC, IRDCtemp);
    if (comIR > maxIR)
      maxIR = comIR;
    if (comIR < minIR)
      minIR = comIR;

    
    double redRMS = calculateRMS(maxRed, minRed);
    double IRRMS = calculateRMS(maxIR, minIR);
    int SpO2 = calculateSpO2((int)redRMS, (int)IRRMS, redDC, IRDC);

    displayFrequencyAndBPM((float)heartRate, SpO2);

  if(currentMillis - previousMillis >= 2000){
    previousMillis = currentMillis;
    maxIR = 0;
    minIR = 0;
    minRed = 0;
    maxRed = 0;
    redDC = 0;
    IRDC = 0;
    periodMarker = comRed;
    if (firstRun = false){
      int i;
      
      for(i = 1; i <= periodCount; i++){}
        periodHolder += (periodTimes[i] - periodTimes[i -1]);
        periodTotal = (double)periodHolder;
        periodTimes[i -1] = 0.0;
      }
      heartRate =  60.0 * (1.0/ ( (double)( periodTotal / periodCount )));
    }
    periodTimes[periodCount] = 0.0;
    periodCount = 0;
    periodHolder = 0.0;
    firstRun = false;
}

void ISR_falling(){
    digitalWrite(CONTROL_PINA, HIGH);
    digitalWrite(CONTROL_PINB, LOW);
    digitalWrite(INFRARED, HIGH);
}

void ISR_rising(){
    digitalWrite(CONTROL_PINA, LOW);
    digitalWrite(CONTROL_PINB, HIGH);
    digitalWrite(INFRARED, LOW);
}