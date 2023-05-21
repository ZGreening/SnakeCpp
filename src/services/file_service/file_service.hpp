#ifndef FILE_SERVICE_H
#define FILE_SERVICE_H

#include "game.hpp"
#include <vector>
#include <memory>

class FileService
{
private:
    static inline void updateSeparatorIndices(const std::string &line, size_t &left, size_t &right)
    {
        left = right + 1;
        right = line.find('-', left);
    }

public:
    /**
     * @brief Save the settings of the last game
     *
     * @param game The game to save the settings of
     * @throws std::invalid_argument
     */
    static void saveSettings(const Game &game);

    /**
     * @brief Saves the game score
     *
     * @param game The game to save
     * @throws std::invalid_argument
     */
    static void saveScore(const Game &game);

    /**
     * @brief Check if settings file exists
     */
    static bool hasSettingsFile();

    /**
     * @brief Load the last settings used
     *
     * @throws std::invalid_argument
     * @return
     */
    static std::unique_ptr<Game> loadSettings();

    /**
     * @brief Load the saved scores by page
     *
     * @throws std::out_of_range Thrown if page doesn't exist. scores will contain last existing page.
     * @throws std::invalid_argument Thrown if file doesn't exist. scores will be empty.
     * @return std::vector<Game> The saved game scores
     */
    static void loadScores(std::vector<Game> &scores, const int page = 0, const int numPerPage = 5);
};

#endif