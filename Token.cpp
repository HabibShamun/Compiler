#ifndef TOKEN_HPP
#define TOKEN_HPP

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

#endif