/*
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                                              ______________________________________________
                                             |                                              |  
                ^ +Y                         |                      /\                      |  
                |                      ___   |     LEFT            /  \         RIGHT       |   ___
              __|__                   /   \  |  ___________       /    \      ___________   |  /   \
             /  |  \                  |   |  | |  PINS A   |     /__  __\    |   PINS B  |  |  |   |
            /       \                 |MOT|__| |DirA = 10  |        ||       |DirB = 2   |  |__|MOT|
    -X ----| - -|- - |-----> +X       | A |--| |SlpA = 11  |        ||       |SlpB = 3   |  |--| B |
            \       /                 |   |  | |PulseA = 12|        ||       |PulseB = 4 |  |  |   |
             \__|__/                  \___/  | '-----------'        ||       '-----------'  |  \___/
                |                            |                      ||                      |
                | -Y                         |                                              |  
                                             |______________________________________________|  


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/


/* MOTOR A */
// pins
int DirA = 10;                           // Borne Dir motor A
int SlpA = 11;                           // Borne Sleep motor A
int PulseA = 12;                         // Borne Pulse motor A    <---------------------------------------------------- Changement no de pins
// Variables
int sensA = 1;                          // Direction Motor B
unsigned long previousLeftStep = 0;     // Time of last step
bool previousLeftState = false;         // State of last step

/* MOTOR B */
// pins
int DirB = 2;                           // Borne Dir Motor B
int SlpB = 3;                           // Borne Sleep du Driver Motor B
int PulseB = 4;                         // Borne Pulse Motor B   <---------------------------------------------------- Changement no de pins
// variables
int sensB = -1;                         // Direction Motor B
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
int Slp = 0;

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


bool ComputeWheelStep(unsigned long now, unsigned long previousStep, bool previousState, int dir, int sens, int pulse, int Slp){
  // result variable
  bool res = false;

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
    delaius = (unsigned long) delai;
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
      //previousState = !previousState; // etait = ~ previousLeftState;
      digitalWrite(pulse, previousState);
      // update last step time
      // THIS MAY CAUSE A FAIL, IDK IF THE previousStep variable is updated globally
      //previousStep = now;
      res = true;
    }
  }
  else{  digitalWrite(Slp, LOW);} //  <---------------------------------------------------- Ajout ici
  return res;
}

void loop()
{
  // get current time
  unsigned long now = micros();
  // Compute step for left wheel
  bool moveLeft = ComputeWheelStep(now, previousLeftStep, previousLeftState, DirA, sensA, PulseA, SlpA);
  if (moveLeft){
    previousLeftStep = now;
    previousLeftState = !previousLeftState;
  }
  // Compute step for right wheel
  bool moveRight = ComputeWheelStep(now, previousRightStep, previousRightState, DirB, sensB, PulseB, SlpB);
  if (moveRight){
    previousRightStep = now;
    previousRightState = !previousRightState;
  }
  // wait before next iteration
  delayMicroseconds(20);
  //Serial.println(freqY);

}
