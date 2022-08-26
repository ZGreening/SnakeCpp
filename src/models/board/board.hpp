#ifndef Board_H
#define Board_H

#include "point.hpp"

/**
 * @brief The class representing the board the snake moves through
 *
 */
class Board
{
private:
    /**
     * @brief The bottom right corner of the board
     *
     * @note The board starts at Point(1,1) and includes the bottomRightCorner
     */
    const Point bottomRightCorner;

public:
    /**
     * @brief Get the Width object
     *
     * @return int
     */
    int getWidth() const { return bottomRightCorner.x; }

    /**
     * @brief Get the Height object
     *
     * @return int
     */
    int getHeight() const { return bottomRightCorner.y; }

    /**
     * @brief Construct a new Board object
     *
     * @param width The width of the board
     * @param height The height of the board
     */
    Board(int width, int height) : bottomRightCorner{width, height} {}

    /**
     * @brief Checks to see if the point is within the board boundaries
     *
     * @param pointToCheck The point to check
     * @return true if the point is in the board
     * @return false if the point is not in the board
     */
    bool isInBoard(const Point &pointToCheck);
};

#endif