#pragma once

struct Ball
{
    float x = 0.0f;
    float y = 0.0f;

    float vx = 0.0f;
    float vy = 0.0f;

    float halfSize = 0.02f;

    void Reset(int dir);
    void Update(float dt);
    void BounceY();

public:
    float baseSpeed = 0.8f;
    float speedMul = 1.0f;
};
