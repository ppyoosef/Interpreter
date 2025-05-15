#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"
#include "ast.h"

class Parser {
public:
    Parser(Lexer& l);
    std::vector<std::unique_ptr<Statement>> parse();

private:
    Lexer& lexer;
    Token current;

    void advance();
    void expect(TokenType type);
    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Statement> statement();
    std::unique_ptr<IfBlock> parseIf();
    std::unique_ptr<Expr> parseStatement();
    std::unique_ptr<WhileLoop> parseWhile();
    std::vector<std::unique_ptr<Statement>> parseBlock();

};
#endif
