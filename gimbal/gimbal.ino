#include <Servo.h>
#include <Wire.h>

// ---- VARIABLES ----
const byte MPU_ADDR = 0x68;
byte lowByte;
byte highByte;
int16_t accelX;
int16_t accelY;
Servo servo;

void setup() {

  Serial.begin(9600);
  // ---- SERVO SETUP ----
  servo.attach(9); // use digital 9 for servo
  servo.write(90); // default is set to pointing upward

  // ---- MPU-6050 STETUP ----
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B); // first = register byte
  Wire.write(0x00); // second = data we want to store
  byte error = Wire.endTransmission(); // returns a code indiciating if there was a sucessful transmission

  if (error == 0) {
    Serial.print("Succesfully Woken Up"); // 0 = success
  }
  else {
    Serial.print("Error Occurred, Error Code: ");
    Serial.print(error);
  }
}

void loop() {

  // ---- SELECT TARGET ADDRESS ----
  Wire.beginTransmission((MPU_ADDR));
  Wire.write(0x3B); // address of ACCEL_XOUT_H
  Wire.endTransmission(false); // keep I2C connection open

  // ---- START REQUESTING DATA ----
  Wire.requestFrom(0x68, 4, true); // ask mpu6050 for 4 bytes to be sent over
  if (Wire.available()) { // checks if there's available bits to be received
    highByte = Wire.read(); // first byte sent over is high byte
    lowByte = Wire.read(); // second byte sent over is low byte
    accelX = ((highByte << 8) | lowByte);

    highByte = Wire.read();
    lowByte = Wire.read();
    accelY = ((highByte << 8) | lowByte);
  }
  float angleYandX = atan2(accelY, accelX) * 180 / PI; // need to convert radians -> degrees
                                                        // atan2 gives angle given 2 horizontal and vertical components

  // ---- PRINT DATA TO SERIAL ----
  Serial.print("X: ");
  Serial.print(accelX);
  Serial.print(" Y: ");
  Serial.print(accelY);
  Serial.print(" Calculated Angle: ");
  Serial.println(angleYandX);

  // ---- WRITE CORRECT ANGLE TO SERVO -----
  servo.write((-1)*angleYandX);
}