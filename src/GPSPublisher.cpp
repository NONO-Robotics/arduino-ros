#include "GPSPublisher.h"


// Static buffer for the frame_id. This is key.
// Its memory persists for the entire lifetime of the program.
static char frame_id_buffer[50];


GPSPublisher::GPSPublisher(
    rcl_node_t* node, 
    rcl_allocator_t* allocator, 
    String topic_name, 
    String frameId)
{
    // Create the publisher
    rcl_ret_t ret = rclc_publisher_init_default(
        &publisher,
        node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, NavSatFix),
        toCharArray(topic_name));

    if (ret != RCL_RET_OK)
    {
        logger.error("Failed to create GPS publisher.");
        return;
    }

    this->frameId = frameId;

    // 1. Assign our static buffer to the message pointer.
    msg.header.frame_id.data = frame_id_buffer;
    // 2. Inform the message about the buffer capacity.
    msg.header.frame_id.capacity = sizeof(frame_id_buffer);

    // 3. Copy the frame_id content to the buffer.
    strncpy(msg.header.frame_id.data, toCharArray(frameId), msg.header.frame_id.capacity - 1);
    msg.header.frame_id.data[msg.header.frame_id.capacity - 1] = '\0'; // Ensure null-termination

    // 4. Set the size of the string.
    msg.header.frame_id.size = strlen(msg.header.frame_id.data);

    msgWriter = new NavSatFixMsgWriter(&msg);

    logger.info("GPS publisher created on " + topic_name + " topic.");
}

void GPSPublisher::publish(GPSData* data)
{
    msgWriter->write(data);

    this->prepareMsg();

    if(RCL_RET_ERROR == rcl_publish(&publisher, &msg, NULL)) {
        logger.error("Failed to publish GPS data.");
    }
}

void GPSPublisher::prepareMsg()
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    msg.header.stamp.sec = ts.tv_sec;
    msg.header.stamp.nanosec = ts.tv_nsec;
}