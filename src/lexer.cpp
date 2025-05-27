#include "lexer.h"
#include <cctype> // Para std::isspace, std::isdigit, std::isalpha

Lexer::Lexer(const std::string& src)
    : source(src), character(0), read_position(0), position(0) {
    read_char();
}

void Lexer::read_char() {
    if (read_position >= source.size()) {
        character = 0; // Null char para EOF
    } else {
        character = source[read_position];
    }
    position = read_position;
    read_position++;
}

char Lexer::peek_character() const {
    if (read_position >= source.size()) {
        return 0;
    }
    return source[read_position];
}

void Lexer::skip_whitespace() {
    while (std::isspace(character)) {
        read_char();
    }
}

bool Lexer::is_number(char ch) const {
    return std::isdigit(static_cast<unsigned char>(ch));
}

bool Lexer::is_letter(char ch) const {
    return std::isalpha(static_cast<unsigned char>(ch)) || ch == '_';
}

std::string Lexer::read_number() {
    size_t initial_position = position;
    while (is_number(character)) {
        read_char();
    }
    return source.substr(initial_position, position - initial_position);
}

std::string Lexer::read_literal() {
    size_t initial_position = position;
    while (is_letter(character) || is_number(character)) {
        read_char();
    }
    return source.substr(initial_position, position - initial_position);
}

Token Lexer::next_token() {
    skip_whitespace();

    Token token;

    switch (character) {
        case '=':
            if (peek_character() == '=') {
                char current_char = character;
                read_char();
                token = Token(TokenType::EQ, std::string(1, current_char) + character);
            } else {
                token = Token(TokenType::ASSIGN, std::string(1, character));
            }
            break;
        case '!':
            if (peek_character() == '=') {
                char current_char = character;
                read_char();
                token = Token(TokenType::NOT_EQ, std::string(1, current_char) + character);
            } else {
                token = Token(TokenType::BANG, std::string(1, character));
            }
            break;
        case '+':
            token = Token(TokenType::PLUS, std::string(1, character));
            break;
        case '-':
            token = Token(TokenType::MINUS, std::string(1, character));
            break;
        case '*':
            token = Token(TokenType::ASTERISK, std::string(1, character));
            break;
        case '/':
            token = Token(TokenType::SLASH, std::string(1, character));
            break;
        case '<':
            token = Token(TokenType::LT, std::string(1, character));
            break;
        case '>':
            token = Token(TokenType::GT, std::string(1, character));
            break;
        case ';':
            token = Token(TokenType::SEMICOLON, std::string(1, character));
            break;
        case ',':
            token = Token(TokenType::COMMA, std::string(1, character));
            break;
        case '(':
            token = Token(TokenType::LPAREN, std::string(1, character));
            break;
        case ')':
            token = Token(TokenType::RPAREN, std::string(1, character));
            break;
        case '{':
            token = Token(TokenType::LBRACE, std::string(1, character));
            break;
        case '}':
            token = Token(TokenType::RBRACE, std::string(1, character));
            break;
        case 0:
            token = Token(TokenType::EOF_TOKEN, "");
            break;
        default:
            if (is_letter(character)) {
                std::string literal = read_literal();
                TokenType type = lookup_token_type(literal);
                return Token(type, literal);  // ⚠️ cuidado, no avanzar después
            } else if (is_number(character)) {
                std::string number = read_number();
                return Token(TokenType::INT, number); // ⚠️ igual acá
            } else {
                token = Token(TokenType::ILLEGAL, std::string(1, character));
            }
            break;
    }

    read_char();
    return token;
}
