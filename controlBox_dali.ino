/*

    DALI PROGRAM FOR CONTROL BOX

    CLAUDIO PACINI DEVELOPER

*/

#include "dali.h"
#include "command.h"


const int DALI_TX = 3;
const int DALI_RX_A = 0;
const int B_ON = 5;
const int B_UP = 6;
const int B_DOWN = 7;
const int B_TOT = 8;

bool isOn = 0;
bool isUp = 0;

int debounceDelay = 40;


//FUNCTIONS

boolean debounce(int pin);

void setup() {

  Serial.begin(74880);
  dali.setupTransmit(DALI_TX);
  dali.setupAnalogReceive(DALI_RX_A);
  dali.busTest();
  dali.msgMode = true;
  Serial.println(dali.analogLevel);

  pinMode(B_ON, INPUT_PULLUP);
  pinMode(B_UP, INPUT_PULLUP);
  pinMode(B_DOWN, INPUT_PULLUP);

}


void loop() {

  const int delaytime = 500;


  Serial.print("Lettura B_TOT: ");
  Serial.println(debounce(B_ON));
  Serial.print("Lettura B_ON: ");
  Serial.println(debounce(B_TOT));

  delay(delaytime);


  if (debounce(B_ON) == 0) {
    if (isOn == 0) {
      dali.transmit(BROADCAST_C, ON_C);
      Serial.println("Sto accendendo");
    }
    else {
      dali.transmit(BROADCAST_C, OFF_C);
      Serial.println("Sto spengendo");
    }
    isOn = !isOn;
  }

  if (debounce(B_TOT) == 0) {
    if (isUp == false) {
      while (digitalRead(B_TOT) == 0) {
        dali.transmit(BROADCAST_C, UP);
        Serial.println("Sto aumentando l'intensità");
        delay(delaytime);
      }
      isUp = true;
    }
    else {
      while (digitalRead(B_TOT) == 0) {
        dali.transmit(BROADCAST_C, DOWN);
        Serial.println("Sto diminuendo l'intensità");
        delay(delaytime);
      }
      isUp = false;
    }
  }

  if (digitalRead(B_UP) == 0) {
    dali.transmit(BROADCAST_C, UP);
  }

  if (digitalRead(B_DOWN) == 0) {
    dali.transmit(BROADCAST_C, DOWN);
  }

  delay(delaytime);

}


boolean debounce(int pin)
{
  boolean state;
  boolean previousState;
  previousState = digitalRead(pin);
  for (int counter = 0; counter < debounceDelay; counter++) {
    delay(1);
    state = digitalRead(pin);
    if ( state != previousState) {
      counter = 0;
      previousState = state;
    }
  }
  return state;
}
