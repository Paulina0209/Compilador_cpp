#ifndef TOKENS_H
#define TOKENS_H

#include <string>
#include <unordered_map>
#include <iostream>

enum class TokenType {
    ILLEGAL,
    EOF_TOKEN,

    // Identifiers + literals
    IDENT,
    INT,

    // Operators
    ASSIGN,
    PLUS,
    MINUS,
    BANG,
    ASTERISK,
    SLASH,

    LT,
    GT,
    EQ,
    NOT_EQ,

    // Delimiters
    COMMA,
    SEMICOLON,
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,

    // Keywords
    FUNCTION,
    LET,
    TRUE,
    FALSE,
    IF,
    ELSE,
    RETURN,
    FOR,
    WHILE,
};

// 🔁 Declaración de función global
std::string token_type_to_string(TokenType type);

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
};

inline TokenType lookup_token_type(const std::string& literal) {
    static std::unordered_map<std::string, TokenType> keywords = {
        {"fn", TokenType::FUNCTION},
        {"let", TokenType::LET},
        {"true", TokenType::TRUE},
        {"false", TokenType::FALSE},
        {"if", TokenType::IF},
        {"else", TokenType::ELSE},
        {"return", TokenType::RETURN},
        {"for", TokenType::FOR},
        {"while", TokenType::WHILE},
    };

    auto it = keywords.find(literal);
    if (it != keywords.end()) {
        return it->second;
    }
    return TokenType::IDENT;
}

#endif // TOKENS_H