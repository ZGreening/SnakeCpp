#include "direction.hpp"

const bool Directions::areOppositeDirections(Direction first, Direction second)
{
    switch (first)
    {
    case Direction::UP:
        return second == Direction::DOWN;
    case Direction::RIGHT:
        return second == Direction::LEFT;
    case Direction::DOWN:
        return second == Direction::UP;
    case Direction::LEFT:
        return second == Direction::RIGHT;
    }
}