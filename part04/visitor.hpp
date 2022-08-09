// forward declaration
class NumericConstant;
class BinaryExpr;
class UnaryExpr;
class IExprVisitor;

#include <list>

#ifndef VISITOR_H
#define VISITOR_H
#include "misc.hpp"

class IExprVisitor {
public:
    virtual double visit(NumericConstant& num) = 0;
    virtual double visit(BinaryExpr& bin) = 0;
    virtual double visit(UnaryExpr& un) = 0;
};

class TreeEvalVisitor : public IExprVisitor {
public:
    TreeEvalVisitor(){};
    double visit(NumericConstant& num);
    double visit(BinaryExpr& bin);
    double visit(UnaryExpr& un);
};

class FlattenVisitor : public IExprVisitor {
private:
    ItemList* makeItemList(OPERATOR op);
    ItemList* makeItemList(double num);

public:
    std::list<ItemList>* list;

    FlattenVisitor();
    ~FlattenVisitor();

    double visit(NumericConstant& num);
    double visit(BinaryExpr& bin);
    double visit(UnaryExpr& un);
};

#endif // !1
