/*
Homework 2
Student: Ioana-Cristina Ursoiu
Group: 364 (Tuesday 9-12)
*/

int const redLedPin = 9;
int const blueLedPin = 10;
int const greenLedPin = 11;
int const port1Pin = A5;
int const port2Pin = A4;
int const port3Pin = A3;
int port1Value = 0;
int port2Value = 0;
int port3Value = 0;

void setup() {
  pinMode(port1Pin, INPUT);
  pinMode(port2Pin, INPUT);
  pinMode(port3Pin, INPUT);
  Serial.begin(9600);

}

void loop() {
  //Valoarea potentiometrului 1
  port1Value = analogRead(port1Pin);
  int pam1Value = map(port1Value, 0, 1023, 0, 255);

  //Valoarea potentiometrului 2
  port2Value = analogRead(port2Pin);
  int pam2Value = map(port2Value, 0, 1023, 0, 255);

  //Valoarea potentiometrului 3
  port3Value = analogRead(port3Pin);
  int pam3Value = map(port3Value, 0, 1023, 0, 255);

  //Alocarea valorilor potentiometrelor pin-urilor corespunzatoare
  analogWrite(redLedPin, pam1Value);
  analogWrite(blueLedPin, pam2Value);
  analogWrite(greenLedPin, pam3Value);

}
