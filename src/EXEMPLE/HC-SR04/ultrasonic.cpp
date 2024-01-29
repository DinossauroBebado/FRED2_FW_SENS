#include <MAIN/config.h>
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

// ---------------------------------------------------------------------------
// Example NewPing library sketch that pings 3 sensors 20 times a second.
// ---------------------------------------------------------------------------

#include <NewPing.h>

#define SONAR_NUM 3      // Number of sensors.
#define MAX_DISTANCE 200 // Maximum distance (in cm) to ping.

NewPing sonar[SONAR_NUM] = {   // Sensor object array.
  NewPing(TRIG_1, ECHO_1, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping.
  NewPing(TRIG_2, ECHO_2, MAX_DISTANCE),
  NewPing(TRIG_3, ECHO_3, MAX_DISTANCE)
};

void setup() {
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
}

const int pingInterval = 30;  // Interval between pings in milliseconds
unsigned long lastPingTime = 0;  // Variable to store the time of the last ping
void loop() {

for (uint8_t i = 0; i < SONAR_NUM; i++) {
  // Check if enough time has passed since the last ping
  if (millis() - lastPingTime >= pingInterval) {
    Serial.print(i);
    Serial.print("=");
    Serial.print(sonar[i].ping_cm());
    Serial.print("cm ");

    // Update the last ping time
    lastPingTime = millis();
    Serial.println();
  }
}

}
