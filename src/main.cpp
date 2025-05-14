#include "core/lexer.h"
#include "core/parser.h"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {
    std::unordered_map<std::string, int> env;

    if (argc == 2) {
        std::ifstream file(argv[1]);
        if (!file.is_open()) {
            std::cerr << "Could not open file: " << argv[1] << "\n";
            return 1;
        }
        std::string line, content;
        while (std::getline(file, line)) content += line + "\n";
        try {
            Lexer lexer(content);
            Parser parser(lexer);
            auto statements = parser.parse();
            for (auto& stmt : statements)
                stmt->execute(env);
        } catch (std::exception& ex) {
            std::cerr << "Error: " << ex.what() << "\n";
        }
        return 0;
    }

    std::cout << "MiniLang Interpreter (type `exit` to quit)\n";
    std::string line;
    while (true) {
        std::cout << ">>> ";
        if (!std::getline(std::cin, line)) break;
        if (line == "exit") break;

        try {
            Lexer lexer(line);
            Parser parser(lexer);
            auto statements = parser.parse();
            for (auto& stmt : statements)
                stmt->execute(env);
        } catch (std::exception& ex) {
            std::cerr << "Error: " << ex.what() << "\n";
        }
    }

    return 0;
}
