#include <Servo.h>
#include <Metro.h>

// Motor control pins
int speedPin_M1 = 5;     // M1 Speed Control
int speedPin_M2 = 6;     // M2 Speed Control
int directionPin_M1 = 4; // M1 Direction Control
int directionPin_M2 = 7; // M2 Direction Control

// Ultrasonic sensor pins
int URPWM = 3;  // PWM pin for distance measurement
int URTRIG = 10; // Trigger pin for ultrasonic sensor

// Command to enable distance measurement
uint8_t EnPwmCmd[4] = {0x44, 0x02, 0xbb, 0x01};

// Servo and variables
Servo myservo;
int pos = 90;       // Initial servo position (center)
int sweepFlag = 1;  // Flag for servo sweep direction
int actualDistance = 0;  // Measured distance
int leftDistance = 0;  // Distance measured on the left
int rightDistance = 0; // Distance measured on the right

// Timing variables
unsigned long lastMeasureTime = 0;  // Last time distance was measured
const unsigned long SENSOR_UPDATE_INTERVAL = 50;  // Measure distance every 50ms

void setup() {
  myservo.attach(9);  // Attach servo to pin 9
  Serial.begin(9600); // Initialize serial communication
  SensorSetup();      // Configure ultrasonic sensor
  delay(1000);        // Wait for stabilization
}

void loop() {
  // Measure distance at regular intervals
  if (millis() - lastMeasureTime >= SENSOR_UPDATE_INTERVAL) {
    actualDistance = MeasureDistance();
    lastMeasureTime = millis();
    Serial.print("Front Distance: ");
    Serial.println(actualDistance);
  }

  // Obstacle avoidance logic
  if (actualDistance <= 23) {  // Obstacle detected within 25cm
    carStop();  // Stop the robot
    Serial.println("Obstacle detected! Stopping and sweeping...");

    // Sweep left and measure distance
    myservo.write(45);  // Sweep servo to 45° (left)
    delay(500);  // Wait for servo to reach position
    leftDistance = MeasureDistance();
    Serial.print("Left Distance: ");
    Serial.println(leftDistance);

    // Sweep right and measure distance
    myservo.write(135);  // Sweep servo to 135° (right)
    delay(500);  // Wait for servo to reach position
    rightDistance = MeasureDistance();
    Serial.print("Right Distance: ");
    Serial.println(rightDistance);

    // Compare distances and move in the direction with more space
    if (leftDistance > rightDistance) {
      Serial.println("Turning left...");
      carBack(150,150);
      delay(500);
      carStop();
      carTurnLeft(200, 200);  // Turn left
      delay(900);  // Adjust delay for turn duration
      myservo.write(90); 
    } else {
      Serial.println("Turning right...");
      carBack(100,100);
      delay(500);
      carStop();
      carTurnRight(200, 200);  // Turn right
      delay(900);  // Adjust delay for turn duration
      myservo.write(90); 
    }

    // Move forward after turning
    carAdvance(90, 90);
    Serial.println("Moving forward...");
    delay(300);  // Move forward for a short time
  } else {  // No obstacle detected, move forward
    carAdvance(90, 90);
    Serial.println("Moving forward...");
  }
}

void SensorSetup() {
  pinMode(URTRIG, OUTPUT);  // Set trigger pin as output
  digitalWrite(URTRIG, HIGH);  // Trigger the sensor
  pinMode(URPWM, INPUT);  // Set PWM pin as input
  for (int i = 0; i < 4; i++) {
    Serial.write(EnPwmCmd[i]);  // Send command to enable distance measurement
  }
}

int MeasureDistance() {
  digitalWrite(URTRIG, LOW);  // Ensure trigger is LOW before sending HIGH
  digitalWrite(URTRIG, HIGH);  // Trigger the ultrasonic sensor
  unsigned long distance = pulseIn(URPWM, LOW);  // Measure PWM pulse duration
  if (distance == 1000) {  // Invalid reading if distance is 1000
    Serial.print("Invalid");
  } else {
    distance = distance / 50;  // Convert duration to distance (1cm per 50µs)
  }
  return distance;
}

void carStop() {
  digitalWrite(speedPin_M2, 0);
  digitalWrite(directionPin_M1, LOW);
  digitalWrite(speedPin_M1, 0);
  digitalWrite(directionPin_M2, LOW);
}

void carBack(int leftSpeed, int rightSpeed) {
  analogWrite(speedPin_M2, leftSpeed);  // PWM for left motor
  digitalWrite(directionPin_M1, LOW);  // Left motor backward
  analogWrite(speedPin_M1, rightSpeed);  // PWM for right motor
  digitalWrite(directionPin_M2, HIGH);  // Right motor backward
}

void carAdvance(int leftSpeed, int rightSpeed) {
  analogWrite(speedPin_M2, leftSpeed);  // PWM for left motor
  digitalWrite(directionPin_M1, HIGH);  // Left motor forward
  analogWrite(speedPin_M1, rightSpeed);  // PWM for right motor
  digitalWrite(directionPin_M2, LOW);  // Right motor forward
}

void carTurnLeft(int leftSpeed, int rightSpeed) {
  analogWrite(speedPin_M2, leftSpeed);  // PWM for left motor
  digitalWrite(directionPin_M1, LOW);  // Left motor backward
  analogWrite(speedPin_M1, rightSpeed);  // PWM for right motor
  digitalWrite(directionPin_M2, LOW);  // Right motor forward
}

void carTurnRight(int leftSpeed, int rightSpeed) {
  analogWrite(speedPin_M2, leftSpeed);  // PWM for left motor
  digitalWrite(directionPin_M1, HIGH);  // Left motor forward
  analogWrite(speedPin_M1, rightSpeed);  // PWM for right motor
  digitalWrite(directionPin_M2, HIGH);  // Right motor backward
}
