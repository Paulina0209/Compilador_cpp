#include <iostream>
#include <string>
#include "lexer.h"
#include "tokens.h"

int main() {
    const Token EOF_TOKEN(TokenType::EOF_TOKEN, "");

    std::string source;
    while (true) {
        std::cout << ">> ";
        if (!std::getline(std::cin, source)) {
            break;
        }

        if (source == "exit") {
            break;
        }

        Lexer lexer(source);
        Token token = lexer.next_token();
        while (!(token.token_type == EOF_TOKEN.token_type && token.literal == EOF_TOKEN.literal)) {
            std::cout << token.to_string() << std::endl;
            token = lexer.next_token();
        }
    }

    return 0;
}