#include "utility.hpp"
#include <random>
#include <mutex>

using namespace std;
using enum sf::Keyboard::Key;

mutex Utility::ioMutex{};
mt19937 Utility::random(static_cast<unsigned int>(chrono::steady_clock::now().time_since_epoch().count()));

char Utility::sfKeyToChar(sf::Keyboard::Key key)
{
    switch (key)
    {
    case Num0:
        return '0';
    case Num1:
        return '1';
    case Num2:
        return '2';
    case Num3:
        return '3';
    case Num4:
        return '4';
    case Num5:
        return '5';
    case Num6:
        return '6';
    case Num7:
        return '7';
    case Num8:
        return '8';
    case Num9:
        return '9';
    case A:
        return 'a';
    case B:
        return 'b';
    case C:
        return 'c';
    case D:
        return 'd';
    case E:
        return 'e';
    case F:
        return 'f';
    case G:
        return 'g';
    case H:
        return 'h';
    case I:
        return 'i';
    case J:
        return 'j';
    case K:
        return 'k';
    case L:
        return 'l';
    case M:
        return 'm';
    case N:
        return 'n';
    case O:
        return 'o';
    case P:
        return 'p';
    case Q:
        return 'q';
    case R:
        return 'r';
    case S:
        return 's';
    case T:
        return 't';
    case U:
        return 'u';
    case V:
        return 'v';
    case W:
        return 'w';
    case X:
        return 'x';
    case Y:
        return 'y';
    case Z:
        return 'z';
    default:
        throw invalid_argument(to_string(key) + " is not a valid char");
    }
}