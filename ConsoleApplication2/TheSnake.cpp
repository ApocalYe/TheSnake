#include <iostream>
#include <cassert>
#include <chrono>
#include <thread>

#include "Game.h"


using namespace std;


int main() {
	srand((unsigned)time(NULL));
	while (true) {
		Game game(10, 10);
		game.play();
		system("pause");
		system("cls");
	}
}

