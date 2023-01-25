#pragma once

#include <complex>

#include "GameBoard.h"

class TetrisGame
{
public:
	enum MoveElement { None = 0, Left, Right, Drop };

	TetrisGame();

	TetrisGame(int rows, int columns, bool enchanced_);

	void generateNext();
	void spawnNext();

	void pauseGame();
	void unPauseGame();

	void calculateScore();
	void calculateLevel();

	void useGravity();
	void rotate(Element::Rotation rotation);

	void tick();

	void moveElement(bool left);

	void setSpeedDrop(bool drop);

	bool getSpeedDrop() { return speedDrop; }

	void dropElement() { move = Drop; }

	double fixedMoveTime() const
	{
		return std::pow(0.8 - (level - 1) * 0.007, level - 1);
	}

	Element getNextElement() { return nextElement; }
	bool seetling() { return pauseForSettle; }

	Element getCurrenElement() { return currentElement; }

	GameBoard getGameBoard() { return gameBoard; }

	int getLevel() { return level; }
	int getScore() { return score; }

	bool isOver() { return gameOver; }

	int getCleanedLines() { return linesCleared; }

	std::vector<int> getRowsToClear() { return rowsToClear; }
	double getClearAnimationTime() { return clearAnimation; }

private:
	double timeLastGameUpdate = 0;
	double gravityTimer = 0;
	double clearAnimation = 0;
	double settleTimer = 0;

	GameBoard gameBoard;

	Element currentElement;
	Element::Rotation newRotation;

	Element nextElement;
	MoveElement move;

	std::vector<int> rowsToClear;

	int level;
	int linesCleared;
	int score;

	bool enchanced;

	bool gameOver;

	bool speedDrop;

	bool pauseForSettle;
	bool paused;

	void addLineCleaned();
	void updateLevel();
};

