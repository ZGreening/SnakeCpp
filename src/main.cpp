#include "config.hpp"
#include "menu_service.hpp"
#include "plog/Log.h"
#include <filesystem>
#include <memory>

using namespace std;

/**
 * @brief The main method of the program
 *
 * @return int The exit status code
 */
int main()
{
    try
    {
        SnakeConfig::init();
        PLOGI << "Starting Snake";
        auto menu_service(make_unique<MenuService>());
        menu_service->showMainMenuTask().wait();
        PLOGI << "Stopping Snake";
        return 0;
    }
    catch (const exception &exception)
    {
        PLOGF << "An unhandled exception occurred:\n"
              << exception.what();
        return -1;
    }
    catch (...)
    {
        PLOGF << "An unknown unhandled exception occurred";
        return -2;
    }
}
