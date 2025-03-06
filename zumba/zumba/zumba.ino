#include <SoftwareSerial.h>

#define ENA 3  
#define IN1 5  
#define IN2 6  
#define ENB 11 
#define IN3 9  
#define IN4 10 


// Define RX, TX pins for Bluetooth communication
SoftwareSerial BT(0, 1);  // Change pins as per your setup

void setup() {
  Serial.begin(9600);        // Start serial communication (USB)
  BT.begin(9600);     // Start Bluetooth communication (via HC-05)

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  moveCar(0,0,0);
}

void loop() {
  if (BT.available() > 0) {
    String data = BT.readStringUntil('\n');  // Read incoming data
    Serial.println("Received from Bluetooth: " + data);  // Print to Serial Monitor

    // Split the message into values  
    int accel_value, brake_value, steering_value;
    sscanf(data.c_str(), "%d,%d,%d", &accel_value, &brake_value, &steering_value);

    Serial.print("Accel: ");
    Serial.print(accel_value);
    Serial.print(", Brake: ");
    Serial.print(brake_value);
    Serial.print(", Steering: ");
    Serial.println(steering_value);


    moveCar(accel_value,brake_value,steering_value);


  }

  // if (Serial.available() > 0) {
  //   // Send data from USB Serial to Bluetooth
  //   char data = Serial.read();
  //   BT.write(data);
  // }
}




void moveCar(int accel, int brake, int steering) {
  int speed = accel - brake;  // Speed is determined by acceleration - brake

  // Calculate steering effect
  int leftMotorSpeed = speed;
  int rightMotorSpeed = speed;

  if (steering < 127) {  // Turn left
    int turnFactor = map(steering, 127, 0, 0, speed);  // Reduce right motor speed
    rightMotorSpeed -= turnFactor;
  } else if (steering > 127) {  // Turn right
    int turnFactor = map(steering, 127, 255, 0, speed);  // Reduce left motor speed
    leftMotorSpeed -= turnFactor;
  }

  // // Ensure speeds are within valid range (0 to 255)
  // leftMotorSpeed = constrain(leftMotorSpeed, 0, 255);
  // rightMotorSpeed = constrain(rightMotorSpeed, 0, 255);

  // Move the motors
  if (speed > 0) {  // Move forward
    analogWrite(ENA, leftMotorSpeed);
    analogWrite(ENB, rightMotorSpeed);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } else if (speed < 0) {  // Move backward
    speed = -speed;  // Make speed positive
    analogWrite(ENA, leftMotorSpeed);
    analogWrite(ENB, rightMotorSpeed);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  } else {  // Stop
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }
}






// void moveForward() {
//   analogWrite(ENA, 255);
//   analogWrite(ENB, 50);
//   digitalWrite(IN1, HIGH);
//   digitalWrite(IN2, LOW);
//   digitalWrite(IN3, HIGH);
//   digitalWrite(IN4, LOW);
// }

// void moveBackward() {
//   analogWrite(ENA, 150);
//   analogWrite(ENB, 150);
//   digitalWrite(IN1, LOW);
//   digitalWrite(IN2, HIGH);
//   digitalWrite(IN3, LOW);
//   digitalWrite(IN4, HIGH);
// }

// void turnLeft() {
//   analogWrite(ENA, 150);
//   analogWrite(ENB, 150);
//   digitalWrite(IN1, LOW);
//   digitalWrite(IN2, HIGH);
//   digitalWrite(IN3, HIGH);
//   digitalWrite(IN4, LOW);
// }

// void turnRight() {
//   analogWrite(ENA, 150);
//   analogWrite(ENB, 150);
//   digitalWrite(IN1, HIGH);
//   digitalWrite(IN2, LOW);
//   digitalWrite(IN3, LOW);
//   digitalWrite(IN4, HIGH);
// }

// void stopMotors() {
//   digitalWrite(IN1, LOW);
//   digitalWrite(IN2, LOW);
//   digitalWrite(IN3, LOW);
//   digitalWrite(IN4, LOW);
// }