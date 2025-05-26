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
    std::string token_type_to_string(TokenType type) const {
        switch (type) {
            case TokenType::ILLEGAL: return "ILLEGAL";
            case TokenType::EOF_TOKEN: return "EOF";
            case TokenType::IDENT: return "IDENT";
            case TokenType::INT: return "INT";
            case TokenType::ASSIGN: return "ASSIGN";
            case TokenType::PLUS: return "PLUS";
            case TokenType::MINUS: return "MINUS";
            case TokenType::BANG: return "BANG";
            case TokenType::ASTERISK: return "ASTERISK";
            case TokenType::SLASH: return "SLASH";
            case TokenType::LT: return "LT";
            case TokenType::GT: return "GT";
            case TokenType::EQ: return "EQ";
            case TokenType::NOT_EQ: return "NOT_EQ";
            case TokenType::COMMA: return "COMMA";
            case TokenType::SEMICOLON: return "SEMICOLON";
            case TokenType::LPAREN: return "LPAREN";
            case TokenType::RPAREN: return "RPAREN";
            case TokenType::LBRACE: return "LBRACE";
            case TokenType::RBRACE: return "RBRACE";
            case TokenType::FUNCTION: return "FUNCTION";
            case TokenType::LET: return "LET";
            case TokenType::TRUE: return "TRUE";
            case TokenType::FALSE: return "FALSE";
            case TokenType::IF: return "IF";
            case TokenType::ELSE: return "ELSE";
            case TokenType::RETURN: return "RETURN";
            case TokenType::FOR: return "FOR";
            default: return "UNKNOWN";
        }
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
    };

    auto it = keywords.find(literal);
    if (it != keywords.end()) {
        return it->second;
    }
    return TokenType::IDENT;
}

#endif // TOKENS_H
