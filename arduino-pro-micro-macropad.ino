#include "Keyboard.h"
// Library with a lot of the HID definitions and methods
// Can be useful to take a look at it see whats available
// https://github.com/arduino-libraries/Keyboard/blob/master/src/Keyboard.h
#include <Mouse.h> //there are some mouse move functions for encoder_Mode 2 and 3
#include <Keypad.h>
// This library is for interfacing with the 3x4 Matrix
// Can be installed from the library manager, search for "keypad"
// and install the one by Mark Stanley and Alexander Brevig
// https://playground.arduino.cc/Code/Keypad/

const byte ROWS = 3; //four rows
const byte COLS = 4; //four columns

#include <Encoder.h>   
//Library for simple interfacing with encoders (up to two)
//low performance ender response, pins do not have interrupts
Encoder RotaryEncoderA(14, 15); //the LEFT encoder (encoder A)
Encoder RotaryEncoderB(10, 16);  //the RIGHT encoder (encoder B)

char keys[ROWS][COLS] = {
  {'1', '2', '3', '4'},  //  the keyboard hardware is  a 3x4 grid... 
  {'5', '6', '7', '8'},
  {'9', '0', 'A', 'B'},  // these values need  to be single char, so...
};
// The library will return the character inside this array when the appropriate
// button is pressed then look for that case statement. This is the key assignment lookup table.
// Layout(key/button order) looks like this
//     |----------------------------|
//     |                  [2/3]*    |     *TRS breakout connection. Keys 5 and 6 are duplicated at the TRS jack
//     |      [ 1] [ 2] [ 3] [ 4]   |     * Encoder A location = key[1]      
//     |      [ 5] [ 6] [ 7] [ 8]   |     * Encoder B location = Key[4]
//     |      [ 9] [10] [11] [12]   |      NOTE: The mode button is not row/column key, it's directly wired to A0!!
//     |----------------------------|

// Variables that will change:
int modePushCounter = 0;       // counter for the number of button presses
int buttonState = 0;           // current state of the button
int lastButtonState = 0;       // previous state of the button
int mouseMove;

long positionEncoderA  = -999; //encoderA LEFT position variable
long positionEncoderB  = -999; //encoderB RIGHT position variable

const int ModeButton = A0;     // the pin that the Modebutton is attached to
const int pot = A1;           // pot for adjusting attract mode demoTime or mouseMouse pixel value
//const int Mode1= A2;
//const int Mode2= A3; //Mode status LEDs

byte rowPins[ROWS] = {4, 5, A3 };    //connect to the row pinouts of the keypad
byte colPins[COLS] = {6, 7, 8, 9 };  //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );



void setup() {
  // put your setup code here, to run once:
pinMode(ModeButton, INPUT_PULLUP);  // initialize the button pin as a input:  
  Serial.begin(9600); // initialize serial communication:

Serial.begin(9600);
  Keyboard.begin();

}

void checkModeButton(){
  buttonState = digitalRead(ModeButton);
  if (buttonState != lastButtonState) { // compare the buttonState to its previous state
    if (buttonState == LOW) { // if the state has changed, increment the counter
      // if the current state is LOW then the button cycled:
      modePushCounter++;
      Serial.println("pressed");
      Serial.print("number of button pushes: ");
      Serial.println(modePushCounter);
                            // Saves processor resources from updating the neoPixel colors all the time
    } 
    delay(50); // Delay a little bit to avoid bouncing
  }
  lastButtonState = buttonState;  // save the current state as the last state, for next time through the loop
   if (modePushCounter >3){       //reset the counter after 4 presses CHANGE THIS FOR MORE MODES
      modePushCounter = 0;}
}

