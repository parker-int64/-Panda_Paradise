#include "../include/process_frame.hpp"
#define WINDOW1_NAME "Control Panel"
#define CVUI_IMPLEMENTATION
#include "cvui.h"


void process_frame::color_detect(){
    control = cv::Mat(cv::Size(740, 720), CV_8UC3); 
    control = cv::Scalar(49, 52, 49);
#ifdef DEBUG
    cvui::init(WINDOW1_NAME); // 初始化
#endif // DEBUG
    // HSV Green
    low_h = 45;
    high_h = 90;
    low_s = 43;
    high_s = 255;
    low_v = 46;
    high_v = 255;
    low_hsv = cv::Scalar(low_h,low_s,low_v);
    high_hsv = cv::Scalar(high_h,high_s,high_v);
    /* HSV Configure End*/

    blur_kernel_size = 5;
    morph_kernel_size = 10;
    morph_size = cv::Size(morph_kernel_size,morph_kernel_size);
    canny_threshold1 = 100;
    canny_threshold2 = 255;
    find_contours_mode = cv::RETR_TREE;
    find_contours_method = cv::CHAIN_APPROX_SIMPLE;
    select_capture_size = 60;
    show_image = false;
    settings = "./settings.yml";    
    char fps_str[10];
    char coord_2d[20];
    while(1){
        std::this_thread::sleep_for(std::chrono::milliseconds(20)); // this thread sleep 20 ms
        
        while(prdIdx - csmIdx == 0);
        img_data[csmIdx % BUFFER_SIZE].img.copyTo(src);
        ++csmIdx;
#ifdef DEBUG
        // Visual Debug
        visual_debug();
#endif // DEBUG
        double start = (double)cv::getTickCount(); // count begin
        cv::cvtColor(src,hsv_image,cv::COLOR_BGR2HSV);
        // if camera has bright adjust function, then we don't need 
        // to do the following steps.

        // vector<Mat> hsvSplit; // separate hsv channels
        // split(hsv_image,hsvSplit);
        // equalizeHist(hsvSplit[2],hsvSplit[2]); //[2] is [s] channel.
        // merge(hsvSplit,hsv_image); //merge the channel


        
        // inRange() function,create a mask
        cv::inRange(hsv_image, low_hsv,high_hsv,imgThresholded);
    
        element = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(morph_kernel_size,morph_kernel_size));
        cv::morphologyEx(imgThresholded, imgThresholded, cv::MORPH_OPEN,element);
        
        cv::morphologyEx(imgThresholded, imgThresholded, cv::MORPH_CLOSE,element);
        //median blur
        cv::medianBlur(imgThresholded, imgThresholded, blur_kernel_size);
        
        cv::Canny(imgThresholded, canny_img, canny_threshold1, canny_threshold2, 3);

        std::vector<std::vector<cv::Point>> contours; //contours points
        std::vector<cv::Vec4i> hireachy; // hireachy
        cv::findContours(canny_img, contours, hireachy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(-1,-1));


        std::vector<std::vector<cv::Point>> contours_ploy(contours.size());  // polygon contours
        std::vector<cv::Point2f> center(contours.size());      // center of the circle
        std::vector<float> radius(contours.size());   // radius of the circle
        

        std::vector<std::vector<cv::Point> >::iterator itc= contours.begin(); 
        while (itc!=contours.end()){
            
            if( itc->size()< select_capture_size ) {  
                itc= contours.erase(itc);  
            }
            else{  
                ++itc;  
            }  
        } 

        cv::FileStorage fs_coord("../data/coord.yml",cv::FileStorage::WRITE);
        for(size_t i = 0; i < contours.size();i++){
            cv::approxPolyDP(cv::Mat(contours[i]),contours_ploy[i],3,true);
            cv::minEnclosingCircle(contours_ploy[i],center[i],radius[i]);  // get the minimum circle
            cv::circle(src, center[i], radius[i],cv::Scalar(0,0,255), 2, 8);   // draw a red circle
            sprintf(coord_2d,"x:%.2f,y:%.2f",center[i].x,center[i].y);
            cv::putText(src,coord_2d,cv::Point(0,60),cv::FONT_HERSHEY_TRIPLEX,1,cv::Scalar(0,255,0),2,8);
            std::cout <<"Circle center: x = "<<(int)center[i].x << ", y = " << (int)center[i].y<<std::endl; // print the coordinates
            fs_coord << "POINT_2D"<< center; // save 2D coordinates to coord.yml
            fs_coord.release();
        }
        double time = ((double)cv::getTickCount() - start ) / cv::getTickFrequency();
        double fps = 1 / time;
        sprintf(fps_str,"FPS: %.2f",fps);
        cv::putText(src,fps_str,cv::Point(0,30),cv::FONT_HERSHEY_TRIPLEX,1,cv::Scalar(0,0,255),1,8);
        // std::cout << "one frame cost: "<<time * 100<<" ms."<<std::endl;
        // show the results
        cv::imshow("src",src);
        if(show_image == false){
            cv::namedWindow("hsv");
            cv::namedWindow("img threshold");
            cv::namedWindow("canny");
        }
        if(show_image == true){
            cv::imshow("hsv",hsv_image);
            cv::imshow("img threshold",imgThresholded);
            cv::imshow("canny",canny_img);
        }else if(show_image == false){
            cv::destroyWindow("hsv");
            cv::destroyWindow("img threshold");
            cv::destroyWindow("canny");
        }
       

        // char key =(char)cv::waitKey(1);
        // if(key == 27 || key == 'q' || key == 'Q')break;  // Mac does not allow keyboard function in threads.

        if(cv::waitKey(16) >=0 )break;
        
    }
    cap.release();
    cv::destroyAllWindows();
}


