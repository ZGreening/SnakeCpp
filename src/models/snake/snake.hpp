#ifndef Snake_H
#define Snake_H

#include "point.hpp"
#include "direction.hpp"
#include <deque>
#include <unordered_set>
#include <stdexcept>

/**
 * @brief The class representing the snake
 *
 */
class Snake
{
private:
    /**
     * @brief The body represented as a queue
     *
     */
    std::deque<Point> body{};

    /**
     * @brief The body points as a hash set for quick lookup
     *
     */
    std::unordered_set<Point, Point::hasher> bodySegmentsSet{};

    /**
     * @brief The character representing the direction the snake is facing
     *
     */
    Directions::Direction direction{Directions::Direction::RIGHT};

    /**
     * @brief Indicates if the snake has crashed
     *
     */
    bool isCrashed{false};

    /**
     * @brief Pushes a new point onto the body and bodySegmentsSet
     *
     * @param point
     * @param isGrowing
     */
    void push(const Point &point, bool isGrowing = false);

    /**
     * @brief Pops a point off the body and bodySegmentsSet
     *
     * @param point
     */
    void pop();

public:
    /**
     * @brief Get the Head object
     *
     * @return Point& The head of the snake
     */
    const Point &getHead() const
    {
        if (body.size() == 0)
            throw std::invalid_argument("body is empty");
        return body.back();
    }

    /**
     * @brief Get the Tail object
     *
     * @return Point&
     */
    const Point &getTail() const
    {
        if (body.size() == 0)
            throw std::invalid_argument("body is empty");
        return body.front();
    }

    /**
     * @brief Get the body of the snake
     *
     * @return const std::queue<Point>&
     */
    const std::deque<Point> &getBody() const { return body; }

    /**
     * @brief Get the Direction object
     *
     * @return Directions::Direction
     */
    const Directions::Direction getDirection() const { return direction; }

    /**
     * @brief Get the Is Crashed object
     *
     * @return true
     * @return false
     */
    const bool getIsCrashed() const { return isCrashed; }

    /**
     * @brief Construct a new Snake object
     *
     * @param head The head of the snake
     * @param startingSize The initial length of the snake
     */
    Snake(const Point &head, const int startingSize);

    /**
     * @brief Moves the snake head to the new target, moving the tail in the process
     *
     * @param direction The direction to move to
     */
    void move(Directions::Direction direction);

    /**
     * @brief Grows the snake by stretching the head to the new target, leaving the tail in the same place
     *
     * @param direction The direction to move to
     */
    void grow(Directions::Direction direction);

    /**
     * @brief Moves the snake toward the crash point and crashes it.
     *
     * @note The snake will no longer be able to move once crashed.
     * @param direction The direction to crash
     */
    void crash(Directions::Direction direction);

    /**
     * @brief Checks if the point is in the snake body
     *
     * @param pointToCheck The point to check
     * @return true if the point is in the snake
     * @return false if the point is not in the snake
     */
    bool isInSnake(const Point &pointToCheck) const;

    /**
     * @brief Get the char for the snake's head in the direction it is facing
     *
     * @return const char
     */
    const char getDirectionAsChar() const;
};

#endif