#pragma once

class Paddle
{
public:
    Paddle(float scaleY, float speed);

    void Update(float axis, float dt);
    void Clamp(float minY, float maxY);

public:
    float y = 0.0f;
    float scaleY;
    float halfH;
    float speed;
};
