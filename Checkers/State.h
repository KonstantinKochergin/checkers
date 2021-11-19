#pragma once
#include "Color.h"

// возможные состояния игры
enum class GameState : char {
	BLACK_WON,
	WHITE_WON,
	STILL_PLAYING,
};

struct State {
	Color turnColor;	// какая сторона ходит в данный момент
	GameState state;
};