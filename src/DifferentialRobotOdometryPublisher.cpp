#include "DifferentialRobotOdometryPublisher.h"

DifferentialRobotOdometryPublisher::DifferentialRobotOdometryPublisher(rcl_node_t *node, String topic_name)
{
    publisher = new FloatArrayPublisher(MicroRosPublisher::createFloatArray(node, topic_name), 2);
}

void DifferentialRobotOdometryPublisher::publish(const DifferentialRobotOdometry &data)
{
    float w_data[] = {data.getLeftWInRad(), data.getRightWInRad()};

    publisher->publish(w_data);
}

DifferentialRobotOdometryPublisher::~DifferentialRobotOdometryPublisher() {
    delete publisher;
}