#include <iostream>
#include <string>
#include <memory>
#include "lexer.h"
#include "parser.h"
#include "ast.h"

int main() {
    std::string source;
    std::cout << "Escribe código o 'exit' para salir:\n";

    while (true) {
        std::cout << ">> ";
        if (!std::getline(std::cin, source)) {
            break;
        }

        if (source == "exit") {
            break;
        }

        Lexer lexer(source);
        Parser parser(lexer);
        std::unique_ptr<Program> program = parser.parse_program();

        if (program) {
            std::cout << "AST generado:\n";
            std::cout << program->to_string() << std::endl;
        } else {
            std::cerr << "Error al generar el AST.\n";
        }
    }

    return 0;
}