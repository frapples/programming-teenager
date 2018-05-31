
#include<iostream>
#include<string>
#include<cmath>
#include<sstream>
#include<assert.h>



#include "Number.h"

// sign 0 就是0. sign 1 正数， sign -1 负数

Number::Number(const Number &num): sign(num.sign), integer(num.integer), decimal(num.decimal)
{
}

Number::Number(double n)
{
    *this = n;
}

Number::Number(long n)
{
    *this = n;
}

Number::Number(const char* str)
{
    *this = str;
}

Number::Number(std::string str)
{
    *this = str;
}

Number& Number::operator=(const Number &num)
{
    sign = num.sign;
    integer = num.integer;
    decimal = num.decimal;
    return *this;
}

Number& Number::operator=(double n)
{
    sign = (n == 0 ? 0 : (n > 0 ? 1 : -1));
    n *= sign;
    integer = (unsigned long)n;
    decimal = n - (unsigned long)n;
    return *this;
}

Number& Number::operator=(long n)
{
    sign = (n == 0 ? 0 : (n > 0 ? 1 : -1));
    integer = n * sign;
    decimal = 0;
    return *this;
}

Number& Number::operator=(std::string str)
{
    sign = (str.size() != 0 && str[0] == '-') ? -1 : 1;

    auto pos = str.find('.');

    if (pos == str.npos) {
        integer = Integer(str.c_str());
        decimal = 0;
    } else {
        integer = Integer(str.substr(0, pos).c_str());
        decimal = str_to_double("0" + str.substr(pos));
    }

    return *this;
}

Number& Number::operator=(const char* str)
{
    *this = std::string(str);
    return *this;
}



Number Number::operator%(const Number &num) const
{
    Number tmp = *this;
    tmp %= num;
    return tmp;
}


Number& Number::operator%=(const Number &num)
{
    integer = integer % num.integer;
    return *this;
}

Number Number::operator*(const Number &num) const
{
    Number res = *this;
    res.integer = integer * num.integer;
    res.sign = sign * num.sign;
    return res;
}


Number& Number::operator*=(const Number &num)
{
    *this = *this * num;
    return *this;
}


Number Number::operator+(const Number &num) const
{
    Number res = *this;
    res += num;
    return res;
}


Number& Number::operator+=(const Number &num)
{
    if ((sign >= 0 && num.sign >= 0) || 
        (sign <= 0 && num.sign <= 0)) {
        abs_add(num);

        if (sign == 0)
            sign = num.sign;
    }


    if ((sign < 0 && num.sign > 0) ||
        (sign > 0 && num.sign < 0)) {
        sign *= abs_sub(num);
    }

    return *this;
}


Number Number::operator-(const Number &num) const
{
    Number res = *this;
    res -= num;
    return res;
}


Number& Number::operator-=(const Number &num)
{
    *this += -num;
    return *this;
}


Number Number::operator-() const
{
    Number res = *this;
    res.turn();
    return res;
}


Number Number::operator/(const Number &num) const
{
    Number res = *this;
    res.integer = integer / num.integer;
    res.sign = sign * num.sign;
    return res;
}


Number& Number::operator/=(const Number &num)
{
    //TODO:
    return *this;
}






bool Number::operator<(const Number &num) const
{
    return compare(num) == -1;
}

bool Number::operator<= (const Number &num) const
{
    return compare(num) != 1;
}

bool Number::operator>(const Number &num) const
{
    return compare(num) == 1;
}
bool Number::operator>= (const Number &num) const
{
    return compare(num) != -1;
}


bool Number::operator==(const Number &num) const
{
    return compare(num) == 0;
}
bool Number::operator!=(const Number &num) const
{
    return compare(num) != 0;
}






Number Number::power(const Number &pow) const
{
    //TODO:
    return *this;
}


Number Number::sin() const
{
    //TODO:
    return *this;
}


Number Number::tan() const
{
    //TODO:
    return *this;
}

Number Number::cos() const
{
    //TODO:
    return *this;
}


Number Number::factorial() const
{
    Number num;
    num.integer = integer.factorial();
    return num;
}


Number Number::ln()  const
{
    //TODO:
    return *this;
}


Number Number::log10() const
{
    //TODO:
    return *this;
}


std::string Number::to_str(int base)
{
    std::string res;
    if (sign == -1)
        res += '-';

    //FIXME:base为其它值的代码没写
    auto integerStr = integer.to_c_str();
    res += integerStr;
    delete integerStr;

    std::string decimalStr = double_to_str(decimal);
    auto pos = decimalStr.find('.');
    if (pos != decimalStr.npos)
        res += decimalStr.substr(pos);

    return res;
}

void Number::abs_add(const Number& num)
{
    integer += num.integer;
    decimal += num.decimal;

    if (decimal >= 1.0) {
        integer += 1;
        decimal -= 1;
    }
}

int Number::abs_sub(const Number& num)
{
    int absCmp = abs_compare(num);

    if (absCmp >= 0) {
        if (decimal < num.decimal) {
            decimal += 1;
            integer -= 1;
        }
        integer -= num.integer;
        decimal -= num.decimal;

    } else {
        int carry = 0;
        if (num.decimal < decimal)
            carry = 1;

        decimal = num.decimal + carry - decimal;
        integer = num.integer - carry - integer;
    }

    return absCmp;
}

int Number::abs_compare(const Number &num) const
{
    int integerCmp = integer.compare(num.integer);
    if (integerCmp != 0)
        return integerCmp;

    return (decimal == num.decimal) ? 0 : (decimal > num.decimal ? 1 : -1);
}

int Number::compare(const Number &num) const
{
    if (sign < num.sign)
        return -1;
    if (sign > num.sign)
        return 1;

    if (sign == 1)
        return abs_compare(num);

    if (sign == -1)
        return abs_compare(num) * -1;

    return 0;
}

Number integer_mul_double(Integer& n, double real)
{
    Integer n2(double_to_str(real * 100000 * 100000).c_str());
    std::string res = (n * n2).to_c_str();
    return Number(0L);

}


bool Number::debug_check() const
{
    return (sign == 1 || sign == -1 || sign == 0) &&
           (0.0 <= decimal && decimal < 1.0) && 
           ((integer == 0ul && decimal == 0) ? sign == 0 : true);
}

inline void Number::turn()
{
    assert(debug_check());
    sign *= -1;
}

Number::~Number()
{
    // std::cout << "析构函数：" << to_str() << " " << debug_check() << std::endl;
    // std::cout << sign << " " << decimal << std::endl;
    // assert(debug_check());
}

/************************************************/
double str_to_double(std::string str)
{
    double n;
    std::istringstream i(str);
    i >> n;
    return n;
}

std::string double_to_str(double n)
{
    std::ostringstream o;
    o.precision(16);
    // 不让其使用科学计数法
    o.setf(std::ios::fixed);
    o << n;

    // fixed会有多余的0, 去掉

    std::string str = o.str();
    for (size_t i = str.size(); i > 0; i--) {
        if (str[i - 1] != '0') {
            str = str.substr(0, i);
            break;
        }
    }

    assert(str.size() > 0);

    if (str[str.size() - 1] == '.')
        return str.substr(0, str.size() - 1);

    return str;
}

