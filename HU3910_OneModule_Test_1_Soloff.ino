#include <PWM.h>
#include <Adafruit_MPR121.h>

int stringE2 = 10;  // pin for the low E string
int stringE4 = 9;

Adafruit_MPR121 touch = Adafruit_MPR121();


void setup()
{
  //initialize all timers except for 0, to save time keeping functions
  InitTimersSafe(); 

  touch.begin(0x5A);
  touch.setThreshholds(35, 20);

}

void loop()
{
  // // check if the E2 string button is pressed
  // switch(touch.touched()) {
  //   case 
  // }
  
  // E2 string
  if (touch.touched() & (1 << 0)) {
    if (touch.touched() & (1 << 11)) { // 5th fret
      SetPinFrequencySafe(stringE2, 110);
      pwmWrite(stringE2, 128);

    } else if (touch.touched() & (1 << 10)) { // 4th fret
      SetPinFrequencySafe(stringE2, 104);
      pwmWrite(stringE2, 128);

    } else if (touch.touched() & (1 << 9)) { // 3rd fret
      SetPinFrequencySafe(stringE2, 98);
      pwmWrite(stringE2, 128);

    } else if (touch.touched() & (1 << 8)) { // 2nd fret
      SetPinFrequencySafe(stringE2, 92);
      pwmWrite(stringE2, 128);

    } else if (touch.touched() & (1 << 7)) { // 1st fret
      SetPinFrequencySafe(stringE2, 87);
      pwmWrite(stringE2, 128);

    } else { // open
      SetPinFrequencySafe(stringE2, 82);
      pwmWrite(stringE2, 128);

    }
  
  } else {
    pwmWrite(stringE2, 0);
  }
  

  // E4 string
  if (touch.touched() & (1 << 1)) {
    if (touch.touched() & (1 << 11)) { // 5th fret
      SetPinFrequencySafe(stringE4, 440);
      pwmWrite(stringE4, 200);

    } else if (touch.touched() & (1 << 10)) { // 4th fret
      SetPinFrequencySafe(stringE4, 415);
      pwmWrite(stringE4, 200);

    } else if (touch.touched() & (1 << 9)) { // 3rd fret
      SetPinFrequencySafe(stringE4, 392);
      pwmWrite(stringE4, 200);

    } else if (touch.touched() & (1 << 8)) { // 2nd fret
      SetPinFrequencySafe(stringE4, 370);
      pwmWrite(stringE4, 200);

    } else if (touch.touched() & (1 << 7)) { // 1st fret
      SetPinFrequencySafe(stringE4, 350);
      pwmWrite(stringE4, 200);

    } else { // open
      SetPinFrequencySafe(stringE4, 330);
      pwmWrite(stringE4, 200);

    }
  
  } else {
    pwmWrite(stringE4, 0);
  }
    
}