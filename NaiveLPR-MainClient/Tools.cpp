//
// Created by chenchang on 7/20/18.
//

#include "Tools.h"
#include <stdio.h>
#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;


/**
 * Video edge extraction
 *
 * @param source
 * @param goal
 */
void Tools::videoCanny(const char *source, const char *goal) {
    VideoCapture cap(source);
    //检查是否成功打开
    if(!cap.isOpened())
    {
        cerr << "Can not open a camera or file." << endl;
        return;
    }

    VideoWriter writer=VideoWriter(goal,//输出路径以及文件名
                                   CV_FOURCC('D', 'I', 'V', 'X'),//根据输入视频得到其编解码器
                                   (int)cap.get(CV_CAP_PROP_FPS),//根据输入视频得到帧率
                                   Size((int)cap.get( CV_CAP_PROP_FRAME_WIDTH ),//根据输入视频得到视频宽度
                                        (int)cap.get(CV_CAP_PROP_FRAME_HEIGHT)),//根据输入视频得到视频高度
                                   false//输出不为彩色，因为canny后的为灰度图
    );

    if(writer.isOpened()){
        for(;;)
        {
            Mat frame, edges;
//从 cap 中读一帧,存到 frame
            cap >> frame;
//如果未读到图像
            if(frame.empty())
                break;
//将读到的图像转为灰度图
            cvtColor(frame, edges, CV_BGR2GRAY);

            blur(edges, edges, Size(7, 7));

//进行边缘提取操作
            Canny(edges, edges, 0, 30, 3);

            writer << edges;

        }
    }
}

/**
 * Picture edge extraction
 *
 * @param source source pic path
 * @param goal goal pic path
 * @param th1 threshold1
 * @param th2 threshold2
 */
void Tools::picCanny(const char *source, const char *goal, int th1, int th2) {
    //读入图像,并将之转为单通道图像
    Mat im = imread(source, 0);
//请一定检查是否成功读图
    if( im.empty() )
    {
        cout << "Can not load image." << endl;
        return;
    }

    Mat result;

    blur(im, im, Size(3, 3));

    Canny(im, result, th1, th2);
    //第三个参数和第四个参数表示阈值，这二个阈值中当中的小阈值用来控制边缘连接，
    //大的阈值用来控制强边缘的初始分割即如果一个像素的梯度大与上限值，则被认为
    //是边缘像素，如果小于下限阈值，则被抛弃。如果该点的梯度在两者之间则当这个
    //点与高于上限值的像素点连接时我们才保留，否则删除。

    imwrite(goal, result);
}

/**
 * Reverse black and white pixels
 *
 * @param source
 * @param goal
 */
void Tools::confuse(const char *source, const char *goal) {
    //读入图像,并将之转为单通道图像
    Mat im = imread(source, 0);
//请一定检查是否成功读图
    if( im.empty() )
    {
        cout << "Can not load image." << endl;
        return;
    }

    Mat_<uchar> M1 = (Mat_<uchar>&) im;
    for(int i=0; i<M1.rows; ++i){
        uchar *p = M1.ptr(i);
        for(int j=0; j<M1.cols; ++j){
            if(M1(i, j)==0)
                M1(i, j) = 255;
            else
                M1(i, j) = 0;
        }
    }

    imwrite(goal, im);
}



/*optical flow*/

inline bool isFlowCorrect(Point2f u)
{
    return !cvIsNaN(u.x) && !cvIsNaN(u.y) && fabs(u.y) < 1e9;
}

static Vec3b computeColor(float fx, float fy)
{
    static bool first = true;

    // relative lengths of color transitions:
    // these are chosen based on perceptual similarity
    // (e.g. one can distinguish more shades between red and yellow
    //  than between yellow and green)
    const int RY = 15;
    const int YG = 6;
    const int GC = 4;
    const int CB = 11;
    const int BM = 13;
    const int MR = 6;
    const int NCOLS = RY + YG + GC + CB + BM + MR;
    static Vec3i colorWheel[NCOLS];

    if (first){
        int k = 0;

        for (int i = 0; i < RY; ++i, ++k)
            colorWheel[k] = Vec3i(255, 255 * i / RY, 0);

        for (int i = 0; i < YG; ++i, ++k)
            colorWheel[k] = Vec3i(255 - 255 * i / YG, 255, 0);

        for (int i = 0; i < GC; ++i, ++k)
            colorWheel[k] = Vec3i(0, 255, 255 * i / GC);

        for (int i = 0; i < CB; ++i, ++k)
            colorWheel[k] = Vec3i(0, 255 - 255 * i / CB, 255);

        for (int i = 0; i < BM; ++i, ++k)
            colorWheel[k] = Vec3i(255 * i / BM, 0, 255);

        for (int i = 0; i < MR; ++i, ++k)
            colorWheel[k] = Vec3i(255, 0, 255 - 255 * i / MR);

        first = false;
    }

    const float rad = sqrt(fx * fx + fy * fy);
    const float a = atan2(-fy, -fx) / (float)CV_PI;

    const float fk = (a + 1.0f) / 2.0f * (NCOLS - 1);
    const int k0 = static_cast<int>(fk);
    const int k1 = (k0 + 1) % NCOLS;
    const float f = fk - k0;

    Vec3b pix;

    for (int b = 0; b < 3; b++)
    {
        const float col0 = colorWheel[k0][b] / 255.f;
        const float col1 = colorWheel[k1][b] / 255.f;

        float col = (1 - f) * col0 + f * col1;

        if (rad <= 1)
            col = 1 - rad * (1 - col); // increase saturation with radius
        else
            col *= .75; // out of range

        pix[2 - b] = static_cast<uchar>(255.f * col);
    }

    return pix;
}

