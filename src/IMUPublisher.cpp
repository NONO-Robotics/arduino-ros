#include "IMUPublisher.h"

// Buffer estático para el frame_id. Esta es la clave.
// Su memoria persiste durante toda la vida del programa.
static char frame_id_buffer[50];

IMUPublisher::IMUPublisher(
    rcl_node_t* node, 
    rcl_allocator_t* allocator, 
    String topic_name, 
    String frameId)
{
    // Create the publisher
    rcl_ret_t ret = rclc_publisher_init_default(
        &publisher,
        node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, Imu),
        toCharArray(topic_name));

    if (ret != RCL_RET_OK)
    {
        logger.error("Failed to create IMU publisher.");
        return;
    }

    this->frameId = frameId;

    // 1. Asignar nuestro buffer estático al puntero del mensaje.
    msg.header.frame_id.data = frame_id_buffer;
    // 2. Informar al mensaje sobre la capacidad del buffer.
    msg.header.frame_id.capacity = sizeof(frame_id_buffer);

    // 3. Copiar el contenido del frame_id al buffer.
    strncpy(msg.header.frame_id.data, toCharArray(frameId), msg.header.frame_id.capacity - 1);
    msg.header.frame_id.data[msg.header.frame_id.capacity - 1] = '\0'; // Asegurar terminación

    // 4. Establecer el tamaño del string.
    msg.header.frame_id.size = strlen(msg.header.frame_id.data);

    msgWriter = new IMUMsgWriter(&msg);

    logger.info("IMU publisher created on " + topic_name + " topic.");
}

void IMUPublisher::publish(IMUData* data)
{
    MicroRosTimeUtils::setCurrentStamp(&msg.header);

    msgWriter->write(data);

    if(RCL_RET_ERROR == rcl_publish(&publisher, &msg, NULL)) {
        logger.error("Failed to publish IMU data.");
    }
}
