#include"expression.h"
#include<cctype>
#include<iostream>
#include<map>
#include<stack>
#include<assert.h>

#include"Number.h"


ExpressionExecuter::ExpressionExecuter(std::string expr): expression(expr)
{
}


// 带数字带点的是数字
// 若干字母后面还带括号的是一目运算
// 符号是二目运算，-号除外（有时候是一目运算）
//
std::string ExpressionExecuter::calculate()
{
    std::stack<std::string> tmp;
    auto r = to_reverse_polish();
    for (size_t i = 0; i < r.size(); i++) {
        if (isdigit(r[i][0])) {
            tmp.push(r[i]);
        }
        else {
            assert(!tmp.empty());
            auto right = tmp.top();
            tmp.pop();

            assert(!tmp.empty());
            auto left = tmp.top();
            tmp.pop();

            tmp.push(this->op(r[i], left, right));
        }
    }

        assert(tmp.size() == 1);
        return tmp.top();
}


std::vector<std::string> ExpressionExecuter::to_reverse_polish()
{
    std::map<std::string, int> level;
    level["+"] = 1;
    level["-"] = 1;
    level["*"] = 2;
    level["/"] = 2;
    level["%"] = 2;
    level["!"] = 3;


    auto expr = expression;
    std::stack<std::string> tmp;
    std::vector<std::string> res;

    std::string unit;
    while ((unit = parse_next_unit(expr)) != "") {
        if (isdigit(unit[0])) {
            res.push_back(unit);

        } else if (unit == "(") {
            tmp.push(unit);

        } else if (unit == ")") {
            while (tmp.top() != "(") {
                assert(tmp.size() != 0);
                res.push_back(tmp.top());
                tmp.pop();
            }
            tmp.pop();

        } else {
            while (!tmp.empty() && level[tmp.top()] >= level[unit]) {
                res.push_back(tmp.top());
                tmp.pop();
            }
            tmp.push(unit);
        }
    }

    while (!tmp.empty()) {
        res.push_back(tmp.top());
        tmp.pop();
    }

    return res;
}


// 带数字带点的是数字单元
// 若干字母是字母型的运算符单元
// 符号也是运算符单元
//
std::string ExpressionExecuter::parse_next_unit(std::string& expr)
{
    if (expr == "")
        return "";

    if (std::string("+-*/%^!()").find(expr[0]) != std::string::npos) {
        auto res = expr.substr(0, 1);
        expr = expr.substr(1);
        return res;

    } else if (isdigit(expr[0])) {
        for (size_t i = 0; i < expr.size() + 1; i++) {
            if (i == expr.size() || !std::isdigit(expr[i])) {
                auto res = expr.substr(0, i);
                if (i >= expr.size())
                    expr = "";
                else
                    expr = expr.substr(i);
                return res;
            }
        }

    } else if (isalpha(expr[0])) {
        for (size_t i = 0; i < expr.size() + 1; i++) {
            if (i == expr.size() || !std::isalpha(expr[i])) {
                auto res = expr.substr(0, i);
                if (i >= expr.size())
                    expr = "";
                else
                    expr = expr.substr(i);
                return res;
            }
        }
    } else {
        throw new ParseException;
    }


    assert(false);
    return "";
}

std::string ExpressionExecuter::op(std::string op, std::string num1, std::string num2)
{
    Number n1(num1);
    Number n2(num2);

    if (op == "+") {
        return (n1 + n2).to_str();

    } else if (op == "-") {
        return (n1 - n2).to_str();

    } else if (op == "*") {
        return (n1 * n2).to_str();

    } else if (op == "/") {
        return (n1 / n2).to_str();
    } else if (op == "%") {
        return (n1 % n2).to_str();
    }
    return 0;
}

size_t ExpressionExecuter::match(std::string expr, size_t pos)
{
    assert(pos < expr.size());

    int deep = 0;
    for (size_t i = pos; i < expr.size(); i++) {
        if (expr[i] == '(')
            deep++;
        if (expr[i] == ')')
            deep--;
        if (deep == 0)
            return i;
    }

    return std::string::npos;
}
