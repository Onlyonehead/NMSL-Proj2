//
// Created by 杭咨存 on 2018/7/25.
//

#ifndef SRC_SERVER_PLATERECOGONIZATION_H
#define SRC_SERVER_PLATERECOGONIZATION_H


#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

class PlateRecognition {

public:
    static Mat process(Mat);
    static string carType(Mat);

private:
    static int getRowSum(Mat& img, int row);
    static int getColSum(Mat& img, int col);
    static void removeRivet (Mat& img);
};


#endif //SRC_SERVER_PLATERECOGONIZATION_H
