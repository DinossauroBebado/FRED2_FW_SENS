#include <MAIN/config.h>
#include <Arduino.h>
#include <Ultrasonic.h>


/*
------------------   ------------------   ------------------
|  HC-SR04 (1)   |   |  HC-SR04 (2)   |   |  HC-SR04 (3)   |
------------------   ------------------   ------------------
|  VCC   |  +5V  |   |  VCC   |  +5V  |   |  VCC   |  +5V  |
|  GND   |  GND  |   |  GND   |  GND  |   |  GND   |  GND  |
|  TRIG  |  D25  |   |  TRIG  |  D27  |   |  TRIG  |  D12  |
|  ECHO  |  D36  |   |  ECHO  |  D34  |   |  ECHO  |  D32  |

*/




#define ECHO_1 36
#define TRIG_1 25

#define ECHO_2 34
#define TRIG_2 27

#define ECHO_3 32
#define TRIG_3 12


Ultrasonic ultrasonic1(TRIG_1, ECHO_1);	// An ultrasonic sensor HC-04
Ultrasonic ultrasonic2(TRIG_2, ECHO_2);		// An ultrasonic sensor PING)))
Ultrasonic ultrasonic3(TRIG_3, ECHO_3);		// An Seeed Studio ultrasonic sensor


void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.print("Sensor 01: ");
  Serial.print(ultrasonic1.read(CM)); // Prints the distance on the default unit (centimeters)
  Serial.print("cm  ");

  Serial.print("Sensor 02: ");
  Serial.print(ultrasonic2.read(CM)); // Prints the distance making the unit explicit
  Serial.print("cm  ");

  Serial.print("Sensor 03: ");
  Serial.print(ultrasonic3.read(CM)); // Prints the distance in inches
  Serial.println("inc");

  delay(1000);}