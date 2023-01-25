#include "TetrisGame.h"

#include <iostream>
#include <GLFW/glfw3.h>

TetrisGame::TetrisGame()
{
	level = 1;
	linesCleared = 0;
	score = 0;
	paused = false;
	gameOver = false;
	pauseForSettle = false;
	speedDrop = false;
	newRotation = Element::Rotation::None;

	generateNext();

	GameBoard board(20, 10);
	gameBoard = board;

	timeLastGameUpdate = glfwGetTime();

	spawnNext();
}

TetrisGame::TetrisGame(int rows, int columns, bool enchanced_)
{
	level = 1;
	linesCleared = 0;
	score = 0;
	paused = false;
	gameOver = false;
	pauseForSettle = false;
	speedDrop = false;
	enchanced = enchanced_;
	newRotation = Element::Rotation::None;

	generateNext();

	GameBoard board(rows, columns);
	gameBoard = board;

	timeLastGameUpdate = glfwGetTime();

	spawnNext();
}


void TetrisGame::generateNext()
{
	auto type = rand() % 6;
	if (enchanced)
	{
		type = rand() % 10;
	}

	auto color = rand() % 6;

	Element ele = Element(static_cast<Element::Type>(type), static_cast<Element::Color>(color));

	nextElement = ele;
}

void TetrisGame::spawnNext()
{
	currentElement = nextElement;
	generateNext();

	gameBoard.spawn(currentElement);
}

void TetrisGame::pauseGame()
{
	gameBoard.holdElement();
	paused = true;
}

void TetrisGame::unPauseGame()
{
	gameBoard.undHoldElement();
	paused = false;
}

void TetrisGame::calculateScore()
{
	score = linesCleared + level * linesCleared * 100;
}

void TetrisGame::calculateLevel()
{
	level = linesCleared / 10;
}

void TetrisGame::useGravity()
{
	gameBoard.moveElement(currentElement, 0, 1);
}

void TetrisGame::tick()
{
	auto now = glfwGetTime();
	if ((now - timeLastGameUpdate) <= 0.005)
	{
		return;
	}
	timeLastGameUpdate = now;

	// Check for game over
	if (gameBoard.isTopRowOccupied())
	{
		gameOver = true;
		return;
	}

	// Check for full rows & clear it etc
	auto rows = gameBoard.fullRows();
	if (!rows.empty() && clearAnimation == 0.0)
	{
		rowsToClear = gameBoard.fullRows();
		clearAnimation += 0.005;
	}

	if (!rowsToClear.empty())
	{
		clearAnimation += 0.005;
		if (clearAnimation >= 1.5 / level)
		{
			for (int row : rowsToClear)
			{
				addLineCleaned();
				gameBoard.clearRowLines(row);
			}

			switch (rowsToClear.size())
			{
			case 1:
				score += 100 * level;
				break;
			case 2:
				score += 300 * level;
				break;
			case 3:
				score += 500 * level;
				break;
			case 4: // TETRIS!
				score += 800 * level;
				break;
			default:
				score += 10;
			}

			clearAnimation = 0.0;
			rowsToClear = std::vector<int>(0);

			gameBoard.moveRowsDown();
		}
	}

	// Seatle elements
	if (pauseForSettle)
	{
		settleTimer += 0.005;

		if (settleTimer >= 0.5 / level)
		{
			settleTimer = 0;

			auto y = gameBoard.getLowestPosition(currentElement);
			if (y != getGameBoard().getY())
			{
				gameBoard.setY(y);
			}
			
			gameBoard.settleElement(currentElement);
			gameBoard.despawn();

			spawnNext();
			pauseForSettle = false;
		}

		return;
	}

	// Move element
	if (move != None)
	{
		if (move == Drop)
		{
			auto y = gameBoard.getLowestPosition(currentElement);
			score += (y - gameBoard.getY()) * 2;
			gameBoard.setY(y);
		}
		else
		{
			gameBoard.moveElement(currentElement, move == Left ? -1 : 1, 0);
		}

		move = None;
	}

	// Check for collisions
	if (gameBoard.checkForCollisions(currentElement))
	{
		pauseForSettle = true;
	}

	// Gravity
	gravityTimer += 0.005;
	auto timeMove = fixedMoveTime();
	if (speedDrop)
	{
		timeMove /= 8;
	}
	if (gravityTimer >= timeMove)
	{
		gravityTimer = 0;
		useGravity();

		if (speedDrop) score++;
	}

	// Update level count
	updateLevel();
}

void TetrisGame::setSpeedDrop(bool drop)
{
	speedDrop = drop;
}


void TetrisGame::rotate(Element::Rotation rotation)
{
	if (paused || pauseForSettle) return;
	gameBoard.moveElement(currentElement, 0, 0);
	currentElement.rotate(rotation);
}

void TetrisGame::moveElement(bool left)
{
	if (paused || pauseForSettle) return;
	move = (left) ? Left : Right;
}

void TetrisGame::addLineCleaned()
{
	linesCleared++;

	score += 100 * level;
}

void TetrisGame::updateLevel()
{
	auto lvl = linesCleared / 10;
	if (lvl <= 0 || lvl == level)
	{
		return;
	}

	level = lvl;
}
