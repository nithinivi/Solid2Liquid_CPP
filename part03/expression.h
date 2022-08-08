
#pragma once
#ifndef DEBUG_H
#define DEBUG_H
#include "visitor.h"

enum OPERATOR { ILLEGAL = -1, PLUS, MINUS, MUL, DIV };

class Expr //
{
public:
    Expr();
    virtual ~Expr() = default;
    virtual double accept(IExprVisitor& expr_visitor) = 0;
};

Expr::Expr() {}

class NumericConstant : public Expr {
private:
    double value;

public:
    NumericConstant(double value);
    ~NumericConstant();
    double getValue();
    double accept(IExprVisitor& expr_visitor);
};

class BinaryExpr : public Expr {
private:
    Expr *left, *right;
    OPERATOR op;

public:
    BinaryExpr(Expr* l, Expr* r, OPERATOR op);
    ~BinaryExpr();
    Expr& getLeft();
    Expr& getRight();
    OPERATOR getOp();
    double accept(IExprVisitor& expr_visitor);
};

class UnaryExpr : public Expr {
private:
    Expr* exp;
    OPERATOR op;

public:
    UnaryExpr(Expr* exp, OPERATOR op);
    ~UnaryExpr();
    Expr& getExp();
    OPERATOR getOp();
    double accept(IExprVisitor& expr_visitor);
};

#endif // DEBUG
