#include <iostream>
#include "Checkers.h"
#include "Man.h"
using namespace std;

// определение кто ходит первый через консоль
Color firstColor();

int main() {
	try {
		// количество полуходов, которые просчитываются
		constexpr unsigned int movesNumber = 4;

		string filename;
		cout << "Enter input filename: ";
		cin >> filename;

		Checkers game(filename);

		game.runNMoves(movesNumber, game.currentColor()); // проверяет определённое кол-во ходов
		//game.run(firstColor()); // играет, пока кто то не выиграет (при ничьей будет выполняться бесконечно)

		if (game.state() == GameState::STILL_PLAYING) {
			cout << "Game is not over" << endl;;
		}

		size_t lastindex = filename.find_last_of(".");
		filename = filename.substr(0, lastindex) + "-output.txt";
		game.save(filename);

		cout << endl << "Result was saved. File: " << filename << endl;
	}
	catch (exception& e) {
		cerr << e.what() << endl;
	}

	return 0;
}

// Пользователь вводит цвет, ходящий первым, с клавиатуры
Color firstColor() {
	char firstColor;

	while (true) {
		cout << "First move (B or W): _\b";
		cin >> firstColor;
		firstColor = toupper(firstColor);

		if (strchr("BW", firstColor)) break;

		system("cls");
		cout << "Please, pay attention! Let's try again" << endl;;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	return Color(firstColor == 'W' ? Color::WHITE : Color::BLACK);
}
