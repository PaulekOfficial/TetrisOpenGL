#pragma once
#include <utility>
#include <vector>
#include <map>

class Element
{
public:
    enum Color
    {
	    empty = -1,
    	cyan = 0,
    	blue = 1,
    	orange = 2,
    	yellow = 3,
    	green = 4,
    	purple = 5,
    	red = 6
    };
    enum Type
    {
	    none = -1,
    	I,
    	J,
    	L,
    	O,
    	S,
    	T,
    	Z,
        U,
        X,
        D,
        Q,
    };
    enum class Rotation
    {
	    None = -1, Right, Left
    };

    Element() : type(none), color(empty) {}

    Element(Type ttype, Color ccolor);

    Type getType() const;
    int getRotation();
    Color getColor() const;

    std::map<int, std::vector<int>> getShape() const
    {
        return shape;
    }

    void rotate(Rotation rrotation);

private:
    // Type of the element, used to alter shape after rotation
    Type type;

    // Color fot the element :wink:
    Color color;
    int rotation;
    std::map<int, std::vector<int>> orginalShape;
    std::map<int, std::vector<int>> shape;
};

