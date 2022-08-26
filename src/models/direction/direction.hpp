#ifndef DIRECTION_H
#define DIRECTION_H

class Directions
{
public:
    enum Direction
    {
        UP,
        RIGHT,
        DOWN,
        LEFT
    };

    static bool areOppositeDirections(Direction first, Direction second);
};

#endif