#include "Paddle.h"

Paddle::Paddle(float scaleY_, float speed_)
    : scaleY(scaleY_), speed(speed_)
{
    halfH = scaleY * 0.5f;
}

void Paddle::Update(float axis, float dt)
{
    y += axis * speed * dt;
}

void Paddle::Clamp(float minY, float maxY)
{
    if (y < minY) y = minY;
    if (y > maxY) y = maxY;
}
