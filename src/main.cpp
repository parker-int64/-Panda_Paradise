// 2019 Panda Paradise SMU
// Created by Parker 
// Requirements: opencv 3 or later, python (pyserial)
// For linux user: serial_dev.py serial_dev.cpp
// For windows user: serial_com.py serial_com.cpp
#include<iostream>
#include<thread>
#include"../header/Objects.hpp"
#include"../header/imageProcess.hpp"
//#include"../headers/angleSolve.hpp"
#include"../header/serial.hpp"

/*--------------------主函数-----------------*/
int main(int argc, char* argv[]) {

    int color = atoi(argv[1]);
    
    //确定程序后只两个个参数
    
     if (argc != 2)
    {
        std::cerr << "Input incorrect, terminated\n";
        return 0;
    }

    //输出提示
    std::cout << "[Starting Program]" << std::endl;
    std::cout <<"[Usage]set 0 means Red,set 1 for Blue\n";


    //0表示红色
    if (*argv[1] == '0' )
    {
        //Do stuff
        std::cout << "Your setting " << atoi(argv[1])  << " (Red)." << std::endl;
        std::thread read_frame(getimage);
        std::thread process_frame_red(red_processimage);
        read_frame.join();
        process_frame_red.join();
    }

    //1表示绿色
    else if (*argv[1] == '1')
    {
        //Do stuff 
        std::cout << "Your setting " << atoi(argv[1]) << " (Green)." << std::endl;
        std::thread read_frame(getimage);
        std::thread process_frame_green(green_processimage);
        read_frame.join();
        process_frame_green.join();
        
    } 
    else
    {
        std::cerr << "Input incorrect, terminated\n";
        return 0;
    }
    //判断完毕
    
}
