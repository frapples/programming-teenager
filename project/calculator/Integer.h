
#ifndef __INTEGER_H__
#define __INTEGER_H__

#include<stddef.h>

struct IntegerBucket;

// 正整数
class Integer {
public:
    Integer(const Integer &num);
    Integer(const char* str);
    Integer(unsigned long n=0);
    ~Integer();

    Integer& operator= (const Integer &num);
    Integer& operator= (const char* str);
    Integer& operator= (unsigned long num);

    Integer  operator+ (const Integer &num) const;
    Integer& operator+=(const Integer &num);
    Integer  operator- (const Integer &num) const;
    Integer& operator-=(const Integer &num);
    Integer  operator* (const Integer &num) const;
    Integer  operator/ (const Integer &num) const;
    Integer  operator% (const Integer &num) const;


    bool operator< (const Integer &num) const;
    bool operator> (const Integer &num) const;
    bool operator<= (const Integer &num) const;
    bool operator>= (const Integer &num) const;
    bool operator==(const Integer &num) const;
    bool operator!=(const Integer &num) const;

    Integer& operator<<(unsigned count);
    Integer& operator>>(unsigned count);

    int compare(const Integer &num) const;

    Integer power(const Integer &pow) const;
    Integer power(unsigned pow) const;
    Integer factorial() const;

    char* to_c_str() const;

private:

    void mul10_then_add(int n);
    bool equal0() const;
    void assign0();
    void init0();

    void fill_high0(unsigned count);
    void fill_low0(unsigned count);
    void remove_high0();

    void add_high(int n);
    void add_low(int n);
    void remove_high(size_t count);
    void remove_low(size_t count);

    void copy_value(const Integer &num);


    IntegerBucket* head;
    IntegerBucket* tail;
    size_t size;
};



#endif
