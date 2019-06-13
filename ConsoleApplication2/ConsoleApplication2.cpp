// ConsoleApplication2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <conio.h>
#include <iostream>
#include <cassert>
#include <chrono>
#include <thread>


#include "Game.h"

using namespace std;


int main() {
	Game game(10, 10);
	while (1) {
		game.refresh();
		game.display();
		this_thread::sleep_for(chrono::milliseconds(500));

	}
	system("pause");
}

