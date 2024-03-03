#include <Wire.h>




void setup() {
  // Start the serial communication
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  
  if (Serial.available()) Serial.print("Bluetooth Connected Seccessfully");  // Checks whether data is comming from the serial port
}

void loop() {
  if (Serial.available() > 0) {
    // Read the incoming string
    String str = Serial.readString();
    if (str.startsWith("S1")) {
      str = str.substring(3, str.length());
      int S1 = str.toInt();
      //s1 = map(s1, 20, 186, 0, 180)
      //Serial.print(S1);
      S1 = map(S1, 0, 180, 0, 1023);
      analogWrite(2, S1);
    }

    if (str.startsWith("S2")) {
      str = str.substring(3, str.length());
      int S2 = str.toInt();
      //s1 = map(s1, 20, 186, 0, 180)
      //Serial.print(S1);
      S2 = map(S2, 0, 180, 0, 1023);
      analogWrite(3, S2);
    }

    if (str.startsWith("S3")) {
      str = str.substring(3, str.length());
      int S3 = str.toInt();
      //s1 = map(s1, 20, 186, 0, 180)
      //Serial.print(S1);
      S3 = map(S3, 0, 180, 0, 1023);
      analogWrite(4, S3);
    }

    if (str.startsWith("S4")) {
      str = str.substring(3, str.length());
      int S4 = str.toInt();
      //s1 = map(s1, 20, 186, 0, 180)
      //Serial.print(S1);
      S4 = map(S4, 0, 180, 0, 1023);
      analogWrite(5, S4);
    }

    if (str.startsWith("S5")) {
      str = str.substring(3, str.length());
      int S5 = str.toInt();
      //s1 = map(s1, 20, 186, 0, 180)
      //Serial.print(S1);
      S5 = map(S5, 0, 180, 0, 1023);
      analogWrite(6, S5);
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
