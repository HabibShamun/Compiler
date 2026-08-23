#include "Parser.hpp"

#include <sstream>
#include <utility>

Parser::Parser(std::vector<Token> tokens) : tokens_(std::move(tokens)) {}

Program Parser::parseProgram() {
    Program program;

    while (!isAtEnd()) {
        consumeTerminators();
        if (isAtEnd()) {
            break;
        }

        try {
            if (auto stmt = parseDeclarationOrStatement()) {
                program.statements.push_back(std::move(stmt));
            }
        } catch (const ParseError&) {
            synchronize();
        }
    }

    return program;
}

const std::vector<std::string>& Parser::errors() const {
    return errors_;
}

const Token& Parser::current() const {
    return tokens_[pos_];
}

const Token& Parser::previous() const {
    return tokens_[pos_ - 1];
}

bool Parser::isAtEnd() const {
    return current().type == TokenType::EndOfFile;
}

bool Parser::check(TokenType type) const {
    return !isAtEnd() && current().type == type;
}

bool Parser::match(TokenType type) {
    if (!check(type)) {
        return false;
    }

    advance();
    return true;
}

bool Parser::matchAny(std::initializer_list<TokenType> types) {
    for (const TokenType type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }

    return false;
}

const Token& Parser::advance() {
    if (!isAtEnd()) {
        ++pos_;
    }

    return previous();
}

const Token& Parser::expect(TokenType type, const std::string& message) {
    if (check(type)) {
        return advance();
    }

    errorAt(current(), message);
    throw ParseError(message);
}

void Parser::errorAt(const Token& token, const std::string& message) {
    std::ostringstream out;
    out << "Line " << token.line << ", column " << token.column << ": "
        << message << " near '" << token.lexeme << "'";
    errors_.push_back(out.str());
}

void Parser::synchronize() {
    while (!isAtEnd()) {
        if (matchAny({TokenType::Semicolon, TokenType::Newline})) {
            return;
        }
        advance();
    }
}

void Parser::consumeTerminators() {
    while (matchAny({TokenType::Semicolon, TokenType::Newline})) {
    }
}

void Parser::consumeStatementEnd(const std::string& context) {
    if (matchAny({TokenType::Semicolon, TokenType::Newline})) {
        consumeTerminators();
        return;
    }

    if (check(TokenType::EndOfFile)) {
        return;
    }

    errorAt(current(), "Expected end of line or ';' after " + context);
    throw ParseError("missing statement terminator");
}

StmtPtr Parser::parseDeclarationOrStatement() {
    if (isTypeToken(current().type)) {
        return parseVarDeclaration();
    }

    return parseStatement();
}

StmtPtr Parser::parseVarDeclaration() {
    const Token typeToken = advance();
    const ValueType declaredType = typeFromToken(typeToken.type);
    const Token name = expect(TokenType::Identifier, "Expected variable name after type keyword");

    ExprPtr initializer;
    if (match(TokenType::Equal)) {
        initializer = parseExpression();
    }

    consumeStatementEnd("variable declaration");
    return std::make_unique<VarDeclStmt>(declaredType, name.lexeme, std::move(initializer), typeToken.line);
}

StmtPtr Parser::parseStatement() {
    if (check(TokenType::Identifier)) {
        return parseAssignment();
    }

    if (check(TokenType::KwPrint)) {
        return parsePrint();
    }

    errorAt(current(), "Expected declaration, assignment, or print statement");
    throw ParseError("expected simple statement");
}

StmtPtr Parser::parseAssignment() {
    const Token name = advance();
    expect(TokenType::Equal, "Expected '=' after variable name");
    ExprPtr value = parseExpression();
    consumeStatementEnd("assignment");
    return std::make_unique<AssignStmt>(name.lexeme, std::move(value), name.line);
}

StmtPtr Parser::parsePrint() {
    const Token keyword = advance();
    ExprPtr value = parseExpression();
    consumeStatementEnd("print statement");
    return std::make_unique<PrintStmt>(std::move(value), keyword.line);
}