void encoderA()
{
  long newPos = RotaryEncoderA.read()/4; //When the encoder lands on a valley, this is an increment of 4.
  
  if (newPos != positionEncoderA && newPos > positionEncoderA) {
    positionEncoderA = newPos;
    //Serial.println(positionEncoderA);
    Keyboard.press(KEY_LEFT_ARROW);
    Keyboard.release(KEY_LEFT_ARROW);                      }

  if (newPos != positionEncoderA && newPos < positionEncoderA) {
    positionEncoderA = newPos;
    //Serial.println(positionEncoderA);
    Keyboard.press(KEY_RIGHT_ARROW);
    Keyboard.release(KEY_RIGHT_ARROW);                      }
}

void encoderB()
{
  long newPos = RotaryEncoderB.read()/4; //When the encoder lands on a valley, this is an increment of 4.
  if (newPos != positionEncoderB && newPos > positionEncoderB) {
    positionEncoderB = newPos;
    //Serial.println(positionEncoderB);
    Keyboard.press(KEY_UP_ARROW);
    Keyboard.release(KEY_UP_ARROW);                      }

  if (newPos != positionEncoderB && newPos < positionEncoderB) {
    positionEncoderB = newPos;
    //Serial.println(positionEncoderB);
    Keyboard.press(KEY_DOWN_ARROW);
    Keyboard.release(KEY_DOWN_ARROW);                      }
}


//=============Encoder A & B Function ====== Set 1 =========================================================
void encoderA_Mode1(){
  long newPos = RotaryEncoderA.read()/2; 
  if (newPos != positionEncoderA && newPos < positionEncoderA) {
    positionEncoderA = newPos;
     //tab increase
    Keyboard.write(9); //tab key
     }

  if (newPos != positionEncoderA && newPos > positionEncoderA) {
    positionEncoderA = newPos;
    //tab decrease
    Keyboard.press(KEY_LEFT_SHIFT); 
    Keyboard.write(9); //tab key
    Keyboard.release(KEY_LEFT_SHIFT);                      }

}

void encoderB_Mode1(){
  long newPos = RotaryEncoderB.read()/2; 
  if (newPos != positionEncoderB && newPos < positionEncoderB) {
    positionEncoderB = newPos;
      //Font decrease | Arduino IDE
    Keyboard.press(KEY_LEFT_CTRL); 
    Keyboard.press('-');
    Keyboard.release('-');  Keyboard.release(KEY_LEFT_CTRL);         }

  if (newPos != positionEncoderB && newPos > positionEncoderB) {
    positionEncoderB = newPos;
      //Font increase | Arduino IDE
    Keyboard.press(KEY_LEFT_CTRL); 
    Keyboard.press('=');
    Keyboard.release('=');   Keyboard.release(KEY_LEFT_CTRL);       }
}

//=============Encoder A & B Function ====== Set 2 =========================================================
void encoderA_Mode2(){ //testing some encoder wheel pay control for arcade games; centede, tempest...

  long newPos = RotaryEncoderA.read()/2; 
  if (newPos != positionEncoderA && newPos > positionEncoderA) {
    positionEncoderA = newPos;
    //Serial.println(mouseMove);
     Mouse.move(-mouseMove,0,0); //moves mouse right... Mouse.move(x, y, wheel) range is -128 to +127
                        }

  if (newPos != positionEncoderA && newPos < positionEncoderA) {
    positionEncoderA = newPos;
    Mouse.move(mouseMove,0,0); //moves mouse left... Mouse.move(x, y, wheel) range is -128 to +127
                         }
}

void encoderB_Mode2(){
  long newPos = RotaryEncoderB.read()/2; //When the encoder lands on a valley, this is an increment of 2.
  if (newPos != positionEncoderB && newPos < positionEncoderB) {
    positionEncoderB = newPos;
    Mouse.move(0,-mouseMove,0);                                                           }

  if (newPos != positionEncoderB && newPos > positionEncoderB) {
    positionEncoderB = newPos;
    Mouse.move(0,mouseMove,0);            
                                                              }
}

