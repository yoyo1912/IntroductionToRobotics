/*
Homework 4
Student: Ioana-Cristina Ursoiu
Group: 364 (Tuesday 9-12)
*/

// Pins 7-segment display
const int pinA = 12;
const int pinB = 10;
const int pinC = 9;
const int pinD = 8;
const int pinE = 7;
const int pinF = 6;
const int pinG = 5;
const int pinDP = 4;

// Pins joystick
const int pinSW = 2;
const int pinX = A0;
const int pinY = A1;

const int segSize = 8;

int segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

const int segmentNeighbors[8][4] = {
  // UP    DOWN  LEFT  RIGHT
  {0, 6, 5, 1}, // a  (0)
  {0, 6, 5, 1}, // b  (1)
  {6, 3, 4, 7}, // c  (2)
  {6, 3, 4, 2}, // d  (3)
  {6, 3, 4, 2}, // e  (4)
  {0, 6, 5, 1}, // f  (5)
  {0, 3, 6, 6}, // g  (6)
  {7, 7, 2, 7}  // dp (7)
};

const int noOfSegments = 8;

byte segmentState[noOfSegments] = {0}; // 0: OFF, 1: ON
byte currentSegmentBlinking = 0;
bool lastButtonState = HIGH;
bool buttonState = HIGH;
int currentSegment = 7; // Start at the decimal point
bool buttonPressed = false;
unsigned long buttonPressStartTime = 0;
unsigned long lastJoystickChangeTime = 0;
bool joystickStateChanged = true;

unsigned long lastBlinkTime = 0;
unsigned long blinkInterval = 500;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 300;

void setup() {
  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);
  }
  pinMode(pinSW, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinSW), handleInterrupt, FALLING);
  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);

  digitalWrite(segments[pinDP], HIGH); // Turn on the decimal point
}

void loop() {
  int xValue = analogRead(pinX);
  int yValue = analogRead(pinY);

  // If joystick moves UP, DOWN, LEFT, RIGHT it moves to the corresponding segment 
  // If you move UP for example and the segment for UP is N/A, then it won't move
  if (millis() - lastJoystickChangeTime >= 800) {
    joystickStateChanged = false;
    if (xValue > 900 && yValue > 400 && yValue < 600) {
      moveSegment(segmentNeighbors[currentSegment][0]);
      lastJoystickChangeTime = millis();
      joystickStateChanged = true;
    } 
    else if (xValue < 100 && yValue > 400 && yValue < 600) {
      moveSegment(segmentNeighbors[currentSegment][1]);
      lastJoystickChangeTime = millis();
      joystickStateChanged = true;
    } 
    else if (yValue < 100 && xValue > 400 && xValue < 600) {
      moveSegment(segmentNeighbors[currentSegment][2]);
      lastJoystickChangeTime = millis();
      joystickStateChanged = true;
    } 
    else if (yValue > 900 && xValue > 400 && xValue < 600) {
      moveSegment(segmentNeighbors[currentSegment][3]);
      lastJoystickChangeTime = millis();
      joystickStateChanged = true;
    }
  }

  // If the button is pressed, it will toggle the segment state
  // If the button is long pressed (5 sec) it resets the display (every segment is turned off and the decimal point becomes the current segment)
  if (buttonPressed) {
    if (!buttonPressStartTime) {
      buttonPressStartTime = millis();
    }
    if (millis() - buttonPressStartTime >= 5000) {
      // Long press, reset display
      resetDisplay();
      buttonPressed = false;
    } 
    else if ((millis() - lastDebounceTime) > debounceDelay) {
      lastDebounceTime = millis();      
      buttonState = digitalRead(pinSW);
      if (buttonState != lastButtonState) {
        segmentState[currentSegment] = !segmentState[currentSegment];
        lastButtonState = !lastButtonState;
        buttonPressed = false;
      }
    }
    
  } 
  else {
    buttonPressStartTime = 0;
    buttonPressed = false;
  }

  displaySegments();
  handleBlinking();
}

// Move the segment
void moveSegment(int newSegment) {
  if (newSegment != currentSegment) {
    currentSegmentBlinking = segmentState[newSegment];
    currentSegment = newSegment;
  }
}

// Reset the display after you keep pressing for 5 sec
void resetDisplay() {
  for (int i = 0; i < noOfSegments; i++) {
    segmentState[i] = 0; // Turn off all segments
  }
  currentSegment = 7; // Start at the decimal point
}

// Display the state of the segments
void displaySegments() {
  for (int i = 0; i < noOfSegments; i++) {
    if(i != currentSegment)
      digitalWrite(segments[i], segmentState[i]);
  }
}

// Handle the blinking of the current segment
void handleBlinking() {
  if (millis() - lastBlinkTime >= blinkInterval) {
    lastBlinkTime = millis();
    currentSegmentBlinking = !currentSegmentBlinking;
    digitalWrite(segments[currentSegment], currentSegmentBlinking);
  }
}

// Handle interrupt
void handleInterrupt() {
  buttonPressed = true;
}

//PS. Interrupts are currsed, especially at 1 am