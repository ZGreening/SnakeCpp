#ifndef Point_H
#define Point_H

#include "direction.hpp"
#include <compare>
#include <functional>

/**
 * @brief The struct representing a 2D point
 *
 */
struct Point
{
    /**
     * @brief The x dimension of the point
     *
     */
    const int x{-1};

    /**
     * @brief The y dimension of the point
     *
     */
    const int y{-1};

    /**
     * @brief Construct a new Point object
     *
     * @param x The x dimension of the point
     * @param y The y dimension of the point
     */
    Point(const int x, const int y) : x{x}, y{y} {}

    /**
     * @brief Construct a new Point object
     *
     */
    Point() = default;

    /**
     * @brief Get the Point in the direction specified
     *
     * @param direction The direction to get the point of
     * @return Point The point adjacent in the direction specified
     */
    Point getAdjacentPoint(Directions::Direction direction) const
    {
        switch (direction)
        {
        case Directions::Direction::UP:
            return Point{x, y - 1};
        case Directions::Direction::RIGHT:
            return Point{x + 1, y};
        case Directions::Direction::DOWN:
            return Point{x, y + 1};
        case Directions::Direction::LEFT:
            return Point{x - 1, y};
        }
    }

    /**
     * @brief Define default comparison operators
     *
     */
    std::strong_ordering operator<=>(const Point &otherPoint) const = default;

    /**
     * @brief Overide the copy assignment operator to assign const members
     *
     * @param otherPoint
     * @return Point&
     */
    Point &operator=(const Point &otherPoint)
    {
        int *tmpX = const_cast<int *>(&x);
        *tmpX = otherPoint.x;
        int *tmpY = const_cast<int *>(&y);
        *tmpY = otherPoint.y;
        return *this;
    }

    /**
     * @brief The hasher for generating point hashes
     *
     */
    struct hasher
    {
        std::size_t operator()(const Point &point) const
        {
            const std::hash<int> hasher{};
            return hasher(point.x) ^ hasher(point.y);
        }
    };
};

#endif