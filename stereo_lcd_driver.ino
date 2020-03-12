#include <Encoder.h>
#include "StereoLCD.h"

// LCD
const static int INH_PIN = 7;
const static int CE_PIN = 6;
const static int CLK_PIN = 5;
const static int DATA_PIN = 4;

// Rotary Encoder
const static int UP_PIN = 2;
const static int DOWN_PIN = 3;

uint8_t counter = 66;
long currentPosition = 0;

Encoder knob(DOWN_PIN, UP_PIN);

StereoLCD screen(INH_PIN, CE_PIN, CLK_PIN, DATA_PIN);


void setup() {

  // LCD setup
  screen.enable();

  Serial.begin(9600);

}


void loop() {
  // put your main code here, to run repeatedly:


long newPosition = knob.read();
if (newPosition != currentPosition) {
  counter += newPosition - currentPosition;
  currentPosition = newPosition;
}

/*
screen.buff[0] = 0;
screen.buff[1] = 0;
screen.send();
delay(1000);
*/

/*
screen.writeDigit(0, counter % 67);
screen.writeDigit(1, counter % 67);
screen.writeDigit(2, counter % 67);
screen.writeDigit(3, counter % 67);
screen.writeDigit(4, counter % 67);
screen.writeDigit(5, counter % 67);
screen.writeDigit(6, counter % 67);
screen.writeDigit(7, counter % 67);
*/

screen.send();
delay(100);



for (uint8_t i = 0; i < 67 ; i++) {
  screen.writeDigit(0, i);
  screen.writeDigit(1, i);
  screen.writeDigit(2, i);
  screen.writeDigit(3, i);
  screen.writeDigit(4, i);
  screen.writeDigit(5, i);
  screen.writeDigit(6, i);
  screen.writeDigit(7, i);

  screen.send();
  delay(300);
}


/*
digitalWrite(LED_BUILTIN, HIGH);
delay(500);
digitalWrite(LED_BUILTIN, LOW);
delay(500);
*/
}
