#include <cctype>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>

enum Token
{
    ILLEGAL_TOKEN = 0,
    TOK_NULL,

    TOK_PLUS,
    TOK_SUB,
    TOK_MUL,
    TOK_DIV,
    TOK_OPAREN,
    TOK_CPAREN,
    TOK_DOUBLE,

};

enum Operator
{
    ILLEGAL = -1,
    PLUS,
    MINUS,
    MUL,
    DIV
};


class RuntimeContext
{
public:
    RuntimeContext(){};
};

class Expr //
{
public:
    Expr();
    virtual ~Expr() = default;
    virtual double evaluate(RuntimeContext * ctx) = 0;
};

Expr::Expr()
{
}


class NumericConstant : public Expr
{
private:
    double value;

public:
    NumericConstant(double value);
    virtual ~NumericConstant();
    double evaluate(RuntimeContext * ctx);
};

NumericConstant::NumericConstant(double value)
{
    this->value = value;
}

NumericConstant::~NumericConstant()
{
}

double NumericConstant::evaluate(RuntimeContext * ctx)
{
    return value;
}


class BinaryExpr : public Expr
{
private:
    Expr *ex1, *ex2;
    Operator op;

public:
    BinaryExpr(Expr * left, Expr * right, Operator op);
    ~BinaryExpr();
    double evaluate(RuntimeContext * ctx);
};

BinaryExpr::BinaryExpr(Expr * left, Expr * right, Operator op)
{
    this->ex1 = left;
    this->ex2 = right;
    this->op = op;
}

BinaryExpr::~BinaryExpr()
{
    delete ex1;
    delete ex2;
}

double BinaryExpr::evaluate(RuntimeContext * ctx)
{
    switch (op)
    {
        case Operator::PLUS:
            return ex1->evaluate(ctx) + ex2->evaluate(ctx);
        case Operator::MINUS:
            return ex1->evaluate(ctx) - ex2->evaluate(ctx);
        case Operator::MUL:
            return ex1->evaluate(ctx) * ex2->evaluate(ctx);
        case Operator::DIV:
            return ex1->evaluate(ctx) / ex2->evaluate(ctx);
        default:
            throw std::runtime_error("Unsupported Operator");
    }
}


class UnaryExpr : public Expr
{
private:
    Expr * exp;
    Operator op;

public:
    UnaryExpr(Expr * exp, Operator op);
    ~UnaryExpr();
    double evaluate(RuntimeContext * ctx);
};

UnaryExpr::UnaryExpr(Expr * exp, Operator op)
{
    this->exp = exp;
    this->op = op;
}

UnaryExpr::~UnaryExpr()
{
    delete exp;
}


double UnaryExpr::evaluate(RuntimeContext * ctx)
{
    switch (op)
    {
        case Operator::PLUS:
            return exp->evaluate(ctx);
        case Operator::MINUS:
            return -exp->evaluate(ctx);
        default:
            throw std::runtime_error("Unsupported Operator");
    }
}


class Lexer
{
private:
    std::string expr;
    int index;
    int length;
    double number;

public:
    Lexer(std::string expr);
    ~Lexer();

    Token getToken();
    double getNumber();
};

Lexer::Lexer(std::string expr)
{
    this->expr = expr;
    this->length = expr.length();
    this->index = 0;
}

Lexer::~Lexer()
{
    expr.erase();
}

Token Lexer::getToken()
{
    Token token = ILLEGAL_TOKEN;
    while (index < length && (expr[index] == ' ' || expr[index] == '\t'))
    {
        index++;
    }

    if (index == length)
    {
        return TOK_NULL;
    }

    switch (expr[index])
    {
        case '+':
            token = TOK_PLUS;
            index++;
            break;
        case '-':
            token = TOK_SUB;
            index++;
            break;
        case '/':
            token = TOK_DIV;
            index++;
            break;
        case '*':
            token = TOK_MUL;
            index++;
            break;
        case '(':
            token = TOK_OPAREN;
            index++;
            break;
        case ')':

            token = TOK_CPAREN;
            index++;
            break;

        default:
            if (std::isdigit(expr[index]))
            {
                int numStartIndex = index;
                while (index < length && isdigit(expr[index]))
                    index++;
                std::string numStr = expr.substr(numStartIndex, index - numStartIndex);
                number = std::stod(numStr);
                token = TOK_DOUBLE;
            }
            else
                throw std::runtime_error("Unknown character found while tokenizing ");
    }
    return token;
}


double Lexer::getNumber()
{
    return number;
}


class RDParser : Lexer
{
private:
    Token current_token;
    Expr * expr();
    Expr * term();
    Expr * factor();

public:
    RDParser(std::string string);
    virtual ~RDParser();
    Expr * callExpr();
};

RDParser::RDParser(std::string string) : Lexer(string){};
RDParser::~RDParser()
{
}

Expr * RDParser::callExpr()
{
    current_token = getToken();
    return expr();
}


Expr * RDParser::expr()
{
    Token l_token;
    Expr * returnValue = term();
    while (current_token == TOK_PLUS || current_token == TOK_SUB)
    {
        l_token = current_token;
        current_token = getToken();
        Expr * right = term();
        returnValue = new BinaryExpr(right, returnValue, l_token == TOK_PLUS ? Operator::PLUS : Operator::MINUS);
    }
    return returnValue;
}


Expr * RDParser::term()
{
    Token l_token;
    Expr * returnValue = factor();
    while (current_token == TOK_MUL || current_token == TOK_DIV)
    {
        l_token = current_token;
        current_token = getToken();
        Expr * right = factor();
        returnValue = new BinaryExpr(right, returnValue, l_token == TOK_PLUS ? Operator::MUL : Operator::DIV);
    }
    return returnValue;
}


Expr * RDParser::factor()
{
    Token l_token;
    Expr * returnValue = NULL;

    if (current_token == TOK_DOUBLE)
    {
        returnValue = new NumericConstant(getNumber());
        current_token = getToken();
    }
    else if (current_token == TOK_OPAREN)
    {
        current_token = getToken();
        returnValue = expr();
        if (current_token != TOK_CPAREN)
            throw std::runtime_error("Missing Closing parens ");
        current_token = getToken();
    }
    else if (current_token == TOK_PLUS || current_token == TOK_SUB)
    {
        l_token = current_token;
        current_token = getToken();
        returnValue = factor();
        returnValue = new UnaryExpr(returnValue, l_token == TOK_PLUS ? Operator::PLUS : Operator::MINUS);
    }
    else
    {
        throw std::runtime_error("Illegal tokens ");
    }
    return returnValue;
}


int main(int argc, char * argv[])
{
    std::string expr_string = "20/5*2+4";

    try
    {
        RuntimeContext * rtx = new RuntimeContext();

        RDParser * parser = new RDParser(expr_string);
        Expr * nd = parser->callExpr();
        std::cout << nd->evaluate(rtx) << std::endl;
        delete parser;
        delete nd;
        delete rtx;
    }
    catch (std::string e)
    {
        std::cout << "error evaluating expression" << e << std::endl;
    }
    return 0;
}
