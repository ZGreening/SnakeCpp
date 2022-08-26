#include "game_service.hpp"
#include "direction.hpp"
#include "utility.hpp"
#include "SFML/Window.hpp"
#include <stdexcept>

void GameService::updateAppleLocation()
{
    // Null check
    if (!game)
        throw std::invalid_argument("game is null");

    // Set the apple to a new location
    game->setApple(game->getRandomVacantPoint());
}

void GameService::render()
{
    // Null check
    if (!game)
        throw std::invalid_argument("game is null");

    // Render the board
    std::string string;
    {
        const std::lock_guard<std::mutex> lock(updateMutex);
        string = game->toString();
    }
    Utility::printSafe(string, true);
}

void GameService::processLogic()
{
    // Null check
    if (!game)
        throw std::invalid_argument("game is null");

    // Ensure thread safe updates
    const std::lock_guard<std::mutex> lock(updateMutex);

    // Get the destination point
    const Point destination{game->getSnake().getHead().getAdjacentPoint(directionMoving)};

    static short lastAte{0};

    // Handle snake movement
    if (destination == game->getApple())
    {
        updateAppleLocation();
        game->getSnake().grow(directionMoving);
        game->setScore(game->getScore() + 10);
        game->setMessage("YUM!!!");
        lastAte = 3;
    }
    else if (game->getSnake().isInSnake(destination) && game->getSnake().getTail() != destination)
    {
        game->getSnake().crash(directionMoving);
        game->setMessage("GAMEOVER!\n\nYou ate your tail!");
    }
    else if (!game->getBoard().isInBoard(destination))
    {
        game->getSnake().crash(directionMoving);
        game->setMessage("GAMEOVER!\n\nYou hit the wall!");
    }
    else
    {
        if (lastAte > 0)
            --lastAte;
        else
            game->setMessage("");
        game->getSnake().move(directionMoving);
    }
}

void GameService::processInput()
{
    // Null check
    if (!game)
        throw std::invalid_argument("game is null");

    // For ignoring pause/unpause if key is held
    static bool justChanged{false};

    // Update game_service based on keyboard input
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
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
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && directionMoving != Directions::Direction::DOWN)
        directionMoving = Directions::Direction::UP;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && directionMoving != Directions::Direction::LEFT)
        directionMoving = Directions::Direction::RIGHT;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && directionMoving != Directions::Direction::UP)
        directionMoving = Directions::Direction::DOWN;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && directionMoving != Directions::Direction::RIGHT)
        directionMoving = Directions::Direction::LEFT;
}

void GameService::createProcessInputTask()
{
    // Check if task exists or game is null
    if (processInputTask.valid())
        throw std::runtime_error("processInputTask already exists");
    if (!game)
        throw std::invalid_argument("game is null");

    // Create task and assign
    processInputTask = std::async(std::launch::async, [this]
                                  { while (!game->isGameOver()) processInput(); });
}

void GameService::createProcessLogicTask()
{
    // Check if task exists or game is null
    if (processLogicTask.valid())
        throw std::runtime_error("processLogicTask already exists");
    if (!game)
        throw std::invalid_argument("game is null");

    // Create task and assign
    processLogicTask = std::async(std::launch::async, [this]
                                  { while(!game->isGameOver()) { if(!gameIsPaused) processLogic(); Utility::pauseThread(gameSpeed); } });
}

void GameService::createRenderTask()
{
    // Check if task exists or game is null
    if (renderTask.valid())
        throw std::runtime_error("renderTask already exists");
    if (!game)
        throw std::invalid_argument("game is null");

    // Create task and assign
    renderTask = std::async(std::launch::async, [this]
                            { while(!game->isGameOver()) { render(); Utility::pauseThread(TIME_BETWEEN_RENDER_MILLISECONDS); } render(); });
}

void GameService::startNewGame(int boardWidth, int boardHeight, int snakeLength)
{
    // Check if game is still running
    if (game && !game->isGameOver())
        throw std::runtime_error("game is still in progress");

    // Create the game
    game = std::make_unique<Game>(std::make_unique<Board>(boardWidth, boardHeight), std::make_unique<Snake>(Point(snakeLength, boardHeight), snakeLength));

    // Set the start message
    game->setMessage("Welcome to\nSnake!\n\nMove the snake around the board, and eat as many apples as you can\n\nAvoid the walls and yourself\n\nWhen you crash, it's gameover!");

    // Render the board
    render();

    // Wait for a key press
    while (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)))
        ;

    // Clear the message
    game->setMessage("");

    // Create the tasks to start the game
    createProcessInputTask();
    createProcessLogicTask();
    createRenderTask();

    // Wait for all tasks to complete before exiting the function
    std::this_thread::yield();
    processInputTask.wait();
    processLogicTask.wait();
    renderTask.wait();
}
