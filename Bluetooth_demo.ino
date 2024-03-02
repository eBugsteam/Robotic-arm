#include <Wire.h>



String str = " ";

void setup() {
  // Start the serial communication
  Serial.begin(9600);

  if (Serial.available() > 0)

    Serial.print("\tBluetooth Connected Seccessfully.\a");  // Checks whether data is comming from the serial port
}

void loop() {
  if (Serial.available() > 0) {
    // Read the incoming string
    str = Serial.readString();

    // Servo 1
    if (str.startsWith("S1")) {
      str = str.substring(3, str.length());
      int S1 = str.toInt();
      Serial.print(S1);
    }

    // Servo 2
    if (str.startsWith("S2")) {
      str = str.substring(3, str.length());
      int S2 = str.toInt();
      Serial.print(S2);
    }

    // Servo 3
    if (str.startsWith("S3")) {
      str = str.substring(3, str.length());
      int S3 = str.toInt();
      Serial.print(S3);
    }




    // Remove any whitespace or newline characters
    // str.trim();

    // Convert the string to an integer
    int num = str.toInt();

    // Print the integer to the serial monitor
    Serial.print("The converted number is: ");
    Serial.println(num);
  }
  delay(100);
}
