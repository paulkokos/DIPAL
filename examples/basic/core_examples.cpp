#include "../../include/DIPAL/Core/Core.hpp"

#include <iostream>

int main() {
  DIPAL::LibraryGuard guard;

  std::cout << "DIPAL Version: " << DIPAL::Core::getVersionString() << '\n';
  std::cout << "Build Info: " << DIPAL::Core::getBuildInfo().toString() << '\n';
  std::cout << "SSE2 Support: "
            << (DIPAL::Core::hasSSE2Support() ? "Yes" : "No") << '\n';
  std::cout << "Optimal Threads: " << DIPAL::Core::getOptimalThreadCount()
            << '\n';

  return 0;
}
