#include<iostream>
#include<string>
#include<sstream>

#include"Number.h"
#include<assert.h>

#include<stdlib.h>
#include<time.h>

using namespace std;


void test();

int main(void)
{
    srand(time(NULL));

    Number a = "5673.01";
    Number b = "1099.99";
    std::cout << a.to_str() << std::endl;
    std::cout << b.to_str() << std::endl;
    std::cout << (a + b).to_str() << std::endl;
    // std::cout << Integer(7000).factorial().to_c_str() << std::endl;
    test();
    return 0; 
}

bool test_operator(double a, double b, char op)
{
    Number na = a;
    Number nb = b;
    double res;
    Number nres;

    if (op == '+') {
        res = a + b;
        nres = na + nb;
    } else if (op == '-') {
        res = a - b;
        nres = na - nb;
    } else if (op == '*') {
        res = a * b;
        nres = na * nb;
    } else if (op == '/') {
        res = a / b;
        nres = na / nb;
    } else {
        assert(false && "op传参传错了");
    }

    double diff = res - str_to_double(nres.to_str());
    diff = diff >= 0 ? diff : -diff;

    if (diff >= 0.001)
        std::cout << diff << "\t" << res << "\t" << nres.to_str() << endl;
    return diff < 0.001;
}

void test_error_report(double a, double b, char op)
{
    std::cout << "测试不通过：" << a << " "<< op << " " << b << std::endl;
}

double rand_double()
{
    double n = rand() % 10000 + (double)(rand() % 10000) / 10000;
    if (rand() % 2)
        n = -n;
    return n;
}

void test()
{
    int count = 10000;
    for (int i = 0; i < count; i++) {
        double a = rand_double();
        double b = rand_double();
        if (!test_operator(a, b, '+'))
            test_error_report(a, b, '+');

        if (!test_operator(a, b, '-'))
            test_error_report(a, b, '-');
    }
}



