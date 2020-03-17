#include "../include/receive_frame.hpp"


// defination
volatile unsigned int prdIdx; 
volatile unsigned int csmIdx;
imgData img_data[BUFFER_SIZE];

void receive_frame::receive(){
    //cap_height = 480;   // capture height
    //cap_width = 640;    // capture weight
    cap.open(0);
#ifdef use_video
    cap.open("../data/video_1.mov"); // read from video files
#endif
    //cap.set(cv::CAP_DSHOW,1); // CAP_DSHOW 0.02~0.04ms
    //cap.set(CAP_FFMPEG,1); // CAP_FFMPEG 0.02~0.03ms
    /*
    if use video:
    //cap.set(CAP_DSHOW,1);     // use direct show 
    */
   // cap.set(cv::CAP_PROP_FRAME_WIDTH,cap_width);
   // cap.set(cv::CAP_PROP_FRAME_HEIGHT,cap_height);

    if(!cap.isOpened()){
        std::cerr << "Error: Cannot open video file or webcamera" << std::endl;
    }

    while(1){
       while(prdIdx - csmIdx >= BUFFER_SIZE);
       cap >> img_data[prdIdx % BUFFER_SIZE].img;
       ++prdIdx;
    }

}
