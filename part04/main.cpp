#include "expression.hpp"
#include "parser.hpp"
#include <iostream>

std::list<ItemList>* flattenedExpr(std::string expr_string) {
    FlattenVisitor* flatten_visitor = new FlattenVisitor();
    RDParser* parser = new RDParser(expr_string);
    Expr* nd = parser->callExpr();
    double value = nd->accept(*flatten_visitor);
    delete parser;
    delete nd;
    return flatten_visitor->list;
};

double evaluateItemLists(std::list<ItemList> lsrs) {
    std::stack<double>* stk = new std::stack<double>;
    for (ItemList itemList : lsrs) {
        if (itemList.knd == Value)
            stk->push(itemList.value);

        else {

            double l = stk->top();
            stk->pop();
            double r = stk->top();
            stk->pop();

            switch (itemList.op) {
            case OPERATOR::PLUS:
                stk->push(l + r);
                break;
            case OPERATOR::MINUS:
                stk->push(l - r);
                break;
            case OPERATOR::MUL:
                stk->push(l * r);
                break;
            case OPERATOR::DIV:
                stk->push(l / r);
                break;

            default:
                break;
            }
        }
    }
    double result = stk->top();
    delete stk;
    return result;
}

int main(int argc, char* argv[]) {
    std::string expr_string = "20";

    try {
        std::list<ItemList>* lrs = flattenedExpr(expr_string);
        double value = evaluateItemLists(*lrs);
        std::cout << value << std::endl;
    } catch (std::string e) {
        std::cout << "error evaluating expression" << e << std::endl;
    }

    return 0;
}
