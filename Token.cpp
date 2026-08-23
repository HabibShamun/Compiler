#include "Token.hpp"
#include <string>
#include <iostream>
#include <cctype>

class Lexer {
private:
    std::string source;
    size_t current;
    bool hadError;

    bool isAtEnd() const {
        return current >= source.length();
    }

    char advance() {
        return source[current++];
    }

    char peek() const {
        if (isAtEnd()) {
            return '\0';
        }

        return source[current];
    }

    void skipWhitespace() {
        while (!isAtEnd()) {
            char c = peek();

            if (c == ' ' || c == '\r' || c == '\t') {
                advance();
            } else {
                break;
            }
        }
    }

    void reportError(const std::string& message) {
        std::cerr << "Lexer Error at position "
                  << current << ": "
                  << message << std::endl;

        hadError = true;
    }

    void scanNumber() {
        while (std::isdigit(static_cast<unsigned char>(peek()))) {
            advance();
        }
    }

    void scanIdentifier() {
        while (std::isalnum(static_cast<unsigned char>(peek())) ||
               peek() == '_') {
            advance();
        }
    }

    void scanString() {
        while (!isAtEnd() && peek() != '"') {
            advance();
        }

        if (isAtEnd()) {
            reportError("Unterminated string.");
            return;
        }

        advance();
    }

    void scanToken() {
        char c = advance();

        if (std::isdigit(static_cast<unsigned char>(c))) {
            scanNumber();
        }
        else if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
            scanIdentifier();
        }
        else if (c == '"') {
            scanString();
        }
        else if (c == '+' || c == '-' ||
                 c == '*' || c == '/' ||
                 c == '=' || c == '<' ||
                 c == '>' || c == '(' ||
                 c == ')' || c == '{' ||
                 c == '}' || c == ';') {
            // Recognized character.
        }
        else {
            reportError("Unexpected character.");
        }
    }

public:
    Lexer(const std::string& source)
        : source(source), current(0), hadError(false) {
    }

    void scan() {
        while (!isAtEnd()) {
            skipWhitespace();

            if (isAtEnd()) {
                break;
            }

            scanToken();
        }
    }

    bool hasError() const {
        return hadError;
    }
};

int main() {
    std::string source =
        "int number = 123;\n"
        "print \"Hello\";";

    Lexer lexer(source);
    lexer.scan();

    if (lexer.hasError()) {
        std::cout << "Lexical analysis completed with errors."
                  << std::endl;
    } else {
        std::cout << "Lexical analysis completed successfully."
                  << std::endl;
    }

    return 0;
}