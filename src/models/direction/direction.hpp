#ifndef DIRECTION_H
#define DIRECTION_H

class Directions
{
public:
    enum class Direction
    {
        UP,
        RIGHT,
        DOWN,
        LEFT
    };

    const static bool areOppositeDirections(Direction first, Direction second);
};

#endif