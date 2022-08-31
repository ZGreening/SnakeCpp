#include "board.hpp"
#include <string>
#include <sstream>

const std::string Board::createBoardString()
{
    // Create stream
    std::stringstream toReturn;

    // Iterate through board
    for (int row{-1}; row <= bottomRightCorner.y + 1; row++)
    {
        for (int column{-1}; column <= bottomRightCorner.x + 1; ++column)
        {
            if (row < 0)
            {
                if (column < 0)
                    toReturn << '/';
                else if (column > bottomRightCorner.x)
                    toReturn << '\\';
                else
                    toReturn << '-';
            }
            else if (row > bottomRightCorner.y)
            {
                if (column < 0)
                    toReturn << '\\';
                else if (column > bottomRightCorner.x)
                    toReturn << '/';
                else
                    toReturn << '-';
            }
            else if (column < 0 || column > bottomRightCorner.x)
            {
                toReturn << '|';
            }
            else
                toReturn << ' ';
        }
        toReturn << '\n';
    }

    // Convert stream to string and return
    return toReturn.str();
}

const bool Board::isInBoard(const Point &pointToCheck) const
{
    return pointToCheck.x >= 0 && pointToCheck.x <= bottomRightCorner.x && pointToCheck.y >= 0 && pointToCheck.y <= bottomRightCorner.y;
}

const int Board::getIndex(const Point &pointToGetIndexOf) const
{
    return ((bottomRightCorner.x + 4) * (pointToGetIndexOf.y + 1)) + (pointToGetIndexOf.x + 1);
}