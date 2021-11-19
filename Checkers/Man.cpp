#include "Man.h"

Man::Man(const Position& position, Color color)
	: Figure(position, color) {}

Moves Man::possibleMoves(const Board& board, bool onlyAttack) const {
	Moves chainMoves, oneStepMoves;

	// проверяем соседей:
	// (-1, -1) . (-1, 1)
	//	   .	o	 .
	// ( 1,  1) . ( 1, 1)
	for (int x = -1; x < 2; x += 2) {
		int nx = position.getX() + x;
		if (!board.onBoard(nx, 0)) continue;

		for (int y = -1; y < 2; y += 2) {
			int	ny = position.getY() + y;

			if (!board.onBoard(nx, ny))
				continue;

			// если текущая клетка не пустая проверяем, противник ли там и можем ли мы его съесть
			if (!board.isEmpty(nx, ny)) {

				// если на текущий клетки фигура противника
				if (board.data[nx][ny]->getColor() != color) {
					nx += x;
					ny += y;

					// если следующая клетка пуста (можно произвести взятие)
					if (board.onBoard(nx, ny) &&
						board.isEmpty(nx, ny)) {
						Position current(nx, ny),
							opponentPosition(nx - x, ny - y);

						Board newBoard = boardCopy(board);
						moveFigure(&newBoard, position, current);

						chainMoves.emplace_back(std::vector<Position>(1, current));

						if (onQueenPositionIf(newBoard, current))
							queenMove(newBoard, current, chainMoves);
						else
							eatMove(newBoard, current, opponentPosition, &chainMoves, chainMoves.size() - 1);
					}

					nx -= x;
					ny -= y;
				}
			}
			// иначе делаем обычный ход
			else if (chainMoves.empty() && !onlyAttack) {
				if ((color == Color::WHITE && y < 0) ||
					(color == Color::BLACK && y > 0))
					continue;

				oneStepMoves.emplace_back(std::vector<Position>(1, Position(nx, ny)));
			}
		}
	}

	// если есть варианты рубить, возвращем ходы со взятием, тк рубить обязаны
	if (!chainMoves.empty())
		return std::move(chainMoves);

	return std::move(oneStepMoves);
}

// взятие фигуры, мутирем параметр board, сохраняя в него результат
void Man::eatMove(
	Board& board,
	const Position& current,
	const Position& opponent,
	Moves* moves,
	size_t idx 
) const {
	size_t sizeBefore = -1;
	bool finishBranch = false;

	// проверяем соседние клетки, по сути повторение кода выше, по хорошему надо вынести отдельно
	// (-1, -1) . (-1, 1)
	//	   .	o	 .
	// ( 1,  1) . ( 1, 1)
	for (int x = -1; x < 2; x += 2) {
		int nx = current.getX() + x;
		if (!board.onBoard(nx, 0)) continue;

		for (int y = -1; y < 2; y += 2) {
			int	ny = current.getY() + y;

			if (!board.onBoard(nx, ny) ||
				Position(nx, ny) == opponent)
				continue;

			if (!board.isEmpty(nx, ny) &&
				board.data[nx][ny]->getColor() != color) {

				nx += x;
				ny += y;

				if (board.onBoard(nx, ny) && board.isEmpty(nx, ny)) {
					Position to(nx, ny), opponentPosition(nx - x, ny - y);
					if (finishBranch) {
						finishBranch = false;

						moves->emplace_back(std::vector<Position>(sizeBefore, current));
						std::copy((*moves)[idx].begin(), (*moves)[idx].begin() + sizeBefore, moves->back().begin());
						idx += 1;
					}
					sizeBefore = (*moves)[idx].size();
					(*moves)[idx].emplace_back(to);

					Board newBoard = boardCopy(board);
					moveFigure(&newBoard, current, to);

					if (onQueenPositionIf(newBoard, to))
						queenMove(newBoard, to, *moves);
					else
						eatMove(newBoard, to, opponentPosition, moves, idx);

					finishBranch = true;
				}
			}
		}
	}
}

