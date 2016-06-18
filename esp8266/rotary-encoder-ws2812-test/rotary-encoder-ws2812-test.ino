

//Encoder Library
// http://www.pjrc.com/teensy/td_libs_Encoder.html
#include <Encoder.h>

//I'm using EC11 encoder with 20 phisical steps, each step generates 4
//steps in library position - why?
//(tested with ESP8266 and Arduino Nano with the same behaviour)


Encoder myEnc(12, 14); // D5, D6 on ESP
//Encoder myEnc(2, 3); // interupt pins on Arduino Nano

void setup() {
  Serial.begin(9600);
  Serial.println("Basic Encoder Test:");
}

long oldPosition  = -999;

void loop() {
  long newPosition = myEnc.read() / 4 % 60;
  if (newPosition < 0) { 
    myEnc.write(60*4);
    newPosition = 0;
  }
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    Serial.println(newPosition);
  }
}
