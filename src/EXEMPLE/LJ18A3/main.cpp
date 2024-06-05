#include <Arduino.h>

int inductiveSensor = 0;
int metalDetection = 0;
void setup() {
  // pinMode(39, INPUT);
  Serial.begin(115200);
}
 
void loop() {
  inductiveSensor = analogRead(39);
  if(inductiveSensor < 3800)       
    metalDetection = 0;
  else 
    metalDetection = 1;
  if(metalDetection)
    Serial.println("Object Detected");
  else
    Serial.println("No Object");
  delay(500);
}
