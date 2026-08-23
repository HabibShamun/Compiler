#include "PrettyPrinter.hpp"

#include <iomanip>
#include <ostream>

namespace {

void indent(std::ostream& out, int depth) {
    for (int i = 0; i < depth; ++i) {
        out << "  ";
    }
}

}

void printTokens(const std::vector<Token>& tokens, std::ostream& out) {
}

void printAst(const Program& program, std::ostream& out) {
}

void printSymbols(const std::vector<Symbol>& symbols, std::ostream& out) {
}#include "PrettyPrinter.hpp"

#include <iomanip>
#include <ostream>

namespace {

void indent(std::ostream& out, int depth) {
    for (int i = 0; i < depth; ++i) {
        out << "  ";
    }
}

}

void printTokens(const std::vector<Token>& tokens, std::ostream& out) {
    for (const Token& token : tokens) {
        out << std::setw(14) << tokenTypeName(token.type)
            << "  " << std::setw(12) << std::quoted(token.lexeme)
            << "  line " << token.line
            << ", col " << token.column << '\n';
    }
}

void printAst(const Program& program, std::ostream& out) {
}

void printSymbols(const std::vector<Symbol>& symbols, std::ostream& out) {
}#include "PrettyPrinter.hpp"

#include <iomanip>
#include <ostream>

namespace {

void indent(std::ostream& out, int depth) {
    for (int i = 0; i < depth; ++i) {
        out << "  ";
    }
}

void printExpr(const Expr& expr, std::ostream& out, int depth) {
    indent(out, depth);

    if (const auto* number = dynamic_cast<const NumberExpr*>(&expr)) {
        out << "Number: " << number->value << '\n';
        return;
    }

    if (const auto* string = dynamic_cast<const StringExpr*>(&expr)) {
        out << "String: " << std::quoted(string->value) << '\n';
        return;
    }

    if (const auto* variable = dynamic_cast<const VariableExpr*>(&expr)) {
        out << "Variable: " << variable->name << '\n';
        return;
    }

    out << "UnknownExpr\n";
}

}

void printTokens(const std::vector<Token>& tokens, std::ostream& out) {
    for (const Token& token : tokens) {
        out << std::setw(14) << tokenTypeName(token.type)
            << "  " << std::setw(12) << std::quoted(token.lexeme)
            << "  line " << token.line
            << ", col " << token.column << '\n';
    }
}

void printAst(const Program& program, std::ostream& out) {
}

void printSymbols(const std::vector<Symbol>& symbols, std::ostream& out) {
}#include "PrettyPrinter.hpp"

#include <iomanip>
#include <ostream>

namespace {

void indent(std::ostream& out, int depth) {
    for (int i = 0; i < depth; ++i) {
        out << "  ";
    }
}

void printExpr(const Expr& expr, std::ostream& out, int depth) {
    indent(out, depth);

    if (const auto* number = dynamic_cast<const NumberExpr*>(&expr)) {
        out << "Number: " << number->value << '\n';
        return;
    }

    if (const auto* string = dynamic_cast<const StringExpr*>(&expr)) {
        out << "String: " << std::quoted(string->value) << '\n';
        return;
    }

    if (const auto* variable = dynamic_cast<const VariableExpr*>(&expr)) {
        out << "Variable: " << variable->name << '\n';
        return;
    }

    if (const auto* unary = dynamic_cast<const UnaryExpr*>(&expr)) {
        out << "Unary: " << unary->op << '\n';
        printExpr(*unary->expr, out, depth + 1);
        return;
    }

    if (const auto* binary = dynamic_cast<const BinaryExpr*>(&expr)) {
        out << "Binary: " << binaryOpName(binary->op) << '\n';
        printExpr(*binary->left, out, depth + 1);
        printExpr(*binary->right, out, depth + 1);
        return;
    }

    out << "UnknownExpr\n";
}

void printStmt(const Stmt& stmt, std::ostream& out, int depth) {
    indent(out, depth);
    out << "UnknownStmt\n";
}

}

void printTokens(const std::vector<Token>& tokens, std::ostream& out) {
    for (const Token& token : tokens) {
        out << std::setw(14) << tokenTypeName(token.type)
            << "  " << std::setw(12) << std::quoted(token.lexeme)
            << "  line " << token.line
            << ", col " << token.column << '\n';
    }
}

void printAst(const Program& program, std::ostream& out) {
    out << "Program\n";
    for (const auto& stmt : program.statements) {
        printStmt(*stmt, out, 1);
    }
}

void printSymbols(const std::vector<Symbol>& symbols, std::ostream& out) {
}