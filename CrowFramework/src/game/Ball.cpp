#include "Ball.h"

void Ball::Reset()
{
    x = 0.0f;
    y = 0.0f;

    float speed = 0.5f;
    vx = speed;
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
