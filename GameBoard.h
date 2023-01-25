#pragma once
#include <vector>
#include <map>

#include "Element.h"

class GameBoard
{
public:
    GameBoard() : rows(20), cols(20) {}

    GameBoard(int rows, int columns);

    void holdElement();
    void undHoldElement();

    void spawn(Element& element);
    void despawn();

    void moveElement(Element& element, int col, int row);
    void rotateElement(Element::Rotation rotation);

    bool checkForCollisions(Element& element);
    bool checkForCollisions(std::map<int, std::vector<int>> shape, int x_, int y_);
    int dropElement(Element& element);

    void moveRowsDown();

    void clearRowLines(int row);
    void clearColumnLines(int cols);
    void clearBoard();

    std::vector<int> fullRows();

    bool isTopRowOccupied();
    bool isRowOccupied(int row);

    int getLowestPosition(Element element);

    void settleElement(Element& element);

    int getX()
    {
        return x;
    }

    int getY()
    {
        return y;
    }

    void setY(int value)
    {
        y = value;
    }

    std::map<int, std::vector<Element::Color>> getElements() const
    {
        return elements;
    }
    
private:
    bool spawned;
    int rows, cols;
    int x, y;

    std::map<int, std::vector<Element::Color>> elements;

    bool canBeMoved;

    bool isPositionPossible(int row, int col, const Element& element) const;

    std::vector<int> findLinesToClear();
};

