#pragma once
#include "Token.hpp"
#include <string>
#include <vector>

class Lexer {
public:
    explicit Lexer(std::string source);
    std::vector<Token> tokenize();
    const std::vector<std::string>& errors() const;

private:
    std::string source_;
    std::size_t pos_ = 0;
    int line_ = 1;
    int column_ = 1;
    std::vector<Token> tokens_;
    std::vector<std::string> errors_;

    bool isAtEnd() const;
    char current() const;
    char peek() const;
    char advance();
    bool match(char expected);

    void addToken(TokenType type, const std::string& lexeme, int line, int column);
    void addError(const std::string& message, int line, int column);
};