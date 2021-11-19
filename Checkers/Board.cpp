#include "Board.h"

Board::Board(State _state, int _blackQueenN, int _whiteQueenN)
	: state(_state), data{ nullptr },
	blackQueenN(_blackQueenN), whiteQueenN(_whiteQueenN) {}

bool Board::isEmpty(char x, char y) const {
	if (!onBoard(x, y)) throw BoardException();
	return data[x][y] == nullptr;
}

bool Board::isEmpty(const Position& position) const {
	if (!onBoard(position)) throw BoardException();
	return data[position.getX()][position.getY()] == nullptr;
}

bool Board::onBoard(char x, char y) const {
	return x >= 0 && x < BOARD_SIZE
		&& y >= 0 && y < BOARD_SIZE;
}

bool Board::onBoard(const Position& pos) const {
	return onBoard(pos.getX(), pos.getY());
}

double Board::score() const {
	return double(white.size()) - double(black.size()) + whiteQueenN * 0.5 - blackQueenN * 0.5;
}

int Board::getBlackQueenN() const {
	return blackQueenN;
}

int Board::getWhiteQueenN() const {
	return whiteQueenN;
}

Figure*& Board::operator[](const Position& position) {
	if (!onBoard(position))
		throw BoardException();

	return data[position.getX()][position.getY()];
}

void Board::changeTurn() {
	state.turnColor = state.turnColor == Color::WHITE ? Color::BLACK : Color::WHITE;
}

void Board::changeGameState(GameState _state) {
	state.state = _state;
}

void Board::increaseQueen(Color _color) {
	if (_color == Color::WHITE) whiteQueenN++;
	else blackQueenN++;
}

std::pair<bool, Position> Board::between(const Position& from, const Position& to, Color target) const {
	if (onBoard(from) && onBoard(to) && !(from == to)) {
		char signX = from.getX() > to.getX() ? -1 : 1;
		char signY = from.getY() > to.getY() ? -1 : 1;

		auto maxStep = data[from.getX()][from.getY()]->isQueen() ? BOARD_SIZE : 2;
		for (char step = 1; step < maxStep; step++) {
			char nx = from.getX() + step * signX,
				ny = from.getY() + step * signY;

			if (nx == to.getX() && ny == to.getY()) break;

			if (onBoard(nx, ny) &&
				!isEmpty(nx, ny) &&
				data[nx][ny]->getColor() == target) {
				return { true, Position(nx, ny) };
			}
		}
	}

	return { false, Position(0, 0) };
}

void Board::printHistory(std::ostream& out) const {
	out << (state.turnColor == Color::WHITE ? " White: " : " Black: ");
	for (size_t i = 0; i < history.size() - 1; i++)
		out << history[i] << " -> ";
	out << history.back() << std::endl;
}

std::ostream& operator<<(std::ostream& out, const Board& board) {
	out << "   ";
	for (int i = 0; i < BOARD_SIZE; i++)
		out << "+---";
	out << "+" << std::endl;

	for (int x = 0; x < BOARD_SIZE; x++) {
		out << " " << BOARD_SIZE - x << " | ";
		for (int y = 0; y < BOARD_SIZE; y++) {
			char s = ' ';

			if (!board.isEmpty(y, BOARD_SIZE - 1 - x)) {
				if (board.data[y][BOARD_SIZE - 1 - x]->isQueen())
					s = board.data[y][BOARD_SIZE - 1 - x]->getColor() == Color::WHITE ? 'W' : 'B';
				else
					s = board.data[y][BOARD_SIZE - 1 - x]->getColor() == Color::WHITE ? 'w' : 'b';
			}

			out << s << " | ";
		}
		out << "\n";
		out << "   ";
		for (int i = 0; i < BOARD_SIZE; i++)
			out << "+---";
		out << "+" << std::endl;
	}

	out << "    ";
	for (int i = 0; i < BOARD_SIZE; i++)
		out << " " << char('A' + i) << "  ";
	out << std::endl;

	return out;
}