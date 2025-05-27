#include "tokens.h"

// Función libre (fuera de la clase Token)
std::string token_type_to_string(TokenType type) {
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
        case TokenType::MINUS: return "MINUS";
        case TokenType::RBRACE: return "RBRACE";
        case TokenType::RPAREN: return "RPAREN";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::SLASH: return "SLASH";
        case TokenType::ASTERISK: return "ASTERISK";
        case TokenType::BANG: return "BANG";
        case TokenType::LT: return "LT";
        case TokenType::GT: return "GT";
        case TokenType::NOT_EQ: return "NOT_EQ";
        case TokenType::TRUE: return "TRUE";
        case TokenType::FALSE: return "FALSE";
        case TokenType::IF: return "IF";
        case TokenType::ELSE: return "ELSE";
        case TokenType::RETURN: return "RETURN";
        case TokenType::WHILE: return "WHILE";
        default: return "UNKNOWN";
    }
}
