#include "Application.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
  try {
    cbm::Application cbm(argc, argv);
  } catch (std::exception const& e) {
    std::cerr << "FATAL: " << e.what() << "\n";
    return EXIT_FAILURE;
  }

  // do something
  return EXIT_SUCCESS;
}
