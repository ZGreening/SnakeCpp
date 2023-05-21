#include "game_service.hpp"
#include "direction.hpp"
#include "utility.hpp"
#include "file_service.hpp"
#include "SFML/Window.hpp"
#include <stdexcept>
#include <memory>

constexpr auto &KEYP = sf::Keyboard::isKeyPressed;
using enum sf::Keyboard::Key;
using namespace std;

void GameService::updateAppleLocation()
{
    // Null check
    if (!game)
        throw invalid_argument("game is null");

    // Set the apple to a new location
    game->setApple(game->getRandomVacantPoint());
}

void GameService::render()
{
    // Null check
    if (!game)
        throw invalid_argument("game is null");

    // Render the board
    string string;
    {
        const lock_guard<mutex> lock(updateMutex);
        string = game->toString();
    }
    Utility::printSafe(string, true);
}

void GameService::processLogic()
{
    // Null check
    if (!game)
        throw invalid_argument("game is null");

    // Ensure thread safe updates
    const lock_guard<mutex> lock(updateMutex);

    // Get the destination point
    const Point destination{game->getSnake().getHead().getAdjacentPoint(inputDirection)};

    static short lastAte{0};

    // Handle snake movement
    if (destination == game->getApple())
    {
        updateAppleLocation();
        game->getSnake().grow(inputDirection);
        game->setScore(game->getScore() + 10);
        game->setMessage("YUM!!!");
        lastAte = 3;
    }
    else if (game->getSnake().isInSnake(destination) && game->getSnake().getTail() != destination)
    {
        game->getSnake().crash(inputDirection);
        game->setMessage("GAMEOVER!\n\nYou ate your tail!");
    }
    else if (!game->getBoard().isInBoard(destination))
    {
        game->getSnake().crash(inputDirection);
        game->setMessage("GAMEOVER!\n\nYou hit the wall!");
    }
    else
    {
        if (lastAte > 0)
            --lastAte;
        else
            game->setMessage("");
        game->getSnake().move(inputDirection);
    }
}

void GameService::processInput()
{
    // Null check
    if (!game)
        throw invalid_argument("game is null");

    // For ignoring pause/unpause if key is held
    static bool justChanged{false};

    // Update game_service based on keyboard input
    if (KEYP(Escape))
    {
        if (!justChanged)
        {
            gameIsPaused = !gameIsPaused;
            if (gameIsPaused)
                game->setMessage("PAUSED");
            else
                game->setMessage("");
            justChanged = true;
        }
    }
    else
    {
        justChanged = false;
    }

    // Update direction moving
    if (KEYP(W) && game->getSnake().getDirection() != Directions::Direction::DOWN)
        inputDirection = Directions::Direction::UP;
    if (KEYP(D) && game->getSnake().getDirection() != Directions::Direction::LEFT)
        inputDirection = Directions::Direction::RIGHT;
    if (KEYP(S) && game->getSnake().getDirection() != Directions::Direction::UP)
        inputDirection = Directions::Direction::DOWN;
    if (KEYP(A) && game->getSnake().getDirection() != Directions::Direction::RIGHT)
        inputDirection = Directions::Direction::LEFT;
}

void GameService::createProcessInputTask()
{
    // Check if task exists or game is null
    if (processInputTask.valid())
        throw runtime_error("processInputTask already exists");
    if (!game)
        throw invalid_argument("game is null");

    // Create task and assign
    processInputTask = async(launch::async, [this]
                             { while (!game->isGameOver()) processInput(); });
}

void GameService::createProcessLogicTask()
{
    // Check if task exists or game is null
    if (processLogicTask.valid())
        throw runtime_error("processLogicTask already exists");
    if (!game)
        throw invalid_argument("game is null");

    // Create task and assign
    processLogicTask = async(launch::async, [this]
                             { while(!game->isGameOver()) { if(!gameIsPaused) processLogic(); Utility::pauseThread(game->getGameSpeed()); } });
}

void GameService::createRenderTask()
{
    // Check if task exists or game is null
    if (renderTask.valid())
        throw runtime_error("renderTask already exists");
    if (!game)
        throw invalid_argument("game is null");

    // Create task and assign
    renderTask = async(launch::async, [this]
                       { while(!game->isGameOver()) { render(); Utility::pauseThread(TIME_BETWEEN_RENDER_MILLISECONDS); } render(); });
}

void GameService::saveScore(const string &playerName)
{
    auto fileService{make_unique<FileService>()};
    game->setPlayerName(playerName);
    fileService->saveScore(*game);
}

void GameService::saveSettings()
{
    auto fileService{make_unique<FileService>()};
    fileService->saveSettings(*game);
}

void GameService::startNewGame(int boardWidth, int boardHeight, int snakeLength, double gameSpeed)
{
    // Check if game is still running
    if (game && !game->isGameOver())
        throw runtime_error("game is still in progress");

    // Create the game and start it
    startNewGame(make_unique<Game>(make_unique<Board>(boardWidth, boardHeight), make_unique<Snake>(Point(snakeLength, boardHeight), snakeLength), gameSpeed));
}

void GameService::startNewGame(unique_ptr<Game> game)
{
    // Check if game is still running
    if (this->game && !this->game->isGameOver())
        throw runtime_error("game is still in progress");

    // Start the passed game
    this->game = std::move(game);

    // Set the start message
    this->game->setMessage("Welcome to\nSnake!\n\nMove the snake around the board, and eat as many apples as you can\n\nAvoid the walls and yourself\n\nWhen you crash, it's gameover!");

    // Render the board
    render();

    // Wait for a key press
    while (!(KEYP(Escape) || KEYP(A) || KEYP(D) || KEYP(W) || KEYP(S)))
        ;

    // Clear the message
    this->game->setMessage("");

    // Create the tasks to start the game
    createProcessInputTask();
    createProcessLogicTask();
    createRenderTask();

    // Wait for all tasks to complete before exiting the function
    this_thread::yield();
    processInputTask.wait();
    processLogicTask.wait();
    renderTask.wait();
}
