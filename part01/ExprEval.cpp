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

class Stack
{
private:
    double * stack;
    int stack_pointer;

public:
    Stack();
    virtual ~Stack();

    void clear();
    void push(double number);
    double pop();
};

Stack::Stack()
{
    this->stack = new double[255];
    this->stack_pointer = 0;
}
Stack::~Stack()
{
    delete stack;
}

void Stack::clear()
{
    stack_pointer = 0;
}

void Stack::push(double number)
{
    if (stack_pointer == 255)
        throw std::runtime_error("Stack Overflow ");
    stack[stack_pointer++] = number;
}
double Stack::pop()
{
    if (stack_pointer == 0)
        throw std::runtime_error("Stack Overflow ");
    return stack[--stack_pointer];
}


class RDParser : Lexer
{
private:
    Token current_token;
    Stack * stack = new Stack();
    void expr();
    void term();
    void factor();

public:
    RDParser(std::string string);
    virtual ~RDParser();
    double callExpr();
};

RDParser::RDParser(std::string string) : Lexer(string){};
RDParser::~RDParser()
{
    delete stack;
}

double RDParser::callExpr()
{
    stack->clear();
    current_token = getToken();
    expr();
    double result = stack->pop();
    return result;
}

void RDParser::expr()
{
    Token l_token;
    term();
    while (current_token == TOK_PLUS || current_token == TOK_SUB)
    {
        l_token = current_token;
        current_token = getToken();
        term();
        double l = stack->pop();
        double r = stack->pop();
        stack->push(l_token == TOK_PLUS ? (l + r) : (r - l));
    }
}


void RDParser::term()
{
    Token l_token;
    factor();
    while (current_token == TOK_MUL || current_token == TOK_DIV)
    {
        l_token = current_token;
        current_token = getToken();
        factor();
        double l = stack->pop();
        double r = stack->pop();
        stack->push(l_token == TOK_MUL ? (l * r) : (r / l));
    }
}
void RDParser::factor()
{
    Token l_token;
    if (current_token == TOK_DOUBLE)
    {
        stack->push(getNumber());
        current_token = getToken();
    }
    else if (current_token == TOK_OPAREN)
    {
        current_token = getToken();
        expr();
        if (current_token != TOK_CPAREN)
            throw std::runtime_error("Missing Closing parens ");
        current_token = getToken();
    }
    else if (current_token == TOK_PLUS || current_token == TOK_SUB)
    {
        l_token = current_token;
        current_token = getToken();
        factor();
        double l = stack->pop();
        stack->push(l_token == TOK_PLUS ? (l) : (-l));
    }
    else
    {
        throw std::runtime_error("Illegal tokens ");
    }
}


int main(int argc, char * argv[])
{
    std::string expr_string = "20/5*2+4";

    try
    {
        RDParser * parser = new RDParser(expr_string);
        double nd = parser->callExpr();
        std::cout << nd << std::endl;
    }
    catch (std::string e)
    {
        std::cout << "error evaluating expression" << e << std::endl;
    }
    return 0;
}
