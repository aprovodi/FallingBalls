#pragma once

#include <algorithm>

#include "Utils.hpp"

/* Abstract Shape that is supposed to move on the screen */
class MShape
{
public:
	typedef std::shared_ptr<MShape> MShapePtr;

	virtual void update() = 0;
	virtual void draw() = 0;

	std::pair<float, float> getSpeed() const { return speed_; }

	virtual void increaseSpeed(float scale) = 0;
	virtual void decreaseSpeed(float scale) = 0;
	//virtual bool isPointInside(const std::pair<int,int>& p) = 0;
	virtual bool isBoundaryReached() = 0;

	static void sqInitialSpeed(float s) { initial_speed = std::min(initial_speed * s, max_initial_speed); }
	static void hfInitialSpeed(float s) { initial_speed = std::max(initial_speed / s, min_initial_speed); }

	virtual ~MShape() { }

protected:
	static float initial_speed;
	static const float max_initial_speed;
	static const float min_initial_speed;
	std::pair<float, float> speed_;
};

class FallingBall : public MShape
{ 
public:
	typedef std::shared_ptr<FallingBall> FallingBallPtr;

	FallingBall() : ballRadius(50.f), ballX(Utils::WINDOW_WIDTH/2.f), ballY(0.0f) {	initialise(); } 
	FallingBall(float r, float x, float y) : ballRadius(r), ballX(x), ballY(y) { initialise(); } 
	void initialise()
	{
		speed_ = std::pair<float, float>(0.0f, MShape::initial_speed / ballRadius);
		updateDistanceToCenter();
		setCost();
	}

	// Getters
	float getRadius() const { return ballRadius; }
	float getX() const { return ballX; }
	float getY() const { return ballY;  }
	int getCost() const { return ballCost; }

	// Function to calculate cost of the ball, depending on  the speed
	void setCost() { ballCost = 20*speed_.second; }

	void increaseSpeed(float scale) { speed_.second = std::min(speed_.second * scale, max_initial_speed/ballRadius); setCost(); }
	void decreaseSpeed(float scale) { speed_.second = std::max(speed_.second / scale, min_initial_speed/ballRadius); setCost(); }

	bool isPointInside(const std::pair<int,int>& p) { return (sqrt((ballX-p.first)*(ballX-p.first) + (ballY-p.second)*(ballY-p.second)) <= ballRadius); }
	bool isBoundaryReached() { return (ballY + ballRadius) > Utils::WINDOW_HEIGHT; }

	void update() { ballY += speed_.second; updateDistanceToCenter(); }
	void draw()
	{
		// Use triangular segments to form a circle
		glBegin(GL_TRIANGLE_FAN);

		glColor4f(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());

		// Center of circle
		glVertex2f((GLfloat)getX(), (GLfloat)getY());
		int numSegments = 100;
		GLfloat angle;
		// Last vertex same as first vertex
		for (int i = 0; i <= numSegments; i++) {
			// 360 deg for all segments
			angle = i * 2.0f * 3.14f / numSegments;
			glVertex2f(getX() + cos(angle) * getRadius(), getY() + sin(angle) * getRadius());
		}
		glEnd();
	}

private:
	float ballRadius;  // Radius of the bouncing ball
	float ballX, ballY;  // Ball's center (x, y) position
	Utils::RandColour4f color;  // Ball's color
	float distanceToCenter;
	int ballCost;

	// Currently not used. Calculating distance from the center of main screen to sort balls in a SET by this value
	void updateDistanceToCenter() { distanceToCenter = sqrt((ballX-Utils::WINDOW_WIDTH/2.f)*(ballX-Utils::WINDOW_WIDTH/2.f) + (ballY-Utils::WINDOW_HEIGHT/2.f)*(ballY-Utils::WINDOW_HEIGHT/2.f)); }
};
