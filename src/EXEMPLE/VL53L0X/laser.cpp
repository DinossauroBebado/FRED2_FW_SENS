#include <Adafruit_VL53L0X.h>
#include <Arduino.h>



// Define which Wire objects to use, may depend on platform
// or on your configurations.
#define SENSOR1_WIRE Wire
#define SENSOR2_WIRE Wire



typedef struct {
  Adafruit_VL53L0X *psensor; // pointer to object
  TwoWire *pwire;
  int id;            // id for the sensor
  int shutdown_pin;  // which pin for shutdown;
  int interrupt_pin; // which pin to use for interrupts.
  Adafruit_VL53L0X::VL53L0X_Sense_config_t
      sensor_config;     // options for how to use the sensor
  uint16_t range;        // range value used in continuous mode stuff.
  uint8_t sensor_status; // status from last ranging in continuous.
} sensorList_t;

// Actual object, could probably include in structure above61
Adafruit_VL53L0X sensor1;
Adafruit_VL53L0X sensor2;


// Setup for 4 sensors
sensorList_t sensors[] = {

    {&sensor1, &SENSOR1_WIRE, 0x30, 18, 1,
    Adafruit_VL53L0X::VL53L0X_SENSE_DEFAULT, 0, 0},
    {&sensor2, &SENSOR2_WIRE, 0x29, 19, 1,
     Adafruit_VL53L0X::VL53L0X_SENSE_DEFAULT, 0, 0},
    

};

const int COUNT_SENSORS = sizeof(sensors) / sizeof(sensors[0]);

const uint16_t ALL_SENSORS_PENDING = ((1 << COUNT_SENSORS) - 1);
uint16_t sensors_pending = ALL_SENSORS_PENDING;
uint32_t sensor_last_cycle_time;

void Initialize_sensors() {
  bool found_any_sensors = false;
  // Set all shutdown pins low to shutdown sensors
  for (int i = 0; i < COUNT_SENSORS; i++)
    digitalWrite(sensors[i].shutdown_pin, LOW);
  delay(10);

  for (int i = 0; i < COUNT_SENSORS; i++) {
    // one by one enable sensors and set their ID
    digitalWrite(sensors[i].shutdown_pin, HIGH);
    delay(10); // give time to wake up.
    if (sensors[i].psensor->begin(sensors[i].id, false, sensors[i].pwire,
                                  sensors[i].sensor_config)) {
      found_any_sensors = true;
    } else {
      Serial.print(i, DEC);
      Serial.print(F(": failed to start\n"));
    }
  }
  if (!found_any_sensors) {
    Serial.println("No valid sensors found");
    while (1)
      ;
  }
}
//====================================================================
// Simple Sync read sensors.
//====================================================================
void read_sensors() {

  uint16_t ranges_mm[COUNT_SENSORS];
  uint32_t stop_times[COUNT_SENSORS];

  uint32_t start_time = millis();
  for (int i = 0; i < COUNT_SENSORS; i++) {
    ranges_mm[i] = sensors[i].psensor->readRange();
    stop_times[i] = millis();
  }
  uint32_t delta_time = millis() - start_time;


  Serial.print(delta_time, DEC);
  Serial.print(F(" | "));
  for (int i = 0; i < COUNT_SENSORS; i++) {

    Serial.print(ranges_mm[i], DEC);
    Serial.print(F(" "));
   
    
    start_time = stop_times[i];
  }
  Serial.println();
}


//====================================================================
// Setup
//====================================================================
void setup() {
  Serial.begin(115200);
  Wire.begin();

  // wait until serial port opens ... For 5 seconds max
  while (!Serial && millis() < 5000)
    ;

  // initialize all of the pins.
  Serial.println(F("VL53LOX_multi start, initialize IO pins"));
  for (int i = 0; i < COUNT_SENSORS; i++) {
    pinMode(sensors[i].shutdown_pin, OUTPUT);
    digitalWrite(sensors[i].shutdown_pin, LOW);

  }
  Serial.println(F("Starting..."));
  Initialize_sensors();
}

//====================================================================
// loop
//====================================================================
void loop() {
 
    read_sensors();
    
}