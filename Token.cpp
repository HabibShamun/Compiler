#include "Token.hpp"
#include <string>
#include <cctype>

class Lexer {
private:
    std::string source;
    size_t current;

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

    char peekNext() const {
        if (current + 1 >= source.length()) {
            return '\0';
        }

        return source[current + 1];
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

        if (!isAtEnd()) {
            advance();
        }
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
    }

public:
    Lexer(const std::string& source)
        : source(source), current(0) {
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
};