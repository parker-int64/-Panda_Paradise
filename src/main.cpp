#include "../include/process_frame.hpp"
#include "../include/receive_frame.hpp"
#define DEBUG
int main(int argc,const char**argv){
    //thread里面不能有按键操作。。。。mac的按键操作在主线程
    //重新写？
    process_frame pf; //类的声明
    std::thread t1 (&process_frame::receive,pf);
    std::thread t2 (&process_frame::color_detect,pf);
    t1.join();
    t2.join();
    //t1.detach();
    //pf.color_detect();
    //t2.join();
    
    return 0;
}
