#include "parser.h"
#include <stdexcept>
#include <iostream>

Parser::Parser(Lexer& l) : lexer(l) {
    next_token();
    next_token();

    prefix_parse_fns[TokenType::IDENT] = [this]() { return parse_identifier(); };
    prefix_parse_fns[TokenType::INT] = [this]() { return parse_integer_literal(); };
    prefix_parse_fns[TokenType::BANG] = [this]() { return parse_prefix_expression(); };
    prefix_parse_fns[TokenType::MINUS] = [this]() { return parse_prefix_expression(); };
    prefix_parse_fns[TokenType::TRUE] = [this]() { return parse_boolean(); };
    prefix_parse_fns[TokenType::FALSE] = [this]() { return parse_boolean(); };
    prefix_parse_fns[TokenType::LPAREN] = [this]() { return parse_grouped_expression(); };
    prefix_parse_fns[TokenType::FUNCTION] = [this]() { return parse_function_literal(); };
    prefix_parse_fns[TokenType::IF] = [this]() { return parse_if_expression(); };

    infix_parse_fns[TokenType::PLUS] = [this](auto left) { return parse_infix_expression(std::move(left)); };
    infix_parse_fns[TokenType::MINUS] = [this](auto left) { return parse_infix_expression(std::move(left)); };
    infix_parse_fns[TokenType::SLASH] = [this](auto left) { return parse_infix_expression(std::move(left)); };
    infix_parse_fns[TokenType::ASTERISK] = [this](auto left) { return parse_infix_expression(std::move(left)); };
    infix_parse_fns[TokenType::EQ] = [this](auto left) { return parse_infix_expression(std::move(left)); };
    infix_parse_fns[TokenType::NOT_EQ] = [this](auto left) { return parse_infix_expression(std::move(left)); };
    infix_parse_fns[TokenType::LT] = [this](auto left) { return parse_infix_expression(std::move(left)); };
    infix_parse_fns[TokenType::GT] = [this](auto left) { return parse_infix_expression(std::move(left)); };
    infix_parse_fns[TokenType::LPAREN] = [this](auto left) { return parse_call_expression(std::move(left)); };
}

void Parser::next_token() {
    current_token = peek_token;
    peek_token = lexer.next_token();
}

bool Parser::expect_peek(TokenType t) {
    if (peek_token.token_type == t) {
        next_token();
        return true;
    }
    errors.push_back("Expected " + token_type_to_string(t) + ", got " + token_type_to_string(peek_token.token_type));
    return false;
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
    if (current_token.token_type == TokenType::WHILE) {
        return parse_while_statement();
    }
    return parse_expression_statement();
}

std::unique_ptr<Statement> Parser::parse_expression_statement() {
    Token token = current_token;
    auto expr = parse_expression(Precedence::LOWEST);
    if (peek_token.token_type == TokenType::SEMICOLON) next_token();
    return std::make_unique<ExpressionStatement>(token, std::move(expr));
}

std::unique_ptr<Statement> Parser::parse_let_statement() {
    Token let_token = current_token;
    if (!expect_peek(TokenType::IDENT)) return nullptr;
    std::string name = current_token.literal;
    if (!expect_peek(TokenType::ASSIGN)) return nullptr;
    next_token();
    auto value = parse_expression(Precedence::LOWEST);
    if (peek_token.token_type == TokenType::SEMICOLON) next_token();
    return std::make_unique<LetStatement>(let_token, name, std::move(value));
}

std::unique_ptr<Statement> Parser::parse_while_statement() {
    Token token = current_token;
    if (!expect_peek(TokenType::LPAREN)) return nullptr;
    next_token();
    auto condition = parse_expression(Precedence::LOWEST);
    if (!expect_peek(TokenType::RPAREN)) return nullptr;
    if (!expect_peek(TokenType::LBRACE)) return nullptr;
    auto body = parse_block_statement();
    return std::make_unique<WhileStatement>(token, std::move(condition), std::move(body));
}

std::unique_ptr<BlockStatement> Parser::parse_block_statement() {
    Token token = current_token;
    std::vector<std::unique_ptr<Statement>> statements;
    next_token();
    while (current_token.token_type != TokenType::RBRACE &&
           current_token.token_type != TokenType::EOF_TOKEN) {
        auto stmt = parse_statement();
        if (stmt) statements.push_back(std::move(stmt));
        next_token();
    }
    auto block = std::make_unique<BlockStatement>(token);
    block->statements = std::move(statements);
    return block;
}

std::unique_ptr<Expression> Parser::parse_expression(Precedence precedence) {
    auto prefix_fn = prefix_parse_fns.find(current_token.token_type);
    if (prefix_fn == prefix_parse_fns.end()) return nullptr;
    auto left = prefix_fn->second();
    while (peek_token.token_type != TokenType::SEMICOLON &&
           precedence < peek_precedence()) {
        auto infix_fn = infix_parse_fns.find(peek_token.token_type);
        if (infix_fn == infix_parse_fns.end()) break;
        next_token();
        left = infix_fn->second(std::move(left));
    }
    return left;
}

