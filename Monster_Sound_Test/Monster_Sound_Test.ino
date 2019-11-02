
#include "MonsterSound.h"

#define ACTION_BUTTON A0
 
MonsterSound sounds;

void setup() 
{  
  // initialize serial communication:
  Serial.begin(9600);

  pinMode(ACTION_BUTTON, INPUT_PULLUP);

  sounds.initialize(); // Monster Sounds
  
  sounds.playRoar();
  delay(6000);
  sounds.playRoar();
  delay(6000);
  
}

void loop() 
{
  if (digitalRead(ACTION_BUTTON) == LOW)   // using a pullup, thus LOW means the button is pressed
  {
    sounds.playRoar();
    delay(6000);
  }
}


