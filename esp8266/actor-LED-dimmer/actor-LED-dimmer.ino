// dimmer with two button control
// supports two separate dimmers
// first version without OpenHAB connectivity

// library for button management using callbacks 
// https://github.com/r89m/Button https://github.com/r89m/PushButton
#include <Button.h>
#include <ButtonEventCallback.h>
#include <PushButton.h>
#include <Bounce2.h> // https://github.com/thomasfredericks/Bounce-Arduino-Wiring

// callback scheduling via SimpleTimer.h not widely used on ESP, why
// manual install of http://github.com/jfturcot/SimpleTimer ?
// build-in Ticker.h (for ESP only)?
// try https://github.com/Makuna/Task ?
#include <Ticker.h>

//----- configuration --------------------------------------------
// buttons and outputs
PushButton button1_up = PushButton(4, ENABLE_INTERNAL_PULLUP);
PushButton button1_down = PushButton(0, ENABLE_INTERNAL_PULLUP);
PushButton button2_up = PushButton(2, ENABLE_INTERNAL_PULLUP);
PushButton button2_down = PushButton(14, ENABLE_INTERNAL_PULLUP);
// don't use gpio15 (build-in pull-down resistor needed for bootup process)

#define output1 5
#define output2 3

// brightness is a byte 0-255
#define firstBrightness 200 // initial value of remembered brightness
#define shortPress 400      // max. duration of short press (ms)
#define minBrightness 0    // minimal brightness (when fading)
#define maxBrightness 255   // max. brightness

#define holdInterval 500    // min. duration of long press (ms)
#define fadeInterval 50    // ms between fade steps when long press
#define fadeStep 5         // change of brightness on each fade step
#define fancyOffInterval 50// ms between fancyOff steps
#define fancyOffStep   5   // change of brightness on each fancyOff step


//----------------------------------------------------------------

Ticker fancyOff1; // Ticker.h object to call a callback 
Ticker fancyOff2; // Ticker.h object to call a callback 

void setup() {
  Serial.begin(115200);

  // button setup via callbacks - each callback can handle multiple buttons
  // When the button is released
  button1_up.onRelease(onButtonReleased);
  button1_down.onRelease(onButtonReleased);
  button2_up.onRelease(onButtonReleased);
  button2_down.onRelease(onButtonReleased);
  
  // Once the button has been held for xxx ms call onButtonHeld. Call it again every yyy ms until it is let go
  button1_up.onHoldRepeat(holdInterval, fadeInterval, onButtonHeld);
  button1_down.onHoldRepeat(holdInterval, fadeInterval, onButtonHeld);
  button2_up.onHoldRepeat(holdInterval, fadeInterval, onButtonHeld);
  button2_down.onHoldRepeat(holdInterval, fadeInterval, onButtonHeld);

  pinMode(output1, OUTPUT);
  pinMode(output2, OUTPUT);

  analogWriteRange(255); // default on ESP is 1023
  //analogWriteFreq(600); //??? 1000 Hz is default

  setOutput(0, 0);
  
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
  button2_up.update();
  button2_down.update();

  // Ticker.h doesn't need to call (it is attached to hardware interupt)
}


// some button released, duration reports back the total time that the button was held down
// if shortly pressed, switch on or off the output
void onButtonReleased(Button& btn, uint16_t duration){

  if(duration < shortPress) {
    if(btn.is(button1_up)){  // switch on 
      brightness1 = rememberedBrightness1;
      fancyOff1.detach();; // stop the fancyOff if we are switching on early
      Serial.print("1 up ");
    } else if (btn.is(button1_down)){ // switch off
      if (brightness1 > 0){
        rememberedBrightness1 = brightness1; // remember last brightness before switching off
        //no brightness1 = 0; because the actual switch off will be provided by fancyOff callback
        fancyOff1.attach_ms(fancyOffInterval, fancyOff1CB);
      }  
      Serial.print("1 down ");
    } else if (btn.is(button2_up)){  // switch on 
      brightness2 = rememberedBrightness2;
      fancyOff2.detach();; // stop the fancyOff if we are switching on early
      Serial.print("2 up ");
    } else if (btn.is(button2_down)){ // switch off
      if (brightness2 > 0){
        rememberedBrightness2 = brightness2; // remember last brightness before switching off
        //no brightness2 = 0; because the actual switch off will be provided by fancyOff callback
        fancyOff2.attach_ms(fancyOffInterval, fancyOff2CB);
      }  
      Serial.print("2 down ");
    }
  }

  setOutput(brightness1, brightness2);
  
  Serial.print("buttonReleased end brightness: 1:");
  Serial.print(brightness1);
  Serial.print(" 2:");
  Serial.println(brightness2);
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
  } else if (btn.is(button1_down)){ // fade down, fading down to zero is like switching off but without any change to remembered brightness
    Serial.print("1 fade down ");
    if ((brightness1 - fadeStep) >= minBrightness) {
      brightness1 -= fadeStep;
    } 
  } else if (btn.is(button2_up)){  // fade up
    Serial.print("2 fade up ");
    brightness2 += fadeStep;
    if (brightness2 > maxBrightness) {
      brightness2 = maxBrightness;
    }
  } else if (btn.is(button2_down)){ // fade down, fading down to zero is like switching off but without any change to remembered brightness
    Serial.print("2 fade down ");
    if ((brightness2 - fadeStep) >= minBrightness) {
      brightness2 -= fadeStep;
    }
  }

  setOutput(brightness1, brightness2);
    
  Serial.print("buttonHeld end brightness: 1:");
  Serial.print(brightness1);
  Serial.print(" 2:");
  Serial.println(brightness2);

}

// callback called each xxx ms if we want to switch the light off slowly
void fancyOff1CB() {
  if ((brightness1 - fancyOffStep) > 0 ) {
    brightness1 -= fancyOffStep;
  } else {
    brightness1 = 0;
    fancyOff1.detach(); //stop calling this callback
  }
  setOutput(brightness1, brightness2);
}

void fancyOff2CB() {
  if ((brightness2 - fancyOffStep) > 0 ) {
    brightness2 -= fancyOffStep;
  } else {
    brightness2 = 0;
    fancyOff2.detach(); //stop calling this callback
  }
  setOutput(brightness1, brightness2);
}

void setOutput(int brightness1, int brightness2) {
  analogWrite(output1, brightness1);
  analogWrite(output2, brightness2);
  
  Serial.print("setOutput: 1:");
  Serial.print(brightness1);
  Serial.print(" 2:");
  Serial.println(brightness2);
}

