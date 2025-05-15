#include "lexer.h"
#include <cctype>
#include <stdexcept>
#include <iostream>

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

char Lexer::peek() const {
    return pos < src.size() ? src[pos] : '\0';
}

char Lexer::get() {
    return pos < src.size() ? src[pos++] : '\0';
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
    if (value == "while") return {TokenType::While, value};
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

    if (ch == '>') {
        if (current == '=') {
            advance();
            return {TokenType::Gte, ">="};
        }

        return {TokenType::Gt, ">"};
    }

    if (ch == '<') {
        if (current == '=') {
            advance();
            return {TokenType::Lte, "<="};
        }

        return {TokenType::Lt, "<"};
    }

    if (ch == '=') {
        if (current == '=') {
            advance();
            return {TokenType::EqualEqual, "=="};
        }

        return {TokenType::Assign, "="};
    }

    if (ch == '!') {
        if (current == '=') {
            advance();
            return {TokenType::NotEqual, "!="};
        }

        throw std::runtime_error("Unexpected character after '!'");
    }


    if (ch == '+') return {TokenType::Plus, "="};
    if (ch == '-') return {TokenType::Minus, "-"};
    if (ch == '*') return {TokenType::Mul, "*"};
    if (ch == '/') return {TokenType::Div, "/"};
    if (ch == '(') return {TokenType::LParen, "()"};
    if (ch == ')') return {TokenType::RParen, ")"};
    // if (ch == '=') return {TokenType::Assign, "="};
    if (ch == '\n') return {TokenType::Newline, "\n"};

    throw std::runtime_error("Unexpected character");
}
