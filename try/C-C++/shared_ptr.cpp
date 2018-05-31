#include<string>
#include<iostream>
#include<sstream>
#include<vector>
#include<memory>

using namespace std;

// shared_ptr是智能指针，位于boost库和c++11以上的STL库中。
// c++11以上的STL库中，位于头文件memory

int main()
{
    shared_ptr<vector<void *> > p(new vector<void *>);
    
    cout << p;


    return 0;
}
