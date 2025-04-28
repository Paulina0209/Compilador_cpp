#include "tokens.h"

TokenType lookup_token_type(const std::string& literal) {
    static const std::unordered_map<std::string, TokenType> keywords = {
        {"function", TokenType::FUNCTION},
        {"let", TokenType::LET},
        {"for", TokenType::FOR}
    };

    auto it = keywords.find(literal);
    if (it != keywords.end()) {
        return it->second;
    }
    return TokenType::IDENT;
}

std::string Token::token_type_to_string(TokenType type) const {
    switch (type) {
        case TokenType::ASSIGN: return "ASSIGN";
        case TokenType::COMMA: return "COMMA";
        case TokenType::EOF_TOKEN: return "EOF";
        case TokenType::EQ: return "EQ";
        case TokenType::FOR: return "FOR";
        case TokenType::FUNCTION: return "FUNCTION";
        case TokenType::IDENT: return "IDENT";
        case TokenType::ILLEGAL: return "ILLEGAL";
        case TokenType::INT: return "INT";
        case TokenType::LBRACE: return "LBRACE";
        case TokenType::LET: return "LET";
        case TokenType::LPAREN: return "LPAREN";
        case TokenType::PLUS: return "PLUS";
        case TokenType::RBRACE: return "RBRACE";
        case TokenType::RPAREN: return "RPAREN";
        case TokenType::SEMICOLON: return "SEMICOLON";
        default: return "UNKNOWN";
    }
}