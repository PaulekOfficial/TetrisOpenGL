#include "GameBoard.h"

#include <iostream>

GameBoard::GameBoard(int rows, int columns) : rows(rows), cols(columns), canBeMoved(true)
{
	//Init board - ocupy places
	clearBoard();
}

void GameBoard::holdElement()
{
	canBeMoved = false;
}

void GameBoard::undHoldElement()
{
	canBeMoved = true;
}

void GameBoard::spawn(Element& element)
{
	if (spawned) return;

	spawned = true;
	x = (cols - element.getShape().at(0).size()) / 2;
}

void GameBoard::despawn()
{
	x = 5;
	y = 0;
}


void GameBoard::moveElement(Element& element, int col, int row)
{
	if (!canBeMoved)
	{
		std::cout << "Cannot be moved!" << std::endl;
		return;
	}

	x += col;
	y += row;

	// Prevent from going outside board
	if (x + element.getShape().at(0).size() > (cols))
	{
		x = cols - element.getShape().at(0).size();
	}

	if (x < 0)
	{
		x = 0;
	}

	if (y + element.getShape().size() > (rows))
	{
		y = rows - element.getShape().size();
	}

	if (y < 0)
	{
		y = 0;
	}
}

void GameBoard::rotateElement(Element::Rotation rotation)
{
	//TODO don't works idk why
	//actualElement->rotate(rotation);
}

bool GameBoard::checkForCollisions(Element& element)
{
	auto shape = element.getShape();

	return checkForCollisions(shape, x, y);
}

bool GameBoard::checkForCollisions(std::map<int, std::vector<int>> shape, int x_, int y_)
{
	for (int row = 0; row < shape.size(); row++)
	{
		for (int col = 0; col < shape.at(row).size(); col++)
		{
			// Add 1 to col in order to check for collistion under block
			if ((shape.size() + y_) >= rows)
			{
				return true;
			}

			if (shape.at(row).at(col) == 0)
			{
				continue;
			}

			// Fix collisions crash
			if (col + x_ >= cols)
			{
				x = cols - 1 - shape.size();
				continue;
			}
			// Add +1 to row to check block below shape
			if (elements.at(row + y_ + 1).at(col + x_) != Element::Color::empty)
			{
				return true;
			}
		}
	}

	return false;
}

std::vector<int> GameBoard::fullRows()
{
	auto vect = std::vector<int>();
	for (int row = 0; row < rows; row++)
	{
		int colCount = 0;
		for (int col = 0; col < cols; col++)
		{
			if (elements.at(row).at(col) == Element::Color::empty)
			{
				continue;
			}
			colCount++;
		}

		if (colCount < cols)
		{
			continue;
		}

		vect.push_back(row);
	}

	return vect;
}

int GameBoard::dropElement(Element& element)
{
	int colsOccupied = element.getShape().size();
	for (int col = y; col <= colsOccupied + y; col++)
	{
		if (elements.at(x).at(col) == Element::Color::empty)
		{
			continue;
		}

		settleElement(element);
		break;
	}

	return 0;
}

void GameBoard::clearRowLines(int row)
{
	std::vector<Element::Color> col(cols, Element::Color::empty);
	elements[row] = col;
}

void GameBoard::clearColumnLines(int cols)
{
	for (int i = 0; i <= rows; i++)
	{
		elements.at(i).at(cols) = Element::Color::empty;
	}
}

void GameBoard::clearBoard()
{
	//Init board - ocupy places
	std::vector<Element::Color> col(cols, Element::Color::empty);

	for (int i = 0; i < rows; i++)
	{
		elements.insert(std::make_pair(i, col));
	}
}

void GameBoard::moveRowsDown()
{
	auto map = std::map<int, std::vector<Element::Color>>();
	//Init board - ocupy places
	std::vector<Element::Color> col(cols, Element::Color::empty);

	for (int i = 0; i < rows; i++)
	{
		map.insert(std::make_pair(i, col));
	}

	int lastRow = 19;
	for (int row = rows - 1; row >= 0; row--)
	{
		if (!isRowOccupied(row))
		{
			continue;
		}

		map[lastRow] = elements.at(row);
		lastRow--;
	}

	elements = map;
}

bool GameBoard::isTopRowOccupied()
{
	return isRowOccupied(0);
}

bool GameBoard::isRowOccupied(int row)
{
	for (int col = 0; col < cols; col++)
	{
		if (elements.at(row).at(col) == Element::empty)
		{
			continue;
		}

		return true;
	}

	return false;
}

void GameBoard::settleElement(Element& element)
{
	for (int row = 0; row < element.getShape().size(); row++)
	{
		for (int col = 0; col < element.getShape().at(row).size(); col++)
		{
			if (element.getShape().at(row).at(col) == 0)
			{
				std::cout << "NULL" << std::endl;
				continue;
			}

			elements[row + y][col + x] = element.getColor();
		}
	}
}

bool GameBoard::isPositionPossible(int row, int col, const Element& element) const
{
	if (elements.at(row).at(col) != Element::Color::empty) return false;
	return true;
}

std::vector<int> GameBoard::findLinesToClear()
{
	std::vector<int> v;

	for (int i = 0; i <= rows; i++)
	{
		int sum = 0;
		for (int ii = 0; ii <= cols; ii++)
		{
			if (elements.at(i).at(ii) != Element::Color::empty)
			{
				sum++;
			}
		}

		if (sum >= rows) v.push_back(i);
	}

	return v;
}

int GameBoard::getLowestPosition(Element element)
{
	int i = 0;
	while (i < rows)
	{
		if (checkForCollisions(element.getShape(), x, y + i)) break;

		i++;
	}

	return y + i;
}
