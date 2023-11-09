#include <Adafruit_SSD1306.h>  // Driver for the OLED

#define OLED_RESET 1         // OLED reset pin definition
#define SCREEN_ADDRESS 0x3C  // Address for OLED
#define CONTROL_PINA  9    // Pin 11 on MUX
#define CONTROL_PINB  8    // Pin 10 on MUX
#define INHIBIT       10   // Pin 6 on MUX
#define RED      7    // Pin 12 on MUX
#define INFRARED           6    // Pin 2 on MUX
// Ground goes to ay and by that goes to MUX pin 1 and 13
// Out signal is MUX pin 14 for ax and MUX pin 15 for bx

// Infrared 1.6 V to 1.2 V Bradley    1.5 V to 0.8 V Nate
// Red 1.4V to 1.2 V       Bradley    1.4 V to 1.2V  Nate

Adafruit_SSD1306 display(OLED_RESET);  // Creates a display object for OLED

// Define constants related to analog signal processing
const int analogPin = A1;           // Analog pin for input signal
const int zeroCrossingValue = 307;  // Value corresponding to the zero-crossing voltage (1.5V in this case)
const int buffer = 5;               // A buffer value to account for noise around the zero-crossing value

bool wasBelowZero = true;            // Tracks last read value was below the zero-crossing
unsigned long lastCrossingTime = 0;  // Keeps track of the time (in microseconds) of the last detected zero-crossing

void setup() {
  pinMode(INHIBIT, OUTPUT);
  pinMode(CONTROL_PINA, OUTPUT);
  pinMode(CONTROL_PINB, OUTPUT);
  pinMode(INFRARED, OUTPUT);
  pinMode(RED, OUTPUT);

  Serial.begin(115200);  // Initialize serial communication, this value for speed and reliability

  // Try to initialize the OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));  // Display error message if initialization fails
    for (;;)
      ;  // Enter an infinite loop, effectively halting the program
  }

  display.clearDisplay();  // Clears OLED screen
  displayTitleScreen();
  delay(5000);
}

void loop() {
  digitalWrite(INHIBIT, LOW);
  digitalWrite(CONTROL_PINA, LOW); 
  digitalWrite(CONTROL_PINB, HIGH); 
  digitalWrite(RED, HIGH);
  digitalWrite(INFRARED, LOW);
  
  int value = analogRead(analogPin);  // Reads A1

  //For debugging purposes
  Serial.print("Analog Value: ");
  Serial.println(value);              // Print the read analog value to the serial monitor

  // Check if a positive-going zero-crossing is detected
  if (wasBelowZero && value >= (zeroCrossingValue + buffer)) {
    unsigned long currentCrossingTime = micros(); // Get the current timestamp
    unsigned long period = currentCrossingTime - lastCrossingTime; // Calculate the time difference since the last zero-crossing

    // Only calculate the frequency if the period is valid (greater than zero)
    if (period > 0) {
      float frequency = 1000000.0 / period;  // Calculate frequency from the period
      float bpm = frequency * 60;            // Convert the frequency to BPM (Beats Per Minute)

      // Print calculated frequency and BPM to the serial monitor
      Serial.print("Frequency: ");
      Serial.println(frequency);
      Serial.print("BPM: ");
      Serial.println(bpm);

      displayFrequencyAndBPM(frequency, bpm); // Update the OLED display with the new frequency and BPM values

      lastCrossingTime = currentCrossingTime; // Update the timestamp for the next zero-crossing detection
    }
    wasBelowZero = false;  // Reset for next cycle
  }
  // Check if the waveform has gone below the zero-crossing minus buffer
  else if (value < (zeroCrossingValue - buffer)) {
    wasBelowZero = true;   // Signal is now below the zero-crossing, triggers if statement above to do its calculations
  }
  delay(2); // Waits 500 Hz, 2 ms

  digitalWrite(INHIBIT, LOW);
  digitalWrite(CONTROL_PINA, HIGH);  
  digitalWrite(CONTROL_PINB, LOW);  
  digitalWrite(RED, LOW);
  digitalWrite(INFRARED, HIGH);
  
   if (wasBelowZero && value >= (zeroCrossingValue + buffer)) {
    unsigned long currentCrossingTime = micros(); // Get the current timestamp
    unsigned long period = currentCrossingTime - lastCrossingTime; // Calculate the time difference since the last zero-crossing

    // Only calculate the frequency if the period is valid (greater than zero)
    if (period > 0) {
      float frequency = 1000000.0 / period;  // Calculate frequency from the period
      float bpm = frequency * 60;            // Convert the frequency to BPM (Beats Per Minute)

      // Print calculated frequency and BPM to the serial monitor
      Serial.print("Frequency: ");
      Serial.println(frequency);
      Serial.print("BPM: ");
      Serial.println(bpm);

      displayFrequencyAndBPM(frequency, bpm); // Update the OLED display with the new frequency and BPM values

      lastCrossingTime = currentCrossingTime; // Update the timestamp for the next zero-crossing detection
    }
    wasBelowZero = false;  // Reset for next cycle
  }
  // Check if the waveform has gone below the zero-crossing minus buffer
  else if (value < (zeroCrossingValue - buffer)) {
    wasBelowZero = true;   // Signal is now below the zero-crossing, triggers if statement above to do its calculations
  }
  delay(2); // Waits 500 Hz, 2ms
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
void displayFrequencyAndBPM(float freq, float bpm) {
  display.clearDisplay();       // Clear the display
  display.setTextSize(1);       // Set text size
  display.setTextColor(WHITE);  // Set text color

  // Display Title
  display.setCursor(23, 0);
  display.print("Measurements");
  display.setCursor(0, 2);
  display.print("____________________");

  // Display frequency
  display.setCursor(0, 9);  // Position the cursor
  display.print("Frequency: ");
  display.print(freq);
  display.println(" Hz");

  // Display BPM
  display.setCursor(0, 17);  // Position the cursor
  display.print("BPM: ");
  display.print(bpm);

  //Dislay SpO2 %
  display.setCursor(0, 24);
  display.print("SpO2: ");
  display.println(" %");

  display.display();  // Refresh the OLED screen to show the updated values
}
