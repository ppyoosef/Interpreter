#include "lexer.h"
#include <cctype>
#include <stdexcept>

Lexer::Lexer(const std::string& src) : src(src), pos(0), current(src[0]) {}

void Lexer::advance() {
    if (++pos < src.size())
        current = src[pos];
    else
        current = '\0';
}

void Lexer::skipWhitespace() {
    while (isspace(current)) advance();
}

Token Lexer::identifier() {
    std::string value;
    while (isalnum(current) || current == '_') {
        value += current;
        advance();
    }
    if (value == "print") return {TokenType::Print, value};
    if (value == "if") return {TokenType::If, value};
    if (value == "else") return {TokenType::Else, value};
    if (value == "end") return {TokenType::End, value};
    return {TokenType::Identifier, value};
}

Token Lexer::number() {
    std::string value;
    while (isdigit(current)) {
        value += current;
        advance();
    }
    return {TokenType::Number, value};
}

Token Lexer::nextToken() {
    skipWhitespace();
    if (current == '\0') return {TokenType::Eof, ""};

    if (isalpha(current)) return identifier();
    if (isdigit(current)) return number();

    char ch = current;
    advance();
    switch (ch) {
        case '=': return {TokenType::Assign, "="};
        case '+': return {TokenType::Plus, "+"};
        case '-': return {TokenType::Minus, "-"};
        case '*': return {TokenType::Mul, "*"};
        case '/': return {TokenType::Div, "/"};
        case '(': return {TokenType::LParen, "("};
        case ')': return {TokenType::RParen, ")"};
        case '>': return {TokenType::Gt, ">"};
        case '<': return {TokenType::Lt, "<"};
        case ';': return {TokenType::Semi, ";"};
    }

    throw std::runtime_error("Unknown character: " + std::string(1, ch));
}
