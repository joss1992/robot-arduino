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

/* LED */
// pins
int LedPIN = 13;  // Switch

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
int freqX = 0;
int freqY = 0;// frequency between to steps of the engines, to be computed
int minDelay = 50;   // minimum delay between engine steps
int maxDelay = 4900;  // maximum delay between engine steps (above=no speed)

void setup()
{
  // setup joysticks pins
  pinMode(X_pin, INPUT);
  pinMode(Y_pin, INPUT);
  pinMode(SW_pin, INPUT);

  // setup engine pins
  pinMode(PulseA,OUTPUT);
  pinMode(DirA,OUTPUT);
  pinMode(SlpA, OUTPUT);

  // setup engine pins
  pinMode(PulseB,OUTPUT);
  pinMode(DirB,OUTPUT);
  pinMode(SlpB, OUTPUT);
  
   // setup led pin
  pinMode(LedPIN, OUTPUT);

  // get initial joystick values
  init_var_X = analogRead(X_pin);
  init_var_Y = analogRead(Y_pin);

  //Serial.begin (115200);
}


bool ComputeWheelStep(unsigned long now, unsigned long previousStep, bool previousState, int dir, int sens, int pulse, int Slp){
  // result variable
  bool res = false;

  // read joystick values
  val_var_X = analogRead(X_pin); // varie de 0 à 1023
  val_var_Y = analogRead(Y_pin);// varie de 0 à 1023
  val_sw = analogRead(SW_pin);

  // compute frequency
  freqX = val_var_X - init_var_X; // varie de -511 à 512
  freqY = val_var_Y - init_var_Y; // varie de -511 à 512
  
  // infer engines steps delay from frequency
  int delai;
  delai = maxDelay + 100 - 10*abs(freqY)- 10*sens*freqX;

  // if delay small enough, move
  if ((abs(freqX) > 50) || ( abs(freqY) > 50)){
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
      if (sens*(freqY+freqX) >= 0){
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


  digitalWrite(LedPIN, HIGH);
  delay(150);
  digitalWrite(LedPIN, LOW);
  delay(100);
  digitalWrite(LedPIN, HIGH);
  delay(150);
  digitalWrite(LedPIN, LOW);
  delay(100);
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
  delayMicroseconds(10);
  //Serial.println(freqY);

}
