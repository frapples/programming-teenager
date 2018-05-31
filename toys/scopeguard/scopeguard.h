// 参考： https://github.com/hjcapple/ScopeGuard/blob/master/src/ScopeGuard.h
#ifndef __SCOPE_GUARD__H__

#include <utility>

template <typename Function>
class ScopeGuard {
public:
    ScopeGuard(Function&& f) : function(std::forward<Function>(f))
    {
    }

    ~ScopeGuard() {
        function();
    }

    // 这个函数似乎不能删除，std::forward需要它
    ScopeGuard(const ScopeGuard&) = default;
    // 这个语法是将它定义为删除的函数，你就无法使用了（之前是将它们设为私有达到）
    ScopeGuard& operator=(const ScopeGuard&) = delete;
    Function function;
};


template <typename Function>
ScopeGuard<Function> make_scope_guard(Function&& f)
{
    return ScopeGuard<Function>(std::forward<Function>(f));
}


enum class ScopeGuardOnExit
    {
    };

template <typename Function>
inline ScopeGuard<Function> operator+(ScopeGuardOnExit, Function&& fn)
{
    return ScopeGuard<Function>(std::forward<Function>(fn));
}

#define __SCOPEGUARD_CONCATENATE_IMPL(s1, s2) s1##s2
#define __SCOPEGUARD_CONCATENATE(s1, s2) __SCOPEGUARD_CONCATENATE_IMPL(s1, s2)

#define ON_SCOPE_EXIT                                                   \
    auto __SCOPEGUARD_CONCATENATE(ext_exitBlock_, __LINE__) = ScopeGuardOnExit() + [&]()


#endif // __SCOPE_GUARD__H__
