
#include "../include/process_frame.hpp"
#include "../include/receive_frame.hpp"

int main(int argc,const char**argv){
    /* Linux / Windows*/
    process_frame pf;
    std::thread t1 (&process_frame::receive,pf);
    std::thread t2 (&process_frame::color_detect,pf);
    t1.join();
    t2.join();

    /* mac */
    // t1.detach();
    // pf.color_detect();
    /* mac*/
    
    
    return 0;
}
