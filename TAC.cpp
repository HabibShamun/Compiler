#include "TAC.hpp"

#include <iomanip>
#include <ostream>
#include <sstream>
#include <stdexcept>

TACBinOp::TACBinOp(std::string dest, std::string left, std::string op, std::string right)
    : dest(std::move(dest)), left(std::move(left)), op(std::move(op)), right(std::move(right)) {}

void TACBinOp::print(std::ostream& out) const { out << dest << " = " << left << ' ' << op << ' ' << right; }
const char* TACBinOp::kind() const { return "BinOp"; }

TACCopy::TACCopy(std::string dest, std::string source)
    : dest(std::move(dest)), source(std::move(source)) {}

void TACCopy::print(std::ostream& out) const { out << dest << " = " << source; }
const char* TACCopy::kind() const { return "Copy"; }

TACPrint::TACPrint(std::string source) : source(std::move(source)) {}

void TACPrint::print(std::ostream& out) const { out << "print " << source; }
const char* TACPrint::kind() const { return "Print"; }

TACLabel::TACLabel(std::string name) : name(std::move(name)) {}

void TACLabel::print(std::ostream& out) const { out << name << ':'; }
const char* TACLabel::kind() const { return "Label"; }

TACJump::TACJump(std::string target) : target(std::move(target)) {}

void TACJump::print(std::ostream& out) const { out << "goto " << target; }
const char* TACJump::kind() const { return "Jump"; }

TACBranch::TACBranch(std::string condition, std::string target)
    : condition(std::move(condition)), target(std::move(target)) {}

void TACBranch::print(std::ostream& out) const { out << "if " << condition << " goto " << target; }
const char* TACBranch::kind() const { return "Branch"; }

const std::vector<TACInstrPtr>& TACGenerator::generate(const Program& program) {
    instructions_.clear();
    tempCount_ = 0;
    labelCount_ = 0;
    for (const auto& statement : program.statements) {
        generateStmt(*statement);
    }
    return instructions_;
}

const std::vector<TACInstrPtr>& TACGenerator::instructions() const { return instructions_; }

std::string TACGenerator::newTemp() {
    return "t" + std::to_string(tempCount_++);
}

std::string TACGenerator::newLabel(const char* prefix) {
    return std::string(prefix) + std::to_string(labelCount_++);
}

void TACGenerator::emit(TACInstrPtr instruction) {
    instructions_.push_back(std::move(instruction));
}

void TACGenerator::generateStmt(const Stmt& stmt) {
    if (const auto* declaration = dynamic_cast<const VarDeclStmt*>(&stmt)) {
        if (declaration->initializer) {
            emit(std::make_unique<TACCopy>(declaration->name, generateExpr(*declaration->initializer)));
        }
        return;
    }

    if (const auto* assignment = dynamic_cast<const AssignStmt*>(&stmt)) {
        emit(std::make_unique<TACCopy>(assignment->name, generateExpr(*assignment->value)));
        return;
    }

    if (const auto* print = dynamic_cast<const PrintStmt*>(&stmt)) {
        emit(std::make_unique<TACPrint>(generateExpr(*print->value)));
        return;
    }

    if (const auto* block = dynamic_cast<const BlockStmt*>(&stmt)) {
        generateBlock(*block);
        return;
    }

    if (const auto* ifStmt = dynamic_cast<const IfStmt*>(&stmt)) {
        const std::string thenLabel = newLabel("L_then_");
        const std::string endLabel = newLabel("L_end_");
        const std::string condition = generateExpr(*ifStmt->condition);

        if (ifStmt->elseBranch) {
            const std::string elseLabel = newLabel("L_else_");
            emit(std::make_unique<TACBranch>(condition, thenLabel));
            emit(std::make_unique<TACJump>(elseLabel));
            emit(std::make_unique<TACLabel>(thenLabel));
            generateBlock(*ifStmt->thenBranch);
            emit(std::make_unique<TACJump>(endLabel));
            emit(std::make_unique<TACLabel>(elseLabel));
            generateBlock(*ifStmt->elseBranch);
            emit(std::make_unique<TACLabel>(endLabel));
        } else {
            emit(std::make_unique<TACBranch>(condition, thenLabel));
            emit(std::make_unique<TACJump>(endLabel));
            emit(std::make_unique<TACLabel>(thenLabel));
            generateBlock(*ifStmt->thenBranch);
            emit(std::make_unique<TACLabel>(endLabel));
        }
        return;
    }

    if (const auto* whileStmt = dynamic_cast<const WhileStmt*>(&stmt)) {
        const std::string conditionLabel = newLabel("L_while_");
        const std::string bodyLabel = newLabel("L_body_");
        const std::string endLabel = newLabel("L_end_");
        emit(std::make_unique<TACLabel>(conditionLabel));
        const std::string condition = generateExpr(*whileStmt->condition);
        emit(std::make_unique<TACBranch>(condition, bodyLabel));
        emit(std::make_unique<TACJump>(endLabel));
        emit(std::make_unique<TACLabel>(bodyLabel));
        generateBlock(*whileStmt->body);
        emit(std::make_unique<TACJump>(conditionLabel));
        emit(std::make_unique<TACLabel>(endLabel));
        return;
    }

    throw std::runtime_error("TAC generator encountered an unknown statement");
}

void TACGenerator::generateBlock(const BlockStmt& block) {
    for (const auto& statement : block.statements) {
        generateStmt(*statement);
    }
}

std::string TACGenerator::generateExpr(const Expr& expr) {
    if (const auto* number = dynamic_cast<const NumberExpr*>(&expr)) {
        return std::to_string(number->value);
    }
    if (const auto* string = dynamic_cast<const StringExpr*>(&expr)) {
        return quoteString(string->value);
    }
    if (const auto* variable = dynamic_cast<const VariableExpr*>(&expr)) {
        return variable->name;
    }
    if (const auto* unary = dynamic_cast<const UnaryExpr*>(&expr)) {
        const std::string operand = generateExpr(*unary->expr);
        const std::string result = newTemp();
        emit(std::make_unique<TACBinOp>(result, "0", unary->op, operand));
        return result;
    }
    if (const auto* binary = dynamic_cast<const BinaryExpr*>(&expr)) {
        const std::string left = generateExpr(*binary->left);
        const std::string right = generateExpr(*binary->right);
        const std::string result = newTemp();
        emit(std::make_unique<TACBinOp>(result, left, binaryOpName(binary->op), right));
        return result;
    }

    throw std::runtime_error("TAC generator encountered an unknown expression");
}

std::string TACGenerator::quoteString(const std::string& value) {
    std::ostringstream out;
    out << std::quoted(value);
    return out.str();
}

void printTAC(const std::vector<TACInstrPtr>& instructions, std::ostream& out) {
    out << "#    Kind         Instruction\n";
    out << "---- ------------ ------------------------------\n";
    for (std::size_t i = 0; i < instructions.size(); ++i) {
        out << std::setw(4) << i << " " << std::left << std::setw(12) << instructions[i]->kind()
            << " ";
        instructions[i]->print(out);
        out << '\n' << std::right;
    }
}
