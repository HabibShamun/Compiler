#include "Token.hpp"

const char* tokenTypeName(TokenType type) {
    switch (type) {
        case TokenType::Number: return "NUMBER";
        case TokenType::String: return "STRING";
        case TokenType::Identifier: return "IDENTIFIER";

        case TokenType::KwInt: return "KW_INT";
        case TokenType::KwString: return "KW_STRING";
        case TokenType::KwPrint: return "KW_PRINT";
        case TokenType::KwIf: return "KW_IF";
        case TokenType::KwElse: return "KW_ELSE";
        case TokenType::KwWhile: return "KW_WHILE";

        case TokenType::Plus: return "PLUS";
        case TokenType::Minus: return "MINUS";
        case TokenType::Star: return "STAR";
        case TokenType::Slash: return "SLASH";

        case TokenType::Equal: return "EQUAL";
        case TokenType::EqualEqual: return "EQUAL_EQUAL";
        case TokenType::BangEqual: return "BANG_EQUAL";

        case TokenType::Less: return "LESS";
        case TokenType::LessEqual: return "LESS_EQUAL";
        case TokenType::Greater: return "GREATER";
        case TokenType::GreaterEqual: return "GREATER_EQUAL";

        case TokenType::LParen: return "LPAREN";
        case TokenType::RParen: return "RPAREN";
        case TokenType::LBrace: return "LBRACE";
        case TokenType::RBrace: return "RBRACE";

        case TokenType::Semicolon: return "SEMICOLON";
        case TokenType::Newline: return "NEWLINE";
        case TokenType::EndOfFile: return "EOF";
        case TokenType::Unknown: return "UNKNOWN";
    }

    return "UNKNOWN";
}