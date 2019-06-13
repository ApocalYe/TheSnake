#include "Game.h"
#include "Snake.h"
#include <conio.h>
#include <iostream>

using namespace std;


Game::Game(const int width, const int height) :width(width), height(height)
{
	//获取默认标准显示缓冲区句柄
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	//隐藏缓冲区的光标
	CONSOLE_CURSOR_INFO cci;
	cci.bVisible = 0;
	cci.dwSize = 1;
	SetConsoleCursorInfo(hOutput, &cci);

	current_direction = RIGHT;
}


Game::~Game()
{
}

void Game::display()
{
	COORD coord = { 0,0 };

	SetConsoleCursorPosition(hOutput, coord);

	bool* display_matrix = new bool[width*height]();
	int snake_length = snake.getLength();
	for (int index = 0; index < snake_length; index++) {
		*(display_matrix + snake.x(index) + snake.y(index) * width) = 1;
	}
	
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (*(display_matrix + y * width + x)) {
				cout << "■";
			}
			else {
				cout << "□";
			}
		}
		cout << "\n";
	}
}

void Game::refresh()
{
	Direction move = readMove();
	if (move != NOKEY)
		current_direction = move;
	snake.go(current_direction, 0);
}


Direction Game::readMove()
{
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		return LEFT;
	if (GetAsyncKeyState(VK_UP) & 0x8000)
		return UP;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		return RIGHT;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		return DOWN;
	return NOKEY;
}
