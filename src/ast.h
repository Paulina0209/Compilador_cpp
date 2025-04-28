#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <memory> // Para std::unique_ptr
#include "tokens.h"

class Node {
public:
    virtual ~Node() = default;
    virtual std::string token_literal() const = 0;
    virtual std::string to_string() const = 0;
};

class Statement : public Node {
};

class Expression : public Node {
};

class Program : public Node {
public:
    std::vector<std::unique_ptr<Statement>> statements;

    std::string token_literal() const override {
        if (!statements.empty()) {
            return statements[0]->token_literal();
        }
        return "";
    }

    std::string to_string() const override {
        std::string result;
        for (const auto& stmt : statements) {
            result += stmt->to_string() + "\n";
        }
        return result;
    }
};

class LetStatement : public Statement {
public:
    Token token;
    std::string name;
    std::unique_ptr<Expression> value;

    LetStatement(const Token& tok, const std::string& nm, std::unique_ptr<Expression> val)
        : token(tok), name(nm), value(std::move(val)) {}

    std::string token_literal() const override {
        return token.literal;
    }

    std::string to_string() const override {
        return token_literal() + " " + name + " = " + (value ? value->to_string() : "") + ";";
    }
};

class Identifier : public Expression {
public:
    Token token;
    std::string value;

    Identifier(const Token& tok, const std::string& val)
        : token(tok), value(val) {}

    std::string token_literal() const override {
        return token.literal;
    }

    std::string to_string() const override {
        return value;
    }
};

#endif // AST_H