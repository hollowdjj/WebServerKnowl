#include "Utility.h"
#include <getopt.h>
#include <stdlib.h>
#include <regex>

///////////////////////////
//   Global    Variables //
///////////////////////////
std::mutex GlobalVar::mutex_{};
int GlobalVar::total_user_num_ = 0;
std::chrono::seconds GlobalVar::slot_interval_ = std::chrono::seconds(1);            /* NOLINT */
std::chrono::seconds GlobalVar::client_header_timeout_ = std::chrono::seconds(60);   /* NOLINT */
std::chrono::seconds GlobalVar::client_body_timeout_ = std::chrono::seconds(60);     /* NOLINT */
std::chrono::seconds GlobalVar::keep_alive_timeout_ = std::chrono::seconds(60);      /* NOLINT */
int GlobalVar::slot_num_ = 60;
const int kMaxBufferSize = 4096;
char GlobalVar::favicon[555] = {
        '\x89', 'P',    'N',    'G',    '\xD',  '\xA',  '\x1A', '\xA',  '\x0',
        '\x0',  '\x0',  '\xD',  'I',    'H',    'D',    'R',    '\x0',  '\x0',
        '\x0',  '\x10', '\x0',  '\x0',  '\x0',  '\x10', '\x8',  '\x6',  '\x0',
        '\x0',  '\x0',  '\x1F', '\xF3', '\xFF', 'a',    '\x0',  '\x0',  '\x0',
        '\x19', 't',    'E',    'X',    't',    'S',    'o',    'f',    't',
        'w',    'a',    'r',    'e',    '\x0',  'A',    'd',    'o',    'b',
        'e',    '\x20', 'I',    'm',    'a',    'g',    'e',    'R',    'e',
        'a',    'd',    'y',    'q',    '\xC9', 'e',    '\x3C', '\x0',  '\x0',
        '\x1',  '\xCD', 'I',    'D',    'A',    'T',    'x',    '\xDA', '\x94',
        '\x93', '9',    'H',    '\x3',  'A',    '\x14', '\x86', '\xFF', '\x5D',
        'b',    '\xA7', '\x4',  'R',    '\xC4', 'm',    '\x22', '\x1E', '\xA0',
        'F',    '\x24', '\x8',  '\x16', '\x16', 'v',    '\xA',  '6',    '\xBA',
        'J',    '\x9A', '\x80', '\x8',  'A',    '\xB4', 'q',    '\x85', 'X',
        '\x89', 'G',    '\xB0', 'I',    '\xA9', 'Q',    '\x24', '\xCD', '\xA6',
        '\x8',  '\xA4', 'H',    'c',    '\x91', 'B',    '\xB',  '\xAF', 'V',
        '\xC1', 'F',    '\xB4', '\x15', '\xCF', '\x22', 'X',    '\x98', '\xB',
        'T',    'H',    '\x8A', 'd',    '\x93', '\x8D', '\xFB', 'F',    'g',
        '\xC9', '\x1A', '\x14', '\x7D', '\xF0', 'f',    'v',    'f',    '\xDF',
        '\x7C', '\xEF', '\xE7', 'g',    'F',    '\xA8', '\xD5', 'j',    'H',
        '\x24', '\x12', '\x2A', '\x0',  '\x5',  '\xBF', 'G',    '\xD4', '\xEF',
        '\xF7', '\x2F', '6',    '\xEC', '\x12', '\x20', '\x1E', '\x8F', '\xD7',
        '\xAA', '\xD5', '\xEA', '\xAF', 'I',    '5',    'F',    '\xAA', 'T',
        '\x5F', '\x9F', '\x22', 'A',    '\x2A', '\x95', '\xA',  '\x83', '\xE5',
        'r',    '9',    'd',    '\xB3', 'Y',    '\x96', '\x99', 'L',    '\x6',
        '\xE9', 't',    '\x9A', '\x25', '\x85', '\x2C', '\xCB', 'T',    '\xA7',
        '\xC4', 'b',    '1',    '\xB5', '\x5E', '\x0',  '\x3',  'h',    '\x9A',
        '\xC6', '\x16', '\x82', '\x20', 'X',    'R',    '\x14', 'E',    '6',
        'S',    '\x94', '\xCB', 'e',    'x',    '\xBD', '\x5E', '\xAA', 'U',
        'T',    '\x23', 'L',    '\xC0', '\xE0', '\xE2', '\xC1', '\x8F', '\x0',
        '\x9E', '\xBC', '\x9',  'A',    '\x7C', '\x3E', '\x1F', '\x83', 'D',
        '\x22', '\x11', '\xD5', 'T',    '\x40', '\x3F', '8',    '\x80', 'w',
        '\xE5', '3',    '\x7',  '\xB8', '\x5C', '\x2E', 'H',    '\x92', '\x4',
        '\x87', '\xC3', '\x81', '\x40', '\x20', '\x40', 'g',    '\x98', '\xE9',
        '6',    '\x1A', '\xA6', 'g',    '\x15', '\x4',  '\xE3', '\xD7', '\xC8',
        '\xBD', '\x15', '\xE1', 'i',    '\xB7', 'C',    '\xAB', '\xEA', 'x',
        '\x2F', 'j',    'X',    '\x92', '\xBB', '\x18', '\x20', '\x9F', '\xCF',
        '3',    '\xC3', '\xB8', '\xE9', 'N',    '\xA7', '\xD3', 'l',    'J',
        '\x0',  'i',    '6',    '\x7C', '\x8E', '\xE1', '\xFE', 'V',    '\x84',
        '\xE7', '\x3C', '\x9F', 'r',    '\x2B', '\x3A', 'B',    '\x7B', '7',
        'f',    'w',    '\xAE', '\x8E', '\xE',  '\xF3', '\xBD', 'R',    '\xA9',
        'd',    '\x2',  'B',    '\xAF', '\x85', '2',    'f',    'F',    '\xBA',
        '\xC',  '\xD9', '\x9F', '\x1D', '\x9A', 'l',    '\x22', '\xE6', '\xC7',
        '\x3A', '\x2C', '\x80', '\xEF', '\xC1', '\x15', '\x90', '\x7',  '\x93',
        '\xA2', '\x28', '\xA0', 'S',    'j',    '\xB1', '\xB8', '\xDF', '\x29',
        '5',    'C',    '\xE',  '\x3F', 'X',    '\xFC', '\x98', '\xDA', 'y',
        'j',    'P',    '\x40', '\x0',  '\x87', '\xAE', '\x1B', '\x17', 'B',
        '\xB4', '\x3A', '\x3F', '\xBE', 'y',    '\xC7', '\xA',  '\x26', '\xB6',
        '\xEE', '\xD9', '\x9A', '\x60', '\x14', '\x93', '\xDB', '\x8F', '\xD',
        '\xA',  '\x2E', '\xE9', '\x23', '\x95', '\x29', 'X',    '\x0',  '\x27',
        '\xEB', 'n',    'V',    'p',    '\xBC', '\xD6', '\xCB', '\xD6', 'G',
        '\xAB', '\x3D', 'l',    '\x7D', '\xB8', '\xD2', '\xDD', '\xA0', '\x60',
        '\x83', '\xBA', '\xEF', '\x5F', '\xA4', '\xEA', '\xCC', '\x2',  'N',
        '\xAE', '\x5E', 'p',    '\x1A', '\xEC', '\xB3', '\x40', '9',    '\xAC',
        '\xFE', '\xF2', '\x91', '\x89', 'g',    '\x91', '\x85', '\x21', '\xA8',
        '\x87', '\xB7', 'X',    '\x7E', '\x7E', '\x85', '\xBB', '\xCD', 'N',
        'N',    'b',    't',    '\x40', '\xFA', '\x93', '\x89', '\xEC', '\x1E',
        '\xEC', '\x86', '\x2',  'H',    '\x26', '\x93', '\xD0', 'u',    '\x1D',
        '\x7F', '\x9',  '2',    '\x95', '\xBF', '\x1F', '\xDB', '\xD7', 'c',
        '\x8A', '\x1A', '\xF7', '\x5C', '\xC1', '\xFF', '\x22', 'J',    '\xC3',
        '\x87', '\x0',  '\x3',  '\x0',  'K',    '\xBB', '\xF8', '\xD6', '\x2A',
        'v',    '\x98', 'I',    '\x0',  '\x0',  '\x0',  '\x0',  'I',    'E',
        'N',    'D',    '\xAE', 'B',    '\x60', '\x82',
};

