#ifndef WEBSERVER_UTILITY_H
#define WEBSERVER_UTILITY_H

/*Linux system APIS*/
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <assert.h>

/*STD Headers*/
#include <vector>
#include <queue>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <future>
#include <iostream>
#include <chrono>

/*！
@Author: DJJ
@Description: 定义工具函数以及工具类
@Date: 2021/6/13 上午10:32
*/

/*全局变量结构体*/
struct GlobalVar{
    static const int kMaxUserNum = 100000;                              //最大并发连接数量
    static std::chrono::seconds slot_interval;                          //时间轮的槽间隔
    static std::chrono::seconds timer_timeout ;                         //timer的超时时间
    static int slot_num;                                                //时间轮的槽数
};

/*将文件描述符fd设置为非阻塞模式*/
int SetNonBlocking(int fd);

/*绑定端口号并监听。成功时返回监听socket，否则返回-1s*/
int BindAndListen(int port);

/*ET模式下从文件描述符(非socket)读n个字节的数据*/
ssize_t ReadData(int fd, char* dest, size_t n);

/*ET模式下向文件描述符(非socket)写n个字节的数据*/
ssize_t WriteData(int fd, const char* source, size_t n);

/*线程池*/
class ThreadPool{
private:
   // std::vector<std::pair<std::shared_ptr<Ev>>>
    std::vector<std::thread> workers_;           //线程池
    std::queue<std::function<void()>> tasks_;    //任务队列
    std::mutex mutex_;                           //互斥锁
    std::condition_variable cond_;               //条件变量
    bool stop_;                                  //表示线程池是否启用
public:
    explicit ThreadPool(size_t thread_num);
    ~ThreadPool();

    /*向线程池中添加任务*/
    template<typename F,typename... Args>
    auto AddTaskToPool(F&& f,Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>;

    /*返回线程池中的线程数量*/
    decltype(workers_.size()) size() {return workers_.size();};
};

/*模板成员函数的定义也必须写在头文件中*/
template<typename F,typename... Args>
auto ThreadPool::AddTaskToPool(F&& f,Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;
    auto task = std::make_shared<std::packaged_task<return_type()>>
            (std::bind(std::forward<F>(f),std::forward<Args>(args)...));
    std::future<return_type> res = task->get_future();

    {
        std::unique_lock<std::mutex> locker(mutex_);
        if(stop_)
            throw std::runtime_error("add task to a stoped threadpool");

        tasks_.emplace([task](){(*task)();});
    }

    cond_.notify_one();
    return res;
}
#endif