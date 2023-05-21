#include "snake.hpp"
#include "direction.hpp"
#include <stdexcept>

using namespace std;

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
        throw invalid_argument("point already exists");

    body.push_back(point);
    bodySegmentsSet.insert(point);
}

void Snake::pop()
{
    bodySegmentsSet.erase(body.front());
    body.pop_front();
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
        body.push_back(getHead().getAdjacentPoint(direction));
        isCrashed = true;
    }
}

bool Snake::isInSnake(const Point &pointToCheck) const
{
    return bodySegmentsSet.find(pointToCheck) != bodySegmentsSet.end();
}

const char Snake::getDirectionAsChar() const
{
    switch (direction)
    {
    case Directions::Direction::UP:
        return '^';
    case Directions::Direction::RIGHT:
        return '>';
    case Directions::Direction::DOWN:
        return 'v';
    case Directions::Direction::LEFT:
        return '<';
    }
}