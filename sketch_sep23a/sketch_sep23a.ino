#include <Servo.h>

Servo servo;

void setup() {
  // put your setup code here, to run once:
  servo.attach(A0);
  servo.write(90);
  delay(10000);
  servo.writeMicroseconds(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  delay(1000);
  
}