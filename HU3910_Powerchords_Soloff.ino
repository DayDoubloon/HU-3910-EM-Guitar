#include <PWM.h>
#include <Adafruit_MPR121.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

/*
Arrays for variables about each string
*/

// real frequencies for each string and fret combination
float freqReal[7][13] = { // freqReal[string][fret]
  {    0,     1,     2,     3,     4,     5,     6,     7,     8,     9,    10,    11,    12}, // fret numbers, not actual data
  {330.0, 352.7, 373.3, 393.1, 415.7, 440.3, 466.6, 493.9, 523.3, 554.4, 589.3, 622.3, 662.3}, // 1st string, E4
  {247.0, 265.3, 279.3, 295.8, 312.9, 331.3, 351.2, 370.0, 392.1, 415.3, 440.0, 466.6, 496.2}, // 2nd string, B3
  {196.0, 212.1, 222.4, 235.6, 249.1, 263.4, 277.2, 293.7, 311.1, 329.6, 349.2, 370.0, 392.0}, // 3rd string, G3
  {147.0, 158.3, 166.9, 176.5, 186.8, 197.4, 209.3, 221.5, 235.3, 248.8, 264.1, 279.2, 295.0}, // 4th string, D3
  {110.0, 118.5, 124.9, 132.0, 140.2, 147.7, 156.1, 165.5, 175.3, 185.7, 196.4, 208.0, 220.6}, // 5th string, A2
  { 82.5,  89.1,  94.0, 100.2, 106.0, 112.0, 117.6, 123.8, 131.4, 139.5, 147.6, 156.4, 166.0}, // 6th string, E2

}; 

// duty cycle in %, string:  1    2    3    4    5    6
float dutyCycle[7] = {  0,  50,  50,  50,  50,  30,  20}; // dutyCycle[string]

// pin for each string, string:  1   2   3   4   5   6
int stringPin[7] = {         0,  7,  6,  5,  4,  3,  2}; // stringPin[string]


/*
Function and variables for determining what the inputs are
*/

// capacitive touch boards
Adafruit_MPR121 strings = Adafruit_MPR121();
Adafruit_MPR121 frets = Adafruit_MPR121();

// highest fret available
int maxFret = 12;

// what frets are pressed, in the order of high to low, 0 means no input
int fretsPlaying[6] = {0, 0, 0, 0, 0, 0};

// what strings are pressed, in the order of E2 to E4, 0 means no input
int stringsPlaying[6] = {0, 0, 0, 0, 0, 0};

// what strings are not pressed, in the order of E2 to E4, 0 means no input
int stringsNotPlaying[6] = {0, 0, 0, 0, 0, 0};

// determine what string and fret combination should be played based on power chord shapes
void powerChordInputGroups() { // determines pairs of fret and string inputs
  int stringInput = strings.touched();
  int fretInput = frets.touched();

  // set all the values in the arrays to zero
  for (int i = 0; i < 6; i++) {
    fretsPlaying[i] = 0;
    stringsPlaying[i] = 0;
    stringsNotPlaying[i] = 0;
  }

  int fretIndex = 0;

  // determining the frets that are pressed, in order of high to low
  for (int i = 0; i <= maxFret - 1; i++) {
    if ((fretInput & (1 << i)) && (fretIndex < 6)) {
      fretsPlaying[fretIndex] = i + 1;
      fretIndex++;
    }
  }

  // set the fret to the highest played on all the other strings
  for (int i = 1; i < 6; i++) {
    if (fretsPlaying[i] == 0) {
      fretsPlaying[i] = fretsPlaying[i - 1];
    }
  }

  int stringIndex = 0;
  int stringNotIndex = 0;

  // determining the strings that are pressed, in order of E2 to E4
  for (int i = 6; i >= 0; i--) {
    if ((stringInput & (1 << i)) && (stringIndex < 6)) {
      stringsPlaying[stringIndex] = i;
      stringIndex++;
    } else if (!(stringInput & (1 << i))) {
      stringsNotPlaying[stringNotIndex] = i;
      stringNotIndex++;
    }
  }
}

void playPowerChords() {
  for (int i = 0; i < 6; i++) {
    // Serial.print(stringsPlaying[i]);
    // Serial.println(fretsPlaying[i]);
    if (stringsPlaying[i] != 0) {
      playFrequency(stringsPlaying[i], fretsPlaying[i], true);
      //Serial.println("playing" + i);
    } 

    if (stringsNotPlaying[i] != 0) {
      playFrequency(stringsNotPlaying[i], fretsPlaying[i], false);
    }
  }
}

bool stringsPressed[7] = {false, false, false, false, false, false, false};

