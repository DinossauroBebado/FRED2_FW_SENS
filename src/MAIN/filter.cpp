/*
  Biblioteca de filtros
  Referências e funções:  Matthew Landolfa - HC-SR04_Filter, rizkymille - ultrasonic-hc-sr04-kalman-filter
*/

#include "filter.h"
#include "MedianFilter.h"

int inRange = 45;           // Wide Range First sight of Target
int TargetRange = 12;       // Minimal Parking Range to Target
const int NoiseReject = 25; // Percentage of reading closeness for rejection filter
long lastTime = 0;
const unsigned int maxDuration = 11650;
const long speed_of_sound = 29.1;

Kalman::Kalman()
{
  static const int R = 40;
  static const int H = 1.00;
  static int Q = 10;
  static int P = 0;
}

//Double antes
int Kalman::filter(int U)
{
  static int U_hat = 0;
  static int K = 0;
  K = P * H / (H * P * H + R);
  U_hat += +K * (U - H * U_hat);
  P = (1 - K * H) * P + Q;
  return U_hat;
}

int moving_average_filter(int *vector, int size){
    int return_value = 0;
    for (int i = 0; i < size; i++){
        return_value = return_value + vector[i];
    }

    return_value = return_value/size; 
    return return_value;
}

/*long filtered_ultradist(long time)
{
  long filtered_dist = 0;
  if (time <= 8)
    time = ((inRange + 1) * speed_of_sound * 2); // Rejects very low readings, kicks readout to outside detection range
  if (lastTime == 0)
    lastTime = time;
  // Compensation parameters for intial start-up
  if (time > (5 * maxDuration))
    time = lastTime;
  // Rejects any reading defined to be out of sensor capacity (>1000)
  // Sets the fault reading to the last known "successful" reading
  if (time > maxDuration)
    time = maxDuration;
  // Caps Reading output at defined maximum distance (~200)
  if ((time - lastTime) < ((-1) * (NoiseReject / 100) * lastTime))
    filtered_dist = (lastTime / 2) * 0.0343; // Noise filter for low range drops
  else
    filtered_dist = (time / 2) * 0.0343;
  lastTime = time; // Stores "successful" reading for filter compensation
  return filtered_dist;
}*/