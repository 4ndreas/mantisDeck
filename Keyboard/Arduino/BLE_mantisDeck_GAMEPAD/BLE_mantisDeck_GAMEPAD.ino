/*
 * This example turns the ESP32 into a Bluetooth LE gamepad that presses buttons and moves axis
 *
 * At the moment we are using the default settings, but they can be canged using a BleGamepadConfig instance as parameter for the begin function.
 *
 * Possible buttons are:
 * BUTTON_1 through to BUTTON_16
 * (16 buttons by default. Library can be configured to use up to 128)
 *
 * Possible DPAD/HAT switch position values are:
 * DPAD_CENTERED, DPAD_UP, DPAD_UP_RIGHT, DPAD_RIGHT, DPAD_DOWN_RIGHT, DPAD_DOWN, DPAD_DOWN_LEFT, DPAD_LEFT, DPAD_UP_LEFT
 * (or HAT_CENTERED, HAT_UP etc)
 *
 * bleGamepad.setAxes sets all axes at once. There are a few:
 * (x axis, y axis, z axis, rx axis, ry axis, rz axis, slider 1, slider 2)
 *
 * Library can also be configured to support up to 5 simulation controls
 * (rudder, throttle, accelerator, brake, steering), but they are not enabled by default.
 *
 * Library can also be configured to support different function buttons
 * (start, select, menu, home, back, volume increase, volume decrease, volume mute)
 * start and select are enabled by default
 */

#include <Arduino.h>
#include <BleGamepad.h>

//BleGamepad bleGamepad;
BleGamepad bleGamepad("mantisSticks", "betabots", 100);

#define LEFT_X_PIN    34
#define LEFT_Y_PIN    32
#define LEFT_Z_PIN    36

#define RIGHT_X_PIN   35
#define RIGHT_Y_PIN   33
#define RIGHT_Z_PIN   39

#define LEFT_BUTTON_PIN   25
#define RIGHT_BUTTON_PIN  26

void setup()
{
    Serial.begin(115200);
    Serial.println("Starting BLE work!");

    pinMode(LEFT_BUTTON_PIN, INPUT_PULLUP);
    pinMode(RIGHT_BUTTON_PIN, INPUT_PULLUP);
    
    
    bleGamepad.begin();
    // The default bleGamepad.begin() above enables 16 buttons, all axes, one hat, and no simulation controls or special buttons
}

int lastButton1State = 0;
int lastButton2State = 0;

void loop()
{
    if (bleGamepad.isConnected())
    {
        int leftX = 32767 - overSample(LEFT_X_PIN);
        int leftY = overSample(LEFT_Y_PIN);
        int leftZ = overSample(LEFT_Z_PIN);
        
        int rightX = overSample(RIGHT_X_PIN);
        int rightY = overSample(RIGHT_Y_PIN);
        int rightZ = overSample(RIGHT_Z_PIN);

        int currentButton1State = digitalRead(LEFT_BUTTON_PIN);
        int currentButton2State = digitalRead(RIGHT_BUTTON_PIN);

        if(lastButton1State != currentButton1State){
          if(currentButton1State == LOW){
            bleGamepad.press(BUTTON_1);
          }
          else{
            bleGamepad.release(BUTTON_1);
          }
          lastButton1State = currentButton1State;
        }

        
        if(lastButton2State != currentButton2State){
          if(currentButton2State == LOW){
            bleGamepad.press(BUTTON_2);
          }
          else{
            bleGamepad.release(BUTTON_2);
          }
          lastButton2State = currentButton2State;
        }
      
        bleGamepad.setAxes(leftX, leftY, leftZ, rightZ, rightX, rightY, 0, 0);

        Serial.print("lX:");
        Serial.print(leftX);
        Serial.print(" lY:");
        Serial.print(leftY);
        Serial.print(" lZ:");
        Serial.print(leftZ);
        Serial.print(" lB:");
        Serial.print(currentButton1State);                
        
        Serial.print(" rX:");
        Serial.print(rightX);
        Serial.print(" rY:");
        Serial.print(rightY);
        Serial.print(" rZ:");
        Serial.print(rightZ);
        Serial.print(" rB:");
        Serial.println(currentButton2State);    

        delay(10);

//      bleGamepad.pressStart();
//      bleGamepad.releaseStart();

    }
}


int16_t overSample(int pin)
{
  int32_t rawValue = 0;
  for( int i = 0; i<8; i++)
  {  
    rawValue +=(int32_t)analogRead(pin);
  }
  //rawValue /= 2;
  rawValue = constrain(rawValue,0,32767);
  return(int16_t)rawValue;
}
