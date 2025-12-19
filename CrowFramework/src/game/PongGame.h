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
	void ResetRound(int serveDir);

public:
    Paddle left;
    Paddle right;
    Ball   ball;

    int leftScore = 0;
    int rightScore = 0;

    bool scoredThisFrame = false;
};
