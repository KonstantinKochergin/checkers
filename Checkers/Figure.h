#pragma once
#include "Color.h"
#include "Position.h"

#include <vector>
#include <array>
#include <iostream>

struct Board;

// вектор векторов позиций, объ€вил здесь, чтобы сократить код
using Moves = std::vector<std::vector<Position>>;

// Ѕазовый класс фигуры, от которого наследуетс€ шашка и дамка
class Figure {
protected:
	Color color;
	Position position;

public:
	explicit Figure(const Position&, Color);
	virtual ~Figure() = default;

	virtual Moves possibleMoves(const Board&, bool onlyAttack = false) const = 0;
	virtual bool isQueen() const = 0;
	bool isWhite() const;
	bool isBlack() const;
	Color getColor() const;

	const Position& getPosition() const;
	void setPosition(char, char);
	void setPosition(const Position&);
	char getX() const;
	char getY() const;
};