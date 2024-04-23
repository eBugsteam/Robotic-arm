// Libraries
#include <Wire.h>
#include <Servo.h>

// Define pins
#define sensor0 A0
#define sensor1 A1
#define sensor2 A2
#define sensor3 A3


// Global Variables
double sensor0_value = 0, sensor1_value = 0, sensor2_value = 0, sensor3_value = 0, current_temp = 0;
float glove_average = 0, gripper_value = 0, gripper_average = 0, glove_value = 0;
int16_t roll_angle, pitch_angle;
bool glove_wear = false, status = true;

int i, j;

Servo servo;
Servo servo1;
Servo servo2;
int lastdata;

void setup() {

  Serial.begin(9600);
  Wire.begin();
  servo.attach(3);
  servo.write(170);
}

void loop() {


  while (status) {
    for (i = 170; i >= 10; i -= 10) {


      servo.write(i);
      delay(100);

      sensor0_value = floor(analogRead(sensor0) / 100);  // Divide sensor value to 100 for getting stable data
      sensor1_value = floor(analogRead(sensor1) / 100);  // Divide sensor value to 100 for getting stable data
      sensor2_value = floor(analogRead(sensor2) / 100);  // Divide sensor value to 100 for getting stable data
      sensor3_value = floor(analogRead(sensor3) / 100);  // Divide sensor value to 100 for getting stable data

      lastdata = i;

      glove_average = floor((sensor0_value + sensor1_value) / 2);    // Calculate average value of two FSR
      gripper_average = floor((sensor2_value + sensor3_value) / 2);  // Calculate average value of two FSR



      Serial.print("Glove Average: ");
      Serial.print(glove_average);
      Serial.print("\t");
      Serial.print("Gripper Average: ");
      Serial.print(gripper_average);
      Serial.print("\t");
      Serial.print("i:");
      Serial.print(i);

      Serial.print("\n");



      if (abs(glove_average - gripper_average) >= 2) {
        status = false;
        servo.write(i);
        break;
      }
    }
  }

  servo.write(lastdata);


  sensor0_value = floor(analogRead(sensor0) / 100);  // Divide sensor value to 100 for getting stable data
  sensor1_value = floor(analogRead(sensor1) / 100);  // Divide sensor value to 100 for getting stable data
  sensor2_value = floor(analogRead(sensor2) / 100);  // Divide sensor value to 100 for getting stable data
  sensor3_value = floor(analogRead(sensor3) / 100);  // Divide sensor value to 100 for getting stable data

  glove_average = floor((sensor0_value + sensor1_value) / 2);    // Calculate average value of two FSR
  gripper_average = floor((sensor2_value + sensor3_value) / 2);  // Calculate average value of two FSR

 // if(abs(glove_average - gripper_average <= 2)) status = true;

  Serial.print("Glove Average: ");
  Serial.print(glove_average);
  Serial.print("\t");
  Serial.print("Gripper Average: ");
  Serial.print(gripper_average);
  Serial.print("\t");
  Serial.print("i:");
  Serial.print(i);
  Serial.print("\n");

  delay(10);
}
