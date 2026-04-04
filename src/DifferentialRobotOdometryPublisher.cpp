#include "DifferentialRobotOdometryPublisher.h"

DifferentialRobotOdometryPublisher::DifferentialRobotOdometryPublisher(
    rcl_node_t *node, 
    String topic_name,
    String frameId)
{
    publisher = new Vector3StampedPublisher(
        MicroRosPublisher::createVector3Stamped(node, topic_name),
       frameId
    );
}

DifferentialRobotOdometryPublisher::~DifferentialRobotOdometryPublisher()
{
    if (publisher) delete publisher;
}

void DifferentialRobotOdometryPublisher::publish(const DifferentialRobotOdometry &data)
{
    if (publisher) publisher->publish(data.getLeftWInRad(), data.getRightWInRad());
}
