/* Engine A */
// pins
int PulseA = 4;                         // Borne Pulse
int DirA = 3;                           // Borne Dir
int SlpA = 2;                           // Borne Sleep
// Variables
int sensA = 1;                          // Direction
unsigned long previousLeftStep = 0;     // Time of last step
bool previousLeftState = false;         // State of last step

/* Engine B */
// pins
int PulseB = 7;                         // Borne Pulse
int DirB = 6;                           // Borne Dir
int SlpB = 5;                           // Borne Sleep du Driver
// variables
int sensB = -1;                         // Direction
unsigned long previousRightStep = 0;    // Time of last step
bool previousRightState = false;        // State of last step

/* Joystick */
// pins
int SW_pin = A2;  // Switch
int X_pin = A0;  // Borne VAR X du Switch
int Y_pin = A1;  // Borne VAR X du Switch

// variables
int val_sw = 0;
int val_var_X = 0;
int val_var_Y = 0;
int init_var_X = 0;
int init_var_Y = 0;

/* Led */
// pin
int ledPin = 13;  // LED Integree

/* Global variables */
int freqY = 0;        // frequency between to steps of the engines, to be computed
int minDelay = 200;   // minimum delay between engine steps
int maxDelay = 2000;  // maximum delay between engine steps (above=no speed)

void setup()
{
  // setup joysticks pins
  pinMode(X_pin, INPUT);
  pinMode(Y_pin, INPUT);
  pinMode(SW_pin, INPUT);

  // setup engine pins
  pinMode(PulseA,OUTPUT);
  pinMode(DirA,OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(Slp, OUTPUT);

  // get initial joystick values
  init_var_X = analogRead(X_pin);
  init_var_Y = analogRead(Y_pin);

  //Serial.begin (115200);
}


void ComputeWheelStep(unsigned long previousStep, bool previousState, int dir, int sens, int pulse){
  // read joystick values
  val_var_X = analogRead(X_pin);
  val_var_Y = analogRead(Y_pin);
  val_sw = analogRead(SW_pin);

  // compute frequency
  freqY = val_var_Y - init_var_Y;

  // infer engines steps delay from frequency
  int delai;
  delai = 5000 - 10*abs(freqY);

  // if delay small enough, move
  if (delai <= maxDelay){
    // if delay too small, threshold it
    if (delai <= minDelay){
      delai = minDelay;
    }
    // convert int delay into unsigned long type (to handle big numbers)
    unsigned long delaius;
    delaius = (unsigned long) delaiA;
    unsigned long now = micros();
    // if last step is far enough, make new step
    if (now - previousStep >= delaius){
      digitalWrite(Slp,HIGH);
      // choose the direction of the step
      if (sens*freqY >= 0){
        digitalWrite(dir, HIGH);
      }
      else {
        digitalWrite(dir,LOW);
      }
      // execute the step
      previousState = !previousState; // etait = ~ previousLeftState;
      digitalWrite(pulse, previousState);
      // update last step time
      // THIS MAY CAUSE A FAIL, IDK IF THE previousStep variable is updated globally
      previousStep = now;
    }
  }
}

void loop()
{
  // Compute step for left wheel
  ComputeWheelStep(previousLeftStep, previousLeftState, DirA, sensA, PulseA);
  // Compute step for right wheel
  ComputeWheelStep(previousRightStep, previousRightState, DirB, sensB, PulseB);
  delayMicroseconds(20);
  //Serial.println(freqY);

}
