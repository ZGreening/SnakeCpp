#ifndef MENU_SERVICE_H
#define MENU_SERVICE_H

#include "point.hpp"
#include "game.hpp"
#include "game_service.hpp"
#include "SFML/Window.hpp"
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
    std::unique_ptr<Game> menuGame{std::make_unique<Game>(std::make_unique<Board>(30, 20), std::make_unique<Snake>(Point(5, 20), 5))};

    /**
     * @brief The game service instance
     *
     */
    std::unique_ptr<GameService> gameService{std::make_unique<GameService>()};

    /**
     * The board animation
     */
    std::future<void> animation{};

    /**
     * The last pressed keyboard key
     */
    sf::Keyboard::Key lastPressedKey{};

    /**
     * @brief The cancellation to stop the animation
     */
    bool isAnimationPlaying{false};

public:
    /**
     * @brief Sets the menu as showing or not
     */
    void setMenuShowing(bool setter);

    /**
     * @brief Shows the welcome menu.
     *
     * @return std::future<void>
     */
    std::future<void> showMainMenuTask() { return std::async(&MenuService::showMainMenu, this); }

private:
    /**
     * @brief Prompts the user for a boolean (y for true, n for false)
     *
     * @param prompt The prompt
     */
    bool promptForBoolean(std::string prompt);

    /**
     * @brief Prompts the user for a string
     *
     * @param prompt The prompt
     * @param maxLength The max length of the string allowed
     */
    std::string promptForString(std::string prompt, short maxLength);

    /**
     * @brief Uses the provided string as a prompt and waits for the user to enter a number within the max and min values (inclusive)
     *
     * @param prompt The prompt
     * @param min The min value (inclusive)
     * @param max The max value (inclusive)
     * @return int The value picked by the user
     */
    int promptForInteger(std::string prompt, int min, int max);

    /**
     * @brief Get the Board Width from the user
     *
     * @return int
     */
    int getBoardWidth();

    /**
     * @brief Get the Board Height from the user
     *
     * @return int
     */
    int getBoardHeight();

    /**
     * @brief Plays the animation
     *
     * @note The animation is random steps of the snake
     */
    void playBoardAnimation();

    /**
     * @brief Stops the board animation
     */
    void stopBoardAnimation()
    {
        isAnimationPlaying = false;
        if (animation.valid())
            animation.wait();
    }

    /**
     * @brief Plays the board animation
     */
    void playBoardAnimationTask()
    {
        // If the animation is not playing, start it again
        if (!animation.valid() || animation.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
            animation = std::async(std::launch::async, &MenuService::playBoardAnimation, this);
    }

    /**
     * @brief Get the Game Speed from the user
     *
     * @return double The game speed picked from the user
     */
    double getGameSpeed();

    /**
     * @brief Get the Initial Snake Length based on the size of the board
     *
     * @return int The initial Snake Length
     */
    int getInitialSnakeLength();

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

    /**
     * @brief Shows the scores menu
     */
    void showScoresMenu();

    /**
     * Checks if the key was just pressed and modifies lastPressedKey
     */
    const bool KEYP(sf::Keyboard::Key key);
};

#endif