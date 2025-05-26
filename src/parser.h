#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>
#include "lexer.h"
#include "ast.h"

enum class Precedence {
    LOWEST,
    ASSIGN,
    EQUALS,      // ==
    LESSGREATER, // > or <
    SUM,         // +
    PRODUCT,     // *
    PREFIX,      // -X or !X
    CALL         // myFunction(X)
};

inline std::unordered_map<TokenType, Precedence> precedences = {
    {TokenType::EQ, Precedence::EQUALS},
    {TokenType::NOT_EQ, Precedence::EQUALS},
    {TokenType::LT, Precedence::LESSGREATER},
    {TokenType::GT, Precedence::LESSGREATER},
    {TokenType::PLUS, Precedence::SUM},
    {TokenType::MINUS, Precedence::SUM},
    {TokenType::SLASH, Precedence::PRODUCT},
    {TokenType::ASTERISK, Precedence::PRODUCT},
    {TokenType::LPAREN, Precedence::CALL},
};

class Parser {
public:
    explicit Parser(Lexer& lexer);
    std::unique_ptr<Program> parse_program();
    std::vector<std::string> errors;

private:
    Lexer& lexer;
    Token current_token;
    Token peek_token;

    using PrefixParseFn = std::function<std::unique_ptr<Expression>()>;
    using InfixParseFn = std::function<std::unique_ptr<Expression>(std::unique_ptr<Expression>)>;

    std::unordered_map<TokenType, PrefixParseFn> prefix_parse_fns;
    std::unordered_map<TokenType, InfixParseFn> infix_parse_fns;

    void next_token();
    bool expect_peek(TokenType t);
    bool cur_token_is(TokenType t);
    bool peek_token_is(TokenType t);

    std::unique_ptr<Statement> parse_statement();
    std::unique_ptr<Statement> parse_let_statement();
    std::unique_ptr<BlockStatement> parse_block_statement();
    std::unique_ptr<Statement> parse_expression_statement();
    std::unique_ptr<Statement> parse_function_literal_statement();

    std::unique_ptr<Expression> parse_expression(Precedence precedence);
    std::unique_ptr<Expression> parse_identifier();
    std::unique_ptr<Expression> parse_integer_literal();
    std::unique_ptr<Expression> parse_prefix_expression();
    std::unique_ptr<Expression> parse_infix_expression(std::unique_ptr<Expression> left);
    std::unique_ptr<Expression> parse_grouped_expression();

    std::unique_ptr<FunctionLiteral> parse_function_literal();
    std::vector<std::string> parse_function_parameters();

    Precedence peek_precedence();
    Precedence cur_precedence();

    Precedence get_precedence(TokenType type);
};

#endif // PARSER_H
