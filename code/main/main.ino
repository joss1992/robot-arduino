int PulseA = 4;   // Borne Step du Driver
int DirA = 3;     // Borne DirA du Driver
int Slp = 2;     // Borne Sleep du Driver

int val_sw = 0;

int SW_pin = A2;  // Switch
int X_pin = A0;  // Borne VAR X du Switch
int Y_pin = A1;  // Borne VAR X du Switch
int val_var_X = 0;
int val_var_Y = 0;
int init_var_X = 0;
int init_var_Y = 0;
int delaiA = 0;
int freqY = 0;
int minDelay = 200;
int maxDelay = 2000;


int ledPin = 13;  // LED Integree

unsigned long previousLeftStep = 0;
bool previousLeftState = false;

void setup()
{
  pinMode(X_pin, INPUT);
  pinMode(Y_pin, INPUT);
  pinMode(SW_pin, INPUT);

  pinMode(PulseA,OUTPUT);
  pinMode(DirA,OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(Slp, OUTPUT);

  //Serial.begin (115200);

  init_var_X = analogRead(X_pin);
  init_var_Y = analogRead(Y_pin);
}

void ComputeWheelStep(){
  // read joystick values
  val_var_X = analogRead(X_pin);
  val_var_Y = analogRead(Y_pin);
  val_sw = analogRead(SW_pin);

  // compute delay
  freqY = val_var_Y - init_var_Y;
  delaiA = 5000 - 10*abs(freqY);
  if (delaiA <= maxDelay){
    if (delaiA <= minDelay){
      delaiA = minDelay;
    }

    unsigned long delaiAus = 0;
    delaiAus = (unsigned long) delaiA;
    unsigned long now = micros();
    if (now - previousLeftStep >= delaiAus){
      digitalWrite(Slp,HIGH);
      if (freqY >= 0){
        digitalWrite(DirA, HIGH);
      }
      else {
        digitalWrite(DirA,LOW);
      }

      previousLeftState = !previousLeftState; // etait = ~ previousLeftState;
      digitalWrite(PulseA, previousLeftState);

      previousLeftStep = now;
    }
  }
}

void loop()
{
  ComputeWheelStep();
  delayMicroseconds(20);
  //Serial.println(freqY);

}
