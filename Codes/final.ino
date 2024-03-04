#include <Wire.h>
#include <Servo.h>
#include <MPU6050.h>


#define sensor0 A0
#define sensor1 A1
#define sensor2 A2
#define sensor3 A3

double sensor0_value, sensor1_value, sensor2_value, sensor3_value;


float glove_average = 0;
float gripper_value = 0;
int16_t ax, ay, az, roll_angle, pitch_angle;
int16_t gx, gy, gz;

Servo servo;
Servo servo1;
Servo servo2;
MPU6050 sensor;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();
  servo1.attach(3);
  servo2.attach(4);
  servo.attach(7);
  servo.write(170);
  Serial.println("Initializing the sensor");
  sensor.initialize();
  Serial.println(sensor.testConnection() ? "Successfully Connected" : "Connection failed!");
  delay(1000);
  Serial.println("Taking Values from the sensor");
  delay(1000);
}

void loop() {

  sensor.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  roll_angle = map(ax, -17000, 17000, 0, 180);
 // pitch_angle = map(ay, -17000, 17000, 0, 180);

  servo1.write(roll_angle);

  sensor0_value = floor(analogRead(sensor0) / 100);
  sensor1_value = floor(analogRead(sensor1) / 100);
  sensor2_value = analogRead(sensor2);
  sensor3_value = analogRead(sensor3);

  glove_average = (sensor0_value + sensor1_value) / 2;

  gripper_value = map(glove_average, 0, 4, 170, 10);
  

  servo.write(gripper_value);

  if (Serial.available() > 0) {
    // Read the incoming string
    String str = Serial.readString();
    if (str.startsWith("S1")) {
      str = str.substring(3, str.length());
      int S1 = str.toInt();
      S1 = map(S1, 20, 186, 0, 180);
      //Serial.print(S1);
      //S1 = map(S1, 0, 180, 0, 1023);
      //analogWrite(2, S1);
      servo2.write(S1);
    }

    int num = str.toInt();

    // Print the integer to the serial monitor
    Serial.print("The converted number is: ");
    //Serial.println(num);
  }

  Serial.print("Sensor1: ");
  Serial.print(floor(sensor0_value));
  Serial.print("\t");
  Serial.print("Sensor2: ");
  Serial.print(floor(sensor1_value));
  Serial.print("\t");
  Serial.print("Glove Average: ");
  Serial.print(glove_average);
  Serial.print("\t");
  Serial.print("Gripper degree: ");
  Serial.print(gripper_value);
  Serial.print("\t");
  Serial.print("servo");
  Serial.print(servo.read());
  Serial.print("\t");
  Serial.print("Roll:");
  Serial.print(roll_angle);
  Serial.print("\n");

}
