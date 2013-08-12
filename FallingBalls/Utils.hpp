#pragma once

#include <stdlib.h>

class Utils
{
public:
	// These values are not supposed to be here, but we don't have classes for rendering
	static const int WINDOW_WIDTH = 795, WINDOW_HEIGHT = 500;

	static float randRange(float min, float max)
	{
		return min + (((float)rand() / (float)RAND_MAX) * (max - min));
	}
	static int randRange(int min, int max)
	{
		return ((int)rand() % (max - min + 1)) + min;
	}

	// Simple random colour class
	class RandColour4f
	{
	public:
		RandColour4f()
		{
			red   = randRange(0.0f, 1.0f);
			green = randRange(0.0f, 1.0f);
			blue  = randRange(0.0f, 1.0f);
			alpha = randRange(0.0f, 1.0f);
		}
		// Getters
		float getRed()   { return red;   }
		float getGreen() { return green; }
		float getBlue()  { return blue;  }
		float getAlpha() { return alpha; }

	private:
		float red, green, blue, alpha;
	};
};
