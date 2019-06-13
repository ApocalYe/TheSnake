#pragma once
#include "Snake.h"
#include <Windows.h>

class Game
{
public:
	Game(int width, int height);
	~Game();
	void display();
	void refresh();
private:
	Direction current_direction;
	HANDLE hOutput;
	Direction readMove();
	const int width, height;
	Snake snake = Snake(5, width, height);
};

