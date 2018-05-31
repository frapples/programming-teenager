#include <memory>
#include <string>
#include <cstdio>

using namespace std;

int main()
{
    // shared_ptr采用引用计数的智能指针
    {
        string* p_s = new string("test");
        shared_ptr<string> p(p_s);
        printf("%d %s\n", p->length(), p->c_str());
    }


    {
        auto p = make_shared<string>("12345");
        printf("%d %s\n", p->length(), p->c_str());
    }

    {
        // unique_ptr是独占对象的智能指针，不能拷贝，不能赋值，不过可以移动
        unique_ptr<int> p_i(new int(12));
    }

    {
        // weak_ptr 是弱引用的智能指针，解决RC的循环引用问题
        auto p = make_shared<string>("12345");
        // 从一个shared_ptr创建weak_ptr
        weak_ptr<string> wp(p);
        // 使用weak_ptr需要使用lock()来访问对象，它会检查其指向的对象是否存在
        if (auto np = wp.lock()) {
            printf("%s\n", np->c_str());
        }
    }

    return 0;
}

