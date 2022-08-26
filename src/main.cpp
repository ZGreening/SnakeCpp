#include "menu_service.hpp"
#include <memory>

/**
 * @brief The main method of the program
 *
 * @param argv The command line arguments count
 * @param args The command line arguments
 * @return int The exit status code
 */
int main()
{
    // TODO Add score saving system
    // TODO Add reload last settings
    std::unique_ptr<MenuService> menu_service(std::make_unique<MenuService>());
    menu_service->showMainMenuTask().wait();
    return 0;
}
