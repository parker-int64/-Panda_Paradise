#ifndef __PROCESS_FRAME_HPP
#define __PROCESS_FRAME_HPP

#include <iostream>
#include <opencv2/opencv.hpp>
#include "receive_frame.hpp"
#include <string>
#include <thread>

#endif
class process_frame: public receive_frame{
    public:

        //process_frame(){};
        void color_detect();
        void visual_debug();
        // void tracker(cv::Mat &src);
    private:
        cv::Mat src; // source image
        cv::Mat hsv_image; // hsv image
        int low_h;
        int high_h;
        int low_s;
        int high_s;
        int low_v;
        int high_v;
        cv::Scalar low_hsv; // low HSV
        cv::Scalar high_hsv;    // high HSV
        cv::Mat imgThresholded; // thresholded image
        cv::Mat element;    // morphology element
        cv::Mat canny_img;  // canny image
        int morph_kernel_size;
        cv::Size morph_size;
        int blur_kernel_size;
        double canny_threshold1;
        double canny_threshold2;
        int find_contours_mode;
        int find_contours_method;
        int select_capture_size;
        std::string settings;
        bool show_image; // show debug images
        cv::Mat control;
};

