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

int ledPin = 13;  // LED Integree


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


void loop()
{
  val_var_X = analogRead(X_pin);
  val_var_Y = analogRead(Y_pin);
  val_sw = analogRead(SW_pin);
  freqY = val_var_Y - init_var_Y;
  delaiA = 5000 - 10*abs(freqY);
  if (delaiA <= minDelay){
    digitalWrite(Slp,HIGH);
    if (freqY >= 0){
      digitalWrite(DirA,HIGH);
      digitalWrite(ledPin,HIGH);
      digitalWrite(PulseA,HIGH);
      delayMicroseconds(minDelay);
      digitalWrite(PulseA,LOW);
    }
    else {
      digitalWrite(DirA,LOW);
      digitalWrite(ledPin,HIGH);
      digitalWrite(PulseA,HIGH);
      delayMicroseconds(minDelay);
      digitalWrite(PulseA,LOW);
    }
    
    delayMicroseconds(minDelay);
  }
  else if (delaiA >= 2000){
    digitalWrite(Slp,LOW);
    digitalWrite(ledPin,HIGH);
    delay(200);
    digitalWrite(ledPin,LOW);
    delay(200);
  }
  else {
    digitalWrite(Slp,HIGH);
    if (freqY >= 0){
      digitalWrite(DirA,HIGH);
      digitalWrite(ledPin,HIGH);
      digitalWrite(PulseA,HIGH);
      delayMicroseconds(delaiA);
      digitalWrite(PulseA,LOW);
    }
    else {
      digitalWrite(DirA,LOW);
      digitalWrite(ledPin,HIGH);
      digitalWrite(PulseA,HIGH);
      delayMicroseconds(delaiA);
      digitalWrite(PulseA,LOW);
    }
    delayMicroseconds(delaiA);
  }
  //Serial.println(freqY);
  
}

