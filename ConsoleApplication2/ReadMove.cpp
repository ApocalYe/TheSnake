#include "ReadMove.h"

#include <conio.h>

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define ARROW_KEY 244

InputDetection::InputDetection()
{
}


InputDetection::~InputDetection()
{
}

Direction InputDetection::readMove()
{
	if (_kbhit()) {  //is true when a key was pressed
		int c = _getch();   //capture the key code and insert into c
		if (c == ARROW_KEY) {
			c = _getch();
			switch (c)
			{
			case KEY_UP:
				return UP;
				break;
			case KEY_DOWN:
				return DOWN;
				break;
			case KEY_LEFT:
				return LEFT;
				break;
			case KEY_RIGHT:
				return RIGHT;
				break;
			default:
				return OTHER;
				break;
			}
		}
	}
}
