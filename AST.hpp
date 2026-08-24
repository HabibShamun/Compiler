#pragma once

#include <memory>
#include <string>
#include <vector>

enum class ValueType {
    Int,
    String,
    Bool,
    Unknown
};

inline const char* valueTypeName(ValueType type) {
    switch (type) {
        case ValueType::Int: return "shonkha";
        case ValueType::String: return "lekha";
        case ValueType::Bool: return "bool";
        case ValueType::Unknown: return "unknown";
    }

    return "unknown";
}

enum class BinaryOp {
    Add,
    Subtract,
    Multiply,
    Divide,
    Equal,
    NotEqual,
    Less,
    LessEqual,
    Greater,
    GreaterEqual
};

inline const char* binaryOpName(BinaryOp op) {
    switch (op) {
        case BinaryOp::Add: return "+";
        case BinaryOp::Subtract: return "-";
        case BinaryOp::Multiply: return "*";
        case BinaryOp::Divide: return "/";
        case BinaryOp::Equal: return "==";
        case BinaryOp::NotEqual: return "!=";
        case BinaryOp::Less: return "<";
        case BinaryOp::LessEqual: return "<=";
        case BinaryOp::Greater: return ">";
        case BinaryOp::GreaterEqual: return ">=";
    }

    return "?";
}