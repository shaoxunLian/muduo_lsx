#include "EventLoopThread.h"
#include "EventLoop.h"
#include <memory>
using namespace std;
EventLoopThread::EventLoopThread(const ThreadInitCallback &cb,
                                const string &name):
                                loop_(nullptr),
                                exiting_(false),
                                thread_(bind(&EventLoopThread::threadFunc, this), name),
                                mutex_(),
                                cond_(), //注意这里初始化的时候也不用放mutex，人家也没有这样的初始化构造函数
                                callback_(cb)
{}
EventLoopThread::~EventLoopThread()
{
    exiting_ = true;
    if(loop_ != nullptr)
    {
        loop_->quit();
        thread_.join(); //等待这个子线程结束。
    }
}

EventLoop* EventLoopThread::startLoop()
{
    thread_.start(); //启动底层新线程
    //必须要等待threadFunc的loop_初始化好了下面才能继续执行
    EventLoop* loop = nullptr;
    {   
        unique_lock<mutex> lock(mutex_);
        while(loop_ == nullptr)
        {//https://segmentfault.com/a/1190000006679917 
        //为什么要判断loop_ == nullptr，因为条件变量被通知后，挂起的线程就被唤醒，但是唤醒也有可能是假唤醒，
        //或者是因为超时等异常情况，所以被唤醒的线程仍要检查条件是否满足，所以 wait 是放在条件循环里面。
        //cv.wait(lock, [] { return ready; }); 相当于：while (!ready) { cv.wait(lock); }。
            cond_.wait(lock);//这么看if(cond_.notify_one()?){上锁执行下一步}else{解锁等待}
        }
        loop = loop_; //这里相当于做了一个线程间的通信操作，另外一个线程初始化的loop_，在这个线程里面获取
    }
    //这里为什么不直接返回loop_，因为操作loop_的时候需要上锁，为了及时解锁完成相应操作，先定义一个EventLoop*指针然后赋值，之后解锁返回
    return loop;
}

void EventLoopThread::threadFunc()
{
    //该方法是在单独的新线程里面运行的。
    EventLoop loop; //创建一个独立的EventLoop,和上面的线程是一一对应的，
    //在面试的时候要是能把one loop per thread给说到具体的方法上，就能认同你了。
    if(callback_)
    {
        callback_(&loop);
    }
    {
        unique_lock<mutex> lock(mutex_);
        loop_ = &loop; //EventLoopThread里面绑定的loop对象就是在这个线程里面创建的
        cond_.notify_one();
    }
    //这个loop是在线程函数的线程栈上创建的
    loop.loop(); //EventLoop loop => Poller.poll开始监听远端连接或者已连接的fd的事件
    unique_lock<mutex> lock(mutex_);
    loop_ = nullptr; //如果走到这里说明服务器程序要关闭了，不进行事件循环了。
    //为什么这里要加锁，大概是悲观锁的思想吧，这个loop_毕竟会被多个程序使用
}

/*
EventLoop *loop_;
bool exiting_;
Thread thread_;
std::mutex mutex_;
std::condition_variable cond_;
ThreadInitCallback callback_;
*/