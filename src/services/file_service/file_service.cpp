
#include "file_service.hpp"
#include "game.hpp"
#include "board.hpp"
#include "snake.hpp"
#include "config.hpp"
#include "plog/Log.h"
#include <string>
#include <memory>
#include <vector>
#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace std;

void FileService::saveSettings(const Game &game)
{
    // Open the file and check for fail
    PLOGI << "Saving settings";
    ofstream file(GAME_DIRECTORY + "settings.dat", ofstream::trunc);
    if (file.fail())
        throw invalid_argument("Failed to create save file at: " + GAME_DIRECTORY + "settings.dat");

    // Save the settings
    PLOGD << "Width: " << game.getBoard().getWidth() << endl
          << "Height: " << game.getBoard().getHeight() << endl
          << "Speed: " << game.getGameSpeed() << endl
          << "Snake Length: " << game.getSnake().getBody().size() << endl;
    file << game.getBoard().getWidth() << endl
         << game.getBoard().getHeight() << endl
         << game.getGameSpeed() << endl
         << game.getSnake().getBody().size() << endl;

    // Close the file
    file.close();
}

void FileService::saveScore(const Game &game)
{
    // Open the file and check for fail
    PLOGI << "Saving score";
    ofstream file(GAME_DIRECTORY + "scores.dat", ofstream::app);
    if (file.fail())
        throw invalid_argument("Failed to create save file at: " + GAME_DIRECTORY + "scores.dat");

    // Save the score
    PLOGD << "Player: " << game.getPlayerName() << endl
          << "Width: " << game.getBoard().getWidth() << endl
          << "Height: " << game.getBoard().getHeight() << endl
          << "Game Speed: " << game.getGameSpeed() << endl
          << "Snake Length: " << game.getSnake().getBody().size() << endl
          << "Score: " << game.getScore() << endl;
    file << game.getPlayerName() << '-'
         << game.getBoard().getWidth() << '-'
         << game.getBoard().getHeight() << '-'
         << game.getGameSpeed() << '-'
         << game.getSnake().getBody().size() << '-'
         << game.getScore() << endl;

    // Close the file
    file.close();
}

bool FileService::hasSettingsFile()
{
    PLOGI << "Checking for settings file";
    ifstream file(GAME_DIRECTORY + "settings.dat");
    bool toReturn = file.good();
    file.close();
    return toReturn;
}

unique_ptr<Game> FileService::loadSettings()
{
    // Open the settings file
    PLOGI << "Loading settings";
    ifstream file(GAME_DIRECTORY + "settings.dat");
    if (file.fail())
        throw invalid_argument("Failed to open save file at: " + GAME_DIRECTORY + "settings.dat");

    // Declare ptr to return
    PLOGD << "Parsing settings file";
    unique_ptr<Game> game;

    // Read the file into the object
    try
    {
        string line;
        getline(file, line);
        int boardWidth = stoi(line);
        getline(file, line);
        int boardHeight = stoi(line);
        getline(file, line);
        int gameSpeed = stoi(line);
        getline(file, line);
        int snakeLength = stoi(line);

        game = make_unique<Game>(make_unique<Board>(boardWidth, boardHeight), make_unique<Snake>(Point(snakeLength, boardHeight), snakeLength), gameSpeed);
    }
    catch (const invalid_argument &exception)
    {
        PLOGE << "An error occurred while parsing file " << GAME_DIRECTORY << "settings.dat" << exception.what();
        file.close();
        throw invalid_argument("Failed to open save file at: " + GAME_DIRECTORY + "settings.dat");
    }

    // Close the file
    PLOGD << "Successfully parsed";
    file.close();

    // Return the settings
    return game;
}

void FileService::loadScores(vector<Game> &scores, int page, const int numPerPage)
{
    // Clear vector for set of scores
    scores.clear();

    // Initialize current page
    auto currentPage{0};

    // Open the scores file
    ifstream file(GAME_DIRECTORY + "scores.dat");
    if (file.fail())
        throw invalid_argument("Failed to open save file at: " + GAME_DIRECTORY + "scores.dat");

    // Read scores from the file
    string line;
    bool readNewLine{getline(file, line)};
    while (readNewLine)
    {
        try
        {
            // Parse game
            size_t leftIndex{};
            size_t rightIndex{line.find('-')};
            string playerName = line.substr(leftIndex, rightIndex);
            updateSeparatorIndices(line, leftIndex, rightIndex);
            int boardWidth = stoi(line.substr(leftIndex, rightIndex - leftIndex));
            updateSeparatorIndices(line, leftIndex, rightIndex);
            int boardHeight = stoi(line.substr(leftIndex, rightIndex - leftIndex));
            updateSeparatorIndices(line, leftIndex, rightIndex);
            double gameSpeed = stoi(line.substr(leftIndex, rightIndex - leftIndex));
            updateSeparatorIndices(line, leftIndex, rightIndex);
            int snakeLength = stoi(line.substr(leftIndex, rightIndex - leftIndex));
            updateSeparatorIndices(line, leftIndex, rightIndex);
            int score = stoi(line.substr(leftIndex, rightIndex - leftIndex));

            // Create game and add it to the vector
            scores.push_back(Game(make_unique<Board>(boardWidth, boardHeight), make_unique<Snake>(Point(), snakeLength), gameSpeed, playerName));

            // Read next line
            readNewLine = static_cast<bool>(getline(file, line));

            // If there is another line to parse and the page
            if (scores.size() == numPerPage && readNewLine)
            {
                // If the page of games we've loaded belong to the requested page, break
                if (currentPage == page)
                    break;

                // Otherwise, increment current page number and clear data from the vector to return
                scores.clear();
                ++currentPage;
            }

            if (!readNewLine && currentPage != page)
            {
                throw out_of_range("Last page is " + to_string(currentPage));
            }
        }
        catch (const invalid_argument &exception)
        {
            PLOGE << "An error occurred while parsing one game score: " << exception.what();
            continue;
        }
    }

    // Close the file
    file.close();
}