//=============Encoder A & B Function ====== Set 3 =========================================================
void encoderA_Mode3(){
  long newPos = RotaryEncoderA.read()/2; 
  if (newPos != positionEncoderA && newPos > positionEncoderA) {
    positionEncoderA = newPos;
    Mouse.press(MOUSE_LEFT); //holds down the mouse left click
    Mouse.move(0,4,0); //moves mouse down... Mouse.move(x, y, wheel) range is -128 to +127
    Mouse.release(MOUSE_LEFT); //releases mouse left click
                                                               }

  if (newPos != positionEncoderA && newPos < positionEncoderA) { 
    positionEncoderA = newPos;
    Mouse.press(MOUSE_LEFT); //holds down the mouse left click
    Mouse.move(0,-4,0); //moves mouse up... Mouse.move(x, y, wheel) range is -128 to +127
    Mouse.release(MOUSE_LEFT); //releases mouse left click                   
                                                              }
}

void encoderB_Mode3(){
  long newPos = RotaryEncoderB.read()/2;
  if (newPos != positionEncoderB && newPos > positionEncoderB) {
    positionEncoderB = newPos;
    Mouse.press(MOUSE_LEFT); //holds down the mouse left click
    Mouse.move(-4,0,0); //moves mouse left... Mouse.move(x, y, wheel) range is -128 to +127  
    Mouse.release(MOUSE_LEFT); //releases mouse left click
                                                               }

  if (newPos != positionEncoderB && newPos < positionEncoderB) {
    positionEncoderB = newPos;
    Mouse.press(MOUSE_LEFT); //holds down the mouse left click
    Mouse.move(4,0,0); //moves mouse right... Mouse.move(x, y, wheel) range is -128 to +127
    Mouse.release(MOUSE_LEFT); //releases mouse left click                   
                                                              }
}

void loop() {
  // put your main code here, to run repeatedly:
char key = keypad.getKey();
mouseMove = (analogRead(pot)); //reading the analog input, pot = pin A1
mouseMove = map(mouseMove, 0,1023, 1,124); //remap the analog pot values fron 1 to 124
checkModeButton();

  switch (modePushCounter) {                  // switch between keyboard configurations:
    case 0:                                   //Application Alpha or MODE 0. Example = Every button ends your Zoom call
    encoderA_Mode1();                         //custom function for encoder A
    encoderB_Mode1();                         //custom function for encoder A     
       
if (key) {
    //Serial.println(key);
    switch (key) {
      case '1': Keyboard.press(KEY_LEFT_CTRL);  
                Keyboard.print('s');                          //Save
                break;
      case '2': Keyboard.press(KEY_LEFT_CTRL);  
                Keyboard.print('r');                          //Verify
                break; 
      case '3': Keyboard.press(KEY_LEFT_CTRL);  
                Keyboard.print('u');                          //Upload
                break;
      case '4': Keyboard.press(KEY_LEFT_CTRL);  
                Keyboard.press(KEY_LEFT_SHIFT);
                Keyboard.print('m');                          //Serial Monitor
                break; 
      case '5': Keyboard.press(KEY_LEFT_CTRL);                //undo
                Keyboard.print('z'); 
                break;
      case '6': Keyboard.press(KEY_LEFT_CTRL);                //redo
                Keyboard.print('y'); 
                break;
      case '7': Keyboard.press(KEY_LEFT_CTRL);  
                Keyboard.press(KEY_LEFT_SHIFT); 
                Keyboard.print('g');                          //Find Previous
                break; 
      case '8': Keyboard.press(KEY_LEFT_CTRL);  
                Keyboard.print('g');                          //Find Next
                break; 
      case '9': Keyboard.press(KEY_LEFT_CTRL);  
                Keyboard.print('c');                          //Copy
                break;
      case '0': Keyboard.press(KEY_LEFT_CTRL);  
                Keyboard.print('v');                          //Paste
                break;
      case 'A': Keyboard.press(KEY_LEFT_CTRL);  
                Keyboard.print('/');                          //Comment / Uncomment
                break;
      case 'B': Keyboard.press(KEY_LEFT_CTRL);        
                Keyboard.print('f');                          // Find
                break;

    }
    delay(100); Keyboard.releaseAll(); // this releases the buttons 
  }
      break;

  }
}




