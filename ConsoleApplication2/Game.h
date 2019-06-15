#pragma once
#include "Snake.h"
#include "Food.h"
#include <Windows.h>

enum Direction { UP, DOWN, LEFT, RIGHT, OTHER, NOKEY };

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


	class Snake
	{
	public:
		Snake(Game& parent, int init_length);
		~Snake();
		int x(int index);
		int y(int index);
		int getLength() const;
		void go(Direction direction);
		void deleteTail();
		bool detectCrash(); //Return if it crashes or not. 
	private:
		Game& parent;
		int length;
		struct BodyBlock {
			int x, y;
			BodyBlock* pointer_to_next;
		};
		void createBodyBlock(int x, int y); //Create a new BodyBlock at the head of the snake. And redefine this block as the new head. 
		BodyBlock* head_pt;
		BodyBlock* tail_pt;
		BodyBlock* current_pt;
		int current_index;
	};

	Snake snake;


	class Food
	{
	public:
		void init();
		Food(Game& parent);
		~Food();
		bool eaten();
		int x() { return food_x; }
		int y() { return food_y; }
	private:
		Game& parent;
		int food_x, food_y;
	};

	Food food;


	void display(int invert = false);
	void refresh();
	void gameOver();
};

