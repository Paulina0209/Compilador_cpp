#include <iostream>
#include <string>
#include <memory>
#include <sstream>
#include "lexer.h"
#include "parser.h"
#include "evaluator.h"
#include "environment.h"

int main() {
    std::cout << "Escribe tu programa (usa varias líneas si quieres). Escribe 'run' para ejecutarlo o 'exit' para salir.\n";

    std::string line;
    std::stringstream source_buffer;

    auto env = std::make_shared<Environment>();  // ✅ entorno persistente entre ejecuciones

    while (true) {
        std::cout << ">> ";
        std::getline(std::cin, line);

        if (line == "exit") break;

        if (line == "run") {
            std::string source = source_buffer.str();
            source_buffer.str("");
            source_buffer.clear();

            Lexer lexer(source);
            Parser parser(lexer);
            auto program = parser.parse_program();

            if (!parser.errors.empty()) {
                std::cerr << "Errores de parsing:\n";
                for (const auto& err : parser.errors) {
                    std::cerr << "  - " << err << "\n";
                }
                continue;
            }

            auto result = eval(program.get(), env);
            if (result) {
                std::cout << "Resultado: " << result->inspect() << "\n";
            } else {
                std::cout << "Resultado nulo o error de ejecución.\n";
            }

            continue;
        }

        source_buffer << line << "\n";
    }

    return 0;
}