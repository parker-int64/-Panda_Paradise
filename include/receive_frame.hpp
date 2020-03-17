#ifndef __RECEIVE_FRAME_HPP
#define __RECEIVE_FRAME_HPP


#include <iostream>
#include <opencv2/opencv.hpp>
#include <thread>

extern volatile unsigned int prdIdx;    // producer index declaration
extern volatile unsigned int csmIdx;    // consumer index declaration
#define BUFFER_SIZE 1
struct imgData{
    cv::Mat img;
    
};
extern imgData img_data[BUFFER_SIZE];

class receive_frame{
    public: 
        cv::VideoCapture cap;
        cv::Mat control_panel;
        //receive_frame();
        void receive();
    private:
        int cap_height;  // capture height 
        int cap_width;   // capture width
};

#endif 