void Man::queenMove(Board& board, const Position& current, Moves& chainMoves) const {

	auto moves = board[current]->possibleMoves(board, true);
	if (moves.empty())
		chainMoves.emplace_back(std::vector<Position>(1, current));
	else {
		std::vector<Position> baseChain(chainMoves.back().begin(), chainMoves.back().end());
		for (size_t moveIdx = 0; moveIdx < moves.size(); moveIdx++) {
			chainMoves.back().insert(chainMoves.back().end(), moves[moveIdx].begin(), moves[moveIdx].end());

			if (moveIdx != moves.size() - 1)
				chainMoves.emplace_back(baseChain);
		}
	}
}

bool Man::onQueenPositionIf(const Board& board, const Position& pos) const {
	if (board.isEmpty(pos))
		return (color == Color::WHITE && pos.getY() == BOARD_SIZE - 1) ||
		(color == Color::BLACK && pos.getY() == 0);

	return false;
}

void moveFigure(Board* _board, const Position& from, const Position& to) {
	// variable shadowing (just not to write (*_board))
	Board& board = *_board;

	if (!board.onBoard(from) || !board.onBoard(to))
		throw BoardException();

	bool removeBlackColor = board[from]->isWhite();
	std::vector<std::shared_ptr<Figure>>* checkers = nullptr;

	board[to] = board[from];
	board[from] = nullptr;
	board[to]->setPosition(to);

	// checks if a figure becomes a queen
	bool isQueen = (board[to]->isWhite() && to.getY() == BOARD_SIZE - 1) ||
		(board[to]->isBlack() && to.getY() == 0);

	if (isQueen && !board[to]->isQueen()) {
		checkers = board[to]->isWhite() ? &board.white : &board.black;

		for (size_t figureIdx = 0; figureIdx < checkers->size(); figureIdx++)
			if ((*checkers)[figureIdx]->getPosition() == to) {
				Figure* queen = dynamic_cast<Figure*>(new Queen(to, board[to]->getColor()));

				board[to] = queen;
				(*checkers)[figureIdx].reset(queen);
				board.increaseQueen(board[to]->getColor());

				break;
			}
	}

	auto [hasOpponent, opponent] = board.between(to, from, removeBlackColor ? Color::BLACK : Color::WHITE);

	// if it is a kill move
	if (hasOpponent)
		killOpponent(_board, opponent, removeBlackColor);

	board.history.emplace_back(to);
}

void killOpponent(Board* _board, const Position& opponentPosition, bool removeBlack) {
	Board& board = *_board;

	if (!board.onBoard(opponentPosition)) return;

	auto checkers = removeBlack ? &board.black : &board.white;
	size_t i = 0;
	for (i; i < checkers->size(); i++) {
		if (&*(*checkers)[i] == board[opponentPosition]) {
			// kills an opponent
			board[opponentPosition] = nullptr;
			(*checkers)[i].swap((*checkers).back());
			checkers->pop_back();
		}
	}

	if (checkers->empty())
		board.changeGameState(removeBlack ? GameState::WHITE_WON : GameState::BLACK_WON);
}

bool Man::isQueen() const { return false; }

bool Man::onQueenPosition() const {
	return isWhite() && position.getY() == BOARD_SIZE - 1 ||
		isBlack() && position.getY() == 0;
}

// deep copy of a given board
Board boardCopy(const Board& board) {
	Board newBoard(board.state, board.getBlackQueenN(), board.getWhiteQueenN());
	auto checkers = &board.white;
	auto newCheckers = &newBoard.white;

	for (int i = 0; i < 2; i++) {

		for (const auto& figure : (*checkers)) {
			Figure* ptr = nullptr;
			if (figure->isQueen())
				ptr = dynamic_cast<Figure*>(new Queen(figure->getPosition(), figure->getColor()));
			else
				ptr = dynamic_cast<Figure*>(new Man(figure->getPosition(), figure->getColor()));

			newCheckers->emplace_back(std::shared_ptr<Figure>(ptr));
			newBoard[figure->getPosition()] = ptr;
		}

		checkers = &board.black;
		newCheckers = &newBoard.black;
	}

	return std::move(newBoard);
}