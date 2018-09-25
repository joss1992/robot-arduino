/*Arduino pro mini :

Arduino Pro Mini Board has 14 digital pins that can be used as input or output, where the pins work at the 5V voltage, 
and each pin can provide or receive 20mA current, and has pull-up resistance of about 20-50k ohm (by default in Disconnect position). 
The maximum value is 40mA, which is avoided as much as possible to avoid damage to the microcontroller chip
Some pins that have special functions on Arduino pro mini are as follows.

- Serial, consisting of 2 pins: pin 0 (RX) and pin 1 (TX) used to receive (RX) and send (TX) serial data.
- External Interrupts, ie pin 2 and pin 3. Both pins can be used to enable interrupts. Use the attachInterrupt () function
- PWM: Pin 3, 5, 6, 9, 10, and 11 provide an 8-bit PWM output using the analogWrite () function
- SPI: Pin 10 (SS), 11 (MOSI), 12 (MISO), and 13 (SCK) support SPI communications using the SPI Library
- LED: Pin 13. On pin 13 connected built-in led controlled by digital pin no 13.

The Arduino Pro Mini has 8 analog inputs, marked A0 through A7. Each of these analog pins has 10 bits resolution (so it can have 1024 values).
By default, the pins are measured from ground to 5V, but can also use REF pins by using the analogReference () function.

In addition to the Analog A6 and A7 Pin can not be used as a digital pin, just as analog. Some other pins on this board are as follows.

I2C: Pin A4 (SDA) and A5 (SCL). This pin supports I2C (TWI) communication using Wire Library.
Reset. Connect to LOW to reset the microcontroller. Usually used to be connected with switches used as a reset button.*/

/* MOTOR A */
// pins
int PulseA = 4;                         // Borne Pulse motor A
int DirA = 3;                           // Borne Dir motor A
int SlpA = 2;                           // Borne Sleep motor A
// Variables
int sensA = 1;                          // Direction Motor B
unsigned long previousLeftStep = 0;     // Time of last step
bool previousLeftState = false;         // State of last step

/* MOTOR B */
// pins
int PulseB = 7;                         // Borne Pulse Motor B
int DirB = 6;                           // Borne Dir Motor B
int SlpB = 5;                           // Borne Sleep du Driver Motor B
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
