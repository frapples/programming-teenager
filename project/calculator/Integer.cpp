

#include<stdlib.h>
#include<assert.h>

#include"Integer.h"


/* 供内部使用的链表结构 */
struct IntegerBucket
{
    char bit;
    IntegerBucket* prev;
    IntegerBucket* next;

    void* operator new(size_t size);
    void  operator delete(void* p);
};


Integer::Integer(unsigned long n)
{
    init0();
    (*this) = n;
}

Integer::Integer(const Integer &num)
{
    init0();
    copy_value(num);
}

Integer::Integer(const char* str)
{
    init0();
    (*this) = str;

}

Integer& Integer::operator= (const char* str)
{
    assign0();
    for (auto p = str; *p != '\0'; p++) {
        if ('0' <= *p && *p <= '9') {
            mul10_then_add(*p - '0');
        }
    }
    return (*this);
}

Integer& Integer::operator= (unsigned long n)
{
    assign0();
    while (n != 0) {
        add_high(n % 10);
        n /= 10;
    }
    (*this) >> 1; // 除以10
    return *this;
}

Integer& Integer::operator= (const Integer &num)
{
    assign0();
    copy_value(num);
    return *this;
}

Integer::~Integer()
{
    size_t i = 0;
    auto current = head;
    while (current != NULL) {
        auto next = current->next;
        delete current;
        current = next;
        i++;
    }

    assert(size == i);
}

void Integer::copy_value(const Integer &num)
{
    assert(equal0());

    for (auto p = num.head; p != NULL; p = p->next)
        mul10_then_add(p->bit);

    assert(size == num.size);
}


Integer Integer::operator+(const Integer &num) const
{
    Integer n = num;
    n += (*this);
    return n;
}


Integer Integer::operator-(const Integer &num) const
{

    Integer n = (*this);
    n -= num;
    return n;
}


Integer& Integer::operator+=(const Integer &num)
{
    if (num.size > size)
        fill_high0(num.size - size);

    int carry = 0;
    auto current = tail, p = num.tail;
    while (current != NULL) {

        int add = current->bit + (p == NULL ? 0 : p->bit) + carry;
        current->bit = add % 10;
        carry = add / 10;

        current = current->prev;
        if (p != NULL) p = p->prev;
    }
    if (carry == 1) {
        add_high(1);
    }

    return (*this);
}


// 只能是大数减小数
Integer& Integer::operator-=(const Integer &num)
{
    assert(*this >= num && "正整数类只允许大数减小数");

    int carry = 0;
    auto current = tail, p = num.tail;
    while (current != NULL) {
        int tmp = p == NULL ? 0 : p->bit;
        current->bit += carry;
        if (current->bit < tmp) {
            current->bit += 10;
            carry = -1;
        } else {
            carry = 0;
        }
        current->bit -= tmp;

        current = current->prev;
        if (p != NULL) p = p->prev;
    }

    remove_high0();
    return (*this);
}


/*
Integer Integer::operator*(const Integer &num)
{
    Integer res;

    int bitWeight = 0; // 位权
    for (auto p = tail; p != NULL; p = p->prev, bitWeight++) {
        Integer tmp = num;
        res += (tmp.mul_to(p->bit)) << bitWeight;
    }
    return res;
}
*/

/*
Integer& Integer::mul_to(unsigned num)
{
    assert(num >= 0 && num <= 9);
    assert(head->prev == NULL);

    int carry = 0;
    for (auto p = tail; p != NULL; p = p->prev) {
        int res = p->bit * num + carry;
        p->bit = res % 10;

        // 进位
        carry = res / 10;
        }

    if (carry > 0) {
        (*this) >> 1;
        head->bit = carry;
    }

    return (*this);
}
*/
// 改了个效率更高的写法
Integer Integer::operator*(const Integer &num) const
{

    int resSize =  num.size + size + 2;
    
    
    auto bits = new char[resSize]();

    size_t weight1 = 0;
    for (auto p1 = tail; p1 != NULL; p1 = p1->prev, weight1++) {
        size_t weight2 = 0;
        for (auto p2 = num.tail; p2 != NULL; p2 = p2->prev, weight2++) {

            assert(weight1 < size && weight2 < num.size);

            bits[weight1 + weight2] += p1->bit * p2->bit;
            int tmp = bits[weight1 + weight2];
            bits[weight1 + weight2] = tmp % 10;

            bits[weight1 + weight2 + 1] += tmp / 10;
        }
    }

    Integer res;
    bool frist = true;
    for (size_t i = resSize; i >= 1; i--) {

        assert(0 <= bits[i - 1] && bits[i - 1] <= 9);

        if (bits[i - 1] != 0)
            frist = false;

        if (!frist) {
            res.mul10_then_add(bits[i - 1]);
        }
    }

    delete[] bits;
    return res;
}


Integer Integer::operator%(const Integer &num) const
{
    return (*this) - (*this) / num * num;
}

Integer Integer::operator/(const Integer &num) const
{
    // Integer cache[] = {0, num * 1, num * 2, num * 3, num * 4, num * 5, num * 6, num * 7, num * 8, num * 9};
    // Integer res;
    assert(!(num.equal0()) && "被除数为0");

    if (equal0())
        return *this;

    Integer res;

    Integer tmp = *this;
    size_t weight = size - 1;
    for (auto p = head; p != NULL; p = p->next, weight--)
    {
        int b = -1;
        for (int i = 1; i <= 10; i++) {
            if (((num * i) << weight) > tmp) {
                b = i - 1;
                break;
            }
        }
        assert(b != -1);
        res.mul10_then_add(b);
        tmp -= ((num * b) << weight);
    }
    res.remove_high0();

    return res;
}

