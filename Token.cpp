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

    void skipWhitespace() {
        while (!isAtEnd() &&
               std::isspace(static_cast<unsigned char>(peek()))) {
            advance();
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

            advance();
        }
    }
};