#pragma once

#include <string>

enum class TokenType {
    Number,
    String,
    Identifier,

    KwInt,
    KwString,
    KwPrint,
    KwIf,
    KwElse,
    KwWhile,

    Plus,
    Minus,
    Star,
    Slash,
    Equal,
    EqualEqual,
    BangEqual,
    Less,
    LessEqual,
    Greater,
    GreaterEqual,

    LParen,
    RParen,
    LBrace,
    RBrace,
    Semicolon,
    Newline,
    EndOfFile,
    Unknown
};

struct Token {
    TokenType type;
    std::string lexeme;
    int line;
    int column;
};

const char* tokenTypeName(TokenType type);