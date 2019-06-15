#include "Food.h"
#include <random>


Food::Food(Snake *snake, int width, int height) : snake(snake), width(width), height(height)
{
	bool usable = false;
	do
	{
		food_x = std::rand() % width;
		food_y = std::rand() % height;
		usable = !eaten();
	} while (!usable);
}


Food::~Food()
{
}

bool Food::eaten()
{
	int snake_length = snake->getLength();
	for (int index = 0; index < snake_length; index++) {
		if ((snake->x(index) == food_x) && (snake->y(index) == food_y)) {
			return true;
		}
	}
	return false;
}
