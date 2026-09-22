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

    if (const auto* varDecl = dynamic_cast<const VarDeclStmt*>(&stmt)) {
        out << "VarDecl: " << valueTypeName(varDecl->declaredType) << ' ' << varDecl->name << '\n';
        if (varDecl->initializer) {
            printExpr(*varDecl->initializer, out, depth + 1);
        }
        return;
    }

    if (const auto* assign = dynamic_cast<const AssignStmt*>(&stmt)) {
        out << "Assign: " << assign->name << '\n';
        printExpr(*assign->value, out, depth + 1);
        return;
    }

    if (const auto* print = dynamic_cast<const PrintStmt*>(&stmt)) {
        out << "Print\n";
        printExpr(*print->value, out, depth + 1);
        return;
    }

    if (const auto* block = dynamic_cast<const BlockStmt*>(&stmt)) {
        out << "Block\n";
        for (const auto& child : block->statements) {
            printStmt(*child, out, depth + 1);
        }
        return;
    }

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

    if (const auto* varDecl = dynamic_cast<const VarDeclStmt*>(&stmt)) {
        out << "VarDecl: " << valueTypeName(varDecl->declaredType) << ' ' << varDecl->name << '\n';
        if (varDecl->initializer) {
            printExpr(*varDecl->initializer, out, depth + 1);
        }
        return;
    }

    if (const auto* assign = dynamic_cast<const AssignStmt*>(&stmt)) {
        out << "Assign: " << assign->name << '\n';
        printExpr(*assign->value, out, depth + 1);
        return;
    }

    if (const auto* print = dynamic_cast<const PrintStmt*>(&stmt)) {
        out << "Print\n";
        printExpr(*print->value, out, depth + 1);
        return;
    }

    if (const auto* block = dynamic_cast<const BlockStmt*>(&stmt)) {
        out << "Block\n";
        for (const auto& child : block->statements) {
            printStmt(*child, out, depth + 1);
        }
        return;
    }

    if (const auto* ifStmt = dynamic_cast<const IfStmt*>(&stmt)) {
        out << "If\n";
        indent(out, depth + 1);
        out << "Condition\n";
        printExpr(*ifStmt->condition, out, depth + 2);
        indent(out, depth + 1);
        out << "Then\n";
        printStmt(*ifStmt->thenBranch, out, depth + 2);
        if (ifStmt->elseBranch) {
            indent(out, depth + 1);
            out << "Else\n";
            printStmt(*ifStmt->elseBranch, out, depth + 2);
        }
        return;
    }

    if (const auto* whileStmt = dynamic_cast<const WhileStmt*>(&stmt)) {
        out << "While\n";
        indent(out, depth + 1);
        out << "Condition\n";
        printExpr(*whileStmt->condition, out, depth + 2);
        indent(out, depth + 1);
        out << "Body\n";
        printStmt(*whileStmt->body, out, depth + 2);
        return;
    }

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
    out << "Name              Type       Line   Scope\n";
    out << "-----------------------------------------\n";
    for (const Symbol& symbol : symbols) {
        out << std::left << std::setw(17) << symbol.name
            << std::setw(11) << valueTypeName(symbol.type)
            << std::setw(7) << symbol.declaredLine
            << symbol.scopeDepth << '\n';
    }
}