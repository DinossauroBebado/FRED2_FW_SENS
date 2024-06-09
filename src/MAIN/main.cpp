#include <MAIN/config.h>
#include <MAIN/ultrasonic.h>
#include "led_strip.h"
#include <MAIN/micro_ros.h>
#include <MAIN/imu.h>
#include <Adafruit_TCS34725.h>

#include "filter.h"

/* Initialise with default values (int time = 2.4ms, gain = 1x) */
// Adafruit_TCS34725 tcs = Adafruit_TCS34725();

/* Initialise with specific int time and gain values */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);

float r, g, b;
int inductiveSensor = 0;
bool metalDetection = NOT_DETECTED;
bool _imu_connect; 
bool _connect = false;

float imu_orientation[4];           //x, y, z, w
float imu_linaerAcceleration[3];   //x, y, z
float imu_angularVelocity[3];      //x, y, z 

int ultrasonic_range[NUMBER_ULTRASONIC_SENSORS];  
int previousTime; 

// ----------- ultrasonic sensor - distance

void setup(){

  int counter_tries = 0;
  led_strip_init();

  pinMode(LED_BUILD_IN,OUTPUT);
  digitalWrite(LED_BUILD_IN,LOW);

  while (!(tcs.begin()) && counter_tries < 5) 
  {
    Serial.println("Found sensor");
    counter_tries++;
    delay(500);
  } 
  if(counter_tries >= 5)
    Serial.println("No TCS34725 found ... check your connections");
  else
    Serial.println("Found sensor");
  // Now we're ready to get readings!

  _imu_connect = imu_setup();

  while (_imu_connect != 0) 
  {
    _imu_connect = imu_setup();
  }

  Serial.begin(115200);

  previousTime = millis();

  // check that the IMU initializes correctly

  ros_init();

  delay(3000);
  
}


void loop(){

  // if(!rosConnected(nh,_connect)) {
  //   pixels.fill(0x000000);
  //   pixels.show();
  // }
  tcs.getRGB(&r, &g, &b);

  inductiveSensor = analogRead(INDUCTIVE_PIN);
  if(inductiveSensor < DETECTED_THRS)
  {       
    metalDetection = NOT_DETECTED;
  }
  else 
  {
    metalDetection = DETECTED;
  }
  ros_goal_detectors(metalDetection, r, g, b);
  int* ultrasonic_range = ultrasonic_measurments(previousTime); 
  ros_ultrasonic(ultrasonic_range);

  float* imu_orientation = orientation(); 
  float* imu_angularVelocity = angular_velocity(); 
  float* imu_linaerAcceleration = linear_acceleration();
  float* imu_orientationCovariance = orientation_covariance();
  float* imu_linaerAccelerationCovariance = linear_acceleration_covariance(); 
  float* imu_angularVelocityCovariance = angular_velocity_covariance(); 
  
  ros_imu(imu_orientation, 
          imu_angularVelocity, 
          imu_linaerAcceleration,
          imu_orientationCovariance,imu_linaerAccelerationCovariance,imu_angularVelocityCovariance); 


  rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));
  delay(10);
}   