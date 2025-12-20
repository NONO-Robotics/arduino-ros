#pragma once

#include "TwoWheelsRobotW.h"
#include "FourWheelsRobotW.h"

class DifferentialRobotOdometry
{
private:
    float leftW, rightW;

public:
    DifferentialRobotOdometry()
    {
        leftW = 0.0;
        rightW = 0.0;
    }

    float getLeftWInRad() const { return leftW; }

    float getRightWInRad() const { return rightW; }

    void updateFrom(TwoWheelsRobotW robotW)
    {
        updateFrom({robotW.left, robotW.right, robotW.left, robotW.right});
    }

    void updateFrom(FourWheelsRobotW robotW)
    {
        leftW = (robotW.fl + robotW.bl) / 2.0f;
        rightW = (robotW.fr + robotW.br) / 2.0f;
    }
};
