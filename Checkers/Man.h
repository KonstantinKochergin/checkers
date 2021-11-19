#pragma once
#include "Figure.h"
#include "Board.h"
#include "Queen.h"

#include <algorithm>

// Шашка
class Man : public Figure {
	void eatMove(Board&, const Position& current, const Position& opponent, Moves*, size_t idx) const;
	void queenMove(Board&, const Position& current, Moves&) const;

	bool onQueenPositionIf(const Board&, const Position&) const;

public:
	explicit Man(const Position&, Color);
	virtual ~Man() = default;

	Moves possibleMoves(const Board&, bool onlyAttack = false) const override;
	bool isQueen() const override;
	bool onQueenPosition() const;
};

Board boardCopy(const Board&);
void moveFigure(Board*, const Position& from, const Position& to);
void killOpponent(Board*, const Position& opponentPosition, bool removeBlack);