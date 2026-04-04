#include "RosMessage.h"
#include "Logger.h"
#include <rosidl_runtime_c/string_functions.h>

std_msgs__msg__Float32 *createFloatMessage()
{
  std_msgs__msg__Float32 *msg = new std_msgs__msg__Float32();
  msg->data = 0;
  return msg;
}

std_msgs__msg__Int32 *createIntMessage()
{
  std_msgs__msg__Int32 *msg = new std_msgs__msg__Int32();
  msg->data = 0;
  return msg;
}

std_msgs__msg__Float32MultiArray *createFloatArrayMessage(size_t length) 
{
  std_msgs__msg__Float32MultiArray *msg = new std_msgs__msg__Float32MultiArray();
  msg->data.data = (float *)malloc(sizeof(float) * length);
  if (msg->data.data == NULL) {
    delete msg;
    return NULL;
  }
  msg->data.size = length;
  msg->data.capacity = length;
  return msg;
}

geometry_msgs__msg__Vector3Stamped *createVector3StampedMessage(String frameId) 
{
  geometry_msgs__msg__Vector3Stamped *msg = geometry_msgs__msg__Vector3Stamped__create();
  if (msg == NULL) return NULL;

  if (!rosidl_runtime_c__String__assign(&msg->header.frame_id, frameId.c_str())) {
    geometry_msgs__msg__Vector3Stamped__destroy(msg);
    return NULL;
  }

  return msg;
}
