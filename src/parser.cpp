#include "parser.h"
#include <stdexcept>
#include <iostream>

Parser::Parser(Lexer& l) : lexer(l) {
    next_token();
    next_token();
}

void Parser::next_token() {
    current_token = peek_token;
    peek_token = lexer.next_token();
}

bool Parser::expect_peek(TokenType t) {
    if (peek_token.token_type == t) {
        next_token();
        return true;
    } else {
        return false;
    }
}

std::unique_ptr<Program> Parser::parse_program() {
    auto program = std::make_unique<Program>();

    while (current_token.token_type != TokenType::EOF_TOKEN) {
        auto stmt = parse_statement();
        if (stmt) {
            program->statements.push_back(std::move(stmt));
        }
        next_token();
    }

    return program;
}

std::unique_ptr<Statement> Parser::parse_statement() {
    if (current_token.token_type == TokenType::LET) {
        return parse_let_statement();
    }
    return nullptr;
}

std::unique_ptr<Statement> Parser::parse_let_statement() {
    Token let_token = current_token;

    if (!expect_peek(TokenType::IDENT)) {
        return nullptr;
    }

    std::string name = current_token.literal;

    if (!expect_peek(TokenType::ASSIGN)) {
        return nullptr;
    }

    next_token();

    auto value = parse_expression(Precedence::LOWEST);
    if (!value) return nullptr;

    if (peek_token.token_type == TokenType::SEMICOLON) {
        next_token();
    }

    return std::make_unique<LetStatement>(let_token, name, std::move(value));
}

Precedence Parser::get_precedence(TokenType type) {
    auto it = precedences.find(type);
    if (it != precedences.end()) {
        return it->second;
    }
    return Precedence::LOWEST;
}

std::unique_ptr<Expression> Parser::parse_expression(Precedence precedence) {
    std::unique_ptr<Expression> left;

    switch (current_token.token_type) {
        case TokenType::IDENT:
            left = parse_identifier();
            break;
        default:
            return nullptr;
    }

    while (peek_token.token_type != TokenType::SEMICOLON &&
           precedence < get_precedence(peek_token.token_type)) {

        if (peek_token.token_type == TokenType::PLUS ||
            peek_token.token_type == TokenType::MINUS ||
            peek_token.token_type == TokenType::SLASH ||
            peek_token.token_type == TokenType::ASTERISK) {
            next_token();
            left = parse_infix_expression(std::move(left));
        } else {
            break;
        }
    }

    return left;
}

std::unique_ptr<Expression> Parser::parse_identifier() {
    return std::make_unique<Identifier>(current_token, current_token.literal);
}

std::unique_ptr<Expression> Parser::parse_infix_expression(std::unique_ptr<Expression> left) {
    Token op_token = current_token;
    Precedence precedence = get_precedence(op_token.token_type);

    next_token();

    auto right = parse_expression(precedence);
    if (!right) return nullptr;

    return std::make_unique<InfixExpression>(op_token, std::move(left), op_token.literal, std::move(right));
}
