#include "GPSPublisher.h"


// Buffer estático para el frame_id. Esta es la clave.
// Su memoria persiste durante toda la vida del programa.
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

    // 1. Asignar nuestro buffer estático al puntero del mensaje.
    msg.header.frame_id.data = frame_id_buffer;
    // 2. Informar al mensaje sobre la capacidad del buffer.
    msg.header.frame_id.capacity = sizeof(frame_id_buffer);

    // 3. Copiar el contenido del frame_id al buffer.
    strncpy(msg.header.frame_id.data, toCharArray(frameId), msg.header.frame_id.capacity - 1);
    msg.header.frame_id.data[msg.header.frame_id.capacity - 1] = '\0'; // Asegurar terminación

    // 4. Establecer el tamaño del string.
    msg.header.frame_id.size = strlen(msg.header.frame_id.data);

    logger.info("GPS publisher created on " + topic_name + " topic.");
}

void GPSPublisher::publish(GPSData* data)
{
    data->writeTo(msg);

    this->prepareMsg();

    rcl_publish(&publisher, &msg, NULL);
}

void GPSPublisher::prepareMsg()
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    msg.header.stamp.sec = ts.tv_sec;
    msg.header.stamp.nanosec = ts.tv_nsec;
}