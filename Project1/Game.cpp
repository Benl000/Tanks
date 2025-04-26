#include "Game.h"
#include <iostream>

void Game::run()
{
	setMode(RUN);
	init();
}

void Game::init()
{
	board.init();
	mainMenu();
}

void Game::setMode(Status s)
{
	mode = s;
}

void Game::mainMenu() 
{
	system("cls");
	cout << "Choose an option: " << endl;
	cout << "=== Tank Battle ===" << endl;
	cout << "(1) Start a new game" << endl;
	cout << "(8) Instructions" << endl;
	cout << "(9) Exit" << endl;


}