#include <MAIN/config.h>
#include <Ultrasonic.h>
#include <Arduino.h>

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

// objects for the hc-sr04
Ultrasonic ultrasonic1(TRIG_1, ECHO_1); 
Ultrasonic ultrasonic2(TRIG_2, ECHO_2); 
Ultrasonic ultrasonic3(TRIG_3, ECHO_3); 


void setup(){

    Serial.begin(115200); 

}

void loop(){
    Serial.print("Sensor 01 (m): ");
    Serial.println(ultrasonic1.read(CM));

    Serial.print("Sensor 02 (m): ");
    Serial.println(ultrasonic2.read(CM));

    Serial.print("Sensor 03 (m): ");
    Serial.println(ultrasonic3.read(CM));
}