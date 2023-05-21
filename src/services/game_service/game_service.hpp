#ifndef GAME_SERVICE_H
#define GAME_SERVICE_H

#include "file_service.hpp"
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
    std::atomic<Directions::Direction> inputDirection{Directions::Direction::RIGHT};

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
     * @brief Saves the score with the provided player name
     *
     * @param playerName The player name to associate with the saved score
     */
    void saveScore(const std::string &playerName);

    /**
     * @brief Saves the settings of the game
     */
    void saveSettings();

    /**
     * @brief Creates and starts a new game using previous settings and returns a task
     *
     * @return std::future<void>
     * @throws std::invalid_argument Thrown if unable to start a game using old settings
     */
    std::future<void> startNewGameTask()
    {
        auto fileService{std::make_unique<FileService>()};
        auto settings = fileService->loadSettings();
        return std::async(static_cast<void (GameService::*)(std::unique_ptr<Game>)>(&GameService::startNewGame), this, std::move(settings));
    }

    /**
     * @brief Creates and starts a new game and returns a task
     *
     * @param boardWidth
     * @param boardHeight
     * @param snakeLength
     * @return std::future<void>
     */
    std::future<void> startNewGameTask(const int boardWidth, const int boardHeight, const int snakeLength, const double gameSpeed)
    {
        return std::async(static_cast<void (GameService::*)(const int, const int, const int, const double)>(&GameService::startNewGame), this, boardWidth, boardHeight, snakeLength, gameSpeed);
    }

private:
    /**
     * @brief Starts the new game passed
     *
     * @note This function blocks while the game is running and exits when the game ends
     * @param game The game to start
     */
    void startNewGame(std::unique_ptr<Game> game);

    /**
     * @brief Creates and starts a new game
     *
     * @note This function blocks while the game is running and exits when the game ends
     * @param boardWidth
     * @param boardHeight
     * @param snakeLength
     * @param gameSpeed
     */
    void startNewGame(const int boardWidth, const int boardHeight, const int snakeLength, const double gameSpeed);

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
};

#endif