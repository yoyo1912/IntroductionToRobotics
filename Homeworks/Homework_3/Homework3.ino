const int floor1LedPin = 5;
const int floor2LedPin = 4;
const int floor3LedPin = 3;
const int elevatorStateLedPin = 2;
const int buzzerPin = 6;

byte stateElevatorStateLed = HIGH;

const int floor1ButtonPin = 11;
const int floor2ButtonPin = 10;
const int floor3ButtonPin = 9;

const int travelTime = 3000; // Time between floors (3 seconds)
const int openCloseTime = 1500; // Time for doors to close (1.5 second)
const int arriveTime = 100; // Time for arrive (0.5 seconds)

int startFloor = 1;
int currentFloor = 1;
int targetFloor = 1;
bool moving = false;
bool doorsClose = true;
unsigned long lastFloorChangeTime = 0;
unsigned long lastDoorActionTime = 0;
unsigned long lastStateChangeElevatorStateLed = 0;

void setup() {
  pinMode(floor1ButtonPin, INPUT_PULLUP);
  pinMode(floor2ButtonPin, INPUT_PULLUP);
  pinMode(floor3ButtonPin, INPUT_PULLUP);
  pinMode(floor1LedPin, OUTPUT);
  pinMode(floor2LedPin, OUTPUT);
  pinMode(floor3LedPin, OUTPUT);
  pinMode(elevatorStateLedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  updateElevatorState();
}

void loop() {
  if(!moving && doorsClose)
    readButtons();
  if(moving)
    handleArrival(); 
}

// It reads at what floor you want to go if the elevator is not moving and the doors are closed
// I used the elevator at university as an example
void readButtons() {
    startFloor = currentFloor;
  if (digitalRead(floor1ButtonPin) == LOW) 
    targetFloor = 1;  
  else if (digitalRead(floor2ButtonPin) == LOW) 
    targetFloor = 2; 
  else if (digitalRead(floor3ButtonPin) == LOW) 
    targetFloor = 3;
  if (targetFloor != currentFloor)
    moveElevator();
}

void handleArrival()
{
  // Plays the moving sound and makes the led elevator state blink
  if(millis() - lastFloorChangeTime < travelTime && moving)
    playBuzzerSound("move");
  
  // If the travel Time for one floor passed and the current floor is not the target floor it incremets/decrements the target floor
  // It also updates the floor (the specific led lights up)
  // It moves the elevator to get the time for the last floor
  if (millis() - lastFloorChangeTime >= travelTime && currentFloor != targetFloor) 
  {    
    if(targetFloor > currentFloor)
      currentFloor++;
    else
      currentFloor--;
    updateElevatorState();
    moveElevator(); 
  }
  // If the elevator arrived it's not moving anymore, doors open and it plays the arrive sound
  // I know we shouldn't use delay, but without delay my doors are not closing
  else if(currentFloor == targetFloor && millis() - lastFloorChangeTime >= travelTime * abs(targetFloor - startFloor))
    {
      if(doorsClose == true)
        lastDoorActionTime = millis();
      moving = false;
      doorsClose = false;
      playBuzzerSound("arrive");
      delay(1000);
      digitalWrite(elevatorStateLedPin, HIGH);
      if(millis() - lastDoorActionTime >= openCloseTime)
      {
        doorsClose = true;
        playBuzzerSound("doorsClosing");
      }
    }  
    
}

// The elevator starts moving
void moveElevator() {
  lastFloorChangeTime = millis();
  playBuzzerSound("move");
  moving = true;
  doorsClose = false;
  lastStateChangeElevatorStateLed = millis();
}

// It updates the floor you are at
void updateElevatorState() {
  digitalWrite(floor1LedPin, currentFloor == 1);
  digitalWrite(floor2LedPin, currentFloor == 2);
  digitalWrite(floor3LedPin, currentFloor == 3);
  if(!moving)
    digitalWrite(elevatorStateLedPin, HIGH);
}

// It plays the specified sound
void playBuzzerSound(String soundType) {
  if (soundType == "arrive") {
    tone(buzzerPin, 1000, arriveTime); // "Cling" sound when arriving
  } 
  else if (soundType == "move") {
    if(millis() - lastStateChangeElevatorStateLed >= 100)
    {
      digitalWrite(elevatorStateLedPin, stateElevatorStateLed);
      stateElevatorStateLed = !stateElevatorStateLed;
      lastStateChangeElevatorStateLed = millis();
    }
    tone(buzzerPin, 1100); // Sound for moving
  }
  else if (soundType == "doorsClosing")
  {
    tone(buzzerPin, 2000, openCloseTime); // Sound for doors closing
  }
}
