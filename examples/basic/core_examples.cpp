#include "../../include/DIPAL/Core/Core.hpp"

#include <iostream>

int main() {
    DIPAL::LibraryGuard guard;

    std::cout << "DIPAL Version: " << DIPAL::Core::getVersionString() << std::endl;
    std::cout << "Build Info: " << DIPAL::Core::getBuildInfo().toString() << std::endl;
    std::cout << "SSE2 Support: " << (DIPAL::Core::hasSSE2Support() ? "Yes" : "No") << std::endl;
    std::cout << "Optimal Threads: " << DIPAL::Core::getOptimalThreadCount() << std::endl;

    return 0;
}
