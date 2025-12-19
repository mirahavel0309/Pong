#pragma once

struct Ball
{
    float x = 0.0f;
    float y = 0.0f;

    float vx = 0.0f;
    float vy = 0.0f;

    float radius = 0.02f;

    void Reset();
    void Update(float dt);
    void BounceY();
};
