#ifndef UTILITY_H
#define UTILITY_H

#include "SFML/Window.hpp"
#include <thread>
#include <chrono>
#include <string_view>
#include <iostream>
#include <random>
#include <mutex>

class Utility
{
private:
    /**
     * @brief The snake random number generator
     *
     */
    static std::mt19937 random;

    /**
     * @brief The snake io mutex for synchronizing io operations
     *
     */
    static std::mutex ioMutex;

public:
    /**
     * @brief Converts the SFML alphanumeric key to it's corresponding lowercase char
     *
     * @param key The key to convert
     */
    static char sfKeyToChar(sf::Keyboard::Key key);

    /**
     * @brief Get the Io Mutex object
     *
     * @return std::mutex&
     */
    static std::mutex &getIoMutex() { return Utility::ioMutex; }

    /**
     * @brief Get the Random object
     *
     * @return std::mt19937&
     */
    static std::mt19937 &getRandom() { return Utility::random; }

    /**
     * @brief Pauses the current thread for the given number of milliseconds
     *
     * @param timeInMilliseconds The number of milliseconds
     */
    static void pauseThread(int timeInMilliseconds)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(timeInMilliseconds));
    }

    /**
     * @brief Prints the string to the standard stream in a thread safe manner
     *
     * @param string The string to print
     * @param clearScreen Whether or not to clear the console when printing the message
     */
    static void printSafe(std::string_view string, bool clearScreen = false)
    {
        // Get lock for printing
        const std::lock_guard<std::mutex> lock(Utility::getIoMutex());

        // Clear screen using system commands
        if (clearScreen)
        {
#if defined(_WIN32)
            system("cls");
#elif defined(__linux__) || defined(__APPLE__)
            system("clear");
#endif
        }

        // Print the passed string
        std::cout << string << std::endl;
    }
};

#endif