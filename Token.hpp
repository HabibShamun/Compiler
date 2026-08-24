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
    KwWhile
};