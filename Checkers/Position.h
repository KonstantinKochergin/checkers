#pragma once
#include <iostream>
#include <cstring>

const char BOARD_SIZE = 8;

class PositionException : std::exception {};

// класс позиции фигуры на поле
class Position {
	char x, y;
public:
	explicit Position(char _x, char _y);
	explicit Position(const std::string&);
	Position(const Position&);
	Position(Position&&);
	~Position() = default;

	char getX() const;
	char getY() const;

	void set(char _x, char _y);
	void set(const std::string&);
	void setX(char);
	void setY(char);

	bool operator==(const Position&) const;
	Position& operator=(const Position&);

	int distanceTo(const Position&) const;

	friend std::ostream& operator<<(std::ostream&, const Position&);
	friend std::istream& operator>>(std::istream&, Position&);
};
