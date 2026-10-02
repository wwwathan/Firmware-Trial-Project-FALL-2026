#include <Servo.h>
#include <Wire.h>

const byte MPU_ADDR = 0x68;
byte lowByte;
byte highByte;
int16_t accelX;
int16_t accelY;

Servo servo;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  servo.attach(9);
  servo.write(90);
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
  // put your main code here, to run repeatedly:
  Wire.beginTransmission((MPU_ADDR));
  Wire.write(0x3B); // address of ACCEL_XOUT_H
  Wire.endTransmission(false); // keep I2C connection open

  Wire.requestFrom(0x68, 4, true); // ask mpu6050 for 2 bytes to be sent over
  if (Wire.available()) { // checks if there's 
    highByte = Wire.read(); // first byte sent over is high byte
    lowByte = Wire.read(); // second byte sent over is low byte
    accelX = ((highByte << 8) | lowByte);

    highByte = Wire.read();
    lowByte = Wire.read();
    accelY = ((highByte << 8) | lowByte);
  }
  float angleYandX = atan2(accelY, accelX) * 180 / PI; // need to convert radians -> degrees
                                                        // atan2 gives angle given 2 horizontal and vertical components

  Serial.print("X: ");
  Serial.print(accelX);
  Serial.print(" Y: ");
  Serial.print(accelY);
  Serial.print(" Calculated Angle: ");
  Serial.println(angleYandX);

  servo.write((-1)*angleYandX);
  delay(100);

  //delay(500);

}