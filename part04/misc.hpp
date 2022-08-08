#pragma once
#ifndef MISC_H
#define MISC_H
#include "expression.hpp"

enum ExprKind { Operator, Value };

class ItemList {
public:
    ExprKind knd;
    double value;
    OPERATOR op;

    ItemList() { this->op = OPERATOR::ILLEGAL; };

    bool setOperator(OPERATOR op) {
        op = op;
        knd = ExprKind::Operator;
        return true;
    };

    bool setValue(double value) {
        value = value;
        knd = ExprKind::Value;
        return true;
    };
};

class RUNTIME_CONTEXT {
public:
    RUNTIME_CONTEXT(){};
};

#endif // MISC_H
