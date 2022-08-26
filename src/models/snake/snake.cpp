#include "snake.hpp"
#include "direction.hpp"
#include <stdexcept>

Snake::Snake(const Point &head, const int startingSize)
{
    for (int index{startingSize}; index > 0; --index)
    {
        Point point(head.x - index, head.y);
        push(point);
    }
}

void Snake::push(const Point &point, bool isGrowing)
{
    if (isInSnake(point))
        throw std::invalid_argument("point already exists");

    body.push(point);
    bodySegmentsSet.insert(point);
}

void Snake::pop()
{
    bodySegmentsSet.erase(body.front());
    body.pop();
}

void Snake::move(Directions::Direction direction)
{
    if (!Directions::areOppositeDirections(this->direction, direction) && !isCrashed)
    {
        pop();
        push(getHead().getAdjacentPoint(direction));
        this->direction = direction;
    }
}

void Snake::grow(Directions::Direction direction)
{
    if (!Directions::areOppositeDirections(this->direction, direction) && !isCrashed)
    {
        push(getHead().getAdjacentPoint(direction), true);
        this->direction = direction;
    }
}

void Snake::crash(Directions::Direction direction)
{
    if (!Directions::areOppositeDirections(this->direction, direction) && !isCrashed)
    {
        pop();
        body.push(getHead().getAdjacentPoint(direction));
        isCrashed = true;
    }
}

bool Snake::isInSnake(const Point &pointToCheck) const
{
    return bodySegmentsSet.find(pointToCheck) != bodySegmentsSet.end();
}