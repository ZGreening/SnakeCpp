#include "menu_service.hpp"
#include "point.hpp"
#include "utility.hpp"
#include "game_service.hpp"
#include "SFML/Window.hpp"
#include <future>
#include <string>
#include <stdexcept>
#include <climits>

int MenuService::promptForInteger(std::string string, int min, int max) const
{
    // Null check
    if (!game)
        throw std::invalid_argument("game is null");

    // Declare vars
    int toReturn{INT_MIN};
    std::string addedString{};
    std::string debug{};
    static bool justPressed{false};
    game->setMessage(string);

    // Print the message for getting the int
    while (toReturn < min || toReturn > max)
    {
        // If the addedString has changed, reprint it
        if (justPressed)
        {
            const std::lock_guard<std::mutex> lock(Utility::getIoMutex());
            game->setMessage(string + "\n\n\n" + addedString);
        }

        // Check which "important" keys were pressed, if any
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0) && !justPressed && addedString.length() < 10)
        {
            addedString += '0';
            justPressed = true;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1) && !justPressed && addedString.length() < 10)
        {
            addedString += '1';
            justPressed = true;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2) && !justPressed && addedString.length() < 10)
        {
            addedString += '2';
            justPressed = true;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3) && !justPressed && addedString.length() < 10)
        {
            addedString += '3';
            justPressed = true;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4) && !justPressed && addedString.length() < 10)
        {
            addedString += '4';
            justPressed = true;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5) && !justPressed && addedString.length() < 10)
        {
            addedString += '5';
            justPressed = true;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6) && !justPressed && addedString.length() < 10)
        {
            addedString += '6';
            justPressed = true;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7) && !justPressed && addedString.length() < 10)
        {
            addedString += '7';
            justPressed = true;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8) && !justPressed && addedString.length() < 10)
        {
            addedString += '8';
            justPressed = true;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num9) && !justPressed && addedString.length() < 10)
        {
            addedString += '9';
            justPressed = true;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace) && !justPressed && !addedString.empty())
        {
            addedString.pop_back();
            justPressed = true;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && !justPressed && !addedString.empty())
        {
            toReturn = std::stoi(addedString);
            justPressed = true;
        }
        else if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Num0) || sf::Keyboard::isKeyPressed(sf::Keyboard::Num1) || sf::Keyboard::isKeyPressed(sf::Keyboard::Num2) || sf::Keyboard::isKeyPressed(sf::Keyboard::Num3) || sf::Keyboard::isKeyPressed(sf::Keyboard::Num4) || sf::Keyboard::isKeyPressed(sf::Keyboard::Num5) || sf::Keyboard::isKeyPressed(sf::Keyboard::Num6) || sf::Keyboard::isKeyPressed(sf::Keyboard::Num7) || sf::Keyboard::isKeyPressed(sf::Keyboard::Num8) || sf::Keyboard::isKeyPressed(sf::Keyboard::Num9) || sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace)))
        {
            justPressed = false;
        }
    }

    // Return result
    return toReturn;
}

int MenuService::getBoardWidth() const
{
    constexpr short minWidth{30};
    constexpr short maxWidth{200};
    return promptForInteger("Choose the board width\n(Between " + std::to_string(minWidth) + " and " + std::to_string(maxWidth) + ')', minWidth, maxWidth);
}
int MenuService::getBoardHeight() const
{
    constexpr short minHeight{20};
    constexpr short maxHeight{200};
    return promptForInteger("Choose the board height\n(Between " + std::to_string(minHeight) + " and " + std::to_string(maxHeight) + ')', minHeight, maxHeight);
}

double MenuService::getGameSpeed() const
{
    switch (promptForInteger("Choose a game speed\n\nSlow   1\nMedium 2\nFast   3", 1, 3))
    {
    case 1:
        return 1.0 / 5.0 * 1000.0;
    case 2:
        return 1.0 / 10.0 * 1000.0;
    case 3:
        return 1.0 / 15.0 * 1000.0;
    default:
        return -1;
    }
}

int MenuService::getInitialSnakeLength() const
{
    switch (promptForInteger("Choose the starting snake length\n\nSmall  1\nMedium 2\nLarge  3", 1, 3))
    {
    case 1:
        return 3;
    case 2:
        return 5;
    case 3:
        return 8;
    default:
        return -1;
    }
}

bool MenuService::animationHasSafeMove() const
{
    Point head = game->getSnake().getHead();
    Point up = head.getAdjacentPoint(Directions::Direction::UP);
    Point right = head.getAdjacentPoint(Directions::Direction::RIGHT);
    Point down = head.getAdjacentPoint(Directions::Direction::DOWN);
    Point left = head.getAdjacentPoint(Directions::Direction::LEFT);
    return !game->getSnake().isInSnake(up) && game->getBoard().isInBoard(up) || !game->getSnake().isInSnake(right) && game->getBoard().isInBoard(right) || !game->getSnake().isInSnake(down) && game->getBoard().isInBoard(down) || !game->getSnake().isInSnake(left) && game->getBoard().isInBoard(left);
}

void MenuService::playBoardAnimation(bool &isCancelled)
{
    while (!isCancelled)
    {
        // Create a new game if last one was over
        if (!game || game->isGameOver())
            game = std::make_unique<Game>(std::make_unique<Board>(30, 20), std::make_unique<Snake>(Point(5, 20), 5));

        // Declare vars
        std::uniform_int_distribution next{0, 3};
        bool hasSafeMove{animationHasSafeMove()};
        Directions::Direction nextDirection;
        Point destination;

        // Find the next move
        do
        {
            nextDirection = static_cast<Directions::Direction>(next(Utility::getRandom()));
            destination = game->getSnake().getHead().getAdjacentPoint(nextDirection);
        } while (hasSafeMove && (game->getSnake().isInSnake(destination) || !game->getBoard().isInBoard(destination)));

        // Make next move
        if (!hasSafeMove)
        {
            game->getSnake().crash(nextDirection);
        }
        else if (destination == game->getApple())
        {
            game->setApple(game->getRandomVacantPoint());
            game->getSnake().grow(nextDirection);
        }
        else
        {
            game->getSnake().move(nextDirection);
        }

        // Render the resulting board
        Utility::printSafe(game->toString(), true);

        // Pause
        Utility::pauseThread(game->isGameOver() ? 1000 : 200);
    }
}

void MenuService::showMainMenu()
{
    // Null check
    if (!game)
        throw std::invalid_argument("game is null");

    // Declare vars
    bool userQuit{false};
    bool cancellationToken{false};
    std::future<void> animation;

    // Run game until user quits
    while (!userQuit)
    {
        // Set main menu message
        game->setMessage("Welcome to\nSnake!\n\nPress 'enter' to continue or 'escape' to quit");

        // Start board animation
        cancellationToken = false;
        animation = playBoardAnimationTask(cancellationToken);

        // Await key press and update userQuit
        while (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) || (userQuit = sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))))
            ;

        // Check if user quit
        if (userQuit)
            break;

        // Prompt user for settings
        int boardWidth = getBoardWidth();
        int boardHeight = getBoardHeight();
        int snakeLength = getInitialSnakeLength();
        double gameSpeed = getGameSpeed();

        // Stop the board animation task
        cancellationToken = true;

        // Start the game and wait for it to end
        gameService->startNewGameTask(boardWidth, boardHeight, snakeLength, gameSpeed).wait();

        // After the game ends wait so user can see results
        Utility::pauseThread(5000);
    }

    // After user has quit, finish animation and wait for threads to finish
    cancellationToken = true;
    animation.wait();
}
