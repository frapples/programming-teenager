#include <cstdio>
#include "scopeguard.h"

int main()
{
    int* p_i = new int;
    *p_i = 1;

    // ScopeGuard scopeGuard([&] {
    //         std::printf("回收垃圾中。。。");
    //         delete p_i;
    //     });

    // 版本1
    // make_scope_guard([&] {
    //         std::printf("回收垃圾中。。。");
    //         delete p_i;
    //     });

    // 版本2
    // ScopeGuardOnExit() + [&] {
    //     std::printf("回收垃圾中。。。\n");
    //     delete p_i;
    // };

    ON_SCOPE_EXIT {
            std::printf("回收垃圾中。。。\n");
            delete p_i;
    };
    return 0;
}
