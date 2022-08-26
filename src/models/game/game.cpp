#include "game.hpp"
#include "utility.hpp"
#include <sstream>
#include <regex>
#include <vector>
#include <stdexcept>

bool Game::isGameOver()
{
    if (!snake)
        throw std::invalid_argument("snake is null");
    return snake->getIsCrashed();
}

Point Game::getRandomVacantPoint()
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

std::string Game::toString()
{
    // Null check
    if (!snake || !board)
        throw std::invalid_argument("snake or board is null");

    // Evaluate once
    const int boardWidth{board->getWidth()};
    const int boardHeight{board->getHeight()};
    const int topBoundMessage{(boardHeight / 5)};
    int messageIndex{};
    int leftBoundNextMatch;

    // Regex to limit message segments to 2/3 width of board, segments do not split words
    const std::regex regex("[^\\n]{0," + std::to_string(static_cast<int>(boardWidth * 2.0 / 3.0)) + "}(?:\\n|\\ |$)");
    const std::sregex_iterator start = std::sregex_iterator(message.begin(), message.end(), regex);
    const std::sregex_iterator end = std::sregex_iterator();
    std::vector<std::string> matches;

    // Collect string matches
    for (std::sregex_iterator match = start; match != end; ++match)
        matches.push_back(match->str());

    // Evaluate what the left bound of the next match is (for centering each message in the board)
    if (matches.size() > 0)
        leftBoundNextMatch = (boardWidth - matches[0].length() - 1) / 2;

    // Declare char array to build string
    std::stringstream result;
    result << "Score: ";
    result << std::to_string(score);
    result << '\n';

    // Loop through entire char array once
    for (int row{-1}; row <= boardHeight + 1; row++)
    {
        for (int column{-1}; column <= boardWidth + 1; ++column)
        {
            // Get the current point under consideration
            Point currentPoint{column, row};

            // Decide what character should be in each point
            if (messageIndex < matches.size() && row > topBoundMessage + messageIndex && column > leftBoundNextMatch)
            {
                std::string match{matches[messageIndex++]};
                if (match[match.length() - 1] == '\n')
                    match.pop_back();
                result << match;
                column += match.length() - 1;
                if (messageIndex < matches.size())
                    leftBoundNextMatch = (boardWidth - matches[messageIndex].length() - 1) / 2;
            }
            else if (currentPoint == snake->getHead() && isGameOver())
            {
                result << 'X';
            }
            else if (row < 0) // Top or Bottom wall
            {
                if (column < 0)
                    result << '/';
                else if (column > boardWidth)
                    result << '\\';
                else
                    result << '-';
            }
            else if (row > boardHeight)
            {
                if (column < 0)
                    result << '\\';
                else if (column > boardWidth)
                    result << '/';
                else
                    result << '-';
            }
            else if (column < 0 || column > boardWidth) // Left or Right wall
            {
                result << '|';
            }
            else if (currentPoint == snake->getHead()) // Snake head
            {
                switch (snake->getDirection())
                {
                case Directions::Direction::UP:
                    result << '^';
                    break;
                case Directions::Direction::RIGHT:
                    result << '>';
                    break;
                case Directions::Direction::DOWN:
                    result << 'v';
                    break;
                case Directions::Direction::LEFT:
                    result << '<';
                    break;
                }
            }
            else if (currentPoint == apple) // Apple
            {
                result << '@';
            }
            else if (snake->isInSnake(currentPoint)) // Snake body
            {
                result << '*';
            }
            else // All other spaces
            {
                result << ' ';
            }
        }

        // After each row, add a new line char
        result << '\n';
    }

    // Convert char array to string
    return result.str();
}