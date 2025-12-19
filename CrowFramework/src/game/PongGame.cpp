#include "PongGame.h"
#include <GLFW/glfw3.h>
#include <cmath>

static float ClampF(float v, float lo, float hi)
{
	return (v < lo) ? lo : (v > hi) ? hi : v;
}
 
static bool CircleAABB(float cx, float cy, float r,
	float minX, float minY, float maxX, float maxY,
	float& outClosestX, float& outClosestY)
{
	float closestX = ClampF(cx, minX, maxX);
	float closestY = ClampF(cy, minY, maxY);

	float dx = cx - closestX;
	float dy = cy - closestY;

	outClosestX = closestX;
	outClosestY = closestY;

	return (dx * dx + dy * dy) <= (r * r);
}

PongGame::PongGame()
	: left(0.30f, 1.5f)
	, right(0.30f, 1.2f)
{
	ball.Reset(1);
}


void PongGame::UpdatePlayer(float dt, void* windowPtr)
{
	if (gameOver) return;

	GLFWwindow* window = (GLFWwindow*)windowPtr;

	float axis = 0.0f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) axis += 1.0f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) axis -= 1.0f;

	left.Update(axis, dt);

	float minY = -1.0f + left.halfH;
	float maxY = 1.0f - left.halfH;
	left.Clamp(minY, maxY);
}

void PongGame::UpdateAI(float dt)
{
	if (gameOver) return;

	float diff = ball.y - right.y;

	float axis = 0.0f;
	const float deadZone = 0.02f;
	if (diff > deadZone) axis = 1.0f;
	if (diff < -deadZone) axis = -1.0f;

	right.Update(axis, dt);

	float minY = -1.0f + right.halfH;
	float maxY = 1.0f - right.halfH;
	right.Clamp(minY, maxY);
}

void PongGame::UpdateBall(float dt)
{
	if (gameOver) return;

	scoredThisFrame = false;

	ball.Update(dt);

	if (ball.y + ball.halfSize >= 1.0f)
	{
		ball.y = 1.0f - ball.halfSize;
		ball.BounceY();
	}
	else if (ball.y - ball.halfSize <= -1.0f)
	{
		ball.y = -1.0f + ball.halfSize;
		ball.BounceY();
	}

	if (ball.x - ball.halfSize <= -1.0f)
	{
		rightScore++;
		scoredThisFrame = true;
		ResetRound(-1);
	}
	else if (ball.x + ball.halfSize >= 1.0f)
	{
		leftScore++;
		scoredThisFrame = true;
		ResetRound(+1);
	}

	ball.speedMul += timeAccel * dt;
	if (ball.speedMul > maxBallSpeedMul) ball.speedMul = maxBallSpeedMul;

	float curSpeed = std::sqrt(ball.vx * ball.vx + ball.vy * ball.vy);
	if (curSpeed > 0.0001f)
	{
		float targetSpeed = ball.baseSpeed * ball.speedMul;
		float k = targetSpeed / curSpeed;
		ball.vx *= k;
		ball.vy *= k;
	}

	if (leftScore >= 10)
	{
		gameOver = true;
		winner = 1;
	}
	else if (rightScore >= 10)
	{
		gameOver = true;
		winner = 2;
	}


}

void PongGame::HandleCollisions()
{
	const float leftX = -0.9f;
	const float rightX = 0.9f;
	const float paddleHalfW = 0.03f * 0.5f;

	auto Solve = [&](float paddleX, Paddle& p, bool isLeft)
		{
			float ballMinX = ball.x - ball.halfSize;
			float ballMaxX = ball.x + ball.halfSize;
			float ballMinY = ball.y - ball.halfSize;
			float ballMaxY = ball.y + ball.halfSize;

			float paddleMinX = paddleX - paddleHalfW;
			float paddleMaxX = paddleX + paddleHalfW;
			float paddleMinY = p.y - p.halfH;
			float paddleMaxY = p.y + p.halfH;

			if (ballMaxX < paddleMinX || ballMinX > paddleMaxX ||
				ballMaxY < paddleMinY || ballMinY > paddleMaxY)
				return;

			if (isLeft && ball.vx < 0.0f)
			{
				ball.x = paddleMaxX + ball.halfSize;
			}
			else if (!isLeft && ball.vx > 0.0f)
			{
				ball.x = paddleMinX - ball.halfSize;
			}
			else
			{
				return;
			}

			float t = (ball.y - p.y) / p.halfH;
			t = ClampF(t, -1.0f, 1.0f);

			ball.speedMul += hitAccel;
			if (ball.speedMul > maxBallSpeedMul)
				ball.speedMul = maxBallSpeedMul;

			float targetSpeed = ball.baseSpeed * ball.speedMul;

			float newVy = targetSpeed * t;
			float newVx = std::sqrt(ClampF(targetSpeed * targetSpeed - newVy * newVy, 0.0f, 9999.0f));
			newVx = isLeft ? +newVx : -newVx;

			ball.vx = newVx;
			ball.vy = newVy;
		};

	Solve(leftX, left, true);
	Solve(rightX, right, false);
}

void PongGame::ResetRound(int serveDir)
{
	left.y = 0.0f;
	right.y = 0.0f;

	ball.Reset(serveDir);

	ball.x = 0.0f;
	ball.y = 0.0f;
}

void PongGame::ResetMatch()
{
	leftScore = 0;
	rightScore = 0;
	gameOver = false;
	winner = 0;

	ResetRound(1);
}