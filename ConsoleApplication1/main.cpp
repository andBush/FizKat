#include <iostream>
#include <string>
#include <memory>

#include "Fleet.hpp"
#include "Commands.hpp"
#include "Errors.hpp"
#include "Things.hpp"

int main() {
    Fleet fleet;
    std::string line;

    while (std::getline(std::cin, line)) {
        try {
            if (!executeCommand(line, fleet)) {
                break;
            }
        } catch (const FleetError& error) {
            std::cout << "error: " << error.what() << '\n';
        }
    }

    return 0;
}
