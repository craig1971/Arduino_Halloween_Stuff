// Smoke Machine Timer/Controller
//
// Note: All times are in milliseconds.
//
#define SMOKE_SIGNAL         13     // Output to control the smoke machine connect this to a relay on the machine
#define FIRE_NOW_BUTTON      12     // Button to activate the smoke on demand
#define PAUSE_SWITCH         11     // Switch to pause the smoke
#define FIRE_DELAY_INPUT     A0     // Duration input potentiometer
#define REHEAT_DELAY_INPUT   A1     // Interval input potentiometer

#define INPUT_MIN     0       // minimum potentiometer value (typically 0)
#define INPUT_MAX  1023       // maximum potentiometer value (typically 1023)

// Length of time that the smoke will be ON each cycle
// SMOKE_TIME_MIN is the time value assigned when the Pot is all the way down (1  second)
// SMOKE_TIME_MAX is the time value assigned when the Pot is all the way up   (10 seconds)
#define SMOKE_TIME_MIN  1000 * 0.5
#define SMOKE_TIME_MAX  1000 * 10
unsigned long fireDelay = SMOKE_TIME_MIN;  // the current delay setting

// Length of time that the smoke will be OFF each cycle
// REHEAT_DELAY_MIN is the time value assigned when the Pot is all the way down (30  second)
// REHEAT_DELAY_MAX is the time value assigned when the Pot is all the way up   (3 minutes)
// Length of time that the smoke will be OFF each cycle
#define REHEAT_DELAY_MIN  1000 * 60 * 0.5       
#define REHEAT_DELAY_MAX  1000 * 60 * 3
unsigned long reheatDelay    = REHEAT_DELAY_MIN;

unsigned long lastTime = 0; // Start time of the current timmed cycle segment (ON or OFF)

bool fireNowWasJustOn = false;

int REHEAT = 0;
int FIRE   = 1;
int current_mode   = 99;

void setup() {  
  pinMode(FIRE_NOW_BUTTON,    INPUT);
  pinMode(PAUSE_SWITCH,       INPUT);
  pinMode(FIRE_DELAY_INPUT,   INPUT);
  pinMode(REHEAT_DELAY_INPUT, INPUT);
  pinMode(SMOKE_SIGNAL,       OUTPUT);  
  
  turnSmoke_OFF();
}

void loop() {

  if ( digitalRead(PAUSE_SWITCH) ) {
    // Nothing to do
    // but reset the global cycle segment start time value
    lastTime = millis();
  
  } else if (digitalRead(FIRE_NOW_BUTTON)) {
    turnSmoke_ON();
    fireNowWasJustOn = true;

  } else if (fireNowWasJustOn) {
    turnSmoke_OFF();
    
  } else {
  
    readPotentiometers();
    
    unsigned long deltaTime = millis() - lastTime;
  
    if ( current_mode == FIRE ) {
      if (deltaTime >= fireDelay) { turnSmoke_OFF(); }
        
    } else if ( current_mode == REHEAT ) {
      if ( deltaTime >= reheatDelay)  { turnSmoke_ON(); }
      
    } else { 
      // We are in some unknown state.  
      // This should not happen, lets force the of smoke off state.
      turnSmoke_OFF();
    }
  }
}

void readPotentiometers() {
      
  // 1. read the potentiometer value
  // 2. map the pot value to our predefined MIN and MAX values
  // 3. constrain the delay to our predefined MIN and MAX values
  // 4. update the global delay value (fireDelay & reheatDelay)
  
  unsigned long fireInputValue = analogRead(FIRE_DELAY_INPUT);
  fireInputValue = map(fireInputValue, INPUT_MIN, INPUT_MAX, SMOKE_TIME_MIN, SMOKE_TIME_MAX);
  fireDelay = constrain(fireInputValue, SMOKE_TIME_MIN, SMOKE_TIME_MAX);
  
  unsigned long reheatInputValue = analogRead(REHEAT_DELAY_INPUT);  
  reheatInputValue = map(reheatInputValue, INPUT_MIN, INPUT_MAX, REHEAT_DELAY_MIN, REHEAT_DELAY_MAX);
  reheatDelay = constrain(reheatInputValue, REHEAT_DELAY_MIN, REHEAT_DELAY_MAX);

}

void turnSmoke_ON() {
  
  lastTime = millis();   // update the global cycle segment start time value
  current_mode = FIRE;
  digitalWrite(SMOKE_SIGNAL, HIGH);
  Serial.print("smoking " );
  Serial.println(fireDelay);
}


void turnSmoke_OFF() {
  
  lastTime = millis();   // start delay
  current_mode = REHEAT;
  fireNowWasJustOn = false;
  digitalWrite(SMOKE_SIGNAL, LOW);
  Serial.print("reheating " );
  Serial.println(reheatDelay);
}

