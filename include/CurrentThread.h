#pragma once

#include <unistd.h>
#include <sys/syscall.h>

namespace CurrentThread
{
    extern __thread int t_cachedTid; //通过__thread 修饰的变量，在线程中地址都不一样，
    //__thread变量每一个线程有一份独立实体（相当于每个线程都有一份独立的拷贝），各个线程的值互不干扰。

    void cacheTid();

    //inline内联函数，只在当前文件起作用
    inline int tid()
    {
        //相当于if(t_cachedTid == 0)的意思，t_cachedTid等于0说明还没有获取过tid
        if (__builtin_expect(t_cachedTid == 0, 0)) //提供给程序员使用的，目的是将“分支转移”的信息提供给编译器，这样编译器可以对代码进行优化，以减少指令跳转带来的性能下降。
        { //这里的意思时说 t_cachedTid == 0的可能性很小，不过如果t_cachedTid==0那就要调用cacheTid获取线程的tid了。
            cacheTid();
        }
        return t_cachedTid;
    }
}