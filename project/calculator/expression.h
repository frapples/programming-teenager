#ifndef __EXPERSSION_H__
#define __EXPERSSION_H__ 

#include<string>
#include<stdexcept>
#include<vector>


class ExpressionExecuter
{
public:
    ExpressionExecuter(std::string expr);
    std::string calculate();

private:
    std::string expression;
    std::string op(std::string op, std::string num1, std::string num2);

    std::vector<std::string> to_reverse_polish();

    std::string parse_next_unit(std::string &expr);
    size_t match(std::string expr, size_t pos);
};

class ParseException: public std::exception
{

};

#endif
