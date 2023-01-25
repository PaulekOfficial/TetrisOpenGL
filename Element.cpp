#include "Element.h"

#include <iostream>
#include <ostream>


Element::Element(Type ttype, Color ccolor) : type(ttype), color(ccolor), orginalShape({}), shape({}), rotation(-1)
{
    switch (ttype) {
    case I:
        orginalShape = {{0, {1}},
    	                   {1, {1}},
    	                   {2, {1}},
    	                   {3, {1}}};
        shape = orginalShape;
        break;
    case J:
        orginalShape = { {0, {0, 1}},
                            {1, {0, 1}},
                            {2, {1, 1}}};
        shape = orginalShape;
        break;
    case L:
        orginalShape = { {0, {1, 0}},
                             {1, {1, 0}},
                             {2, {1, 1}} };
        shape = orginalShape;
        break;
    case O:
        orginalShape = { {0, {1, 1}},
						     {1, {1, 1}}};
        shape = orginalShape;
        break;
    case S:
        orginalShape = { {0, {0, 1, 1}},
                         {1, {1, 1, 0}} };
        shape = orginalShape;
        break;
    case T:
        orginalShape = { {0, {1, 1, 1}},
                         {1, {0, 1, 0}} };
        shape = orginalShape;
        break;
    case Z:
        orginalShape = { {0, {1, 1, 0}},
                         {1, {0, 1, 1}} };
        shape = orginalShape;
        break;
    case U:
        orginalShape = { {0, {1, 0, 1}},
                         {1, {1, 1, 1}} };
        shape = orginalShape;
        break;
    case X:
        orginalShape = { {0, {1, 0, 1}},
                             {1, {0, 1, 0}}};
        shape = orginalShape;
        break;
    case D:
        orginalShape = { {0, {1, 1, 0}},
                             {1, {1, 1, 1}},
                             {2, {1, 1, 0}} };
        shape = orginalShape;
        break;
    case Q:
        orginalShape = { {0, {0, 1, 0}},
                             {1, {0, 1, 0}},
                             {2, {1, 1, 1}} };
        shape = orginalShape;
        break;
    case none:
    default:
        orginalShape = {};
        shape = {};
        std::cout << "NO SHAPE !!!" << std::endl;
    }
}

//https://stackoverflow.com/questions/61223837/vs2019-c2675-and-c2100-error-from-xtree-in-c-when-compiling-a-single-file-that
Element::Type Element::getType() const
{
    return type;
}

int Element::getRotation()
{
    return -1;
}

Element::Color Element::getColor() const
{
    return color;
}

// Rotate 90 degrees
void Element::rotate(Rotation rrotation)
{
    std::map<int, std::vector<int>> map;
    if (rrotation == Element::Rotation::Right)
    {
	    for (int i = 0; i < shape.size(); i++)
	    {
	        for (int ii = 0; ii < shape.at(i).size(); ii++)
	        {
	            if (map.find(ii) == map.end())
	            {
	                // allays pair
	                std::vector<int> vect(0);
	                map.insert(std::make_pair(ii, vect));
	            }

	            map.at(ii).push_back(shape.at(i).at(ii));
	        }
	    }

		// Mirror
        for (int i = 0; i < map.size(); i++)
        {
            std::vector<int> v;
            for (int ii = map.at(i).size() - 1; ii >= 0; ii--)
            {
                v.push_back(map.at(i).at(ii));
            }
 
            map.at(i) = v;
        }
	}

    if (rrotation == Element::Rotation::Left)
    {
        for (int i = shape.size() - 1; i < shape.size(); i--)
        {
            for (int ii = 0; ii < shape.at(i).size(); ii++)
            {
                if (map.find(ii) == map.end())
                {
                    // allays pair
                    std::vector<int> vect(0);
                    map.insert(std::make_pair(ii, vect));
                }

                map.at(ii).push_back(shape.at(i).at(ii));
            }
        }

        // // Mirror
        // for (int i = 0; i < map.size(); i++)
        // {
        //     std::vector<int> v;
        //     for (int ii = map.at(i).size() - 1; ii >= 0; ii--)
        //     {
        //         v.push_back(map.at(i).at(ii));
        //     }
        //
        //     map.at(i) = v;
        // }

        // Swap
        std::map<int, std::vector<int>> cmap;
        for (int i = 0; i < map.size(); i++)
        {
            std::vector<int> v;
            for (int ii = map.at(i).size() - 1; ii >= 0; ii--)
            {
                v.push_back(map.at(i).at(ii));
            }
        
            cmap[map.size() - 1 - i] = v;
        }

        map = cmap;
    }

    shape = map;
}
