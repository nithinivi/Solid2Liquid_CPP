// forward declaration
class NumericConstant;
class BinaryExpr;
class UnaryExpr;
class IExprVisitor;

#ifndef VISITOR_H
#define VISITOR_H

class IExprVisitor {
public:
    virtual double visit(NumericConstant& num) = 0;
    virtual double visit(BinaryExpr& bin) = 0;
    virtual double visit(UnaryExpr& un) = 0;
};

class StackEvaluator : public IExprVisitor {
public:
    double visit(NumericConstant& num);
    double visit(BinaryExpr& bin);
    double visit(UnaryExpr& un);
};

class TreeEvalVisitor : public IExprVisitor {
public:
    TreeEvalVisitor();
    double visit(NumericConstant& num);
    double visit(BinaryExpr& bin);
    double visit(UnaryExpr& un);
};

#endif // !1
