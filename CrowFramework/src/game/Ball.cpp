#include "Ball.h"

void Ball::Reset(int dir)
{
    x = 0.0f;
    y = 0.0f;

    float speed = 0.8f;
    vx = speed * (dir >= 0 ? 1.0f : -1.0f);
    vy = speed * 0.3f;
}

void Ball::Update(float dt)
{
	x += vx * dt;
	y += vy * dt;
}

void Ball::BounceY()
{
	vy = -vy;
}
