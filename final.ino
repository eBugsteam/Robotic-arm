//Libraries
#include <Wire.h>
#include <MPU6050.h>
#include <Servo.h>


// Pin defines
#define gripper_pin  3


Servo gripper;
MPU6050 sensor;


int16_t ax, ay, az, gripper_angle;
int16_t gx, gy, gz;

void setup()
{
  Serial.begin(4800);
  Wire.begin();
  gripper.attach(gripper_pin);
  Serial.println("Initializing the sensor");
  sensor.initialize();
  Serial.println(sensor.testConnection() ? "Successfully Connected" : "Connection failed!");
  delay(1000);
  Serial.println("Taking Values from the sensor");
  delay(1000);
}


void loop()
{
  sensor.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  gripper_angle = map(ax, -17000, 17000, 0, 180);
  gripper_angle /= 10;

  Serial.println((gripper_angle));
  gripper.write(gripper_angle * 10);


  delay(100);
}