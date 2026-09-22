#pragma once

#include "AST.hpp"

#include <iosfwd>
#include <string>
#include <vector>

struct TACInstr {
    virtual ~TACInstr() = default;
    virtual void print(std::ostream& out) const = 0;
    virtual const char* kind() const = 0;
};

using TACInstrPtr = std::unique_ptr<TACInstr>;

struct TACBinOp final : TACInstr {
    TACBinOp(std::string dest, std::string left, std::string op, std::string right);
    void print(std::ostream& out) const override;
    const char* kind() const override;

    std::string dest;
    std::string left;
    std::string op;
    std::string right;
};

struct TACCopy final : TACInstr {
    TACCopy(std::string dest, std::string source);
    void print(std::ostream& out) const override;
    const char* kind() const override;

    std::string dest;
    std::string source;
};

struct TACPrint final : TACInstr {
    explicit TACPrint(std::string source);
    void print(std::ostream& out) const override;
    const char* kind() const override;

    std::string source;
};

struct TACLabel final : TACInstr {
    explicit TACLabel(std::string name);
    void print(std::ostream& out) const override;
    const char* kind() const override;

    std::string name;
};

struct TACJump final : TACInstr {
    explicit TACJump(std::string target);
    void print(std::ostream& out) const override;
    const char* kind() const override;

    std::string target;
};

struct TACBranch final : TACInstr {
    TACBranch(std::string condition, std::string target);
    void print(std::ostream& out) const override;
    const char* kind() const override;

    std::string condition;
    std::string target;
};

class TACGenerator {
public:
    const std::vector<TACInstrPtr>& generate(const Program& program);
    const std::vector<TACInstrPtr>& instructions() const;

private:
    std::vector<TACInstrPtr> instructions_;
    int tempCount_ = 0;
    int labelCount_ = 0;

    std::string newTemp();
    std::string newLabel(const char* prefix);
    void emit(TACInstrPtr instruction);
    void generateStmt(const Stmt& stmt);
    void generateBlock(const BlockStmt& block);
    std::string generateExpr(const Expr& expr);
    static std::string quoteString(const std::string& value);
};

void printTAC(const std::vector<TACInstrPtr>& instructions, std::ostream& out);
