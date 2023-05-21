#ifndef CONFIG_H
#define CONFIG_H

#include "plog/Init.h"
#include "plog/Formatters/TxtFormatter.h"
#include "plog/Appenders/RollingFileAppender.h"
#include <filesystem>
#include <string>
#include <memory>

// The global platform specific game folder path
#if defined(_WIN32)
const static auto GAME_DIRECTORY{std::string{getenv("HOMEDRIVE")} + std::string{getenv("HOMEPATH")} + "\\AppData\\Roaming\\SnakeCpp\\"};
#elif defined(__linux__)
const static auto GAME_DIRECTORY{std::string{getenv("HOME")} + "/.SnakeCpp/"};
#elif defined(__APPLE__)
const static auto GAME_DIRECTORY{std::string{getenv("HOME")} + "/Library/Application Support/SnakeCpp/"};
#else
const static auto GAME_DIRECTORY{std::string()};
#endif

// The global log file path
const static auto LOG_FILE{GAME_DIRECTORY + "logs/snake.log"};
static auto _appender{std::make_unique<plog::RollingFileAppender<plog::TxtFormatter>>(LOG_FILE.c_str(), 100'000'000, 5)};

namespace SnakeConfig
{
    /**
     * Creates game directories
     */
    static void init()
    {
        // Ensure game directory exists
        if (GAME_DIRECTORY.empty())
            throw "This OS is not supported. Unknown Game Directory";

        // Create folders if they don't exist
        if (!std::filesystem::is_directory(GAME_DIRECTORY))
            std::filesystem::create_directory(GAME_DIRECTORY);
        if (!std::filesystem::is_directory(GAME_DIRECTORY + "logs/"))
            std::filesystem::create_directory(GAME_DIRECTORY + "logs/");

        // Initialize logger
        plog::init(plog::info, _appender.get());
    }
}

#endif