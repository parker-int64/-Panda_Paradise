#include<iostream>
#include"Python.h"
#include "../headers/imageProcess.hpp"
#include "../headers/serial.hpp"
#include<opencv2/opencv.hpp>
#include"unistd.h"
#include<thread>
#define BUFFER_SIZE 1
using namespace cv;
volatile unsigned int prdIdx;
volatile unsigned int csmIdx;
extern int ex_num;
int ex_num;

struct imgData{
    Mat img;
    // int send;
    //unsigned int count_frame;

};

imgData data[BUFFER_SIZE];


Mat frame;
//Mat src_img = imread("E:/Panda/5.jpg");
/*-------------获取图像----------*/
void getimage(){
     VideoCapture cap;
     cap.open("E:\\Panda\\9_720.mp4");
    //cap.open(1);
    //cap.set(CV_CAP_PROP_FRAME_WIDTH,640);
    //cap.set(CV_CAP_PROP_FRAME_HEIGHT,480);
     if(!cap.isOpened()){
         std::cerr <<"unable to open the video.\n";
     }
     while(1){
        while(prdIdx - csmIdx >= BUFFER_SIZE);
        cap >> data[prdIdx % BUFFER_SIZE].img;
        ++prdIdx;  
        
     }   
     cap.release();
     destroyAllWindows();
 }

/*-------------处理图像----------*/
void green_processimage(){
    while(1){
    Mat src_img;
    while(prdIdx - csmIdx == 0);
        data[csmIdx % BUFFER_SIZE].img.copyTo(src_img);
        ++csmIdx;
    
    if (src_img.empty()) {
        std::cerr << "Error,src_img is empty!" << std::endl;
    }
    Mat hsv_image;
    //确认HSV范围Green
    int iLowH = 44;
    int iHighH = 88;
    
    int iLowS = 67;
    int iHighS = 255;

    int iLowV = 59;
    int iHighV = 255; 

    cvtColor(src_img,hsv_image,COLOR_BGR2HSV);

    std::vector<Mat> hsvSplit;
    //split(hsv_image, hsvSplit);
    //equalizeHist(hsvSplit[2],hsvSplit[2]);
    //merge(hsvSplit,hsv_image);
        

    Mat imgThresholded; 
    inRange(hsv_image,  Scalar(iLowH,iLowS,iLowV),  Scalar(iHighH,iHighS,iHighV),  imgThresholded);

    //开操作（去除一些噪点）如果二值化后图片干扰部分依然很多，可适当增大size
    Mat element = getStructuringElement(MORPH_RECT,Size(10,10));
    morphologyEx(imgThresholded, imgThresholded, MORPH_OPEN, element);

    //闭操作，连接一些连通域
    morphologyEx(imgThresholded,imgThresholded,MORPH_CLOSE,element);

    //开操作
    morphologyEx(imgThresholded, imgThresholded, MORPH_OPEN, element);

    //高斯滤波
    GaussianBlur(imgThresholded,imgThresholded,Size(5,5),0,0);
    //namedWindow("GaussianBlur",CV_WINDOW_AUTOSIZE);
    //imshow("GaussianBlur",imgThresholded);//显示图片
        

    
    //边缘处理
    Mat cannyimg;
    Canny(imgThresholded, cannyimg, 128, 255, 3);

    
    std::vector<std::vector<Point>> contours;  //轮廓点集
    std::vector<Vec4i> hireachy;  //层次

    // //开始绘制轮廓
    // for(size_t i = 0; i < (int)contours.size(); i++)
    // {
    //     drawContours(cannyimg, contours, i, Scalar(255), 1, 8);
    // }

    //轮廓图
    //namedWindow("cannyimg",CV_WINDOW_AUTOSIZE);
    //imshow("cannyimg",cannyimg);
    
    //多边形逼近轮廓 + 获取矩形和圆形边界框
    findContours(cannyimg, contours, hireachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(-1,1));
    
    std::vector<std::vector<Point>> contours_ploy(contours.size());  //减少点数后的轮廓的集合,定义图像输出的多边形点集
    std::vector<Rect> ploy_rect(contours.size());  //轮廓点形成的矩形
    //std::vector<Point2f> ccs(contours.size());      //定义圆心坐标
    //std::vector<float> radius(contours.size());   //定义圆的半径

    
    std::vector<std::vector<Point> >::iterator itc= contours.begin(); 
    while (itc!=contours.end()) 
    {  
        if( itc->size()< 85 )
        {  
            itc= contours.erase(itc);  
        }
        else
        {  
            ++itc;  
        }  
    } 

    //std::vector<RotatedRect> minRects(contours.size());  //每个轮廓最终形成的最小的旋转的矩形
    //std::vector<RotatedRect> myellipse(contours.size());    //每个轮廓最终形成的最小椭圆


    
    for(size_t i = 0; i < contours.size(); i++){
        approxPolyDP(Mat(contours[i]),contours_ploy[i],3,true);//减少轮廓点数，为后面的算法提高效率
        ploy_rect[i] = boundingRect(contours_ploy[i]); //得到轮廓周围最小矩形
        //minEnclosingCircle(contours_ploy[i],ccs[i],radius[i]); //得到轮廓周围最小椭圆
        // if(contours_ploy[i].size() >5){
        //     myellipse[i] = fitEllipse(contours_ploy[i]);//得到最小椭圆，若contours_ploy[i]的点数size小于5会报错
        //     minRects[i] = minAreaRect(contours_ploy[i]);//得到一个旋转的矩形，返回旋转矩形
        // }
    }

    // 画多边形轮廓 + 包围的矩形框 + 圆形框
    Mat drawImg;
    drawImg = Mat::zeros(src_img.size(),src_img.type());
    Point2f pts[4];
    RNG rng(12345);

    
    for(size_t t = 0; t < contours.size(); t++){
        Scalar color = Scalar (rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255));
        rectangle(src_img, ploy_rect[t], color, 2, 8); //画矩形
        //circle(drawImg, ccs[t], radius[t], color, 2, 8);//画圆形
        if(contours_ploy[t].size() > 5){
            //ellipse(drawImg, myellipse[t],color, 1, 8);  //画椭圆
            //minRects[t].points(pts); // 得到轮廓所在的旋转的矩形的四个顶点坐标
            // for(int r = 0; r <4; r++){
            //     line(drawImg,pts[r],pts[(r+1) % 4], color, 1, 8);  //通过线，画旋转的矩形
            // }

        }

    }

    imshow("src",src_img);  
    if(contours.size()){ 
        int send_num = 1;
        ex_num = send_num;
        std::cout << "Transfer! " << send_num << std::endl;
    }
    else{
        int send_num = 0;
        ex_num = send_num;
        std::cout << "Transfer! " << ex_num << std::endl;
    } 
    if(waitKey(16) >= 0) 
    break;
    }
    
    
}

