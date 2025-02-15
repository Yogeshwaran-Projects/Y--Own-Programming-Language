#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <stdexcept>
#include <cctype>

// Token types
enum TokenType {
    TOKEN_SUMTHING,  // +
    TOKEN_BYEBYE,    // -
    TOKEN_LOTSOF,    // *
    TOKEN_CUTIT,     // /
    TOKEN_SAYIT,     // Print
    TOKEN_NUMBER,    // Number
    TOKEN_STRING,    // String literal
    TOKEN_EOF        // End of input
};

// Token structure
struct Token {
    TokenType type;
    std::string value;
    int line; // Line number for error reporting
};

// Lexer class to tokenize input
class Lexer {
public:
    Lexer(const std::string& input) : input(input), position(0), line(1) {}

    Token getNextToken() {
        while (position < input.size()) {
            char currentChar = input[position];

            // Skip whitespace
            if (isspace(currentChar)) {
                if (currentChar == '\n') line++;
                position++;
                continue;
            }

            // String literals
            if (currentChar == '"') {
                position++; // Skip the opening quote
                std::string str = readString();
                return {TOKEN_STRING, str, line};
            }

            // Keywords
            if (input.substr(position, 5) == "SAYIT") {
                position += 5;
                return {TOKEN_SAYIT, "SAYIT", line};
            }
            if (input.substr(position, 8) == "SUMTHING") {
                position += 8;
                return {TOKEN_SUMTHING, "SUMTHING", line};
            }
            if (input.substr(position, 6) == "BYEBYE") {
                position += 6;
                return {TOKEN_BYEBYE, "BYEBYE", line};
            }
            if (input.substr(position, 6) == "LOTSOF") {
                position += 6;
                return {TOKEN_LOTSOF, "LOTSOF", line};
            }
            if (input.substr(position, 5) == "CUTIT") {
                position += 5;
                return {TOKEN_CUTIT, "CUTIT", line};
            }

            // Numbers
            if (isdigit(currentChar)) {
                return {TOKEN_NUMBER, readNumber(), line};
            }

            // Unknown character
            throw std::runtime_error("Line " + std::to_string(line) + ": Unknown character '" + std::string(1, currentChar) + "'");
        }

        // End of input
        return {TOKEN_EOF, "", line};
    }

private:
    std::string input;
    size_t position;
    int line;

    std::string readNumber() {
        std::string number;
        while (position < input.size() && isdigit(input[position])) {
            number += input[position++];
        }
        return number;
    }

    std::string readString() {
        std::string str;
        while (position < input.size() && input[position] != '"') {
            str += input[position++];
        }
        if (position >= input.size()) {
            throw std::runtime_error("Line " + std::to_string(line) + ": Unterminated string literal");
        }
        position++; // Skip the closing quote
        return str;
    }
};

#endif