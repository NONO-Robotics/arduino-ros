#pragma once

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

    void updateFrom(
        float fl,
        float fr,
        float bl,
        float br)
    {
        leftW = (fl + bl) / 2.0f;
        rightW = (fr + br) / 2.0f;
    }
};
