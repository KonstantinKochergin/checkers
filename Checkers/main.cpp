#include <iostream>
#include "Checkers.h"
#include "Man.h"
using namespace std;

// ����������� ��� ����� ������ ����� �������
Color firstColor();

int main() {
	try {
		// ���������� ���������, ������� ��������������
		constexpr unsigned int movesNumber = 4;

		string filename;
		cout << "Enter input filename: ";
		cin >> filename;

		Checkers game(filename);

		game.runNMoves(movesNumber, game.currentColor()); // ��������� ����������� ���-�� �����
		//game.run(firstColor()); // ������, ���� ��� �� �� �������� (��� ������ ����� ����������� ����������)

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

// ������������ ������ ����, ������� ������, � ����������
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
