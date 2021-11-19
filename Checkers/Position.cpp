#include "Position.h"

Position::Position(char _x, char _y) {
	x = _x;
	y = _y;
}

Position::Position(const std::string& pos) {
	set(pos);
}

Position::Position(const Position& pos) {
	x = pos.x;
	y = pos.y;
}

Position::Position(Position&& pos)
	: x(pos.x), y(pos.y) {}

char Position::getX() const { return x; }

char Position::getY() const { return y; }

void Position::set(char _x, char _y) {
	setX(_x);
	setY(_y);
}

void Position::set(const std::string& pos) {
	if (pos.size() != 2) throw PositionException();
	x = toupper(pos.front());
	if (x < 'A' || x > 'H')
		throw PositionException();
	x %= 'A';

	y = pos.back();
	if (y < '1' || y > '8')
		throw PositionException();
	y %= '1';
}

void Position::setX(char _x) {
	if (_x < 0 || _x >= BOARD_SIZE)
		throw PositionException();
	x = _x;
}

void Position::setY(char _y) {
	if (_y < 0 || _y >= BOARD_SIZE)
		throw PositionException();
	y = _y;
}

bool Position::operator==(const Position& o) const {
	return x == o.x && y == o.y;
}

Position& Position::operator=(const Position& o) {
	x = o.x;
	y = o.y;
	return *this;
}

int Position::distanceTo(const Position& o) const {
	return abs(x - o.x) + abs(y - o.y);
}

std::ostream& operator<<(std::ostream& out, const Position& pos) {
	out << char('A' + pos.x) << pos.y + 1;
	return out;
}

std::istream& operator>>(std::istream& in, Position& pos) {
	char _x, _y;
	in >> _y >> _x;

	pos = Position(_x, _y);
	return in;
}