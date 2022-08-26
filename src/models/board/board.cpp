#include "board.hpp"

bool Board::isInBoard(const Point &pointToCheck)
{
    return pointToCheck.x >= 0 && pointToCheck.x <= bottomRightCorner.x && pointToCheck.y >= 0 && pointToCheck.y <= bottomRightCorner.y;
}
