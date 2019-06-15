#pragma once

enum Direction { UP, DOWN, LEFT, RIGHT, OTHER, NOKEY };

class Snake
{
public:
	Snake(int init_length, int width, int height);
	~Snake();
	int x(int index);
	int y(int index);
	int getLength() const;
	void go(Direction direction); 
	void deleteTail();
	bool detectCrash(); //Return if it crashes or not. 
private:
	int length;
	int width, height;
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

