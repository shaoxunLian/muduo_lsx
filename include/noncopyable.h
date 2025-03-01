#pragma once  //编译级别 更加轻便，防止重复包含，ifnodefine endif 这个时语言级别的，两者效果一样

namespace muduo{
class noncopyable
{
    /**
     * 派生类可以正常构构造和析构，但是不能拷贝构造和赋值。这种做法一定要好好学习一下！！
     */
    /*
    //让继承这个类的派生类都不可拷贝构造和赋值
    noncopyable被继承后，派生类对象可以正常的构造和析构，但派生类对象无法进行拷贝构造和赋值操作(因为父类要先进行拷贝构造或赋值构造)
    */
public:
    //把拷贝构造和赋值直接delete掉.参考https://blog.csdn.net/lcb_coconut/article/details/114803518
    noncopyable(const noncopyable&) = delete;
    noncopyable& operator=(const noncopyable&) = delete; //如果要做连续赋值的话就返回noncopyable&，不然就返回void
protected:
    noncopyable() = default;
    ~noncopyable() = default;
};
}