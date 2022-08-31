#ifndef Board_H
#define Board_H

#include "point.hpp"
#include <string>

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

    /**
     * @brief Create a Board String object
     *
     * @return const std::string
     */
    const std::string createBoardString();

    /**
     * @brief A string representing the board
     *
     */
    const std::string boardString{createBoardString()};

public:
    /**
     * @brief Get the Width object
     *
     * @return int
     */
    const int getWidth() const { return bottomRightCorner.x; }

    /**
     * @brief Get the Height object
     *
     * @return int
     */
    const int getHeight() const { return bottomRightCorner.y; }

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
    const bool isInBoard(const Point &pointToCheck) const;

    /**
     * @brief Get the index of the point based on the board size
     *
     * @note Does no validation
     * @param pointToGetIndexOf
     * @return const int
     */
    const int getIndex(const Point &pointToGetIndexOf) const;

    /**
     * @brief Get the empty board as a string
     *
     * @return const std::string&
     */
    const std::string &toString() const { return boardString; }
};

#endif