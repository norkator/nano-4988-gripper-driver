/*
 * L298N stepper driver gripper tester (Nano) ATmega328p
 * nano, gripper, film pressure sensor, homing switch, open close gipper push button
 */

// ## Libraries ##
#include <Stepper.h>

// ########################
// ## PIN CONFIGURATIONS ##
// ########################
int motorA1         = 2;  // stepper A1
int motorA2         = 3;  // stepper A2
int motorB1         = 4;  // stepper B1
int motorB2         = 5;  // stepper B2
int endStopPin      = 7;    // homing endstop
int toggleBtnPin    = 12;   // toggles direction, open and close
int firmPressurePin = A0;   // Firm pressure strip sensor pin, sensor connected with 1kΩ resistor.


// Variables
const int stepsPerRevolution = 200;   // steps per revolution
Stepper myStepper(stepsPerRevolution, motorA1, motorA2, motorB1, motorB2);
const int stepperMotorSpeed = 150;    // stepper motor speed
const int gripperStepsFullyOpen = -1700; // how many steps needed to reverse when gripper is fully open
boolean homingDone = false;
boolean gripperOpen = true; // after homing its open
boolean gripperClosed = false;
int gripPressureValue = 0;
int currentStepPosition = 0;
int gripPressureLimit = 700;


// SETUP
void setup() {
  // Init serial
  Serial.begin(9600);

  // Init stepper
  myStepper.setSpeed(stepperMotorSpeed);
  
  // Pin modes
  pinMode(endStopPin, INPUT_PULLUP);
  pinMode(toggleBtnPin, INPUT_PULLUP);
}


// LOOP
void loop() {
  if (!homingDone) {
    homing();
  }

  readGripperPressure();

  // Gripper toggle state
  int tInputVal = digitalRead(toggleBtnPin);
  if (tInputVal == LOW) {
    gripperOpen = !gripperOpen;
    delay(1000); // wait so that toggle is not read again multiple times
  }

  // Gripper control
  if (gripperOpen) {
    if (gripperClosed == true) {
      openGripper();
    }
  } else {
    if (gripperClosed == false) {
      closeGripper();
    }
  }

  turnOffStepper();
}


/**
 * Homing procedure
 * 1. close gripper more till hit estop
 * 2. open gripper fully
 * 3. set homing done
 */
void homing() {
  Serial.println("Homing gripper...");
  int steps = 0;
  int eStopVal = HIGH;
  while (eStopVal == HIGH) {
    eStopVal = digitalRead(endStopPin);
    myStepper.step(1);
    steps++;
  }
  Serial.print("Gripper endstop hit at ");
  Serial.print(steps);
  Serial.println(" steps.. gripper is fully closed");
  myStepper.step(gripperStepsFullyOpen); // open fully
  currentStepPosition = gripperStepsFullyOpen;
  Serial.println("Gripper fully open, saving current position");
  homingDone = true;
}


/**
 * 
 */
void closeGripper() {
  int steps = 0;
  int eStopVal = HIGH;
  while (eStopVal == HIGH && gripPressureValue < gripPressureLimit) {
    readGripperPressure();
    eStopVal = digitalRead(endStopPin);
    myStepper.step(1);
    steps++;
  }
  currentStepPosition = steps;
  gripperClosed = true;
}


/**
 * 
 */
void openGripper() {
  myStepper.step(-currentStepPosition);
  gripperClosed = false;
}


/**
 * Turn off stepper driver
 * solves stepper driver overheating problem
 */
void turnOffStepper() {
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, LOW);
}


/**
 * 
 */
void readGripperPressure() {
  // Read firm pressure sensor (force sensing resistor?)
  gripPressureValue = analogRead(firmPressurePin);
}
