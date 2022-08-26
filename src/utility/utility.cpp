#include "utility.hpp"
#include <random>
#include <mutex>

std::mutex Utility::ioMutex{};
std::mt19937 Utility::random(static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count()));