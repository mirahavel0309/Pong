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
	ball.Reset();
}

void PongGame::UpdatePlayer(float dt, void* windowPtr)
{
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
	ball.Update(dt);

	if (ball.y + ball.radius >= 1.0f)
	{
		ball.y = 1.0f - ball.radius;
		ball.BounceY();
	}
	else if (ball.y - ball.radius <= -1.0f)
	{
		ball.y = -1.0f + ball.radius;
		ball.BounceY();
	}

	if (ball.x < -1.2f || ball.x > 1.2f)
	{
		ball.Reset();
	}
}

void PongGame::HandleCollisions()
{
	const float leftX = -0.9f;
	const float rightX = 0.9f;
	const float paddleHalfW = 0.03f * 0.5f;

	float speed = std::sqrt(ball.vx * ball.vx + ball.vy * ball.vy);
	if (speed < 0.0001f) speed = 0.8f;

	auto Solve = [&](float paddleX, Paddle& p, bool isLeft)
		{
			float minX = paddleX - paddleHalfW;
			float maxX = paddleX + paddleHalfW;
			float minY = p.y - p.halfH;
			float maxY = p.y + p.halfH;

			float cx, cy;
			if (!CircleAABB(ball.x, ball.y, ball.radius, minX, minY, maxX, maxY, cx, cy))
				return;

			if (isLeft && ball.vx < 0.0f)
			{
				ball.x = maxX + ball.radius;
				ball.vx = std::fabs(ball.vx);
			}
			else if (!isLeft && ball.vx > 0.0f)
			{
				ball.x = minX - ball.radius;
				ball.vx = -std::fabs(ball.vx);
			}
			else
			{
				ball.x = isLeft ? (maxX + ball.radius) : (minX - ball.radius);
				return;
			}

			float t = (ball.y - p.y) / p.halfH;
			t = ClampF(t, -1.0f, 1.0f);

			const float spin = 0.75f;

			float newVy = speed * t * spin;
			float newVxMag = std::sqrt(ClampF(speed * speed - newVy * newVy, 0.0f, 9999.0f));

			ball.vy = newVy;
			ball.vx = isLeft ? +newVxMag : -newVxMag;

			ball.vx *= 1.02f;
			ball.vy *= 1.02f;
		};

	Solve(leftX, left, true);
	Solve(rightX, right, false);
}
