#include "SemanticAnalyzer.hpp"

#include <sstream>


SymbolTable::SymbolTable() {
    enterScope();
}

void SymbolTable::enterScope() {
    scopes_.push_back({});
}

void SymbolTable::exitScope() {
    if (scopes_.size() > 1) {
        scopes_.pop_back();
    }
}

bool SymbolTable::declare(const std::string& name, ValueType type, int line) {
    if (existsInCurrentScope(name)) {
        return false;
    }

    Symbol symbol{name, type, line, static_cast<int>(scopes_.size()) - 1};
    scopes_.back()[name] = symbol;
    declarations_.push_back(symbol);
    return true;
}

std::optional<Symbol> SymbolTable::lookup(const std::string& name) const {
    for (auto scope = scopes_.rbegin(); scope != scopes_.rend(); ++scope) {
        const auto found = scope->find(name);
        if (found != scope->end()) {
            return found->second;
        }
    }

    return std::nullopt;
}

bool SymbolTable::existsInCurrentScope(const std::string& name) const {
    return !scopes_.empty() && scopes_.back().count(name) > 0;
}

const std::vector<Symbol>& SymbolTable::declarations() const {
    return declarations_;
}

void SemanticAnalyzer::analyze(const Program& program) {   
    for (const auto& stmt : program.statements) {         
        analyzeStmt(*stmt);                                
    }                                                       
}  

const std::vector<std::string>& SemanticAnalyzer::errors() const {
    return errors_;
}

const SymbolTable& SemanticAnalyzer::symbols() const {
    return symbols_;
}

void SemanticAnalyzer::analyzeStmt(const Stmt& stmt) {  
    if (const auto* varDecl = dynamic_cast<const VarDeclStmt*>(&stmt)) {
        if (!symbols_.declare(varDecl->name, varDecl->declaredType, varDecl->line)) {
            addError(varDecl->line, "Variable '" + varDecl->name + "' is already declared in this scope");
        }

        if (varDecl->initializer) {
            const ValueType actual = typeOf(*varDecl->initializer);
            if (actual != ValueType::Unknown && actual != varDecl->declaredType) {
                addError(varDecl->line, "Cannot assign " + std::string(valueTypeName(actual)) +
                                           " value to " + valueTypeName(varDecl->declaredType) +
                                           " variable '" + varDecl->name + "'");
            }
        }
        return;
    }

    if (const auto* assign = dynamic_cast<const AssignStmt*>(&stmt)) {
        const auto symbol = symbols_.lookup(assign->name);
        if (!symbol) {
            addError(assign->line, "Variable '" + assign->name + "' is used before declaration");
            typeOf(*assign->value);
            return;
        }

        const ValueType actual = typeOf(*assign->value);
        if (actual != ValueType::Unknown && actual != symbol->type) {
            addError(assign->line, "Cannot assign " + std::string(valueTypeName(actual)) +
                                       " value to " + valueTypeName(symbol->type) +
                                       " variable '" + assign->name + "'");
        }
        return;
    }
    if (const auto* print = dynamic_cast<const PrintStmt*>(&stmt)) {     
        typeOf(*print->value);                                        
        return;                                                        
    }                                                                    

    if (const auto* block = dynamic_cast<const BlockStmt*>(&stmt)) {     
        analyzeBlock(*block);                                          
        return;                                                       
    }                                                                

    if (const auto* ifStmt = dynamic_cast<const IfStmt*>(&stmt)) {      
        const ValueType condition = typeOf(*ifStmt->condition);      
        if (condition != ValueType::Unknown && condition != ValueType::Bool) { 
            addError(ifStmt->line, "If condition must be bool; use a comparison such as x > 0");  
        }                                                                

        analyzeBlock(*ifStmt->thenBranch);                          
        if (ifStmt->elseBranch) {                                    
            analyzeBlock(*ifStmt->elseBranch);                        
        }                                                             
        return;                                                        
    }                                                                   

    if (const auto* whileStmt = dynamic_cast<const WhileStmt*>(&stmt)) { /
        const ValueType condition = typeOf(*whileStmt->condition);    
        if (condition != ValueType::Unknown && condition != ValueType::Bool) {  
            addError(whileStmt->line, "While condition must be bool; use a comparison such as x > 0"); 
        }                                                               

        analyzeBlock(*whileStmt->body);                                
    }                 
}              

void SemanticAnalyzer::analyzeBlock(const BlockStmt& block) {
    symbols_.enterScope();
    for (const auto& stmt : block.statements) {
        analyzeStmt(*stmt);
    }
    symbols_.exitScope();
}

ValueType SemanticAnalyzer::typeOf(const Expr& expr) {    
    if (dynamic_cast<const NumberExpr*>(&expr)) {
        return ValueType::Int;
    }

    if (dynamic_cast<const StringExpr*>(&expr)) {
        return ValueType::String;
    }

    if (const auto* variable = dynamic_cast<const VariableExpr*>(&expr)) {
        const auto symbol = symbols_.lookup(variable->name);
        if (!symbol) {
            addError(variable->line, "Variable '" + variable->name + "' is used before declaration");
            return ValueType::Unknown;
        }

        return symbol->type;
    }

    return ValueType::Unknown;
}             

void SemanticAnalyzer::addError(int line, const std::string& message) {
    std::ostringstream out;
    out << "Line " << line << ": " << message;
    errors_.push_back(out.str());
}