int SetNonBlocking(int fd)
{
    int old_option = fcntl(fd,F_GETFL);
    if(old_option == -1) return -1;

    int new_option = old_option | O_NONBLOCK;
    if(fcntl(fd,F_SETFL,new_option) == -1) return -1;

    return 0;
}

void SetSocketNoDelay(int fd)
{
    int enable = 1;
    setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (void*)&enable, sizeof(enable));
}

int BindAndListen(int port)
{
    if(port<0 || port > 65535) return -1;
    int listenfd = socket(PF_INET,SOCK_STREAM,0);
    if(listenfd == -1)
    {
        ::GetLogger()->critical("create listen socket error: {}", strerror(errno));
        close(listenfd);
        return -1;
    }

    /*设置地址重用，实现端口复用，一般服务器都需要设置*/
    int reuse = 1;
    int res = setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof reuse);
    if(res == -1)
    {
        ::GetLogger()->error("set listen socket opt error: {}", strerror(errno));
        close(listenfd);
        return -1;
    }

    /*绑定地址*/
    sockaddr_in server_addr{};
    bzero(&server_addr,sizeof server_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    /*!
        INADDR_ANY泛指本机的意思。主要是考虑到主机具有多个网卡的情况。
        不管数据从哪个网卡过来，只要是绑定的端口号过来的数据，都可以接收。
     */
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    res = bind(listenfd,reinterpret_cast<sockaddr*>(&server_addr),sizeof server_addr);
    if(res == -1)
    {
        ::GetLogger()->critical("listen socket bind error: {}", strerror(errno));
        close(listenfd);
        return -1;
    }

    /*!
      从内核2.2版本之后，listen函数的backlog参数表示的是全连接的数量上限。
      所谓全连接，指的是完成了tcp三次握手处于establish状态的连接。也就是
      说，服务器能够同时与backlog个客户端进行tcp握手以建立连接，accept队
      列的长度在任何时候都最多只能为backlog。在5.4版本之后backlog的默认最
      大值为4096(定义在/proc/sys/net/core/somaxconn)。显然，backlog与
      服务器的最大并发连接数量没有直接的关系，只会影响服务器允许同时发起连
      接的客户端的数量。
     */
    res = listen(listenfd,2048);
    if(res == -1)
    {
        ::GetLogger()->critical("listen error: {}", strerror(errno));
        close(listenfd);
        return -1;
    }

    return listenfd;
}