bool Integer::operator<(const Integer &num) const
{
    return compare(num) == -1;
}



bool Integer::operator<= (const Integer &num) const
{
    return compare(num) != 1;
}

bool Integer::operator>(const Integer &num) const
{
    return compare(num) == 1;
}
bool Integer::operator>= (const Integer &num) const
{
    return compare(num) != -1;
}


bool Integer::operator==(const Integer &num) const
{
    return compare(num) == 0;
}
bool Integer::operator!=(const Integer &num) const
{
    return compare(num) != 0;
}

int Integer::compare(const Integer &num) const
{
    if (size > num.size)
        return 1;
    if (size < num.size)
        return -1;

    for (auto p1 = head, p2 = num.head;
         p1 != NULL && p2 != NULL; 
         p1 = p1->next, p2 = p2->next) {

        if (p1->bit > p2->bit)
            return 1;
        if (p1->bit < p2->bit)
            return -1;
    }
    return 0;
}


Integer Integer::factorial() const
{
    Integer sum = 1;
    for (Integer i = 1; i <= *this; i += 1) {
        sum = sum * i;
    }
    return sum;
}


// 用二分算法计算幂值
Integer Integer::power(unsigned pow) const
{
        if (pow == 0)
            return Integer(1);

        Integer tmp = power(pow / 2);
        tmp = tmp * tmp;

    if (pow % 2 == 1)
        tmp = tmp * (*this);

    return tmp;
}

Integer Integer::power(const Integer &pow) const
{
    if (pow.equal0())
        return Integer(1);


    Integer tmp = power(pow / 2);
    tmp = tmp * tmp;

    if (pow % 2 == 1)
       tmp = tmp * (*this);

       return tmp;
}

char* Integer::to_c_str() const
{
    char* str = new char[size + 1];
    auto t = str;
    for (auto p = head; p != NULL; p = p->next) {

        assert(p->bit >= 0 && p->bit <= 9);

        *(t++) = p->bit + '0';
    }
    *t = '\0';
    return str;
}

/***********************************************/

// 左移动乘10 右移除以10
Integer& Integer::operator<<(unsigned count)
{
    if (!equal0())
        fill_low0(count);
        
    return *this;
}

Integer& Integer::operator>>(unsigned count)
{
    if (!equal0()) {
        if (count >= size)
            assign0();
        else
            remove_low(count);
    }
    return *this;
}

// 把当前数值乘10再加上n
void Integer::mul10_then_add(int n)
{
    assert(0 <= n && n <= 9);
    
    if (equal0())
        tail->bit = n;
    else
        add_low(n);
}


// 判断值是否为0，注意有多余的高位0时不算值为0
bool Integer::equal0() const
{
    return size == 1 && tail->bit == 0;
}

// assign0和init0很像，都是把值设置为0. 
// 区别是assign0会释放已经占用的多余的内存空间。且assign0对未初始化的状态的行为未定义
// 所以初始化时置0请用init0
void Integer::assign0()
{
    remove_low(size - 1);
    tail->bit = 0;
}

void Integer::init0()
{
    head = tail = new IntegerBucket;
    tail->bit = 0;
    head->next = NULL;
    head->prev = NULL;
    size = 1;
}

// 低位补count个0
void Integer::fill_low0(unsigned count)
{
    assert(head != NULL && tail != NULL);

    for (size_t i = 0; i < count; i++) {
        add_low(0);
    }
}

// 高位补count个0
void Integer::fill_high0(unsigned count)
{
    assert(head != NULL && tail != NULL);

    for (size_t i = 0; i < count; i++) {
        add_high(0);
    }
}



// 移除所有无意义的高位0
void Integer::remove_high0()
{
    while (head->bit == 0) {
        if (equal0())
            return;

        remove_high(1);
    }
}

// 下面两个函数分别在低位和高位增加一位，值为n
void Integer::add_low(int n)
{
    assert(0 <= n && n <= 9);

    tail->next = new IntegerBucket;
    tail->next->bit = n;
    tail->next->prev = tail;
    tail->next->next = NULL;

    tail = tail->next;
    size++;
}

void Integer::add_high(int n)
{
    assert(0 <= n && n <= 9);

    auto newNode = new IntegerBucket;
    newNode->bit = n;
    newNode->next = head;
    newNode->prev = NULL;
    head->prev = newNode;
    size++;

    head = newNode;
}

// 下面两个函数会把节点切掉count个直到剩一个
void Integer::remove_high(size_t count)
{
    for (size_t i = 0; i < count; i++) {
        if (head->next == NULL)
            return;
        head = head->next;
        delete head->prev;
        size--;
        head->prev = NULL;
    }
}

void Integer::remove_low(size_t count)
{
    for (size_t i = 0; i < count; i++) {
        if (tail->prev == NULL)
            return;

        tail = tail->prev;
        delete tail->next;
        size--;
        tail->next = NULL;
    }
}

/* ------------------------------------------------------------------ */

#define ALLOC_SIZE 100000
static char alloc[ALLOC_SIZE * sizeof(IntegerBucket)];
static void* freeArray[ALLOC_SIZE];
static size_t freeSize = 0;
static bool first = true;

void* IntegerBucket::operator new(size_t size)
{
    assert(size == sizeof(IntegerBucket));

    if (first) {
        first = false;
        for (size_t i = 0; i < ALLOC_SIZE; i++)
            freeArray[freeSize++] = alloc + i * sizeof(IntegerBucket);
    }

    assert(freeSize != 0 && "内存池用完了");
    return freeArray[--freeSize];
}

void  IntegerBucket::operator delete(void* p)
{
    if (p) {
        freeArray[freeSize++] = p;
    }
}
