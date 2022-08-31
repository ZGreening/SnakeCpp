#include "game.hpp"
#include "utility.hpp"
#include <string>
#include <regex>
#include <vector>
#include <stdexcept>

void Game::setMessage(const std::string &message)
{
    // Clear current matches
    matches.clear();

    // Regex to limit message segments to 2/3 width of board, segments do not split words
    const std::sregex_iterator start = std::sregex_iterator(message.begin(), message.end(), messageSplitterRegex);
    const std::sregex_iterator end = std::sregex_iterator();

    // Collect string matches
    for (std::sregex_iterator match = start; match != end; ++match)
        matches.push_back(match->str());

    // Assign message
    this->message = message;
}

const bool Game::isGameOver() const
{
    if (!snake)
        throw std::invalid_argument("snake is null");
    return snake->getIsCrashed();
}

const Point Game::getRandomVacantPoint() const
{
    // Null check
    if (!board)
        throw std::invalid_argument("board is null");

    // Get a new location not in the snake
    Point newPoint;
    do
    {
        std::uniform_int_distribution getX{1, board->getWidth()};
        std::uniform_int_distribution getY{1, board->getHeight()};
        newPoint = Point{getX(Utility::getRandom()), getY(Utility::getRandom())};
    } while (snake->isInSnake(newPoint));

    // Return the new point
    return newPoint;
}

const std::string &Game::toString()
{
    // Null check
    if (!snake || !board)
        throw std::invalid_argument("snake or board is null");

    // Start with the score header
    gameAsString = "Score: " + std::to_string(score) + '\n';

    // Get length of string while it has just the score header
    short prefixLength{static_cast<short>(gameAsString.length())};

    // Add the empty game board
    gameAsString += board->toString();

    // Add the apple in its location
    gameAsString[board->getIndex(apple) + prefixLength] = '@';

    // Add snake segments
    for (auto segment : snake->getBody())
        gameAsString[board->getIndex(segment) + prefixLength] = '*';

    // Replace the snake head segment with appropriate direction or X char
    gameAsString[board->getIndex(snake->getHead()) + prefixLength] = isGameOver() ? 'X' : snake->getDirectionAsChar();

    // Overwrite matches into board if a message exists
    for (int index{0}; index < matches.size(); ++index)
    {
        // Get match specific information
        const int matchLength{static_cast<int>(matches[index].length())};
        const Point matchStart{(board->getWidth() - matchLength + 1) / 2, (board->getHeight() / 5) + index};
        const bool matchEndsWithNewline{matches[index][matches[index].length() - 1] == '\n'};

        // Add each character skipping the \n character
        for (int matchIndex{0}; matchIndex < (matchEndsWithNewline ? matchLength - 1 : matchLength); ++matchIndex)
            gameAsString[board->getIndex(matchStart) + matchIndex + prefixLength] = matches[index][matchIndex];
    }

    // Return generated string reference
    return gameAsString;
}