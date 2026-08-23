#include "Lexer.hpp"

#include <cctype>
#include <sstream>

Lexer::Lexer(std::string source) : source_(std::move(source)) {}

std::vector<Token> Lexer::tokenize() {
    while (!isAtEnd()) {
        const char ch = current();

        if (ch == ' ' || ch == '\t' || ch == '\r') {
            advance();
        } else if (ch == '\n') {
            addSimpleToken(TokenType::Newline);
        } else if (ch == '+') {
            addSimpleToken(TokenType::Plus);
        } else if (ch == '-') {
            addSimpleToken(TokenType::Minus);
        } else if (ch == '*') {
            addSimpleToken(TokenType::Star);
        } else if (ch == '/') {
            addSimpleToken(TokenType::Slash);
        } else if (ch == '=') {
            const int line = line_;
            const int column = column_;
            advance();
            const bool hasEqual = match('=');
            addToken(hasEqual ? TokenType::EqualEqual : TokenType::Equal,
                     hasEqual ? "==" : "=", line, column);
        } else if (ch == '!') {
            const int line = line_;
            const int column = column_;
            advance();
            if (match('=')) {
                addToken(TokenType::BangEqual, "!=", line, column);
            } else {
                addError("Unexpected character '!'. Did you mean '!='?", line, column);
            }
        } else if (ch == '<') {
            const int line = line_;
            const int column = column_;
            advance();
            const bool hasEqual = match('=');
            addToken(hasEqual ? TokenType::LessEqual : TokenType::Less,
                     hasEqual ? "<=" : "<", line, column);
        } else if (ch == '>') {
            const int line = line_;
            const int column = column_;
            advance();
            const bool hasEqual = match('=');
            addToken(hasEqual ? TokenType::GreaterEqual : TokenType::Greater,
                     hasEqual ? ">=" : ">", line, column);
        } else if (ch == '(') {
            addSimpleToken(TokenType::LParen);
        } else if (ch == ')') {
            addSimpleToken(TokenType::RParen);
        } else if (ch == '{') {
            addSimpleToken(TokenType::LBrace);
        } else if (ch == '}') {
            addSimpleToken(TokenType::RBrace);
        } else if (ch == ';') {
            addSimpleToken(TokenType::Semicolon);
        } else if (ch == '"') {
            readString();
        } else if (std::isdigit(static_cast<unsigned char>(ch))) {
            readNumber();
        } else {
            const int line = line_;
            const int column = column_;
            std::ostringstream out;
            out << "Unknown character '" << ch << "'";
            addError(out.str(), line, column);
            advance();
        }
    }

    tokens_.push_back(Token{TokenType::EndOfFile, "", line_, column_});
    return tokens_;
}

const std::vector<std::string>& Lexer::errors() const {
    return errors_;
}

bool Lexer::isAtEnd() const {
    return pos_ >= source_.size();
}

char Lexer::current() const {
    return isAtEnd() ? '\0' : source_[pos_];
}

char Lexer::peek() const {
    const std::size_t next = pos_ + 1;
    return next >= source_.size() ? '\0' : source_[next];
}

char Lexer::peekNext() const {
    const std::size_t next = pos_ + 2;
    return next >= source_.size() ? '\0' : source_[next];
}

char Lexer::advance() {
    if (isAtEnd()) {
        return '\0';
    }

    const char ch = source_[pos_++];
    if (ch == '\n') {
        ++line_;
        column_ = 1;
    } else {
        ++column_;
    }

    return ch;
}

bool Lexer::match(char expected) {
    if (isAtEnd() || current() != expected) {
        return false;
    }

    advance();
    return true;
}

void Lexer::addToken(TokenType type, const std::string& lexeme, int line, int column) {
    tokens_.push_back(Token{type, lexeme, line, column});
}

void Lexer::addSimpleToken(TokenType type) {
    const int line = line_;
    const int column = column_;
    const std::string lexeme(1, current());
    advance();
    addToken(type, type == TokenType::Newline ? "\\n" : lexeme, line, column);
}

void Lexer::addError(const std::string& message, int line, int column) {
    std::ostringstream out;
    out << "Line " << line << ", column " << column << ": " << message;
    errors_.push_back(out.str());
}

void Lexer::readNumber() {
    const std::size_t start = pos_;
    const int line = line_;
    const int column = column_;

    while (std::isdigit(static_cast<unsigned char>(current()))) {
        advance();
    }

    addToken(TokenType::Number, source_.substr(start, pos_ - start), line, column);
}

void Lexer::readString() {
    const int line = line_;
    const int column = column_;
    advance();

    std::string value;
    while (!isAtEnd() && current() != '"') {
        if (current() == '\n') {
            addError("Unterminated string literal", line, column);
            return;
        }

        if (current() == '\\') {
            advance();
            if (isAtEnd()) {
                addError("Unterminated string escape", line_, column_);
                return;
            }

            const char escaped = advance();
            switch (escaped) {
                case 'n': value += '\n'; break;
                case '"': value += '"'; break;
                case '\\': value += '\\'; break;
                default:
                    value += escaped;
                    break;
            }
        } else {
            value += advance();
        }
    }

    if (isAtEnd()) {
        addError("Unterminated string literal", line, column);
        return;
    }

    advance();
    addToken(TokenType::String, value, line, column);
}