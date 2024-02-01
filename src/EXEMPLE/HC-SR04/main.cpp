#include <MAIN/config.h>
#include <Arduino.h>
#include "ultrasonic.h"

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

int ultrasonic_range[NUMBER_ULTRASONIC_SENSORS]; 
int previousTime; 

void setup(){
    Serial.begin(115200);
    previousTime = millis();

}

void loop(){
    int* ultrasonic_range = ultrasonic_measurments(previousTime); 

    for(int i=0; i<NUMBER_ULTRASONIC_SENSORS;i++){
        Serial.print("| SENSOR ");
        Serial.print(i);
        Serial.print(" :");
        Serial.print(ultrasonic_range[i]);
    }

    Serial.println("");

}