#include <PWM.h>
#include <Adafruit_MPR121.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>


// String variables for pin assignments
int stringE2 = 10;  // pin for the low E string
int stringE4 = 9; // pin for the high E string

// PWM pulse lengths, out of 255
int E2pulse = 20;
int E4pulse = 128;

// Arrays
int frequenciesIdeal[7][6] = { // array of the [string][fret]
  {  0,   1,   2,   3,   4,   5}, // string numbers, not actual data
  {330, 349, 370, 392, 415, 440}, // 1st string, E4
  {247, 262, 278, 294, 311, 330}, // 2nd string, B3
  {196, 208, 220, 233, 247, 262}, // 3rd string, G3
  {147, 156, 165, 175, 185, 196}, // 4th string, D3
  {110, 117, 124, 131, 139, 147}, // 5th string, A2
  { 82,  87,  93,  98, 104, 110}, // 6th string, E2

}; 

float freqReal[7][6] = { // array of the [string][fret]
  {  0,   1,   2,   3,   4,   5}, // string numbers, not actual data
  {329.0, 352.8, 372.7, 394.7, 417.4, 442.1}, // 1st string, E4
  {247, 262, 278, 294, 311, 330}, // 2nd string, B3
  {196, 208, 220, 233, 247, 262}, // 3rd string, G3
  {147, 156, 165, 175, 185, 196}, // 4th string, D3
  {110, 117, 124, 131, 139, 147}, // 5th string, A2
  { 82.5,  89.4,  94.8,  99.4, 105.4, 111.4}, // 6th string, E2

}; 

float dutyCycle[7] = {
  128, // string numbers
  128, // 1st string
  128,
  128, // 3rd string
  128,
  128,
  128, // 6th string
};

int stringPin[7] {
  0, // nothing
  9, // 1st string
  1,
  6, // 3rd string
  1,
  1,
  10, // 6th string
};

// vars for frequency generation
unsigned long time;

unsigned long startTime[7] {
  0, // nothing
  0,
  0,
  0,
  0,
  0,
  0,
};

bool previousState[7] {
  false, // nothing
  false, // 1st string
  false,
  false, // 3rd string
  false,
  false,
  false, // 6th string
};

// functions
int determineInputPairs() { // determines pairs of fret and string inputs

}

void playFrequency(int string, int fret, bool enable) { // play a certain frequency on a certain pin
  time = micros();
  int error = 100;

  int periodInMicros = (1 / freqReal[string][fret]) * 1000000;
  float onTime = (dutyCycle[string]/255) * periodInMicros;
  //int offTime = periodInMicros - onTime;

  //Serial.println(onTime);

  if (!previousState[string] && enable) {
    startTime[string] = time;

  }

  if (previousState[string] && enable) {
    int timeSinceStart = (time - startTime[string]) % periodInMicros;

    //Serial.println(timeSinceStart);

    if ((timeSinceStart >= (0 - error)) && (timeSinceStart <= (0 + error))) {
      digitalWrite(stringPin[string], 255); // check if period has started to turn pin on
      
      //Serial.println("on");

    } 
    
    if ((timeSinceStart >= (onTime - error)) && (timeSinceStart <= (onTime + error))) {
      digitalWrite(stringPin[string], 0); // turn pin off after the onTime has passed

      //Serial.println("off");

    }
  }

  if (!enable) {
    digitalWrite(stringPin[string], 0);
  }

  previousState[string] = enable;
  
  //Serial.println(previousState[string]);

}

// capacitive touch board
Adafruit_MPR121 touch = Adafruit_MPR121();

void setup() {
  //initialize all timers except for 0, to save time keeping functions
  InitTimersSafe(); 

  touch.begin(0x5A);
  touch.setThreshholds(20, 6);

  for (int i = 0; i < 7; i++) {
    pinMode(stringPin[i], OUTPUT);
  }

  Serial.begin(1000000);

}

bool stringE2Enable = false;
int stringE2Fret = 0;

void loop() {
  
  playFrequency(6, 0, true);
  // playFrequency(1, 0, true);
  // playFrequency(3, 0, true);

  // if (touch.touched() && (1 << 0)) {
  //   playFrequency(6, 0, true);
  // }
  // else {
  //   playFrequency(6, 0, false);
  // }

  // if (true) {
  //   playFrequency(6, 0, true);
  // }
  // else {
  //   playFrequency(6, 0, false);
  // }
  
  
  // // E2 string
  // if (touch.touched() & (1 << 0)) {
  //   if (touch.touched() & (1 << 11)) { // 5th fret
  //     stringE2Enable = true;
  //     stringE2Fret = 5;

  //   } else if (touch.touched() & (1 << 10)) { // 4th fret
  //     stringE2Enable = true;
  //     stringE2Fret = 4;

  //   } else if (touch.touched() & (1 << 9)) { // 3rd fret
  //     stringE2Enable = true;
  //     stringE2Fret = 3;

  //   } else if (touch.touched() & (1 << 8)) { // 2nd fret
  //     stringE2Enable = true;
  //     stringE2Fret = 2;

  //   } else if (touch.touched() & (1 << 7)) { // 1st fret
  //     stringE2Enable = true;
  //     stringE2Fret = 1;

  //   } else { // open
  //     stringE2Enable = true;
  //     stringE2Fret = 0;

  //   }
  
  // } else {
  //     stringE2Enable = false;

  // }
  
  // playFrequency(6, stringE2Fret, stringE2Enable);

  // // E4 string
  // if (touch.touched() & (1 << 1)) {
  //   if (touch.touched() & (1 << 11)) { // 5th fret
  //     SetPinFrequencySafe(stringE4, freqReal[1][5]);
  //     pwmWrite(stringE4, E4pulse);

  //   } else if (touch.touched() & (1 << 10)) { // 4th fret
  //     SetPinFrequencySafe(stringE4, freqReal[1][4]);
  //     pwmWrite(stringE4, E4pulse);

  //   } else if (touch.touched() & (1 << 9)) { // 3rd fret
  //     SetPinFrequencySafe(stringE4, freqReal[1][3]);
  //     pwmWrite(stringE4, E4pulse);

  //   } else if (touch.touched() & (1 << 8)) { // 2nd fret
  //     SetPinFrequencySafe(stringE4, freqReal[1][2]);
  //     pwmWrite(stringE4, E4pulse);

  //   } else if (touch.touched() & (1 << 7)) { // 1st fret
  //     SetPinFrequencySafe(stringE4, freqReal[1][1]);
  //     pwmWrite(stringE4, E4pulse);

  //   } else { // open
  //     SetPinFrequencySafe(stringE4, freqReal[1][0]);
  //     pwmWrite(stringE4, E4pulse);

  //   }
  
  // } else {
  //   pwmWrite(stringE4, 0);
  // }
    
}