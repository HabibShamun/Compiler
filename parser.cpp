#include "Parser.hpp"

#include <utility>

Parser::Parser(std::vector<Token> tokens) : tokens_(std::move(tokens)) {}

Program Parser::parseProgram() {
    Program program;

    while (!isAtEnd()) {
        advance();
    }

    return program;
}

const std::vector<std::string>& Parser::errors() const {
    return errors_;
}

const Token& Parser::current() const {
    return tokens_[pos_];
}

bool Parser::isAtEnd() const {
    return current().type == TokenType::EndOfFile;
}

const Token& Parser::advance() {
    if (!isAtEnd()) {
        ++pos_;
    }

    return tokens_[pos_ - 1];
}

