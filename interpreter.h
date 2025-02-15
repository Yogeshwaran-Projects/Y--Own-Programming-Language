#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "lexer.h"
#include <iostream>
#include <stdexcept>

// Interpreter class to execute the program
class Interpreter {
public:
    Interpreter(Lexer& lexer) : lexer(lexer), currentToken(lexer.getNextToken()) {}

    void parse() {
        std::cout << "\033[1;32mRunning program...\033[0m\n";
        while (currentToken.type != TOKEN_EOF) {
            if (currentToken.type == TOKEN_SAYIT) {
                handleSayit();
            } else {
                throw std::runtime_error("Line " + std::to_string(currentToken.line) + ": Unexpected token '" + currentToken.value + "'");
            }
        }
        std::cout << "\033[1;32mProgram finished.\033[0m\n";
    }

private:
    Lexer& lexer;
    Token currentToken;

    void handleSayit() {
        eat(TOKEN_SAYIT);
        std::string output;

        // Handle mixed strings and expressions
        while (currentToken.type != TOKEN_EOF && currentToken.type != TOKEN_SAYIT) {
            if (currentToken.type == TOKEN_STRING) {
                output += currentToken.value;
                eat(TOKEN_STRING);
            } else if (currentToken.type == TOKEN_NUMBER) {
                output += std::to_string(expr()); // Evaluate expression and append to output
            } else {
                throw std::runtime_error("Line " + std::to_string(currentToken.line) + ": Unexpected token '" + currentToken.value + "'");
            }
        }

        std::cout << output << std::endl;
    }

    void eat(TokenType type) {
        if (currentToken.type == type) {
            currentToken = lexer.getNextToken();
        } else {
            throw std::runtime_error("Line " + std::to_string(currentToken.line) + ": Expected token type " + std::to_string(type) + ", got '" + currentToken.value + "'");
        }
    }

    int expr() {
        int result = term();

        while (currentToken.type == TOKEN_SUMTHING || currentToken.type == TOKEN_BYEBYE) {
            if (currentToken.type == TOKEN_SUMTHING) {
                eat(TOKEN_SUMTHING);
                result += term();
            } else if (currentToken.type == TOKEN_BYEBYE) {
                eat(TOKEN_BYEBYE);
                result -= term();
            }
        }

        return result;
    }

    int term() {
        int result = factor();

        while (currentToken.type == TOKEN_LOTSOF || currentToken.type == TOKEN_CUTIT) {
            if (currentToken.type == TOKEN_LOTSOF) {
                eat(TOKEN_LOTSOF);
                result *= factor();
            } else if (currentToken.type == TOKEN_CUTIT) {
                eat(TOKEN_CUTIT);
                int divisor = factor();
                if (divisor == 0) throw std::runtime_error("Line " + std::to_string(currentToken.line) + ": Division by zero");
                result /= divisor;
            }
        }

        return result;
    }

    int factor() {
        Token token = currentToken;
        if (token.type == TOKEN_NUMBER) {
            eat(TOKEN_NUMBER);
            return std::stoi(token.value);
        } else {
            throw std::runtime_error("Line " + std::to_string(token.line) + ": Unexpected token '" + token.value + "'");
        }
    }
};

#endif