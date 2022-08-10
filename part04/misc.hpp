#include <iostream>
#include <stack>
#ifndef MISC_H
#define MISC_H
#include "expression.hpp"

enum ExprKind { Operator, Value };

class ItemList {
public:
    ExprKind knd;
    double value;
    OPERATOR op;

    ItemList() { this->op = ILLEGAL; };

    bool setOperator(OPERATOR op) {
        this->op = op;
        knd = ExprKind::Operator;
        return true;
    };

    bool setValue(double num) {
        value = num;
        knd = ExprKind::Value;
        return true;
    };
};

class RUNTIME_CONTEXT {
public:
    RUNTIME_CONTEXT(){};
};

#endif // MISC_H
