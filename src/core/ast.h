#ifndef AST_H
#define AST_H
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include "lexer.h"

struct Expr {
    virtual ~Expr() = default;
    virtual int eval(std::unordered_map<std::string, int>& env) = 0;
};

struct Number : Expr {
    int value;
    Number(int v) : value(v) {}
    int eval(std::unordered_map<std::string, int>&) override { return value; }
};

struct Variable : Expr {
    std::string name;
    Variable(std::string n) : name(std::move(n)) {}
    int eval(std::unordered_map<std::string, int>& env) override {
        if (!env.count(name)) throw std::runtime_error("Undefined variable: " + name);
        return env[name];
    }
};

struct BinOp : Expr {
    std::unique_ptr<Expr> left, right;
    TokenType op;
    BinOp(std::unique_ptr<Expr> l, TokenType o, std::unique_ptr<Expr> r)
        : left(std::move(l)), op(o), right(std::move(r)) {}
    int eval(std::unordered_map<std::string, int>& env) override {
        int l = left->eval(env), r = right->eval(env);
        switch (op) {
            case TokenType::Plus: return l + r;
            case TokenType::Minus: return l - r;
            case TokenType::Mul: return l * r;
            case TokenType::Div: return l / r;
            case TokenType::Gt: return l > r;
            case TokenType::Lt: return l < r;
            default: throw std::runtime_error("Unsupported binary op");
        }
    }
};

struct Statement {
    virtual ~Statement() = default;
    virtual void execute(std::unordered_map<std::string, int>& env) = 0;
};

struct Assignment : Statement {
    std::string name;
    std::unique_ptr<Expr> value;
    Assignment(std::string n, std::unique_ptr<Expr> v)
        : name(std::move(n)), value(std::move(v)) {}
    void execute(std::unordered_map<std::string, int>& env) override {
        env[name] = value->eval(env);
    }
};

struct Print : Statement {
    std::unique_ptr<Expr> value;
    Print(std::unique_ptr<Expr> v) : value(std::move(v)) {}
    void execute(std::unordered_map<std::string, int>& env) override {
        std::cout << value->eval(env) << "\n";
    }
};

struct IfBlock : Statement {
    std::unique_ptr<Expr> condition;
    std::vector<std::unique_ptr<Statement>> thenBranch;
    std::vector<std::unique_ptr<Statement>> elseBranch;

    IfBlock(std::unique_ptr<Expr> cond) : condition(std::move(cond)) {}

    void execute(std::unordered_map<std::string, int>& env) override {
        auto& branch = condition->eval(env) ? thenBranch : elseBranch;
        for (auto& stmt : branch) stmt->execute(env);
    }
};

#endif
