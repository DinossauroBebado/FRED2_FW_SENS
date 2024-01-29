#include <MAIN/config.h>
#include "Adafruit_VL53L0X.h"
#include "filter.h"

// address we will assign if dual sensor is present
#define LASER1_ADDRESS 0x30
#define LASER2_ADDRESS 0x31

//Filtros de mediana para testes 
MedianFilter LeftLaserFilter(10,0);
MedianFilter RightLaserFilter(10,0);

Adafruit_VL53L0X laserLeft = Adafruit_VL53L0X();
Adafruit_VL53L0X laserRight = Adafruit_VL53L0X();

VL53L0X_RangingMeasurementData_t laserLeft_measure;
VL53L0X_RangingMeasurementData_t laserRight_measure;

// array of the most recent 5 sensor values
int laserLeftArray[5];
int laserRightArray[5]; 

int currentValue_laserLeft;
int currentValue_laserRight; 

int sum_laserLeft;
int sum_laserMiddle;
int sum_laserRight;

void setID() {

  digitalWrite(XSHUNT_1, LOW);    
  digitalWrite(XSHUNT_2, LOW);
  delay(10);
  
  digitalWrite(XSHUNT_1, HIGH);
  digitalWrite(XSHUNT_2, HIGH);
  delay(10);

  digitalWrite(XSHUNT_1, HIGH);
  digitalWrite(XSHUNT_2, LOW);
  delay(10);

  digitalWrite(XSHUNT_2, HIGH);
  delay(10);

}

// funcion to to shift each value on the array so that the new data can slot into position 0
void rollingValue(){
    
    for (int i = 4; i > 0; i--){
        laserLeftArray[i] = laserLeftArray[i-1];
        laserRightArray[i] = laserRightArray[i-1];
    }

    laserLeftArray[0] = currentValue_laserLeft;
    laserRightArray[0] = currentValue_laserRight; 
     
}

// calculate the sample moving average
void moving_average(){
    
    for (int i = 0; i < 5; i++){
        sum_laserLeft = sum_laserLeft + laserLeftArray[i];
        sum_laserRight = sum_laserRight + laserRightArray[i];
    }

    sum_laserLeft = sum_laserLeft/5; 
    sum_laserRight = sum_laserRight/5;
 
}

void laser_init() {

  pinMode(XSHUNT_1, OUTPUT);
  pinMode(XSHUNT_2, OUTPUT);

  digitalWrite(XSHUNT_1, LOW);
  digitalWrite(XSHUNT_2, LOW);

  setID();
}

void laser_measurments(){

    currentValue_laserLeft = laserLeft_measure.RangeMilliMeter; 
    currentValue_laserRight = laserRight_measure.RangeMilliMeter; 

    sum_laserLeft = 0; 
    sum_laserRight = 0;

    moving_average(); 

    // update array of sensor values
    rollingValue();

    //Para testes do filtro de mediana
    LeftLaserFilter.in(currentValue_laserLeft);
    RightLaserFilter.in(currentValue_laserRight);
    
    Serial.print(sum_laserLeft);
    Serial.print(" | ");
    Serial.print(sum_laserRight); 
    Serial.println("");

    Serial.print(LeftLaserFilter.out());
    Serial.print(" | ");
    Serial.print(RightLaserFilter.out()); 
    Serial.println("");

}


