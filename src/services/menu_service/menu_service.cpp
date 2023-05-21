#include "menu_service.hpp"
#include "point.hpp"
#include "utility.hpp"
#include "game_service.hpp"
#include "file_service.hpp"
#include "game.hpp"
#include "SFML/Window.hpp"
#include "plog/Log.h"
#include <future>
#include <string>
#include <sstream>
#include <stdexcept>
#include <climits>

using enum sf::Keyboard::Key;
using namespace std;
constexpr int MENU_PAUSE_TIME{3000};

inline const bool MenuService::KEYP(const sf::Keyboard::Key key)
{
    bool isPressed = sf::Keyboard::isKeyPressed(key);
    if (isPressed)
        lastPressedKey = key;
    return isPressed;
}

string MenuService::promptForString(string prompt, short maxLength)
{
    // Null check
    if (!menuGame)
        throw invalid_argument("game is null");

    // Display prompt
    menuGame->setMessage(prompt);

    // Declare vars
    string addedString{};
    bool justPressed{false};

    // Print the message for getting the int
    while (true)
    {
        // Allow all alpha numeric characters and backspace
        if (KEYP(BackSpace) || KEYP(Num0) || KEYP(Num1) || KEYP(Num2) || KEYP(Num3) || KEYP(Num4) || KEYP(Num5) || KEYP(Num6) || KEYP(Num7) || KEYP(Num8) || KEYP(Num9) || KEYP(A) || KEYP(B) || KEYP(C) || KEYP(D) || KEYP(E) || KEYP(F) || KEYP(G) || KEYP(H) || KEYP(I) || KEYP(J) || KEYP(K) || KEYP(L) || KEYP(M) || KEYP(N) || KEYP(O) || KEYP(P) || KEYP(Q) || KEYP(R) || KEYP(S) || KEYP(T) || KEYP(U) || KEYP(V) || KEYP(Q) || KEYP(R) || KEYP(S) || KEYP(T) || KEYP(U) || KEYP(V) || KEYP(W) || KEYP(X) || KEYP(Y) || KEYP(Z))
        {
            // If key was not just pressed, handle it
            if (!justPressed)
            {
                if (lastPressedKey == BackSpace)
                    addedString.pop_back();
                else if (addedString.length() < maxLength)
                    addedString += Utility::sfKeyToChar(lastPressedKey);

                const lock_guard<mutex> lock(Utility::getIoMutex());
                menuGame->setMessage(prompt + "\n\n\n" + addedString);
            }

            // Continue to next loop
            justPressed = true;
            continue;
        }

        // If enter key was pressed and string is not empty, return the string
        if (KEYP(Enter) && !addedString.empty())
            return addedString;

        // Reset just pressed
        justPressed = false;
    }
}

bool MenuService::promptForBoolean(string prompt)
{
    char result;
    do
        result = promptForString(prompt, 1)[0];
    while (!(result == 'y' || result == 'n'));
    return result == 'y';
}

int MenuService::promptForInteger(string prompt, int min, int max)
{
    int result;
    do
    {
        try
        {
            result = stoi(promptForString(prompt, 10));
        }
        catch (exception &exception)
        {
            PLOGW << "Unable to convert input to an integer " << exception.what();
        }
    } while (result < min || result > max);
    return result;
}

int MenuService::getBoardWidth()
{
    constexpr short minWidth{30};
    constexpr short maxWidth{200};
    return promptForInteger("Choose the board width\n(Between " + to_string(minWidth) + " and " + to_string(maxWidth) + ')', minWidth, maxWidth);
}
int MenuService::getBoardHeight()
{
    constexpr short minHeight{20};
    constexpr short maxHeight{200};
    return promptForInteger("Choose the board height\n(Between " + to_string(minHeight) + " and " + to_string(maxHeight) + ')', minHeight, maxHeight);
}

double MenuService::getGameSpeed()
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

int MenuService::getInitialSnakeLength()
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
    Point head = menuGame->getSnake().getHead();
    Point up = head.getAdjacentPoint(Directions::Direction::UP);
    Point right = head.getAdjacentPoint(Directions::Direction::RIGHT);
    Point down = head.getAdjacentPoint(Directions::Direction::DOWN);
    Point left = head.getAdjacentPoint(Directions::Direction::LEFT);
    return !menuGame->getSnake().isInSnake(up) && menuGame->getBoard().isInBoard(up) || !menuGame->getSnake().isInSnake(right) && menuGame->getBoard().isInBoard(right) || !menuGame->getSnake().isInSnake(down) && menuGame->getBoard().isInBoard(down) || !menuGame->getSnake().isInSnake(left) && menuGame->getBoard().isInBoard(left);
}

