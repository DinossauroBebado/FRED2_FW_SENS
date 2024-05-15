#include <Arduino.h>


void setup() {
  pinMode(39, INPUT);
  Serial.begin(115200);
}

void loop() {
  int sensorValue = digitalRead(3);
    if(sensorValue==LOW)
    {       
        Serial.println("no Object");
        delay(500);
    }
    else
    {
        Serial.println("Object Detected");
        delay(500);
    }
}
