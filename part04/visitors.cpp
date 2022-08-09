#include "expression.hpp"
#include "visitor.hpp"
#include <iostream>
#include <limits>

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

FlattenVisitor::FlattenVisitor() { this->list = new std::list<ItemList>; }

ItemList* FlattenVisitor::makeItemList(OPERATOR op) {
    ItemList* temp = new ItemList();
    temp->setOperator(op);
    return temp;
}
ItemList* FlattenVisitor::makeItemList(double num) {
    ItemList* temp = new ItemList();
    temp->setValue(num);
    return temp;
}

double FlattenVisitor::visit(NumericConstant& num) {

    std::cout << num.getValue() << std::endl;
    ItemList* itemList = FlattenVisitor::makeItemList(num.getValue());
    list->push_back(*itemList);
    return 0;
}
double FlattenVisitor::visit(BinaryExpr& bin) {
    double left = bin.getLeft().accept(*this);
    double right = bin.getRight().accept(*this);
    return 0;
};
double FlattenVisitor::visit(UnaryExpr& un) {
    double exp = un.getExp().accept(*this);
    ItemList* itemList = FlattenVisitor::makeItemList(un.getOp());
    list->push_back(*itemList);
    return 0;
};
