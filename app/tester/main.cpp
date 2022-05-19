#include "Application.hpp"
#include "Parameters.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
  try {
    Parameters par(argc, argv);
    Application app(par);
    app.run();
  } catch (std::exception const& e) {
    std::cerr << "FATAL: " << e.what() << "\n";
    return EXIT_FAILURE;
  }

  std::cerr << "exiting"
            << "\n";
  return EXIT_SUCCESS;
}
