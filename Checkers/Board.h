#pragma once
#include <array>
#include <exception>

#include "Figure.h"
#include "State.h"

class BoardException : std::exception {};

// Состояние доски, для хранения ходов(позиции фигур на доске после хода)
struct Board {
private:
	int blackQueenN = 0, whiteQueenN = 0;

public:
	explicit Board(State _state = { Color::WHITE, GameState::STILL_PLAYING }, int _blackQueenN = 0, int _whiteQueenN = 0);
	~Board() = default;

	std::array<std::array<Figure*, BOARD_SIZE>, BOARD_SIZE> data{ nullptr };
	std::vector<std::shared_ptr<Figure>> black, white;
	State state;
	std::vector<Position> history;

	bool isEmpty(char x, char y) const;
	bool isEmpty(const Position&) const;
	bool onBoard(char x, char y) const;
	bool onBoard(const Position&) const;
	double score() const;

	int getBlackQueenN() const;
	int getWhiteQueenN() const;

	Figure*& operator[](const Position&);

	void changeTurn();
	void changeGameState(GameState);

	void increaseQueen(Color _color);

	std::pair<bool, Position> between(
		const Position& from,
		const Position& to,
		Color target) const;

	friend std::ostream& operator<<(std::ostream&, const Board&);
	void printHistory(std::ostream&) const;
};