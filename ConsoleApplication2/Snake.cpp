#include "Snake.h"
#include <cassert>


Snake::Snake(int init_length, int width, int height):width(width), height(height)
{
	assert(init_length > 0);
	length = 0;
	current_index = 0;
	current_pt = 0;
	head_pt = 0;
	tail_pt = 0;
	while (length < init_length) {
		createBodyBlock(length, (height+1)/2-1);
	}
}


Snake::~Snake()
{
	BodyBlock* next_block,* this_block;
	this_block = tail_pt;
	while (length > 0) {
		next_block = this_block->pointer_to_next;
		delete this_block;
		this_block = next_block;
		length--;
	}
}

int Snake::x(int index)
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


int Snake::y(int index)
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

int Snake::getLength() const
{
	return length;
}


void Snake::createBodyBlock(int x, int y) {
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

void Snake::deleteTail()
{
	BodyBlock* new_tail = tail_pt->pointer_to_next;
	delete tail_pt;
	tail_pt = new_tail;
	current_pt = tail_pt;
	current_index = 0;
	length--;
}

void Snake::go(Direction direction) {
	int next_x = -1, next_y = -1;
	switch (direction) {
	case UP:
		next_x = head_pt->x;
		next_y = (head_pt->y - 1 + height) % height;
		break;
	case DOWN:
		next_x = head_pt->x;
		next_y = (head_pt->y + 1) % height;
		break;
	case LEFT:
		next_x = (head_pt->x - 1 + width) % width;
		next_y = head_pt->y;
		break;
	case RIGHT:
		next_x = (head_pt->x + 1) % width;
		next_y = head_pt->y;
		break;
	}
	assert(next_x >= 0);
	assert(next_y >= 0);
	createBodyBlock(next_x, next_y);
}

bool Snake::detectCrash() {
	for (int index1 = 0; index1 < length; index1++) {
		for (int index2 = index1 + 1; index2 < length; index2++) {
			if ((x(index1) == x(index2)) && (y(index1) == y(index2))) {
				return true;
			}
		}
	}
	return false;
}