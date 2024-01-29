#include <MAIN/config.h>
#include <Ultrasonic.h>
#include "filter.h"

// objects for the hc-sr04
Ultrasonic ultrasonic[NUMBER_ULTRASONIC_SENSORS] = {
    Ultrasonic(TRIG_1, ECHO_1),
    Ultrasonic(TRIG_2, ECHO_2),
    Ultrasonic(TRIG_3, ECHO_3)};

MedianFilter median_filter[NUMBER_ULTRASONIC_SENSORS] = {
    MedianFilter(NUMBER_SAMPLES, 0),
    MedianFilter(NUMBER_SAMPLES, 0),
    MedianFilter(NUMBER_SAMPLES, 0)};
/* 
Kalman kalman_filter[NUMBER_ULTRASONIC_SENSORS] = {
    Kalman(),
    Kalman(),
    Kalman()}; */

int ultrasonicArrays[NUMBER_SAMPLES][NUMBER_ULTRASONIC_SENSORS]; // array of the most recent sensor values

int currentValue_ultrasonic[NUMBER_ULTRASONIC_SENSORS];
int previousValue_ultrasonic[NUMBER_ULTRASONIC_SENSORS] = {0,0,0}; 
int filtered_ultrasonic[NUMBER_ULTRASONIC_SENSORS];
int currentSensor = 0;

uint16_t movingAverage_sample[NUMBER_SAMPLES]; 

// funcion to to shift each value on the array so that the new data can slot uint16_to position 0
 void rollingValue()
{

    for (uint16_t j = 0; j < NUMBER_ULTRASONIC_SENSORS; j++)
    {
        for (uint16_t i = 0; i < NUMBER_SAMPLES-1; i++)
        {
            ultrasonicArrays[i+1][j] = ultrasonicArrays[i][j];
        }

        ultrasonicArrays[0][j] = currentValue_ultrasonic[j];
    }
}

int* ultrasonic_measurments(uint16_t previousTime)
{
    //previousValue_ultrasonic[currentSensor] = ultrasonic[currentSensor].read(); 
   
    if (millis() - previousTime > 35 && currentSensor < NUMBER_ULTRASONIC_SENSORS) {
        currentValue_ultrasonic[currentSensor] = ultrasonic[currentSensor].read(CM);
        median_filter[currentSensor].in((currentValue_ultrasonic[currentSensor])); 

        if (abs((currentValue_ultrasonic[currentSensor] - previousValue_ultrasonic[currentSensor]))> 1) {

            // Serial.pruint16_t(currentValue_ultrasonic[currentSensor]); 
            // Serial.pruint16_t(" | ");
            // Serial.pruint16_t(previousValue_ultrasonic[currentSensor]); 
            // Serial.pruint16_t(" | "); 
            // Serial.pruint16_t(currentValue_ultrasonic[currentSensor] - previousValue_ultrasonic[currentSensor]);
            // Serial.pruint16_tln("\n-----------------------------------------\n"); 
             
            currentValue_ultrasonic[currentSensor] = median_filter[currentSensor].out(); 
            //Serial.pruint16_tln(median_filter[0].out()); 
        }

        previousValue_ultrasonic[currentSensor] = currentValue_ultrasonic[currentSensor];

        // Serial.pruint16_tln(currentValue_ultrasonic[0]); 
        // Serial.pruint16_t(" | ");
        // Serial.pruint16_t(previousValue_ultrasonic[currentSensor]); 
        // Serial.pruint16_t(" | "); 
        // Serial.pruint16_t(currentValue_ultrasonic[currentSensor] - previousValue_ultrasonic[currentSensor]);
        // Serial.pruint16_tln("\n-----------------------------------------\n"); 

        currentSensor = currentSensor + 1; 

    }

    if (currentSensor == NUMBER_ULTRASONIC_SENSORS){
        // for (uint16_t j = 0; j < NUMBER_ULTRASONIC_SENSORS; j++) {
        //     for (uint16_t i = 0; i < NUMBER_SAMPLES; i++) {
        //         movingAverage_sample[i] = ultrasonicArrays[i][j]; 
        //     }
            
        //     filtered_ultrasonic[j] = moving_average_filter(movingAverage_sample, NUMBER_SAMPLES);

        //     Serial.pruint16_tln(filtered_ultrasonic[0]); 

        // }
        // rollingValue(); 
        currentSensor = 0;

    }
    return currentValue_ultrasonic; 
}
