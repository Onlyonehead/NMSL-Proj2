//
// Created by 杭咨存 on 2018/7/25.
//

#ifndef SRC_SERVER_CHARSEGMENT_H
#define SRC_SERVER_CHARSEGMENT_H

#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

class CharSegment {

public:
    static vector<Mat> process(Mat&, Mat&);

private:
    static vector<int> cutLeft(Mat& src, int Tsum, int left, int right);
    static int getColSum(Mat& bimg, int col);
    static int count;

};


#endif //SRC_SERVER_CHARSEGMENT_H
