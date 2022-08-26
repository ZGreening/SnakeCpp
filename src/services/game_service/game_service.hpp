#ifndef GAME_SERVICE_H
#define GAME_SERVICE_H

#include "game.hpp"
#include <future>
#include <stdexcept>
#include <memory>

constexpr int TIME_BETWEEN_RENDER_MILLISECONDS = 1.0 / 20.0 * 1000.0;

class GameService
{
private:
    /**
     * @brief The direction the snake will move/grow next
     *
     */
    std::atomic<Directions::Direction> directionMoving{Directions::Direction::RIGHT};

    /**
     * @brief Whether or not the game is paused
     *
     */
    bool gameIsPaused{false};

    /**
     * @brief A task for processing input
     *
     */
    std::future<void> processInputTask;

    /**
     * @brief A task for processing game logic
     *
     */
    std::future<void> processLogicTask;

    /**
     * @brief A process for rendering the game
     *
     */
    std::future<void> renderTask;

    /**
     * @brief A mutex for performing game updates in sync
     *
     */
    std::mutex updateMutex;

    /**
     * @brief The game
     *
     */
    std::unique_ptr<Game> game;

    /**
     * @brief The game speed
     *
     */
    double gameSpeed;

public:
    /**
     * @brief Get the Game object
     *
     * @return Game&
     */
    Game &getGame() const
    {
        if (!game)
            throw std::invalid_argument("game is null");
        return *game;
    }

    /**
     * @brief Updates the location of the apple
     *
     */
    void updateAppleLocation();

    /**
     * @brief Processes all the logic of the game for a given tick
     *
     */
    void processLogic();

    /**
     * @brief Renders the game board
     *
     */
    void render();

    /**
     * @brief Process user input
     *
     */
    void processInput();

    /**
     * @brief Create a Process Input Task object
     *
     */
    void createProcessInputTask();

    /**
     * @brief Create a Render Task object
     *
     */
    void createRenderTask();

    /**
     * @brief Create a Process Logic Task object
     *
     */
    void createProcessLogicTask();

    /**
     * @brief Creates and starts a new game
     *
     * @note This function blocks while the game is running and exits when the game ends
     * @param boardWidth
     * @param boardHeight
     * @param snakeLength
     * @return std::future<void>
     */
    void startNewGame(int boardWidth, int boardHeight, int snakeLength);

    /**
     * @brief Creates and starts a new game and returns a task
     *
     * @param boardWidth
     * @param boardHeight
     * @param snakeLength
     * @return std::future<void>
     */
    std::future<void> startNewGameTask(int boardWidth, int boardHeight, int snakeLength, double gameSpeed = 200.0)
    {
        this->gameSpeed = gameSpeed;
        return std::async(&GameService::startNewGame, this, boardWidth, boardHeight, snakeLength);
    }
};

#endif