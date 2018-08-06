//
// Created by 杭咨存 on 2018/7/25.
//
#ifndef SRC_SERVER_PLATEDETECTION_H
#define SRC_SERVER_PLATEDETECTION_H


#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

class PlateDetection {

public:
    static Mat process(Mat);

private:
    static Mat on_Sobel(Mat);

    static Mat on_GaussianBlur(Mat);

    static Rect plateSelect(Mat);

};


#endif //SRC_SERVER_PLATEDETECTION_H
