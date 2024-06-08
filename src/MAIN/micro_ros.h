#include <micro_ros_arduino.h>
#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <sensor_msgs/msg/imu.h>
#include <sensor_msgs/msg/range.h>
#include <rmw_microros/rmw_microros.h>
#include <std_msgs/msg/int16.h>
#include <std_msgs/msg/bool.h>
#include <std_msgs/msg/color_rgba.h>

rclc_executor_t executor;
rcl_allocator_t allocator;
rclc_support_t support;
rcl_node_t node;


//subs 
rcl_subscription_t led_manager_sub;
std_msgs__msg__Int16 led_msg;

//pubs 
rcl_publisher_t imu_publisher;
sensor_msgs__msg__Imu imu_msg;

// ultrasound 
rcl_publisher_t ultrasound_publisher_left;
std_msgs__msg__Int16 range_msg_left;

rcl_publisher_t ultrasound_publisher_right;
std_msgs__msg__Int16 range_msg_right;


rcl_publisher_t ultrasound_publisher_back;
std_msgs__msg__Int16 range_msg_back;


rcl_publisher_t inductive_publisher;
std_msgs__msg__Bool inductive_msg;

rcl_publisher_t color_sensor_publisher;
std_msgs__msg__ColorRGBA color_sensor_msg;

int led_color = 0 ;

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


void ros_ultrasonic(int *ultrasonic_range){
  RCSOFTCHECK(rmw_uros_sync_session(1000));
  int64_t time = rmw_uros_epoch_millis();
  
 
  

  // range_msg_left.header.frame_id.data = "left_ultrasonic_link";
 
  range_msg_right.data = ultrasonic_range[2];
  range_msg_back.data = ultrasonic_range[1];
  range_msg_left.data = ultrasonic_range[0]; // Current range value

  RCSOFTCHECK(rcl_publish(&ultrasound_publisher_left, &range_msg_left, NULL));
  RCSOFTCHECK(rcl_publish(&ultrasound_publisher_right, &range_msg_right, NULL));
  RCSOFTCHECK(rcl_publish(&ultrasound_publisher_back, &range_msg_back, NULL));

}



void led_manager_sub_callback(const void *msgin)
{
  const std_msgs__msg__Int16 *msg = (const std_msgs__msg__Int16 *)msgin;
  led_color = msg->data;
  led_strip_controler_ros(led_color);

}


void ros_init()
{

  //set transport for serial 
  set_microros_transports();


  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);  

  delay(1000);

  allocator = rcl_get_default_allocator();

  // create init_options
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));
  //init node 
  RCCHECK(rclc_node_init_default(&node, "micro_ros_sensor_node", "", &support)); 
  //init executor 
  RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));

  // create subscriber for led 
  RCCHECK(rclc_subscription_init_default(
    &led_manager_sub,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int16),
    "cmd/led_strip/color"));

  // Create a publisher for the IMU data
   RCCHECK(rclc_publisher_init_best_effort(
    &imu_publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, Imu),
   "sensor/orientation/imu"));

   //create pub for ultrasounds 

   RCCHECK(rclc_publisher_init_default(
      &ultrasound_publisher_left,
      &node,
       ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int16),
      "sensor/range/ultrasonic/left"));

  RCCHECK(rclc_publisher_init_default(
    &ultrasound_publisher_right,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int16),
    "sensor/range/ultrasonic/right"));
  
  RCCHECK(rclc_publisher_init_default(
  &ultrasound_publisher_back,
  &node,
  ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int16),
  "sensor/range/ultrasonic/back"));

  //create pub for inductive sensor
  RCCHECK(rclc_publisher_init_default(
      &inductive_publisher,
      &node,
       ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool),
      "sensor/inductive/read"));
  //create pub for COLOR SENSOR
  RCCHECK(rclc_publisher_init_default(
      &color_sensor_publisher,
      &node,
       ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, ColorRGBA),
      "sensor/color/read"));

  RCCHECK(rclc_executor_add_subscription(&executor, &led_manager_sub, &led_msg, &led_manager_sub_callback, ON_NEW_DATA));

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

}

void ros_goal_detectors(bool metalDetection, float r, float g, float b)
{
  inductive_msg.data = metalDetection;
  color_sensor_msg.a = 0;
  color_sensor_msg.r = r;
  color_sensor_msg.g = g;
  color_sensor_msg.b = b;
  // Publish IMU data
  RCSOFTCHECK(rcl_publish(&inductive_publisher, &inductive_msg, NULL));
  RCSOFTCHECK(rcl_publish(&color_sensor_publisher, &color_sensor_msg, NULL));
}
