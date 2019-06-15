#include "Game.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <cassert>
#include <random>

using namespace std;


Game::Game(int width, int height, int init_game_speed, int init_snake_length) :width(width), height(height), game_speed(init_game_speed), playing(false), snake(Snake(*this, init_snake_length)), food(*this)
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
				if (!invert)
					cout << "★";
				else
					cout << "☆";
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
		food.init();
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





Game::Snake::Snake(Game& parent, int init_length) :parent(parent)
{
	assert(init_length > 0);
	length = 0;
	current_index = 0;
	current_pt = 0;
	head_pt = 0;
	tail_pt = 0;
	while (length < init_length) {
		createBodyBlock(length, (parent.height + 1) / 2 - 1);
	}
}


Game::Snake::~Snake()
{
	BodyBlock* next_block, * this_block;
	this_block = tail_pt;
	while (length > 0) {
		next_block = this_block->pointer_to_next;
		delete this_block;
		this_block = next_block;
		length--;
	}
}

int Game::Snake::x(int index)
{
	assert(index < length);
	if (current_index > index) {
		current_pt = tail_pt;
		current_index = 0;
	}
	while (current_index < index) {
		current_pt = current_pt->pointer_to_next;
		current_index++;
	}
	return current_pt->x;
}


int Game::Snake::y(int index)
{
	if (index < current_index) {
		current_pt = tail_pt;
		current_index = 0;
	}
	while (index > current_index) {
		current_pt = current_pt->pointer_to_next;
		current_index++;
	}
	return current_pt->y;
}

int Game::Snake::getLength() const
{
	return length;
}


void Game::Snake::createBodyBlock(int x, int y) {
	BodyBlock* pt_new_body_block = new BodyBlock();
	pt_new_body_block->pointer_to_next = 0;
	pt_new_body_block->x = x;
	pt_new_body_block->y = y;
	if (length != 0) {
		head_pt->pointer_to_next = pt_new_body_block;
	}
	else {
		tail_pt = pt_new_body_block;
		current_pt = tail_pt;
	}
	head_pt = pt_new_body_block;
	length++;
}

void Game::Snake::deleteTail()
{
	BodyBlock* new_tail = tail_pt->pointer_to_next;
	delete tail_pt;
	tail_pt = new_tail;
	current_pt = tail_pt;
	current_index = 0;
	length--;
}

void Game::Snake::go(Direction direction) {
	int next_x = -1, next_y = -1;
	switch (direction) {
	case UP:
		next_x = head_pt->x;
		next_y = (head_pt->y - 1 + parent.height) % parent.height;
		break;
	case DOWN:
		next_x = head_pt->x;
		next_y = (head_pt->y + 1) % parent.height;
		break;
	case LEFT:
		next_x = (head_pt->x - 1 + parent.width) % parent.width;
		next_y = head_pt->y;
		break;
	case RIGHT:
		next_x = (head_pt->x + 1) % parent.width;
		next_y = head_pt->y;
		break;
	}
	assert(next_x >= 0);
	assert(next_y >= 0);
	createBodyBlock(next_x, next_y);
}

bool Game::Snake::detectCrash() {
	for (int index1 = 0; index1 < length; index1++) {
		for (int index2 = index1 + 1; index2 < length; index2++) {
			if ((x(index1) == x(index2)) && (y(index1) == y(index2))) {
				return true;
			}
		}
	}
	return false;
}


void Game::Food::init() {
	bool usable = false;
	do
	{
		food_x = std::rand() % parent.width;
		food_y = std::rand() % parent.height;
		usable = !eaten();
	} while (!usable);
}

Game::Food::Food(Game& parent) : parent(parent)
{
	init();
}


Game::Food::~Food()
{
}

bool Game::Food::eaten()
{
	int snake_length = parent.snake.getLength();
	for (int index = 0; index < snake_length; index++) {
		if ((parent.snake.x(index) == food_x) && (parent.snake.y(index) == food_y)) {
			return true;
		}
	}
	return false;
}
