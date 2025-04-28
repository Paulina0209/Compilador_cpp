#ifndef TOKENS_H
#define TOKENS_H

#include <string>
#include <unordered_map>
#include <iostream>

enum class TokenType {
    ASSIGN,
    COMMA,
    EOF_TOKEN,
    EQ,
    FOR,
    FUNCTION,
    IDENT,
    ILLEGAL,
    INT,
    LBRACE,
    LET,
    LPAREN,
    PLUS,
    RBRACE,
    RPAREN,
    SEMICOLON
};

class Token {
public:
    TokenType token_type;
    std::string literal;

    Token() = default;
    Token(TokenType type, const std::string& lit)
        : token_type(type), literal(lit) {}

    std::string to_string() const {
        return "Token(" + token_type_to_string(token_type) + ", " + literal + ")";
    }

private:
    std::string token_type_to_string(TokenType type) const;
};

TokenType lookup_token_type(const std::string& literal);

#endif // TOKENS_H