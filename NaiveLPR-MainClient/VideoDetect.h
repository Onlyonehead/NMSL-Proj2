//
// Created by chenchang on 8/1/18.
//

#ifndef UNTITLED1_VIDEODETECT_H
#define UNTITLED1_VIDEODETECT_H

#include <cstring>
#include <iostream>
#include <opencv/cvaux.h>
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include "opencv2/opencv.hpp"
#include <vector>
#include <stack>

using namespace std;
using namespace cv;

typedef struct _Feather
{
    int label;              // 连通域的label值
    int area;               // 连通域的面积
    Rect boundingbox;       // 连通域的外接矩形框
} Feather;

/*
 * mask
 * flag=0 表示物体已不在指定范围内
 * trackWindow 车辆外围边框
 * flag 用于标记车辆是否还出现在检测框范围内
 * record 车辆计数
 * pic 用于标记是否已保存该车辆的图片
 * frame 当前记录帧数
 * distance　车辆位移像素
 * p 记录车辆中心在前一帧的位置
 */
typedef struct _Trace
{
    Mat mask = Mat::zeros(200, 320, CV_8UC3);
    Mat hist;
    Rect trackWindow;
    int flag = 1;
    int record;
    bool pic = true;
    int frame = 0;
    double distance = 0;
    Point p;
} Trace;

class VideoDetect {
public:

    /**
     * @param path 0:source video path  1:output vodeo path
     *             2:car pictures folder  3:car detail file path
     * @param rat the ratio of selected area (x, y, width, height)
     * @param minarea the minimum area of car detection
     * @param mindetect the minimum area of car count
     * @param initFrame number of frames for background initialize
     * @param fMax number of frames for program initialize
     * @param shadow shadow threshold
     * @param mCore size of structuring element of close operation
     */
    int vOperate_flow(string *path, double *rat, int minarea=350, int mindetect=800,
            int initFrame=30, int fMax=10, double shadow=0.7, int mCore=6);

    int vOperatePeccancy(string *path, double *rat, long *time, int minarea=350, int mindetect=800,
                          int initFrame=30, int fMax=10, double shadow=0.7, int mCore=6);

    bool peccancyDetect(Rect& r, int *line);

    /**
     * @param videoPath the path of source video
     * @param fFrame reference to a mat type object
     */
    void getFirstFrame(string videoPath, Mat &fFrame);
private:

    /**
     * @param src 待检测连通域的二值化图像
     * @param dst 标记后的图像
     * @param featherList 连通域特征集合
     * @param rect　自定义待检测区域
     * @param minarea　连通区域最小面积，默认值为200
     * @return　连通区域数量
     */
    int bwLabel(Mat & src, Mat & dst, vector<Feather> & featherList,  const Rect &rect, int minarea=200);

    /**
    *
    * @param bgm 背景Mat，初始化好的背景也通过它返回
    * @param v 视频源
    * @param totalCount　用于初始化的视频帧数
    */
    void backgroundInit(Mat &bgm, VideoCapture& v, int totalCount = 30);


    int vmin = 10, vmax = 256, smin = 30;
    int hsize = 16;
    float hranges[2] = {0,180};
    const float* phranges = hranges;
    int carCount = 0;
};


#endif //UNTITLED1_VIDEODETECT_H
