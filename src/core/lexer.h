#ifndef LEXER_H
#define LEXER_H
#include <string>
#include <vector>

enum class TokenType {
    Identifier, Number, Assign, Plus, Minus, Mul, Div,
    LParen, RParen, Print, If, Else, End,
    Gt, Lt, Gte, Lte, NotEqual, EqualEqual,
    Semi, Eof, While, Newline
};

struct Token {
    TokenType type;
    std::string value;
};

class Lexer {
public:
    Lexer(const std::string& src);
    Token nextToken();

private:
    std::string src;
    size_t pos;
    char current;

    char peek() const;
    char get();
    void advance();
    void skipWhitespace();
    Token identifier();
    Token number();
};
#endif
