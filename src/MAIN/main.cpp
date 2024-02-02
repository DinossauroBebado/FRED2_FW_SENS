#include <MAIN/config.h>
// #include <MAIN/ultrasonic.h>
#include "led_strip.h"
#include <MAIN/micro_ros.h>
#include <MAIN/imu.h>

#include "filter.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/FreeRTOSConfig.h"
#include "freertos/semphr.h"

SemaphoreHandle_t dataMutex;

TaskHandle_t taskSubHandle = NULL;
TaskHandle_t taskPubHandle = NULL;

void vTaskSub(void *pvParameters);
void vTaskPub(void *pvParameters);

bool _imu_connect; 
bool _connect = false;

float* imu_orientation;
float* imu_angularVelocity;
float* imu_linaerAcceleration;
float* imu_orientationCovariance;
float* imu_linaerAccelerationCovariance;
float* imu_angularVelocityCovariance;

// float imu_orientation[4];           //x, y, z, w
// float imu_linaerAcceleration[3];   //x, y, z
// float imu_angularVelocity[3];      //x, y, z 

// int ultrasonic_range[NUMBER_ULTRASONIC_SENSORS];  
int previousTime; 

// ----------- ultrasonic sensor - distance

void setup(){

  led_strip_init();
  ros_init();

  pinMode(LED_BUILD_IN,OUTPUT);
  digitalWrite(LED_BUILD_IN,LOW);

  //Serial.begin(9600);

  previousTime = millis();

  // check that the IMU initializes correctly
  _imu_connect = imu_setup();

  if(_imu_connect != 0) {
    ESP.restart();
  }
  // Criação das Tasks
  BaseType_t result = xTaskCreatePinnedToCore(vTaskSub, "vTaskSub", configMINIMAL_STACK_SIZE+8192, NULL, 2, &taskSubHandle, PRO_CPU_NUM);
  if (result != pdPASS) {
   //Serial.println("Erro ao criar vTaskSub!");
  }
  result = xTaskCreatePinnedToCore(vTaskPub, "vTaskPub", configMINIMAL_STACK_SIZE+8192, NULL, 2, &taskPubHandle, APP_CPU_NUM);
  if (result != pdPASS) {
   //Serial.println("Erro ao criar vTaskPub!");
  }

}


void loop(){

  vTaskDelay(100);
  // delay(10);
}   

void vTaskSub(void *pvParameters)
{
  while(1)
  {
    if (xSemaphoreTake(dataMutex, portMAX_DELAY) == pdTRUE) {
      imu_orientation = orientation(); 
      imu_angularVelocity = angular_velocity(); 
      imu_linaerAcceleration = linear_acceleration();
      imu_orientationCovariance = orientation_covariance();
      imu_linaerAccelerationCovariance = linear_acceleration_covariance(); 
      imu_angularVelocityCovariance = angular_velocity_covariance(); 
      xSemaphoreGive(dataMutex);
    }
    rclc_executor_spin_some(&executor, RCL_MS_TO_NS(10));
    vTaskDelay(10);
  }
}

void vTaskPub(void *pvParameters)
{
  while(1)
  {
    if (xSemaphoreTake(dataMutex, portMAX_DELAY) == pdTRUE) {
      ros_imu(imu_orientation, 
          imu_angularVelocity, 
          imu_linaerAcceleration,
          imu_orientationCovariance,imu_linaerAccelerationCovariance,imu_angularVelocityCovariance); 
      xSemaphoreGive(dataMutex);
    }
    
    rclc_executor_spin_some(&executor, RCL_MS_TO_NS(10));
    vTaskDelay(10);
  }
}