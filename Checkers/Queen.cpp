#include "Queen.h"

Queen::Queen(const Position& positon, Color color)
	: Figure(positon, color) {}

Moves Queen::possibleMoves(const Board& board, bool onlyAttack) const {
	Moves moves;

	moves = getPossibleAttacks(const_cast<Board&>(board), position);

	if (moves.empty() && !onlyAttack) {

		for (int signX = -1; signX < 2; signX += 2) {
			for (int signY = -1; signY < 2; signY += 2) {
				for (int step = 1; step <= BOARD_SIZE; step++) {
					int nx = position.getX() + step * signX,
						ny = position.getY() + step * signY;

					if (!board.onBoard(nx, ny) || !board.isEmpty(nx, ny))
						break;

					moves.emplace_back(std::vector<Position>(1, Position(nx, ny)));
				}
			}
		}
	}

	return std::move(moves);
}

Moves Queen::getPossibleAttacks(Board& board, const Position& pos) const {
	Moves attacks;

	// проверка соседних клеток
	// (-n, -n) . (-n, n)
	//	   .	o	 .
	// ( n,  n) . ( n, n)
	for (int x = -1; x < 2; x += 2) {
		for (int y = -1; y < 2; y += 2) {
			int	ny = pos.getY() + y;
			int nx = pos.getX() + x;

			while (board.onBoard(nx, ny)) {

				if (!board.isEmpty(nx, ny)) {
					if (board.data[nx][ny]->getColor() == color) break;
					else {
						Position opponentPosition(nx, ny);
						Figure* const tempOpponentStorage = board[opponentPosition];

						do {
							nx += x;
							ny += y;

							if (!board.onBoard(nx, ny)) break;

							if (board.isEmpty(nx, ny)) {
								Position attackPosition(nx, ny);
								board[opponentPosition] = nullptr;

								auto possibleMovesFrom = getPossibleAttacks(board, attackPosition);

								if (possibleMovesFrom.empty()) {
									attacks.emplace_back(std::vector<Position>(1, attackPosition));
								}
								else {
									auto startIdx = attacks.size();

									for (size_t i = startIdx; i < possibleMovesFrom.size(); i++) {
										attacks.emplace_back(std::vector<Position>(1, attackPosition));

										auto possibleMovesIdx = startIdx > 0 ? i % startIdx : i;

										attacks[i].insert(attacks[i].end(),
											possibleMovesFrom[possibleMovesIdx].begin(),
											possibleMovesFrom[possibleMovesIdx].end());
									}
								}
							}
							else break;
						} while (true);

						board[opponentPosition] = tempOpponentStorage;
					}
				}

				nx += x;
				ny += y;
			}
		}
	}

	return attacks;
}

bool Queen::canAttackFrom(const Board& board, const Position& from) const {
	// проверка соседних клеток
	// (-n, -n) . (-n, n)
	//	   .	o	 .
	// ( n,  n) . ( n, n)
	for (int x = -1; x < 2; x += 2) {
		for (int y = -1; y < 2; y += 2) {
			int	ny = from.getY() + y;
			int nx = from.getX() + x;

			while (board.onBoard(nx, ny)) {
				if (!board.isEmpty(nx, ny)) {
					if (board.data[nx][ny]->getColor() == color ||
						board.data[nx][ny]->isQueen()) break;

					nx += x;
					ny += y;

					if (!board.onBoard(nx, ny)) break;

					if (board.isEmpty(nx, ny)) return true;
				}

				nx += x;
				ny += y;
			}
		}
	}
	return false;
}

bool Queen::isQueen() const { return true; }