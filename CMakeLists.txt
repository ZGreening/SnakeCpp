cmake_minimum_required(VERSION 3.23)
set(CMAKE_CXX_STANDARD 20)
cmake_policy(SET CMP0135 NEW)
include(FetchContent)

project(Snake)

if (UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif (UNIX)

add_executable(
    Snake 
    src/main.cpp 
    src/models/board/board.cpp 
    src/models/snake/snake.cpp 
    src/models/game/game.cpp
    src/models/direction/direction.cpp
    src/services/game_service/game_service.cpp
    src/services/menu_service/menu_service.cpp
    src/utility/utility.cpp
)

FetchContent_Declare(sfml URL https://www.sfml-dev.org/files/SFML-2.5.1-sources.zip URL_HASH SHA256=bf1e0643acb92369b24572b703473af60bac82caf5af61e77c063b779471bb7f)
set(SFML_BUILD_GRAPHICS FALSE)
set(SFML_BUILD_AUDIO FALSE)
set(SFML_BUILD_NETWORK FALSE)
FetchContent_MakeAvailable(sfml)
target_link_libraries(Snake sfml-window)

target_include_directories(
    Snake PUBLIC 
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/src/models/board" 
    "${PROJECT_SOURCE_DIR}/src/models/snake" 
    "${PROJECT_SOURCE_DIR}/src/models/direction" 
    "${PROJECT_SOURCE_DIR}/src/models/game" 
    "${PROJECT_SOURCE_DIR}/src/models/point" 
    "${PROJECT_SOURCE_DIR}/src/services/game_service"
    "${PROJECT_SOURCE_DIR}/src/services/menu_service"
    "${PROJECT_SOURCE_DIR}/src/utility"
    "${sfml_SOURCE_DIR}/include" 
)