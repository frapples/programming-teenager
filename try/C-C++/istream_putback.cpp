#include<iostream>
#include<cstdio>
#include<cstdlib>

using namespace std;
// cin.putback()方法接受一个字符，将其退回到cin流中。
// c语言中，类似此功能的函数是ungetc()，第一个参数是要退回的字符，第二个字符是FILE指针。

int main()
{
    
    int ch = std::cin.get();
    cout << (char)ch;
    
    std::cin.putback(ch);
    
    ch = std::cin.get();
    cout << (char)ch;
    return 0;
}
