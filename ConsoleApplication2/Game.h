#pragma once
#include "Snake.h"
#include "Food.h"
#include <Windows.h>

class Game
{
public:
	Game(int width = 10, int height = 10, int init_game_speed = 5, int init_snake_length = 5);
	~Game();
	void play();
private:
	int score;
	bool playing;
	int game_speed; //Frequency to refresh. 
	Direction current_direction;
	HANDLE hOutput;
	Direction readMove();
	const int width, height;
	Snake snake;
	Food food;
	void display(int invert = false);
	void refresh();
	void gameOver();
};

