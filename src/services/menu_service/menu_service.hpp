#ifndef MENU_SERVICE_H
#define MENU_SERVICE_H

#include "point.hpp"
#include "game.hpp"
#include "game_service.hpp"
#include <string_view>
#include <future>
#include <memory>

class MenuService
{
private:
    /**
     * @brief A game object for displaying as a menu with prompts
     *
     */
    std::unique_ptr<Game> game{std::make_unique<Game>(std::make_unique<Board>(30, 20), std::make_unique<Snake>(Point(5, 20), 5))};

    /**
     * @brief
     *
     */
    std::unique_ptr<GameService> gameService{std::make_unique<GameService>()};

    ///////////////////////////////////////////////////////////////////////////////////////////////

    /**
     * @brief Uses the provided string as a prompt and waits for the user to enter a number within the max and min values (inclusive)
     *
     * @param string The prompt
     * @param min The min value (inclusive)
     * @param max The max value (inclusive)
     * @return int The value picked by the user
     */
    int promptForInteger(std::string string, int min, int max) const;

    /**
     * @brief Get the Board Width from the user
     *
     * @return int
     */
    int getBoardWidth() const;

    /**
     * @brief Get the Board Height from the user
     *
     * @return int
     */
    int getBoardHeight() const;

    /**
     * @brief Plays the animation
     *
     * @note The animation is random steps of the snake
     */
    void playBoardAnimation(bool &cancellationToken);

    /**
     * @brief Plays the board animation
     *
     * @param cancellationToken cancels the task
     * @return std::future<void>
     */
    std::future<void> playBoardAnimationTask(bool &cancellationToken) { return std::async(std::launch::async, &MenuService::playBoardAnimation, this, std::ref(cancellationToken)); };

    /**
     * @brief Get the Game Speed from the user
     *
     * @return double The game speed picked from the user
     */
    double getGameSpeed() const;

    /**
     * @brief Get the Initial Snake Length based on the size of the board
     *
     * @return int The initial Snake Length
     */
    int getInitialSnakeLength() const;

    /**
     * @brief Shows and performs the tasks of the main menu
     *
     */
    void showMainMenu();

    /**
     * @brief Returns true if the snake has a move that will not result in a crash.
     *
     * @return true if there is a move that will not crash the snake
     * @return false if there is not a move that will not crash the snake
     */
    bool animationHasSafeMove() const;

public:
    /**
     * @brief Shows the welcome menu.
     *
     * @return std::future<void>
     */
    std::future<void> showMainMenuTask() { return std::async(&MenuService::showMainMenu, this); };
};

#endif