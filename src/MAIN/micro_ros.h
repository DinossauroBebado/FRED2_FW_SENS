#include <micro_ros_arduino.h>
#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <sensor_msgs/msg/imu.h>
#include <rmw_microros/rmw_microros.h>

// Define ROS node and publisher
// extern "C" int clock_gettime(clockid_t unused, struct timespec *tp);

rclc_executor_t executor;
rcl_allocator_t allocator;
rclc_support_t support;
rcl_node_t node;
rcl_publisher_t imu_publisher;
sensor_msgs__msg__Imu imu_msg;

#define LED_PIN 2

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}

void error_loop(){
  for (int i = 0; i<10; i++)
  {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    delay(100);
  }
  ESP.restart();
}

void ros_init()
{
  set_microros_transports();

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);  

  delay(1000);

  allocator = rcl_get_default_allocator();

  // create init_options
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

  RCCHECK(rclc_node_init_default(&node, "micro_ros_sensor_node", "", &support)); 

  // Create a publisher for the IMU data
    rclc_publisher_init_best_effort(
    &imu_publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, Imu),
   "sensor/orientation/imu");

  // Setup IMU message

  // Wait for some time for everything to initialize
  delay(2000);
}

void ros_imu(float *orientation, float *angular_velocity, float *linear_acceleration,float *orientationCovariance,float *linaerAccelerationCovariance,float *angularVelocityCovariance) 
{
  // Read IMU data
  // imu_msg.header.frame_id = 'imu_link';
  // struct timespec tv = {0};
  // clock_gettime(0, &tv);

  imu_msg.header.frame_id.data = "imu_link";
  imu_msg.header.frame_id.size = 10;

  imu_msg.orientation.x = orientation[0];
  imu_msg.orientation.y = orientation[1];
  imu_msg.orientation.z = orientation[2];
  imu_msg.orientation.w = orientation[3]; 

 

  imu_msg.angular_velocity.x = angular_velocity[0];
  imu_msg.angular_velocity.y = angular_velocity[1];
  imu_msg.angular_velocity.z = angular_velocity[2];

  imu_msg.linear_acceleration.x = linear_acceleration[0]; 
  imu_msg.linear_acceleration.y = linear_acceleration[1]; 
  imu_msg.linear_acceleration.z = linear_acceleration[2]; 

      for (int i = 0; i < 9; i ++) {
        imu_msg.orientation_covariance[i] = orientationCovariance[i];
        imu_msg.angular_velocity_covariance[i] =angularVelocityCovariance[i]; 
        imu_msg.linear_acceleration_covariance[i] = linaerAccelerationCovariance[i];
    } 




  RCSOFTCHECK(rmw_uros_sync_session(1000));
	int64_t time = rmw_uros_epoch_millis();

  imu_msg.header.stamp.sec = time;

  // Publish IMU data
  RCSOFTCHECK(rcl_publish(&imu_publisher, &imu_msg, NULL));

  

  // Micro-ROS spin (if needed)
}
