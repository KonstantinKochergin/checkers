#pragma once
#include "Figure.h"
#include "Board.h"

#include <algorithm>

// Дамка
class Queen : public Figure {
	bool canAttackFrom(const Board&, const Position&) const;
public:
	explicit Queen(const Position&, Color);
	virtual ~Queen() = default;

	Moves possibleMoves(const Board&, bool onlyAttack = false) const override;
	Moves getPossibleAttacks(Board& board, const Position& pos) const;
	bool isQueen() const override;
};