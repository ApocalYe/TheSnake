#pragma once

enum Direction { UP, DOWN, LEFT, RIGHT, OTHER };

class InputDetection
{
public:
	InputDetection();
	~InputDetection();
	Direction readMove();
};