// TODO:tracker
// void process_frame::tracker(cv::Mat &src){
    
// }
// TODO:更改脚本命令
void process_frame::visual_debug(){
    cvui::text(control, 80, 7, "Dynamic parameters control panel",1,0xCECECE);
    cvui::window(control,30,50,400,400,"HSV Control Interface");
    cvui::window(control,450,50,250,70,"Blur Kernel Size");
    cvui::window(control,450,140,250,80,"MorphologyEx Kernel Size");
    cvui::window(control,450,240,250,210,"Canny Threshold");
    cvui::window(control,30,470,500,220,"Find Contour Settings");
        // 文字提示信息
    cvui::text(control,45,95,"Low  H",0.6);
    cvui::text(control,45,145,"High H",0.6);
    cvui::text(control,45,215,"Low  S",0.6);
    cvui::text(control,45,265,"High S",0.6);
    cvui::text(control,45,335,"Low  V",0.6);
    cvui::text(control,45,385,"High V",0.6);
    cvui::text(control,460,85,"Size",0.6);
    cvui::text(control,460,175,"Size",0.6);
    cvui::text(control,460,270,"Threshold low",0.6);
    cvui::text(control,460,360,"Threshold High",0.6);
    cvui::text(control,85,495,"Row 1 modes,Row 2 methods",0.6);
        // 拖拽条
    cvui::trackbar(control,120,80,300,&low_h,0,255,1,"%.0Lf",cvui::TRACKBAR_DISCRETE,1);
    cvui::trackbar(control,120,130,300,&high_h,0,255,1,"%.0Lf",cvui::TRACKBAR_DISCRETE,1);
    cvui::trackbar(control,120,200,300,&low_s,0,255,1,"%.0Lf",cvui::TRACKBAR_DISCRETE,1);
    cvui::trackbar(control,120,250,300,&high_s,0,255,1,"%.0Lf",cvui::TRACKBAR_DISCRETE,1);
    cvui::trackbar(control,120,320,300,&low_v,0,255,1,"%.0Lf",cvui::TRACKBAR_DISCRETE,1);
    cvui::trackbar(control,120,370,300,&high_v,0,255,1,"%.0Lf",cvui::TRACKBAR_DISCRETE,1);
    cvui::trackbar(control,500,70,200,&blur_kernel_size,1,9,1,"%.0Lf",cvui::TRACKBAR_DISCRETE,2);
    cvui::trackbar(control,500,163,200,&morph_kernel_size,5,50,1,"%.0Lf",cvui::TRACKBAR_DISCRETE,1);
    cvui::trackbar(control,450,300,240,&canny_threshold1,(double)0,(double)255.0);
    cvui::trackbar(control,450,390,240,&canny_threshold2,(double)0,(double)255.0);
    cvui::trackbar(control,45,520,200,&find_contours_mode,0,4,1,"%.0Lf",cvui::TRACKBAR_DISCRETE,1);
    cvui::trackbar(control,45,570,150,&find_contours_method,1,4,1,"%.0Lf",cvui::TRACKBAR_DISCRETE,1);
    cvui::trackbar(control,45,620,200,&select_capture_size,0,80,1,"%.0Lf",cvui::TRACKBAR_DISCRETE,1);
    // 写出模式和方法
    switch(find_contours_mode){
    case 0:
        cvui::printf(control,245,540,0.4,0xCECECE,"Current Mode:  cv::RETR_EXTERNAL");
        break;
    case 1:
        cvui::printf(control,245,540,0.4,0xCECECE,"Current Mode:  cv::RETR_LIST");
        break;
    case 2:
        cvui::printf(control,245,540,0.4,0xCECECE,"Current Mode:  cv::RETR_CCOMP");
        break;
    case 4:
        cvui::printf(control,245,540,0.4,0xCECECE,"Current Mode:  cv::FLOODFILL");
        break;
    default:
        cvui::printf(control,245,540,0.4,0xCECECE,"Current Mode:  cv::RETR_TREE");
        break;
    }
    switch (find_contours_method){
    case 1:
        cvui::printf(control,200,590,0.4,0xCECECE,"Current Method:  cv::CHAIN_APPROX_NONE");
        break;
    case 3:
        cvui::printf(control,200,590,0.4,0xCECECE,"Current Method:  cv::CHAIN_APPROX_TC89_L1");
        break;
    case 4:
        cvui::printf(control,200,590,0.4,0xCECECE,"Current Method:  cv::CHAIN_APPROX_TC89_KCOS");
        break;
    default:
        cvui::printf(control,200,590,0.4,0xCECECE,"Current Method:  cv::CHAIN_APPROX_SIMPLE");
        break;
    }
    cvui::printf(control,250,640,"Contours < %d will be ereased.",select_capture_size);
    cvui::checkbox(control,340,473,"Show Debug Images",&show_image);
    bool save_button = cvui::button(control,560,470,"Save Parameters");
    if(save_button){
        cv::FileStorage fs(settings,cv::FileStorage::WRITE);
        low_hsv = cv::Scalar(low_h,low_s,low_v);
        high_hsv = cv::Scalar(high_h,high_s,high_v);
        morph_size = cv::Size(morph_kernel_size,morph_kernel_size);
        // 写入参数
        fs << "low_hsv" << low_hsv;
        fs << "high_hsv" << high_hsv;
        fs << "morph_size" << morph_size;
        fs << "median_blur_kernel_size" << blur_kernel_size;
        fs << "canny_threshold1" << canny_threshold1;
        fs << "canny_threshold2" << canny_threshold2;
        fs << "find_contours_mode"<< find_contours_mode;
        fs << "find_contours_method"<< find_contours_method;
        fs << "select_contour_size" << select_capture_size;
        fs.release();
        cvui::text(control,30,630,"Parameters saved to ./settings.yml");
    }
    bool py_udp_button = cvui::button(control,560,520,"Launch UDP Script");
    if(py_udp_button){
        // TODO:更改命令脚本
         system("open -a Terminal ../client_socket_udp.py");
    }
    bool exit_button = cvui::button(control,560,570,"Exit Program");
    if(exit_button){
    }
    cvui::update(); // 更新
    cvui::imshow(WINDOW1_NAME, control);  // 展示
}
