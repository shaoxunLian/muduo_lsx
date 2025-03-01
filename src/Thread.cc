#include "Thread.h"
#include "CurrentThread.h"
#include <semaphore.h>
using namespace std;

atomic<int> numCreated_(0);

Thread::Thread(ThreadFunc func, const std::string &name): 
        started_(false), 
        joined_(false), //是join线程还是detach线程。
        tid_(0), 
        func_(std::move(func)),
        name_(name)
{
    setDefaultName();
}
Thread::~Thread(){
    if(started_ && !joined_){
        thread_->detach(); //分离线程
    }
}
void Thread::start()
{
    started_ = true;
    sem_t sem;
    sem_init(&sem, false, 0);
    thread_ = shared_ptr<thread>(new thread([&] () {
        //获取线程的tid值
        tid_ = CurrentThread::tid(); //子线程获取当前所在线程的tid，注意执行到这一行已经是在新创建的子线程里面了。
        sem_post(&sem);//作用是给信号量的值加上一个“1”。
        func_(); //开启一个新线程，专门执行一个线程函数。
    }));
    //这里必须等待上面的新线程获取了它的tid值才能继续执行。
    //sem_post()使信号量加一，本线程调用sem_wait()函数阻塞等待，信号量来了方可退出阻塞。
    sem_wait(&sem);//它的作用是从信号量的值减去一个“1”，但它永远会先等待该信号量为一个非零值才开始做减法。
    //当这个start函数结束后，就可以放心的访问这个新线程了，因为他的tid已经获取了。
}
void Thread::join()
{ //
    joined_ = true;
    thread_->join();
}


void Thread::setDefaultName()
{
    int num = ++numCreated_;
    if(name_.empty())
    {
        char buf[32] = {0};
        snprintf(buf, sizeof(buf), "Thread%d", num); ///给这个线程一个名字
    }    
}