ExprPtr Parser::parseExpression() {
    return parseEquality();
}

ExprPtr Parser::parseEquality() {
    ExprPtr expr = parseComparison();

    while (matchAny({TokenType::EqualEqual, TokenType::BangEqual})) {
        const Token op = previous();
        ExprPtr right = parseComparison();
        expr = std::make_unique<BinaryExpr>(std::move(expr), binaryOpFromToken(op.type),
                                            std::move(right), op.line);
    }

    return expr;
}

ExprPtr Parser::parseComparison() {
    ExprPtr expr = parseTerm();

    while (matchAny({TokenType::Less, TokenType::LessEqual, TokenType::Greater, TokenType::GreaterEqual})) {
        const Token op = previous();
        ExprPtr right = parseTerm();
        expr = std::make_unique<BinaryExpr>(std::move(expr), binaryOpFromToken(op.type),
                                            std::move(right), op.line);
    }

    return expr;
}

ExprPtr Parser::parseTerm() {
    ExprPtr expr = parseFactor();

    while (matchAny({TokenType::Plus, TokenType::Minus})) {
        const Token op = previous();
        ExprPtr right = parseFactor();
        expr = std::make_unique<BinaryExpr>(std::move(expr), binaryOpFromToken(op.type),
                                            std::move(right), op.line);
    }

    return expr;
}

ExprPtr Parser::parseFactor() {
    ExprPtr expr = parseUnary();

    while (matchAny({TokenType::Star, TokenType::Slash})) {
        const Token op = previous();
        ExprPtr right = parseUnary();
        expr = std::make_unique<BinaryExpr>(std::move(expr), binaryOpFromToken(op.type),
                                            std::move(right), op.line);
    }

    return expr;
}

ExprPtr Parser::parseUnary() {
    if (match(TokenType::Minus)) {
        const Token op = previous();
        return std::make_unique<UnaryExpr>("-", parseUnary(), op.line);
    }

    return parsePrimary();
}

ExprPtr Parser::parsePrimary() {
    if (match(TokenType::Number)) {
        const Token token = previous();
        return std::make_unique<NumberExpr>(std::stoi(token.lexeme), token.line);
    }

    if (match(TokenType::String)) {
        const Token token = previous();
        return std::make_unique<StringExpr>(token.lexeme, token.line);
    }

    if (match(TokenType::Identifier)) {
        const Token token = previous();
        return std::make_unique<VariableExpr>(token.lexeme, token.line);
    }

    if (match(TokenType::LParen)) {
        ExprPtr expr = parseExpression();
        expect(TokenType::RParen, "Expected ')' after expression");
        return expr;
    }

    errorAt(current(), "Expected expression");
    throw ParseError("expected expression");
}

bool Parser::isTypeToken(TokenType type) {
    return type == TokenType::KwInt || type == TokenType::KwString;
}

ValueType Parser::typeFromToken(TokenType type) {
    if (type == TokenType::KwInt) {
        return ValueType::Int;
    }

    if (type == TokenType::KwString) {
        return ValueType::String;
    }

    return ValueType::Unknown;
}

BinaryOp Parser::binaryOpFromToken(TokenType type) {
    switch (type) {
        case TokenType::Plus: return BinaryOp::Add;
        case TokenType::Minus: return BinaryOp::Subtract;
        case TokenType::Star: return BinaryOp::Multiply;
        case TokenType::Slash: return BinaryOp::Divide;
        case TokenType::EqualEqual: return BinaryOp::Equal;
        case TokenType::BangEqual: return BinaryOp::NotEqual;
        case TokenType::Less: return BinaryOp::Less;
        case TokenType::LessEqual: return BinaryOp::LessEqual;
        case TokenType::Greater: return BinaryOp::Greater;
        case TokenType::GreaterEqual: return BinaryOp::GreaterEqual;
        default: return BinaryOp::Equal;
    }
}

