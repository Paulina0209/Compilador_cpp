#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <memory>
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

class BlockStatement : public Statement {
public:
    Token token;
    std::vector<std::unique_ptr<Statement>> statements;

    BlockStatement(const Token& tok) : token(tok) {}

    std::string token_literal() const override {
        return token.literal;
    }

    std::string to_string() const override {
        std::string result = "{ ";
        for (const auto& stmt : statements) {
            result += stmt->to_string() + " ";
        }
        result += "}";
        return result;
    }
};
class IntegerLiteral : public Expression {
public:
    Token token;
    int value;

    IntegerLiteral(const Token& tok, int val)
        : token(tok), value(val) {}

    std::string token_literal() const override {
        return token.literal;
    }

    std::string to_string() const override {
        return std::to_string(value);
    }
};

class FunctionLiteral : public Expression {
public:
    Token token;
    std::vector<std::string> parameters;
    std::unique_ptr<class BlockStatement> body;

    FunctionLiteral(const Token& tok)
        : token(tok) {}

    std::string token_literal() const override {
        return token.literal;
    }

    std::string to_string() const override {
        std::string params;
        for (size_t i = 0; i < parameters.size(); i++) {
            params += parameters[i];
            if (i != parameters.size() - 1) {
                params += ", ";
            }
        }
        return token_literal() + "(" + params + ") " + (body ? body->to_string() : "");
    }
};

class CallExpression : public Expression {
public:
    Token token;
    std::unique_ptr<Expression> function;
    std::vector<std::unique_ptr<Expression>> arguments;

    CallExpression(const Token& tok, std::unique_ptr<Expression> func)
        : token(tok), function(std::move(func)) {}

    std::string token_literal() const override {
        return token.literal;
    }

    std::string to_string() const override {
        std::string args;
        for (size_t i = 0; i < arguments.size(); i++) {
            args += arguments[i]->to_string();
            if (i != arguments.size() - 1) {
                args += ", ";
            }
        }
        return function->to_string() + "(" + args + ")";
    }
};

class ExpressionStatement : public Statement {
public:
    Token token;
    std::unique_ptr<Expression> expression;

    ExpressionStatement(const Token& tok, std::unique_ptr<Expression> expr)
        : token(tok), expression(std::move(expr)) {}

    std::string token_literal() const override {
        return token.literal;
    }

    std::string to_string() const override {
        return expression ? expression->to_string() + ";" : "";
    }
};

class IfExpression : public Expression {
public:
    Token token;
    std::unique_ptr<Expression> condition;
    std::unique_ptr<BlockStatement> consequence;
    std::unique_ptr<BlockStatement> alternative;

    IfExpression(const Token& tok) : token(tok) {}

    std::string token_literal() const override {
        return token.literal;
    }

    std::string to_string() const override {
        std::string out = "if" + condition->to_string() + " " + consequence->to_string();
        if (alternative) {
            out += " else " + alternative->to_string();
        }
        return out;
    }
};

class WhileStatement : public Statement {
public:
    Token token;
    std::unique_ptr<Expression> condition;
    std::unique_ptr<BlockStatement> body;

    WhileStatement(const Token& tok) : token(tok) {}

    std::string token_literal() const override {
        return token.literal;
    }

    std::string to_string() const override {
        return token_literal() + " " + condition->to_string() + " " + (body ? body->to_string() : "");
    }
};

class InfixExpression : public Expression {
public:
    Token token;
    std::unique_ptr<Expression> left;
    std::string op;
    std::unique_ptr<Expression> right;

    InfixExpression(const Token& tok, std::unique_ptr<Expression> l, const std::string& operator_, std::unique_ptr<Expression> r)
        : token(tok), left(std::move(l)), op(operator_), right(std::move(r)) {}

    std::string token_literal() const override {
        return token.literal;
    }

    std::string to_string() const override {
        return "(" + left->to_string() + " " + op + " " + right->to_string() + ")";
    }
};

#endif // AST_H
