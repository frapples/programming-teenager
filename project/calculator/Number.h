#ifndef __NUMBER_H__
#define __NUMBER_H__


#include<string>
#include"Integer.h"

class Number
{
public:
    Number(const Number &num);
    Number(const char* str);
    Number(std::string str);
    Number(long n=0);
    Number(double n);
    ~Number();

    Number& operator= (const Number &num);
    Number& operator= (const char* str);
    Number& operator= (std::string str);
    Number& operator= (long n);
    Number& operator= (double n);

    Number operator- () const;

    Number& operator+=(const Number &num);
    Number& operator-=(const Number &num);
    Number& operator*=(const Number &num);
    Number& operator/=(const Number &num);
    Number& operator%=(const Number &num);

    Number operator+ (const Number &num) const;
    Number operator- (const Number &num) const;
    Number operator* (const Number &num) const;
    Number operator/ (const Number &num) const;
    Number operator% (const Number &num) const;

    bool operator< (const Number &num) const;
    bool operator> (const Number &num) const;
    bool operator<=(const Number &num) const;
    bool operator>=(const Number &num) const;
    bool operator==(const Number &num) const;
    bool operator!=(const Number &num) const;

    Number power(const Number &pow) const;
    Number sin() const;
    Number cos() const;
    Number tan() const;
    Number ln()  const;
    Number log10() const;
    Number factorial() const;

    std::string to_str(int base = 10);


private:
    int sign;
    Integer integer;
    double decimal;

    bool debug_check() const;
    void turn();
    void abs_add(const Number &num);
    int  abs_sub(const Number &num);
    bool abs_lt(const Number &num);
    Number integer_mul_double(Integer& n, double real);
    int abs_compare(const Number &num) const;
    int compare(const Number &num) const;

};

double str_to_double(std::string str);
std::string double_to_str(double n);


#endif