std::string GetTime()
{
    time_t raw_time;
    struct tm* time_info;
    char time_buffer[30]{};
    time(&raw_time);
    time_info = gmtime(&raw_time);
    strftime(time_buffer, sizeof(time_buffer), "%a, %d %b %Y %H:%M:%S GMT", time_info);
    return std::string(time_buffer);
}

ssize_t ReadData(int fd, char* dest, size_t n)
{
    char* pos = dest;       //本次存放读取的数据的首地址
    size_t num = n;         //本次期望读取num个字节的数据
    ssize_t read_sum = 0;   //一共读取的字节数
    ssize_t read_once = 0;  //本次read函数调用成功读取的字节数
    while(num > 0)
    {
        read_once = read(fd, pos, num);
        if(read_once < 0)
        {
            if(errno == EINTR) continue;               //被系统中断就再重新读一次
            else if(errno == EAGAIN) return read_sum;  //当前无数据可读，返回已读取的字节数
            else
            {
                ::GetLogger()->error("read data from filefd {} error: {}", fd, strerror(errno));
                return -1;                        //否则表示发生了错误，返回-1
            }
        }
        else if(read_once == 0) break;            //read函数返回0表示读取完毕，退出循环
        /*更新*/
        pos +=read_once;
        num -= read_once;
        read_sum+= read_once;
    }

    return read_sum;
}

ssize_t ReadData(int fd,std::string& buffer,bool& disconnect)
{
    ssize_t read_once = 0;   //本次读取的字节数
    ssize_t read_sum = 0;    //读取的总字节数
    while(true)
    {
        char temp[kMaxBufferSize];
        memset(temp,'\0',4096);
        read_once = recv(fd,temp,kMaxBufferSize,0);
        if(read_once < 0)
        {
            /*!
             对非阻塞I/O：
             1.若当前没有数据可读，函数会立即返回-1，同时errno被设置为EAGAIN或EWOULDBLOCK。
               若被系统中断打断，返回值同样为-1,但errno被设置为EINTR。对于被系统中断的情况，
               采取的策略为重新再读一次，因为我们无法判断缓冲区中是否有数据可读。然而，对于
               EAGAIN或EWOULDBLOCK的情况，就直接返回，因为操作系统明确告知了我们当前无数据
               可读。
             2.若当前有数据可读，那么recv函数并不会立即返回，而是开始从内核中将数据拷贝到用
               户区，这是一个同步操作，返回值为这一次函数调用成功拷贝的字节数。所以说，非阻
               塞I/O本质上还是同步的，并不是异步的。
             */
            if(errno == EINTR) continue;                                      //被系统中断就再重新读一次
            else if(errno == EAGAIN || errno == EWOULDBLOCK) return read_sum; //当前无数据可读
            else
            {
                ::GetLogger()->error("read data from socket {} error: {}", fd, strerror(errno));
                return -1;                        //否则表示发生了错误，返回-1
            }
        }
        else if(read_once == 0)
        {
            /*一般情况下，recv返回0是由于客户端关闭连接导致的*/
            ::GetLogger()->debug("clinet {} has close the connection", fd);
            disconnect = true;
            break;
        }
        read_sum +=read_once;
        buffer += std::string(std::begin(temp),std::begin(temp)+read_once);  //拼接数据
    }

    return read_sum;
}

