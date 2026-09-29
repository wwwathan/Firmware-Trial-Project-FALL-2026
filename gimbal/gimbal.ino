#include <Servo.h>
#include <Wire.h>

const byte MPU_ADDR = 0x68;
byte lowByte;
byte highByte;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
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

  Wire.requestFrom(0x68, 2, true); // ask mpu6050 for 2 bytes to be sent over
  if (Wire.available()) { // checks if there's 
    highByte = Wire.read(); // first byte sent over is high byte
    lowByte = Wire.read(); // second byte sent over is low byte
  }

  int16_t accelX = ((highByte << 8) | lowByte);
  Serial.println(accelX);

  delay(500);

}