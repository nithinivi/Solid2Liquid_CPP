#include "expression.h"
#include "visitor.h"
#include <limits>

TreeEvalVisitor::TreeEvalVisitor(){};
double TreeEvalVisitor::visit(NumericConstant& num) { return num.getValue(); }
double TreeEvalVisitor::visit(BinaryExpr& bin) {

    double left = bin.getLeft().accept(*this);
    double right = bin.getRight().accept(*this);

    switch (bin.getOp()) {
    case PLUS:
        return left + right;
    case MINUS:
        return left - right;
    case MUL:
        return left * right;
    case DIV:
        return left / right;
    default:
        break;
    }
    return std::numeric_limits<double>::quiet_NaN();
}

double TreeEvalVisitor::visit(UnaryExpr& bin) {

    double exp = bin.getExp().accept(*this);

    switch (bin.getOp()) {
    case PLUS:
        return exp;
    case MINUS:
        return -exp;
    default:
        break;
    }
    return std::numeric_limits<double>::quiet_NaN();
}