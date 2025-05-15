#include "parser.h"
#include <stdexcept>

Parser::Parser(Lexer& l) : lexer(l) {
    advance();
}

void Parser::advance() {
    current = lexer.nextToken();
}

void Parser::expect(TokenType type) {
    if (current.type != type) throw std::runtime_error("Unexpected token");
    advance();
}

std::unique_ptr<Expr> Parser::factor() {
    if (current.type == TokenType::Number) {
        int val = std::stoi(current.value);
        advance();
        return std::make_unique<Number>(val);
    } else if (current.type == TokenType::Identifier) {
        std::string name = current.value;
        advance();
        return std::make_unique<Variable>(name);
    } else if (current.type == TokenType::LParen) {
        advance();
        auto val = expression();
        expect(TokenType::RParen);
        return val;
    }
    throw std::runtime_error("Invalid factor");
}

std::unique_ptr<Expr> Parser::term() {
    auto left = factor();
    while (current.type == TokenType::Mul || current.type == TokenType::Div) {
        TokenType op = current.type;
        advance();
        auto right = factor();
        left = std::make_unique<BinOp>(std::move(left), op, std::move(right));
    }
    return left;
}

std::unique_ptr<Expr> Parser::expression() {
    auto left = term();
    while (current.type == TokenType::Plus || current.type == TokenType::Minus ||
           current.type == TokenType::Gt || current.type == TokenType::Lt || current.type == TokenType::Lte || current.type == TokenType::Gte 
           || current.type == TokenType::EqualEqual || current.type == TokenType::NotEqual) {
        TokenType op = current.type;
        advance();
        auto right = term();
        left = std::make_unique<BinOp>(std::move(left), op, std::move(right));
    }
    return left;
}

std::unique_ptr<Statement> Parser::statement() {
    if (current.type == TokenType::Identifier) {
        std::string name = current.value;
        advance();
        expect(TokenType::Assign);
        auto expr = expression();
        return std::make_unique<Assignment>(name, std::move(expr));
    } else if (current.type == TokenType::Print) {
        advance();
        auto expr = expression();
        return std::make_unique<Print>(std::move(expr));
    } else if (current.type == TokenType::If) {
        return parseIf();
    } else if (current.type == TokenType::While) {
        return parseWhile();
    }

    throw std::runtime_error("Invalid statement");
}

std::unique_ptr<WhileLoop> Parser::parseWhile() {
    advance(); //skip while
    auto cond = expression();
    auto body = parseBlock();
    auto whileBlock = std::make_unique<WhileLoop>(std::move(cond), std::move(body));

    return whileBlock;
}

std::unique_ptr<IfBlock> Parser::parseIf() {
    advance(); // skip 'if'
    auto cond = expression();
    auto ifBlock = std::make_unique<IfBlock>(std::move(cond));

    while (current.type != TokenType::Else && current.type != TokenType::End) {
        ifBlock->thenBranch.push_back(statement());
    }

    if (current.type == TokenType::Else) {
        advance(); // skip 'else'
        while (current.type != TokenType::End)
            ifBlock->elseBranch.push_back(statement());
    }

    expect(TokenType::End);
    return ifBlock;
}

std::vector<std::unique_ptr<Statement>> Parser::parse() {
    std::vector<std::unique_ptr<Statement>> statements;
    while (current.type != TokenType::Eof) {
        statements.push_back(statement());
    }
    return statements;
}

std::vector<std::unique_ptr<Statement>> Parser::parseBlock() {
    std::vector<std::unique_ptr<Statement>> block;
    while (current.type != TokenType::End) {
        block.push_back(statement());
    }
    expect(TokenType::End);
    return block;
}