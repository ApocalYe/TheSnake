#pragma once

#include "Snake.h"

class Food
{
public:
	Food(Snake *snake, int width, int height);
	~Food();
	bool eaten();
	int x() { return food_x; }
	int y() { return food_y; }
private:
	int width, height;
	Snake *snake;
	int food_x, food_y;
};

