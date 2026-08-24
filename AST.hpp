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

struct Expr {
    explicit Expr(int line) : line(line) {}
    virtual ~Expr() = default;

    int line;
};

using ExprPtr = std::unique_ptr<Expr>;

struct NumberExpr final : Expr {
    NumberExpr(int value, int line) : Expr(line), value(value) {}

    int value;
};

struct StringExpr final : Expr {
    StringExpr(std::string value, int line) : Expr(line), value(std::move(value)) {}

    std::string value;
};

struct VariableExpr final : Expr {
    VariableExpr(std::string name, int line) : Expr(line), name(std::move(name)) {}

    std::string name;
};

struct UnaryExpr final : Expr {
    UnaryExpr(std::string op, ExprPtr expr, int line)
        : Expr(line), op(std::move(op)), expr(std::move(expr)) {}

    std::string op;
    ExprPtr expr;
};

struct BinaryExpr final : Expr {
    BinaryExpr(ExprPtr left, BinaryOp op, ExprPtr right, int line)
        : Expr(line), left(std::move(left)), op(op), right(std::move(right)) {}

    ExprPtr left;
    BinaryOp op;
    ExprPtr right;
};