std::unique_ptr<Expression> Parser::parse_identifier() {
    return std::make_unique<Identifier>(current_token, current_token.literal);
}

std::unique_ptr<Expression> Parser::parse_integer_literal() {
    int value = std::stoi(current_token.literal);
    return std::make_unique<IntegerLiteral>(current_token, value);
}

std::unique_ptr<Expression> Parser::parse_boolean() {
    return std::make_unique<BooleanLiteral>(current_token, current_token.token_type == TokenType::TRUE);
}

std::unique_ptr<Expression> Parser::parse_prefix_expression() {
    Token token = current_token;
    std::string op = token.literal;
    next_token();
    auto right = parse_expression(Precedence::PREFIX);
    return std::make_unique<PrefixExpression>(token, op, std::move(right));
}

std::unique_ptr<Expression> Parser::parse_grouped_expression() {
    next_token();
    auto expr = parse_expression(Precedence::LOWEST);
    if (!expect_peek(TokenType::RPAREN)) return nullptr;
    return expr;
}

std::unique_ptr<Expression> Parser::parse_infix_expression(std::unique_ptr<Expression> left) {
    Token token = current_token;
    std::string op = token.literal;
    Precedence precedence = cur_precedence();
    next_token();
    auto right = parse_expression(precedence);
    return std::make_unique<InfixExpression>(token, std::move(left), op, std::move(right));
}

std::unique_ptr<Expression> Parser::parse_function_literal() {
    Token token = current_token;
    if (!expect_peek(TokenType::LPAREN)) return nullptr;
    auto parameters = parse_function_parameters();
    if (!expect_peek(TokenType::LBRACE)) return nullptr;
    std::shared_ptr<BlockStatement> body(parse_block_statement().release());
    auto function = std::make_unique<FunctionLiteral>(token);
    function->parameters = std::move(parameters);
    function->body = std::move(body);
    return function;
}

std::unique_ptr<Expression> Parser::parse_if_expression() {
    Token token = current_token;

    if (!expect_peek(TokenType::LPAREN)) return nullptr;
    next_token();

    auto condition = parse_expression(Precedence::LOWEST);
    if (!condition) return nullptr;

    if (!expect_peek(TokenType::RPAREN)) return nullptr;
    if (!expect_peek(TokenType::LBRACE)) return nullptr;

    auto consequence = parse_block_statement();
    if (!consequence) return nullptr;

    auto if_expr = std::make_unique<IfExpression>(token);
    if_expr->condition = std::move(condition);
    if_expr->consequence = std::move(consequence);

    // Manejar else y else if
    if (peek_token.token_type == TokenType::ELSE) {
        next_token();

        // Si después de else viene if, parseamos otra if expression
        if (peek_token.token_type == TokenType::IF) {
            next_token();
            // Recursivamente parsear otra if expression para else if
            auto else_if = parse_if_expression();
            if (else_if) {
                // Convertir la if expression a un block statement
                auto else_block = std::make_unique<BlockStatement>(Token(TokenType::LBRACE, "{"));
                auto else_stmt = std::make_unique<ExpressionStatement>(else_if->get_token(), std::move(else_if));
                else_block->statements.push_back(std::move(else_stmt));
                if_expr->alternative = std::move(else_block);
            }
        } else if (peek_token.token_type == TokenType::LBRACE) {
            // Else normal con bloque
            next_token();
            if_expr->alternative = parse_block_statement();
        }
    }

    return if_expr;
}

std::vector<std::string> Parser::parse_function_parameters() {
    std::vector<std::string> identifiers;
    if (peek_token.token_type == TokenType::RPAREN) {
        next_token();
        return identifiers;
    }
    next_token();
    identifiers.push_back(current_token.literal);
    while (peek_token.token_type == TokenType::COMMA) {
        next_token();
        next_token();
        identifiers.push_back(current_token.literal);
    }
    if (!expect_peek(TokenType::RPAREN)) return {};
    return identifiers;
}

std::unique_ptr<Expression> Parser::parse_call_expression(std::unique_ptr<Expression> function) {
    Token token = current_token;
    std::vector<std::unique_ptr<Expression>> args;
    if (peek_token.token_type != TokenType::RPAREN) {
        next_token();
        args.push_back(parse_expression(Precedence::LOWEST));
        while (peek_token.token_type == TokenType::COMMA) {
            next_token();
            next_token();
            args.push_back(parse_expression(Precedence::LOWEST));
        }
    }
    if (!expect_peek(TokenType::RPAREN)) return nullptr;
    auto call = std::make_unique<CallExpression>(token, std::move(function));
    call->arguments = std::move(args);
    return call;
}

Precedence Parser::peek_precedence() {
    return get_precedence(peek_token.token_type);
}

Precedence Parser::cur_precedence() {
    return get_precedence(current_token.token_type);
}

Precedence Parser::get_precedence(TokenType type) {
    auto it = precedences.find(type);
    if (it != precedences.end()) return it->second;
    return Precedence::LOWEST;
}