void red_processimage(){
    while(1){
    Mat src_img;
    while(prdIdx - csmIdx == 0);
        data[csmIdx % BUFFER_SIZE].img.copyTo(src_img);
        ++csmIdx;
    
    if (src_img.empty()) {
        std::cerr << " Error, src_img is empty!" << std::endl;
    }
    
   
    Mat hsv_image;
    //临场调试请加补光或者重新调HSV
    //确认HSV范围Red
    int iLowH = 168;
    int iHighH = 178;

    int iLowS = 43;
    int iHighS = 255;

    int iLowV = 46;
    int iHighV = 255;
    cvtColor(src_img,hsv_image,COLOR_BGR2HSV);
    std::vector<Mat> hsvSplit;
    //split(hsv_image, hsvSplit);
    //equalizeHist(hsvSplit[2],hsvSplit[2]);
    //merge(hsvSplit,hsv_image);
        

    Mat imgThresholded; 
    inRange(hsv_image,  Scalar(iLowH,iLowS,iLowV),  Scalar(iHighH,iHighS,iHighV),  imgThresholded);

    //开操作（去除一些噪点）如果二值化后图片干扰部分依然很多，可适当增大size
    Mat element = getStructuringElement(MORPH_RECT,Size(10,10));
    morphologyEx(imgThresholded, imgThresholded, MORPH_OPEN, element);

    //闭操作，连接一些连通域
    morphologyEx(imgThresholded,imgThresholded,MORPH_CLOSE,element);

    //开操作
    morphologyEx(imgThresholded, imgThresholded, MORPH_OPEN, element);

    //高斯滤波
    GaussianBlur(imgThresholded,imgThresholded,Size(5,5),0,0);
    //namedWindow("GaussianBlur",CV_WINDOW_AUTOSIZE);
    //imshow("GaussianBlur",imgThresholded);//显示图片
        

    
    //边缘处理
    Mat cannyimg;
    Canny(imgThresholded, cannyimg, 128, 255, 3);

    
    std::vector<std::vector<Point>> contours;  //轮廓点集
    std::vector<Vec4i> hireachy;  //层次

    // //开始绘制轮廓
    // for(size_t i = 0; i < (int)contours.size(); i++)
    // {
    //     drawContours(cannyimg, contours, i, Scalar(255), 1, 8);
    // }

    //轮廓图
    //namedWindow("cannyimg",CV_WINDOW_AUTOSIZE);
    //imshow("cannyimg",cannyimg);
    
    //多边形逼近轮廓 + 获取矩形和圆形边界框
    findContours(cannyimg, contours, hireachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(-1,1));
    
    std::vector<std::vector<Point>> contours_ploy(contours.size());  //减少点数后的轮廓的集合,定义图像输出的多边形点集
    std::vector<Rect> ploy_rect(contours.size());  //轮廓点形成的矩形
    //std::vector<Point2f> ccs(contours.size());      //定义圆心坐标
    //std::vector<float> radius(contours.size());   //定义圆的半径

    
    std::vector<std::vector<Point> >::iterator itc= contours.begin(); 
    while (itc!=contours.end()) 
    {  
        if( itc->size()< 85 )
        {  
            itc= contours.erase(itc);  
        }
        else
        {  
            ++itc;  
        }  
    } 

    //std::vector<RotatedRect> minRects(contours.size());  //每个轮廓最终形成的最小的旋转的矩形
    //std::vector<RotatedRect> myellipse(contours.size());    //每个轮廓最终形成的最小椭圆


    
    for(size_t i = 0; i < contours.size(); i++){
        approxPolyDP(Mat(contours[i]),contours_ploy[i],3,true);//减少轮廓点数，为后面的算法提高效率
        ploy_rect[i] = boundingRect(contours_ploy[i]); //得到轮廓周围最小矩形
        //minEnclosingCircle(contours_ploy[i],ccs[i],radius[i]); //得到轮廓周围最小椭圆
        // if(contours_ploy[i].size() >5){
        //     myellipse[i] = fitEllipse(contours_ploy[i]);//得到最小椭圆，若contours_ploy[i]的点数size小于5会报错
        //     minRects[i] = minAreaRect(contours_ploy[i]);//得到一个旋转的矩形，返回旋转矩形
        // }
    }

    // 画多边形轮廓 + 包围的矩形框 + 圆形框
    Mat drawImg;
    drawImg = Mat::zeros(src_img.size(),src_img.type());
    Point2f pts[4];
    RNG rng(12345);

    
    for(size_t t = 0; t < contours.size(); t++){
        Scalar color = Scalar (rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255));
        rectangle(src_img, ploy_rect[t], color, 2, 8); //画矩形
        //circle(drawImg, ccs[t], radius[t], color, 2, 8);//画圆形
        if(contours_ploy[t].size() > 5){
            //ellipse(drawImg, myellipse[t],color, 1, 8);  //画椭圆
            //minRects[t].points(pts); // 得到轮廓所在的旋转的矩形的四个顶点坐标
            // for(int r = 0; r <4; r++){
            //     line(drawImg,pts[r],pts[(r+1) % 4], color, 1, 8);  //通过线，画旋转的矩形
            // }

        }

    }

    imshow("src_1",src_img);
     if(contours.size()){
        // sleep(100);
        int send_num = 1;
        ex_num = send_num;
        std::cout << "Transfer! " << send_num << std::endl;
    }
    else{
        int send_num = 0;
        ex_num = send_num;
        std::cout << "Stop! " << send_num << std::endl;
    } 

    if(waitKey(16) >= 0) 
    break;
    
    }
    
}

