#pragma once

#include "Paddle.h"
#include "Ball.h"

class PongGame
{
public:
    PongGame();

    void UpdatePlayer(float dt, void* window);
    void UpdateAI(float dt);
    void UpdateBall(float dt);
    void HandleCollisions();

public:
    Paddle left;
    Paddle right;
    Ball   ball;
};
