#pragma once

#include "AST.hpp"
#include "Token.hpp"

#include <stdexcept>
#include <string>
#include <vector>

class Parser {
public:
    explicit Parser(std::vector<Token> tokens);

    Program parseProgram();
    const std::vector<std::string>& errors() const;

private:
    struct ParseError : std::runtime_error {
        using std::runtime_error::runtime_error;
    };

    std::vector<Token> tokens_;
    std::size_t pos_ = 0;
    std::vector<std::string> errors_;

    const Token& current() const;
    const Token& previous() const;
    bool isAtEnd() const;
    bool check(TokenType type) const;
    bool match(TokenType type);
    const Token& advance();
    const Token& expect(TokenType type, const std::string& message);

    void errorAt(const Token& token, const std::string& message);
    void synchronize();
    void consumeTerminators();
    void consumeStatementEnd(const std::string& context);

    StmtPtr parseDeclarationOrStatement();
    StmtPtr parseVarDeclaration();
    StmtPtr parseStatement();
    StmtPtr parseAssignment();
    StmtPtr parsePrint();

    ExprPtr parseExpression();
    ExprPtr parsePrimary();

    static bool isTypeToken(TokenType type);
    static ValueType typeFromToken(TokenType type);
};

