// dimmer with two button control
// TODO supports two separate dimmers
// first version without OpenHAB connectivity

// library for button management using callbacks 
// https://github.com/r89m/Button https://github.com/r89m/PushButton
#include <Button.h>
#include <ButtonEventCallback.h>
#include <PushButton.h>
#include <Bounce2.h> // https://github.com/thomasfredericks/Bounce-Arduino-Wiring


//----- configuration --------------------------------------------
// buttons and outputs
PushButton button1_up = PushButton(4, ENABLE_INTERNAL_PULLUP);
PushButton button1_down = PushButton(5, ENABLE_INTERNAL_PULLUP);


#define output1 0
#define output2 2

// brightness is a byte 0-255
#define firstBrightness 200 // initial value of remembered brightness
#define shortPress 200      // max. duration of short press (ms)
#define minBrightness 40    // minimal brightness
#define maxBrightness 250   // max. brightness

#define holdInterval 500    // min. duration of long press (ms)
#define fadeInterval 200    // ms between fade steps when long press
#define fadeStep 20         // change of brightness on each fade step

//----------------------------------------------------------------
void setup() {
  Serial.begin(115200);

  // button setup via callbacks - each callback can handle multiple buttons
  // When the button is released
  button1_up.onRelease(onButtonReleased);
  button1_down.onRelease(onButtonReleased);
  
  // Once the button has been held for xxx ms call onButtonHeld. Call it again every yyy ms until it is let go
  button1_up.onHoldRepeat(holdInterval, fadeInterval, onButtonHeld);
  button1_down.onHoldRepeat(holdInterval, fadeInterval, onButtonHeld);

  pinMode(output1, OUTPUT);


  analogWriteRange(255); // default on ESP is 1023
  //analogWriteFreq(600); //??? 1000 Hz is default
  analogWrite(output1,0);


  
  Serial.println("Starting...");
}


// global variables
int brightness1 = 0;
int brightness2 = 0;
int rememberedBrightness1 = firstBrightness;
int rememberedBrightness2 = firstBrightness;


void loop() {

  // Check the state of the buttons
  button1_up.update();
  button1_down.update();

}


// some button released, duration reports back the total time that the button was held down
// if shortly pressed, switch on or off the output
void onButtonReleased(Button& btn, uint16_t duration){

  if(duration < shortPress) {
    if(btn.is(button1_up)){  // switch on 
      brightness1 = rememberedBrightness1;
      Serial.print("1 up ");
    } else if (btn.is(button1_down)){ // switch off
      if (brightness1 > 0){
        rememberedBrightness1 = brightness1; // remember last brightness before switching off
        brightness1 = 0;
      }  
    Serial.print("1 down ");
    }
  }

  analogWrite(output1, brightness1);
  
  Serial.print("buttonReleased end brightness: ");
  Serial.println(brightness1);
}

// 
// duration reports back how long it has been since the button was originally pressed.
// repeatCount tells us how many times this function has been called by this button.
// change the brightness (dimmer function)
void onButtonHeld(Button& btn, uint16_t duration, uint16_t repeatCount){

    Serial.print("button has been held for ");
    Serial.print(duration);
    Serial.print(" ms; this event has been fired ");
    Serial.print(repeatCount);
    Serial.println(" times");

  if(btn.is(button1_up)){  // fade up
    Serial.print("1 fade up ");
    brightness1 += fadeStep;
    if (brightness1 > maxBrightness) {
      brightness1 = maxBrightness;
    }
  } else if (btn.is(button1_down)){ // fade down
    Serial.print("1 fade down ");
    brightness1 -= fadeStep;
    if (brightness1 < minBrightness) {
      brightness1 = minBrightness;
    }
  }

  analogWrite(output1, brightness1);

  Serial.print("buttonHeld end brightness: ");
  Serial.println(brightness1);

}
