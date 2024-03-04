#include <Wire.h>
#include <Servo.h>
#include <Adafruit_MPU6050.h>

#define sensor0 A0
#define sensor1 A1
#define sensor2 A2
#define sensor3 A3

double sensor0_value, sensor1_value, sensor2_value, sensor3_value, current_temp = 0;

bool glove_wear = false;
float glove_average = 0;
float gripper_value = 0;
int16_t roll_angle, pitch_angle;
long unsigned int temp_count = 0;

Servo servo;
Servo servo1;
Servo servo2;

Adafruit_MPU6050 mpu;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();
  servo1.attach(3);
  servo2.attach(4);
  servo.attach(7);
  servo.write(170);
  Serial.println("Initializing the mpu");
  Serial.println(mpu.begin() ? "Successfully Connected" : "Connection failed!");
  delay(1000);
  Serial.println("Taking Values from the mpu");
  delay(1000);
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  current_temp = temp.temperature;
}

void loop() {

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  roll_angle = map(a.acceleration.x, -17000, 17000, 0, 180);

  servo1.write(roll_angle);

  sensor0_value = floor(analogRead(sensor0) / 100);
  sensor1_value = floor(analogRead(sensor1) / 100);
  sensor2_value = analogRead(sensor2);
  sensor3_value = analogRead(sensor3);

  glove_average = (sensor0_value + sensor1_value) / 2;
  gripper_value = map(glove_average, 0, 4, 170, 10);

  servo.write(gripper_value);

  if (Serial.available() > 0) {
   
    String str = Serial.readString();
    if (str.startsWith("S1")) {
      str = str.substring(3, str.length());
      int S1 = str.toInt();
      S1 = map(S1, 20, 186, 0, 180);
      servo2.write(S1);
    }

    int num = str.toInt();
    Serial.print("The converted number is: ");
  }


  if (temp.temperature - current_temp >= 0.09)
    glove_wear = true;
  else glove_wear = false;

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
  Serial.print("\t");
  Serial.print("Temperature:");
  Serial.print(temp.temperature);
  Serial.print("\t");
  Serial.print("Glove:");
  Serial.print(glove_wear);
  Serial.print("\n");

  delay(30);

  temp_count++;
}