ssize_t WriteData(int fd, const char* source, size_t n)
{
    const char* pos = source;      //本次要写入的数据的首地址
    size_t num = n;                //本次期望写入num个字节的数据
    ssize_t write_sum = 0;         //一共写入的字节数
    ssize_t write_once = 0;        //本次write函数调用成功读取的字节数
    while(num > 0)
    {
        write_once = write(fd,pos,num);
        if(write_once < 0)
        {
            if(errno == EINTR) continue;                 //被系统中断打断，就重写一次
            else if(errno == EAGAIN) return write_sum;   //客户端或者服务器自身的缓冲区已经写满了，返回
            else
            {
                ::GetLogger()->error("write data to filefd {} error: {}", fd, strerror(errno));
                return -1;                               //否则表示发生了错误，返回-1
            }
        }
        pos+=write_once;
        num-=write_once;
        write_sum+=write_once;
    }

    return write_sum;
}

ssize_t WriteData(int fd, std::string& buffer, bool& full)
{
    if(buffer.empty()) return 0;

    full = false;
    auto num = buffer.size() + 1;      //期望写出的字节数
    ssize_t write_once = 0;            //本次写出的字节数
    ssize_t write_sum = 0;             //写出的总字节数
    const char* ptr = buffer.c_str();  //转换后的字符串结尾多了一个\0，所以length要加一，否则会乱码
    while(num > 0)
    {
        write_once = send(fd, ptr, num, 0);
        if(write_once < 0)
        {

            if(errno == EINTR) continue;  //被系统中断打断时重新再写一次
            /*!
             使用send函数且是非阻塞socket的情况下，一开始不需要注册EPOLLOUT，直接往socket写数据即可。
             当errno == EAGAIN || errno==EWOULDBLOCK时表示发送缓冲区已经写满了。此时，若数据还没有
             发送完，就需要注册EPOLLOUT，然后通过回调函数在可以往发送区中继续写数据时发送剩余的数据。
             数据发送完后，需取消EPOLLOUT并重新注册EPOLLIN。
             */
            else if(errno == EAGAIN || errno==EWOULDBLOCK)
            {
                full = true;             //发送缓冲区已经写满了，返回
                return write_sum;
            }
            else
            {
                ::GetLogger()->error("write data to socket {} error: {}", fd, strerror(errno));
                return -1;                                                    //否则表示发生了错误，返回-1
            }
        }
        num-=write_once;
        write_sum+=write_once;
        ptr+=write_once;
    }
    /*从buffer中删除已经写出的数据*/
    if(write_sum == buffer.size() + 1) buffer.clear();
    else buffer = buffer.substr(write_sum);
    
    return write_sum;
}

ThreadPool::ThreadPool(size_t thread_num) : stop_(false)
{
    for (int i = 0; i < thread_num; ++i)
    {
        workers_.emplace_back([this]()
                              {
                                  while(true)
                                  {
                                      std::function<void()> task;
                                      /*从任务队列中取出一项任务并执行，若任务队列为空则休眠线程*/
                                      {
                                          std::unique_lock<std::mutex> locker(this->mutex_);
                                          /*!
                                              只要stop_ == true即线程池已经停止使用了就一直休眠
                                              当stop_ == false，即线程池开启时，还要任务队列不为空才会唤醒是，否则休眠
                                           */
                                          this->cond_.wait(locker,[this](){return this->stop_ || !this->tasks_.empty();});
                                          /*当stop_为true且任务队列为空时，线程会被唤醒，此时退出*/
                                          if(this->stop_ && this->tasks_.empty()) return;

                                          task = this->tasks_.front();
                                          this->tasks_.pop();
                                      }
                                      task();
                                  }
                              });
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> locker(mutex_);
        stop_ = true;
    }
    for (auto& item : workers_) item.join();
}

std::shared_ptr<spdlog::logger> GetLogger(std::string path /*../temp/log.txt*/)
{
    try
    {
        static auto logger = spdlog::basic_logger_mt("basic_logger",path);
        return logger;
    }
    catch(const spdlog::spdlog_ex& ex)
    {
        std::cout<<"Log initialization failed: "<<ex.what()<<std::endl;
    }

    return {};
}

std::optional<std::tuple<int,size_t ,std::string>> ParaseCommand(int argc,char* argv[])
{
    const char* str = "p:s:l:";
    int res,port,subreactor_num;
    std::string log_file_path;
    while((res = getopt(argc,argv,str)) != -1)
    {
        switch (res) {
            case 'p':
                port = atoi(optarg);
                break;
            case 's':
                subreactor_num = atoi(optarg);
                break;
            case 'l':{
                std::regex r(R"(^\.\S*)");
                std::smatch results;
                std::string s(optarg);
                std::regex_match(s, results, r);
                if(results.empty())
                {
                    printf("illegal log file path\n");
                    return std::nullopt;
                }
                log_file_path = results[0];
            }break;
            default:
                break;
        }
    }
    return std::make_tuple(port,subreactor_num,log_file_path);
}