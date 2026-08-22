#pragma once

#include "AST.hpp"
#include "Token.hpp"

#include <string>
#include <vector>

class Parser {
public:
    explicit Parser(std::vector<Token> tokens);

    Program parseProgram();
    const std::vector<std::string>& errors() const;

private:
    std::vector<Token> tokens_;
    std::size_t pos_ = 0;
    std::vector<std::string> errors_;

    const Token& current() const;
    bool isAtEnd() const;
    const Token& advance();
};