static void drawOpticalFlow(const Mat_<Point2f>& flow, Mat& dst, float maxmotion = -1)
{
    dst.create(flow.size(), CV_8UC3);
    dst.setTo(Scalar::all(0));

    // determine motion range:
    float maxrad = maxmotion;

    if (maxmotion <= 0)
    {
        maxrad = 1;
        for (int y = 0; y < flow.rows; ++y)
        {
            for (int x = 0; x < flow.cols; ++x)
            {
                Point2f u = flow(y, x);

                if (!isFlowCorrect(u))
                    continue;

                maxrad = max(maxrad, sqrt(u.x * u.x + u.y * u.y));
            }
        }
    }

    for (int y = 0; y < flow.rows; ++y)
    {
        for (int x = 0; x < flow.cols; ++x)
        {
            Point2f u = flow(y, x);

            if (isFlowCorrect(u))
                dst.at<Vec3b>(y, x) = computeColor(u.x / maxrad, u.y / maxrad);
        }
    }
}

void Tools::opticalFlow(const char *source, const char *goal) {
    Mat frame0;
    Mat frame1;
    Mat_<Point2f> flow;
    Ptr<DenseOpticalFlow> tvl1 = createOptFlow_DualTVL1();
    Mat out;

    VideoCapture cap;
    cap.open(source);

    VideoWriter writer=VideoWriter(goal,//输出路径以及文件名
                                   CV_FOURCC('D', 'I', 'V', 'X'),//根据输入视频得到其编解码器
                                   (int)cap.get(CV_CAP_PROP_FPS),//根据输入视频得到帧率
                                   Size((int)cap.get( CV_CAP_PROP_FRAME_WIDTH ),//根据输入视频得到视频宽度
                                        (int)cap.get(CV_CAP_PROP_FRAME_HEIGHT)),//根据输入视频得到视频高度
                                   true//输出为彩色
    );

    if(!writer.isOpened())
        return;

    while(1){
        cap >> frame0;
        if(frame0.empty()){
            cerr<< "video is over!!" << endl;
            break;
        }
        cvtColor(frame0, frame0, CV_BGR2GRAY);
        if(!frame1.empty()){
            const double start = (double)getTickCount();
            tvl1->calc(frame0, frame1, flow);
            const double timeSec = (getTickCount() - start) / getTickFrequency();
            cout << "calcOpticalFlowDual_TVL1 : " << timeSec << " sec" << endl;
            drawOpticalFlow(flow, out);

//            imshow("out", out);
//            imshow("src", frame0);

            writer << out;

            waitKey(10);

//            writer << out;
        }
        frame0.copyTo(frame1);
    }
    waitKey();
}

/*optical flow*/




void Tools::picCompose(const char *pic1, const char *pic2, const char *path, double ratio) {
    Mat image1 = imread(pic1);
    Mat image2 = imread(pic2);
    Mat result;

    addWeighted(image1, ratio, image2, 1-ratio, 0.0, result);

    imwrite(path, result);
}



void Tools::trackbarTest(const char *pic1, const char *pic2) {

}

void Tools::reverse(const char *inputPath, const char *outputPath) {
    VideoCapture capture(inputPath);
    if (!capture.isOpened())
    {
        return;
    }

    VideoWriter writer=VideoWriter(outputPath,//输出路径以及文件名
                                   CV_FOURCC('D', 'I', 'V', 'X'),//根据输入视频得到其编解码器
                                   (int)capture.get(CV_CAP_PROP_FPS),//根据输入视频得到帧率
                                   Size((int)capture.get( CV_CAP_PROP_FRAME_WIDTH ),//根据输入视频得到视频宽度
                                        (int)capture.get(CV_CAP_PROP_FRAME_HEIGHT)),//根据输入视频得到视频高度
                                   true//输出为彩色
    );

    if(!writer.isOpened())
        return;

    vector<Mat> vm;
    while (1)
    {
        Mat frame;
        if (!capture.read(frame))
        {
            break;
        }
        vm.emplace_back(frame);
    }

    while(!vm.empty()){
        writer << vm.back();
        vm.pop_back();
    }
}





