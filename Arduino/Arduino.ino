/*
 * 4988 stepper driver gripper tester (Nano) ATmega328p
 * nano, gripper, film pressure sensor, homing switch, open close gipper push button
 */

// ## Libraries ##
/* none yet */

// ########################
// ## PIN CONFIGURATIONS ##
// ########################
int dirPin          = 2;    // 4988 driver direction pin
int stepPin         = 3;    // 4988 driver ste pin
int endStopPin      = 4;    // homing endstop
int toggleBtnPin    = 12;   // close|open signal (PC817)
int firmPressurePin = A0;   // Firm pressure strip sensor pin, sensor connected with 1kΩ resistor.


// Variables
boolean homingDone = false;
boolean gripperOpen = true; // after homing its open
const int stepsPerRevolution = 200;   // steps per revolution
const int gripperStepsFullyOpen = -1500; // how many steps needed to reverse when gripper is fully open
int gripPressureValue = 0;


// SETUP
void setup() {
  // Init serial
  Serial.begin(9600);
  // Pin modes
  pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(endStopPin, INPUT_PULLUP);
  pinMode(toggleBtnPin, INPUT_PULLUP);
}


// LOOP
void loop() {
  if (!homingDone) {
    homing();
  }

  // Read firm pressure sensor (force sensing resistor?)
  gripPressureValue = analogRead(firmPressurePin);
  Serial.print("Grip pressure value: ");
  Serial.println(gripPressureValue);

  delay(2000);

  // Gripper toggle state
  int tInputVal = digitalRead(toggleBtnPin);
  if (tInputVal == LOW) {
    gripperOpen = !gripperOpen;
    delay(1000); // wait so that toggle is not read again multiple times
  }

  // Gripper control
  if (gripperOpen) {
    /* Todo */
  } else {
    /* Todo */
  }
  
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
  // while (eStopVal == HIGH) {
    // eStopVal = digitalRead(endStopPin);
    // myStepper.step(1);
    // steps++;
  // }
  Serial.print("Gripper endstop hit at ");
  Serial.print(steps);
  Serial.println(" steps.. gripper is fully closed");
  Serial.println("Gripper fully open, saving current position");
  homingDone = true;
}
