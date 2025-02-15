#include <iostream>
#include <fstream>
#include "lexer.h"
#include "interpreter.h"

// Function to read the content of a file
std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return content;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename.ypp>\n";
        return 1;
    }

    std::string filename = argv[1];
    try {
        // Read the program from the file
        std::string input = readFile(filename);
        std::cout << "\033[1;33mLoaded program from " << filename << ":\033[0m\n";
        std::cout << "-----------------------------\n";
        std::cout << input << "\n";
        std::cout << "-----------------------------\n";

        // Tokenize and interpret the program
        Lexer lexer(input);
        Interpreter interpreter(lexer);
        interpreter.parse();
    } catch (const std::exception& e) {
        std::cerr << "\033[1;31mError: " << e.what() << "\033[0m\n";
        return 1;
    }

    return 0;
}