#ifndef Game_H
#define Game_H

#include "point.hpp"
#include "snake.hpp"
#include "board.hpp"
#include <string>
#include <stdexcept>
#include <memory>
#include <regex>
#include <vector>

/**
 * @brief The class representing a game
 *
 */
class Game
{
private:
    /**
     * @brief The name of the player
     */
    std::string playerName;

    /**
     * @brief The game speed the game is played at (milliseconds between logical steps)
     */
    const double gameSpeed;

    /**
     * @brief The score of the game
     *
     */
    int score{0};

    /**
     * @brief The snake playing the game
     *
     */
    std::unique_ptr<Snake> snake;

    /**
     * @brief The game board
     *
     */
    std::unique_ptr<Board> board;

    /**
     * @brief The apple the snake is after
     *
     */
    Point apple;

    /**
     * @brief Regex for splitting message
     *
     */
    const std::regex messageSplitterRegex{"[^\\n]{0," + std::to_string(static_cast<int>(board->getWidth() * 2.0 / 3.0)) + "}(?:\\n|\\ |$)"};

    /**
     * @brief A message for the player
     *
     * @note Displays in the middle of the board
     */
    std::string message{};

    /**
     * @brief Message split into lines based on board width using messageSplitterRegex
     *
     */
    std::vector<std::string> matches;

    /**
     * @brief The game as a string
     *
     */
    std::string gameAsString;

public:
    /**
     * @brief Get the Apple object
     *
     * @return Point
     */
    const Point &getApple() const { return apple; }

    /**
     * @brief Set the Apple object
     *
     * @param apple
     * @return Point
     */
    void setApple(Point apple) { this->apple = apple; }

    /**
     * @brief Get the Snake object
     *
     * @return Snake
     */
    Snake &getSnake() const
    {
        if (!snake)
            throw std::invalid_argument("snake is null");
        return *snake;
    }

    /**
     * @brief Set the Snake object
     *
     * @param snake
     */
    void setSnake(std::unique_ptr<Snake> snake) { this->snake = std::move(snake); }

    /**
     * @brief Get the Board object
     *
     * @return Board
     */
    const Board &getBoard() const
    {
        if (!board)
            throw std::invalid_argument("board is null");
        return *board;
    }

    /**
     * @brief Set the Board object
     *
     * @param board
     */
    void setBoard(std::unique_ptr<Board> board) { this->board = std::move(board); }

    /**
     * @brief Get the Score object
     *
     * @return int
     */
    int getScore() const { return score; }

    /**
     * @brief Set the Score object
     *
     * @param score
     */
    void setScore(const int score) { this->score = score; }

    /**
     * @brief Set the player's name
     *
     * @param playerName The player's name
     */
    void setPlayerName(std::string playerName) { this->playerName = std::move(playerName); }

    /**
     * @brief Get the player's name
     */
    const std::string &getPlayerName() const { return playerName; }

    /**
     * @brief Get the game speed
     */
    const double getGameSpeed() const { return gameSpeed; }

    /**
     * @brief Set the Message object
     *
     * @param message
     */
    void setMessage(const std::string &message);

    /**
     * @brief Returns whether or not the game is over
     *
     * @note The game is over when a crash point is set
     *
     * @return true if there is a crash point
     * @return false if crash point is still unset
     */
    const bool isGameOver() const;

    /**
     * @brief Gets a random vacant point in the board
     *
     * @return Point
     */
    const Point getRandomVacantPoint() const;

    /**
     * @brief Returns a string representation of the game
     *
     * @return std::string& The string of the game
     */
    const std::string &toString();

    /**
     * Returns a string representing the games score
     */
    const std::string getScoreString() { return playerName + ' ' + std::to_string(score) + '\n' + std::to_string(board->getWidth()) + 'x' + std::to_string(board->getHeight()) + ' ' + std::to_string(snake->getBody().size()) + ' ' + std::to_string(gameSpeed); }

    /**
     * Return score string header
     */
    const static std::string getScoreHeader() { return "Player Score\nDimensions Length Speed"; }

    /**
     * @brief Construct a new Game object
     *
     * @param board The new game's board
     * @param snake The new game's snake
     */
    Game(std::unique_ptr<Board> board, std::unique_ptr<Snake> snake, const double gameSpeed = 200.0, const std::string &playerName = "Captain") : board(std::move(board)), snake(std::move(snake)), apple(getRandomVacantPoint()), gameSpeed(gameSpeed), playerName(playerName) {}
};

#endif