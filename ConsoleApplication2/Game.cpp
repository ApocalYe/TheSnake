#include "Game.h"
#include "Snake.h"
#include <iostream>
#include <chrono>
#include <thread>

using namespace std;


Game::Game(int width, int height, int init_game_speed, int init_snake_length) :width(width), height(height), game_speed(init_game_speed), playing(false), snake(Snake(init_snake_length, width, height)), food(&snake, width, height)
{
	//获取默认标准显示缓冲区句柄
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	//隐藏缓冲区的光标
	CONSOLE_CURSOR_INFO cci;
	cci.bVisible = 0;
	cci.dwSize = 1;
	SetConsoleCursorInfo(hOutput, &cci);

	current_direction = RIGHT;
	score = 0;
}


Game::~Game()
{
}

void Game::display(int invert)
{
	COORD coord = { 0,0 };

	SetConsoleCursorPosition(hOutput, coord);

	int* display_matrix = new int[width*height]();

	//Display snake
	int snake_length = snake.getLength();
	for (int index = 0; index < snake_length; index++) {
		*(display_matrix + snake.x(index) + snake.y(index) * width) = 1;
	}
	
	//Display food
	*(display_matrix + food.x() + food.y() * width) = 2;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			switch (*(display_matrix + y * width + x)) {
			case 0:
				if(!invert)
					cout << "□";
				else
					cout << "■";
				break;
			case 1:
				if (!invert)
					cout << "■";
				else
					cout << "□";
				break;
			case 2:
				cout << "★";
				break;
			}
		}
		cout << "\n";
	}
}


void Game::refresh()
{
	Direction move = readMove();
	switch (move) {
	case UP:
		if(current_direction != DOWN)
			current_direction = move;
		break;
	case DOWN:
		if (current_direction != UP)
			current_direction = move;
		break;
	case LEFT:
		if (current_direction != RIGHT)
			current_direction = move;
		break;
	case RIGHT:
		if (current_direction != LEFT)
			current_direction = move;
		break;
	}
	snake.go(current_direction);
	if (food.eaten()) {
		food.~Food();
		food = Food(&snake, width, height);
		score+=snake.getLength();
		cout << "Score: " << score << endl;
		game_speed++;
	}
	else {
		snake.deleteTail();
	}
	if (snake.detectCrash()) {
		gameOver();
	}
}



Direction Game::readMove()
{
	Direction direction_detected = NOKEY;
	if (GetAsyncKeyState(VK_LEFT) & 0x0001)
		direction_detected = LEFT;
	if (GetAsyncKeyState(VK_UP) & 0x0001)
		direction_detected = UP;
	if (GetAsyncKeyState(VK_RIGHT) & 0x0001)
		direction_detected = RIGHT;
	if (GetAsyncKeyState(VK_DOWN) & 0x0001)
		direction_detected = DOWN;
	return direction_detected;
}


void Game::play()
{
	playing = true;
	while (playing) {
		display();
		this_thread::sleep_for(chrono::milliseconds(1000 / game_speed));
		refresh();
	}
}

void Game::gameOver() {
	playing = false;
	for (int i = 0; i < 10;i++) {
		display(false);
		this_thread::sleep_for(chrono::milliseconds(50));
		display(true);
		this_thread::sleep_for(chrono::milliseconds(50));
	}
	cout << "Score: " << score << endl;
	cout << "Game over! " << endl;
}