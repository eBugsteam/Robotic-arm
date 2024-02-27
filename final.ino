//Libraries
#include <Wire.h>
#include <MPU6050.h>
#include <Servo.h>


// Pin defines
//sero
#define servobase_pin 3
#define servomid1_pin 5
#define servomid2_pin 6
#define servorot_pin 9
#define servogrip_pin 10
//sensor
#define sensor0 A0
#define sensor1 A1
#define sensor2 A2
#define sensor3 A3


//Initialize
Servo servobase, servomid1, servomid2, servorot, servogrip;
MPU6050 sensor;

//Global Variables 
double sensor0_value, sensor1_value, sensor2_value, sensor3_value;
float glove_average = 0;
float gripper_degree = 0;
int16_t ax, ay, az, roll_angle, pitch_angle;
int16_t gx, gy, gz;

void setup()
{
  Serial.begin(9600);
  Wire.begin();



  servogrip.attach(servogrip_pin);
  servogrip.write(170);
  servorot.attach(servorot_pin);
  servorot.write(0);
  servomid2.attach(servomid2_pin);
  servomid2.write(0);





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

  roll_angle = map(ax, -17000, 17000, 0, 180);
  pitch_angle = map(ay, -17000, 17000, 0, 180);

  //servo4.write(pitch_angle);
  servorot.write(roll_angle);
  servomid2.write(pitch_angle);

  sensor0_value = analogRead(sensor0);
  sensor1_value = analogRead(sensor1);
  sensor2_value = analogRead(sensor2);
  sensor3_value = analogRead(sensor3);

  glove_average = (sensor0_value + sensor1_value) / 2.0; // Changed to floating point for precision
  gripper_degree = map(glove_average, 0, 1023, 170, 10); // Assuming 10-bit ADC. Change 1023 if bit depth is different.
  
  servogrip.write(gripper_degree);

  //Printing Data for Serial Plotter
  Serial.print("Roll-Angle:");
  Serial.print(roll_angle);
  Serial.print(" ");
  Serial.print("Pitch-Angle:");
  Serial.print(pitch_angle);
  Serial.print(" ");
  Serial.print("Angle:");
  Serial.print(sensor0_value);
  Serial.print(" ");
  Serial.print("Angle:");
  Serial.print(sensor1_value);
  Serial.print(" ");
  //Serial.print(glove_average);
  //Serial.print(" ");
  Serial.print("Angle:");
  Serial.print(gripper_degree);
  Serial.println(); // Ensure to start a new line for the next set of data values

  delay(200);
}
