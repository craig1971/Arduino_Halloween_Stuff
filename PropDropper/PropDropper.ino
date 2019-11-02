
#include "Arduino.h";
#include "SoftwareSerial.h";
#include "DFRobotDFPlayerMini.h";

#include <Servo.h>

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

Servo dropServo;  // create servo object to control a servo
Servo spoolServo;  // create servo object to control a servo

int testButtonPin = 7;
int ledPin        = 13;

int dropServoPin = 9;
int dropServoUp   = 10;
int dropServoDown = 190;

int spoolServoPin = 12;
int spoolServoCcwFull = 45;
int spoolServoCcwSlow = 80;
int spoolServoStop    = 90;
int spoolServoCwSlow  = 100;
int spoolServoCwFull  = 135;

boolean propDown = false;

void setup() { 
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);  
  
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));  
  
  myDFPlayer.volume(20);  //Set volume value. From 0 to 30
  
  pinMode(testButtonPin,  INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);   
}

void loop() {

  // read the state of the pushbutton value:
  int buttonState = digitalRead(testButtonPin);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == LOW) {
    // turn LED on:
    digitalWrite(ledPin, HIGH);
    myDFPlayer.play(1);
    delay(1500);
    dropProp(); 
    delay(6000);
    retrieveProp(); 
    digitalWrite(ledPin, LOW);    
  }
  
  //Print the detail message from DFPlayer to handle different errors and states.
  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read());
  }

  
}

void dropProp() {  
  if (!propDown) { 
    dropServo.attach(dropServoPin);
    dropServo.write(dropServoDown);
    delay(1000);
    dropServo.detach();
    
    propDown = true;
  }
}


void retrieveProp() {
  if (propDown) {
    dropServo.attach(dropServoPin);
    dropServo.write(dropServoUp); 
    delay(1000);
    dropServo.detach();
    
    spoolServo.attach(spoolServoPin);
    spoolServo.write(spoolServoCwFull); 
    delay(3000);
    spoolServo.write(spoolServoStop); 
    spoolServo.detach();
    
    propDown = false;
  }
}

void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}