void MenuService::showScoresMenu()
{
    // Initialize file service
    auto fileService{make_unique<FileService>()};
    auto scores{make_unique<vector<Game>>()};
    auto pageShowing{0};

    // Booleans for user input
    auto userClosed{false};
    auto rightPressed{false};
    auto leftPressed{false};

    // Run menu until break
    while (true)
    {
        // Get scores by current page being viewed
        try
        {
            PLOGI << "Loading score page " + to_string(pageShowing);
            fileService->loadScores(*scores, pageShowing);
        }
        catch (out_of_range &exception)
        {
            PLOGW << "Score page " + to_string(pageShowing) + " does not exist. Showing page " + to_string(pageShowing - 1);
            --pageShowing;
        }
        catch (invalid_argument &exception)
        {
            PLOGW << "No saved scores file";
            pageShowing = 0;
        }

        // Collect score strings
        stringstream stream{};

        stream << Game::getScoreHeader() << '\n';

        for (auto &score : *scores)
            stream << score.getScoreString() << '\n';

        // Print message if no scores found
        if (scores->size() == 0)
            stream << "No saved scores";

        // Remove the last \n char
        stream.seekp(-1, ios_base::end);

        // Display the scores
        menuGame->setMessage(stream.str());

        // Wait for user input
        while (!((userClosed = KEYP(Escape)) || (rightPressed = KEYP(Right)) || (leftPressed = KEYP(Left))))
            ;

        // Close scores menu
        if (userClosed)
        {
            menuGame->setMessage("");
            break;
        }

        // Increment or decrement page number based on the key the user pressed
        if (rightPressed)
            ++pageShowing;
        if (leftPressed && pageShowing > 0)
            --pageShowing;
    }
}

void MenuService::playBoardAnimation()
{
    isAnimationPlaying = true;
    while (isAnimationPlaying)
    {
        // Create a new game if last one was over
        if (!menuGame || menuGame->isGameOver())
            menuGame = make_unique<Game>(make_unique<Board>(50, 30), make_unique<Snake>(Point(5, 20), 5));

        // Declare vars
        uniform_int_distribution next{0, 3};
        bool hasSafeMove{animationHasSafeMove()};
        Directions::Direction nextDirection;
        Point destination;

        // Find the next move
        do
        {
            nextDirection = static_cast<Directions::Direction>(next(Utility::getRandom()));
            destination = menuGame->getSnake().getHead().getAdjacentPoint(nextDirection);
        } while (hasSafeMove && (menuGame->getSnake().isInSnake(destination) || !menuGame->getBoard().isInBoard(destination)));

        // Make next move
        if (!hasSafeMove)
        {
            menuGame->getSnake().crash(nextDirection);
        }
        else if (destination == menuGame->getApple())
        {
            menuGame->setApple(menuGame->getRandomVacantPoint());
            menuGame->getSnake().grow(nextDirection);
            menuGame->setScore(menuGame->getScore() + 10);
        }
        else
        {
            menuGame->getSnake().move(nextDirection);
        }

        // Render the resulting board
        Utility::printSafe(menuGame->toString(), true);

        // Pause
        Utility::pauseThread(menuGame->isGameOver() ? 1000 : 200);
    }
}

void MenuService::showMainMenu()
{
    // Null check
    if (!menuGame)
        throw invalid_argument("game is null");

    // Declare vars
    bool userQuit{false};
    bool showScores{false};

    // Run game until user quits
    while (!userQuit)
    {
        // Play the board animation
        playBoardAnimationTask();

        // Set main menu message
        menuGame->setMessage("Welcome to\nSnake!\n\nPress 'enter' to continue, 'down' to view scores, or 'escape' to quit");

        // Await key press and update userQuit
        while (!(KEYP(Enter) || (userQuit = KEYP(Escape)) || (showScores = KEYP(Down))))
            ;

        // Check if user quit
        if (userQuit)
            break;

        // Show scores menu if requested
        if (showScores)
        {
            showScores = false;
            showScoresMenu();
            continue;
        }

        int boardWidth;
        int boardHeight;
        int snakeLength;
        double gameSpeed;

        if (FileService::hasSettingsFile() && promptForBoolean("Would you like to play with the previous settings?\n'Y' for yes\n'N' for no"))
        {
            // Stop the board animation task
            stopBoardAnimation();

            // Start game with previous settings if they exist
            try
            {
                gameService->startNewGameTask().wait();
            }
            catch (invalid_argument &exception)
            {
                playBoardAnimationTask();
                menuGame->setMessage("An error occurred while trying to start the game using the previous settings");
                Utility::pauseThread(MENU_PAUSE_TIME);
                continue;
            }
        }
        else
        {
            // Prompt user for settings
            boardWidth = getBoardWidth();
            boardHeight = getBoardHeight();
            snakeLength = getInitialSnakeLength();
            gameSpeed = getGameSpeed();

            // Stop the board animation task
            stopBoardAnimation();

            // Start the game and wait for it to end
            gameService->startNewGameTask(boardWidth, boardHeight, snakeLength, gameSpeed).wait();
        }

        // Save the settings of the last game
        gameService->saveSettings();

        // After the game ends wait so user can see results
        Utility::pauseThread(MENU_PAUSE_TIME);

        // Restart animation task
        playBoardAnimationTask();

        // Prompt to save the game
        if (promptForBoolean("Would you like to save your score?\n'Y' for yes\n'N' for no"))
        {
            try
            {
                // Save if requested
                gameService->saveScore(promptForString("Please enter your name:", 10));
                menuGame->setMessage("Game saved");
            }
            catch (exception &exception)
            {
                // Log any errors
                gameService->getGame().setMessage("An error occurred while saving");
                PLOGE << "An error occurred while saving " << exception.what();
            }
        }
        else
        {
            // If not saving, change menu message to give user feedback
            menuGame->setMessage("GAME OVER");
        }

        // Pause thread
        Utility::pauseThread(MENU_PAUSE_TIME);
    }

    // After user has quit, finish animation and wait for threads to finish
    stopBoardAnimation();
}
