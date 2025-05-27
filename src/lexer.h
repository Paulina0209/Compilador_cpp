#ifndef LEXER_H
#define LEXER_H

#include <string>
#include "tokens.h"

class Lexer {
public:
    explicit Lexer(const std::string& source);

    Token next_token();

private:
    std::string source;
    char character;
    size_t read_position;
    size_t position;

    void read_char();
    char peek_character() const;
    void skip_whitespace();
    bool is_number(char ch) const;
    bool is_letter(char ch) const;
    bool is_operator(char ch) const;
    std::string read_number();
    std::string read_literal();
};

#endif // LEXER_H