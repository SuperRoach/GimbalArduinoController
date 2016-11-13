/* Gimbal Controller
 by Brett James / SuperRoach (@SuperRoach on twitter)
 This code is in the public domain.

 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo modecontrol, pitch, yaw;  // create servo object to control a servo

/* 
// Arduino Nano 
int potPin = 2;    // select the input pin for the potentiometer
int ledPin = 13;   // select the pin for the LED
int potVal = 0;       // variable to store the value coming from the sensor
*/

// Arduino Mini Pro
// The board I'm using has a spare breakout for A6/A7 at the rear. these are analog input only,
// Which is okay but it means I can't use one of the pins as power. 
// I need to emulate through a digital pin elsewhere
int potPin = A6;    // select the input pin for the potentiometer
int potPower = 9; // anything that can output a high digital signal.
int ledPin = 13;   // select the pin for the LED
int potVal = 0;       // variable to store the value coming from the sensor
int buttonPin = 8; // Push button
int buttonGnd = 7; // if needed
int buttonState = 0;  

int servoMap = 0;
int previousServoMap = 0;

int modePin = 3;
int pitchPin = 5; // Blue Cable
int yawPin = 6; // White Cable

int gimbalModeState = 0;
String potControllerState = "pitch";

// Debounce stuff
int reading;           // the current reading from the input pin
int previous = LOW;    // the previous reading from the input pin
int state = HIGH;      // the current state of the output pin

// the follow variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long time = 0;         // the last time the output pin was toggled
long mydebounce = 200;   // the debounce time, increase if the output flickers


void setup() {

  
    Serial.begin(9600);

  pinMode(ledPin, OUTPUT);
  pinMode(A6, INPUT);
  pinMode(potPower, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);  
  pinMode(buttonGnd, OUTPUT);  
  digitalWrite(buttonGnd, LOW);
  digitalWrite(potPower, HIGH);
  
  //modecontrol.attach(modePin);  // attaches the Brown and Black cable (number is for brown one)
  //modecontrol.write( setControlMode( 2 ) );

// pitch
  pitch.attach(pitchPin);  // attaches the servo on pin 9 to the servo object
  pitch.write(110);  


// yaw
// 90 - infront
// 150 - Max Left


//  yaw.attach(6);
//  yaw.write(0);
}

void loop() {
  /*
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  */

// Stock: Mode 1, Switch on: Mode 2: Knob controls Pitch
// option: double switch hits mode 3?? tricky to do.

  
    previousServoMap = servoMap;

       int b = checkButton();
       //Serial.println(b);
    buttonState = digitalRead(buttonPin);
    potVal = analogRead(potPin);    // read the value from the sensor

  if (reading == HIGH && previous == LOW && millis() - time > mydebounce) {
    if (state == HIGH)
      buttonState = LOW;
    else
      buttonState = HIGH;

    time = millis();    
  }
    previous = reading;

      if (buttonState == LOW) { 
      /*
        if (gimbalModeState != 2 ) { 
          modecontrol.write( setControlMode( 3 ) );
          //gimbalModeState = 2;
         // Serial.println("mode two");
        }
        */
      } else  {
      /* if (gimbalModeState != 1 ) { 
          modecontrol.write( setControlMode( 1 ) );
          //gimbalModeState = 1;
        //  Serial.println("mode one");
        }
        */
      }
   delay(100);
  //digitalWrite(ledPin, HIGH);  // turn the ledPin on
  //delay(potVal);                  // stop the program for some time
  //digitalWrite(ledPin, LOW);   // turn the ledPin off
  //delay(potVal);                  // stop the program for some time

// Control the Gimbal Mode

if (b == 2) { // Double Click
  digitalWrite(ledPin, HIGH);  // turn the ledPin on
  if (potControllerState == "pitch")
  { potControllerState = "yaw";
    Serial.println("Long Press Activated, setting Pot control to Yaw" );
    delay(40);
    digitalWrite(ledPin, LOW);  // turn the ledPin on
    delay(75);
    digitalWrite(ledPin, HIGH);  // turn the ledPin on
    delay(40);
  } else
  { potControllerState = "pitch";
    Serial.println("Long Press Activated, setting Pot control to Pitch" );
    delay(150);
    digitalWrite(ledPin, LOW);  // turn the ledPin on
    delay(75);
    digitalWrite(ledPin, HIGH);  // turn the ledPin on
    delay(150);
  }

  digitalWrite(ledPin, LOW);  // turn the ledPin on
}

if (b == 4) // Very Long Press
{
   if ( !modecontrol.attached() )
   { modecontrol.attach(modePin);
   Serial.println("Somethings wrong, attaching");
   }
   if (gimbalModeState == 3) {
    gimbalModeState = 0;
   }
   gimbalModeState++;
   modecontrol.write( setControlMode( gimbalModeState ) );
   Serial.println("very long press");
   digitalWrite(ledPin, HIGH);  // turn the ledPin on
   delay(400);
   for (int i=1; i <= gimbalModeState; i++)
   {delay(100);
   digitalWrite(ledPin, LOW);  // turn the ledPin on
   delay(250);
   digitalWrite(ledPin, HIGH);  // turn the ledPin on
   delay(100);
   digitalWrite(ledPin, LOW);  // turn the ledPin on
   }
   Serial.println("well its done for");
}


// Set the servo/pwm value
//Serial.println(previousServoMap);
servoMap = map(potVal, 0, 1023, 0, 180);

if (previousServoMap != servoMap)
  {
   if ( !pitch.attached() )
   { pitch.attach(pitchPin);
   Serial.println("Somethings wrong");
   }
    Serial.println("changing it");

if (potControllerState == "pitch" ) 
{
    pitch.write(servoMap);
} else
{
   if ( !yaw.attached() )
   { yaw.attach(yawPin);
   Serial.println("Yaw Active");
   }
   yaw.write(servoMap);
}
    Serial.println(servoMap);
  }
  else
  {
   // pitch.detach();
  }
}



int setControlMode(int x){
// Set the mode up for the three different states via PWM channel.
// Mode 1: "HEADING FOLLOW" Standard. Looks forward to the front. YAW: Locked, PITCH: Controllable
// Mode 2: "HEADING AND PITCH FOLLOW" Yaw is not automated. YAW: Controllable, PITCH: Controllable
// Mode 3: "HEADING LOCK" All Axis are automated YAW: Locked, PITCH: Locaked

  int result;

switch (x) {
    case 1:
      result = 0;
      break;
    case 2:
      result = 45;
      break;
    case 3:
      result = 90;
      break;
    default:
      result = 0;
    break;
  }
  return result;
}