void simpleStringInputs() {
  int stringInput = strings.touched();

  for (int i = 1; i <= 6; i++) {
    stringsPressed[i] = false;

    if (stringInput & (1 << i)) {
      stringsPressed[i] = true;
    }
  }
}

int highestFret;

void simpleFretInput() {
  int fretInput = frets.touched();
  highestFret = 0;

  if (fretInput & (1 << 1)) {
    highestFret = 1;
  } 

  if (fretInput & (1 << 2)) {
    highestFret = 2;
  }
  
  if (fretInput & (1 << 3)) {
    highestFret = 3;
  } 
  
  if (fretInput & (1 << 4)) {
    highestFret = 4;
  } 
  
  if (fretInput & (1 << 5)) {
    highestFret = 5;
  }

  if (fretInput & (1 << 6)) {
    highestFret = 6;
  }

  if (fretInput & (1 << 7)) {
    highestFret = 7;
  }

  if (fretInput & (1 << 8)) {
    highestFret = 8;
  }

  if (fretInput & (1 << 9)) {
    highestFret = 9;
  }

  if (fretInput & (1 << 10)) {
    highestFret = 10;
  }

  if (fretInput & (1 << 11)) {
    highestFret = 11;
  }

  if (fretInput & (1 << 12)) {
    highestFret = 12;
  }
}


/*
Function and variables for playing a certain frequency on a given string
*/

// time since program started in microseconds
unsigned long time;

// start times, set to zero initially
unsigned long startTime[7] = {0, 0, 0, 0, 0, 0, 0}; // startTime[string]

// previous states, set to false initially
bool previousState[7] = {false, false, false, false, false, false, false}; // previousState[string]

// time since start of that period per string, set to zero initially
int timeSinceStart[7] = {0, 0, 0, 0, 0, 0, 0};

// switch a pin on and off at a given frequency
void playFrequency(int string, int fret, bool enable) { // play a certain frequency on a certain pin
  time = micros();
  int error = 500; // number of microseconds the tme can be off by and still switch the pin state

  int periodInMicros = (1 / freqReal[string][fret]) * 1000000; // period of one oscillation in microseconds of the given string and fret combination
  float onTime = (dutyCycle[string]/100) * periodInMicros; // how long the pin should remain on for

  if (!previousState[string] && enable) { // check if the state of the function has changed
    startTime[string] = time; // set the time for that string to the current time in microseconds
  }

  if (previousState[string] && enable) { // if the function is still enabled
    timeSinceStart[string] = (time - startTime[string]) % periodInMicros; // how long it has been since the last period completed

    if ((timeSinceStart[string] >= (0 - error)) && (timeSinceStart[string] <= (0 + error))) { // check if the period is near the begining
      digitalWrite(stringPin[string], 255); // turn the pin on
    } 
    
    if ((timeSinceStart[string] >= (onTime - error)) && (timeSinceStart[string] <= (onTime + error))) { // check if the period is near the on time
      digitalWrite(stringPin[string], 0); // turn pin off after the onTime has passed
    }
  }

  if (!enable) { // if the enable signal is false
    digitalWrite(stringPin[string], 0); // turn the pin off
  }

  previousState[string] = enable; // record the current state for next iteration
}


/*
Setup and Loop functions
*/

void setup() {
  //initialize all timers except for 0, to save time keeping functions
  InitTimersSafe(); 

  // set address and sensitivities of the touch sensors for the strings
  strings.begin(0x5A);
  strings.setThreshholds(20, 6);

  // set address and sensitivities of the touch sensors for the frets
  frets.begin(0x5D);
  frets.setThreshholds(6, 6);

  // set the strings to be outputting
  pinMode(stringPin[1], OUTPUT);
  pinMode(stringPin[2], OUTPUT);
  pinMode(stringPin[3], OUTPUT);
  pinMode(stringPin[4], OUTPUT);
  pinMode(stringPin[5], OUTPUT);
  pinMode(stringPin[6], OUTPUT);
}

// variable to count loops to prevent checking the I2C bus every loop
int count = 0;

void loop() {
  count++;

  if (count == 100) {
    // simpleStringInputs();
    // simpleFretInput();
    powerChordInputGroups();
    count = 0;
  }

  playPowerChords();

  // playFrequency(1, highestFret, stringsPressed[1]);
  // playFrequency(2, highestFret, stringsPressed[2]);
  // playFrequency(3, highestFret, stringsPressed[3]);
  // playFrequency(4, highestFret, stringsPressed[4]);
  // playFrequency(5, highestFret, stringsPressed[5]);
  // playFrequency(6, highestFret, stringsPressed[6]); 
}