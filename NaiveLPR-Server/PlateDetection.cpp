//
// Created by 杭咨存 on 2018/7/25.
//

#include "PlateDetection.h"

Rect PlateDetection::plateSelect(Mat srcImage){
    int structElementSize = 2;

    Mat midImage, dstImage;

    dstImage.create(srcImage.size(), srcImage.type());

    cvtColor(srcImage, midImage, COLOR_BGR2GRAY);

    midImage = on_GaussianBlur(midImage);

//        blur(midImage, midImage, Size(3, 3));


//        medianBlur(srcGrayImage, midImage, 3);
//        锐化
//        Mat kernel(3,3,CV_32F,Scalar(-1));
//        kernel.at<float>(1,1) = 8.9;
//        filter2D(midImage, midImage, midImage.depth(), kernel);

    midImage = on_Sobel(midImage);

//        Canny(midImage, midImage, 1, 3, 3);


    //二值化
    threshold(midImage, midImage, 100, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

    Mat element_x = getStructuringElement(MORPH_RECT,
                                          Size(2 * structElementSize + 1, 1));

    Mat element_y = getStructuringElement(MORPH_RECT,
                                          Size(1, 2 * structElementSize + 1));

    int PARA;

    if(midImage.size().width * midImage.size().height < 200000){
        PARA = 5;
    }else{
        PARA = 4;
    }

    dilate(midImage, midImage, element_x, Point(-1, -1), PARA);

    erode(midImage, midImage, element_x, Point(-1, -1), 10);

    dilate(midImage, midImage, element_x, Point(-1, -1), 10-PARA);

//        imshow("sb", midImage);
//        waitKey(0);

    erode(midImage, midImage, element_y, Point(-1, -1), 1);

    dilate(midImage, midImage, element_y, Point(-1, -1), 2);

    dilate(midImage, midImage, element_x, Point(-1, -1), 8);

    erode(midImage, midImage, element_x, Point(-1, -1), 7);

    dilate(midImage, midImage, element_y, Point(-1, -1), 2);

    erode(midImage, midImage, element_y, Point(-1, -1), 4);

    dilate(midImage, midImage, element_y, Point(-1, -1), 4);

    dstImage = midImage;

    vector<vector<Point>> contours;

    contours.clear();

    findContours(dstImage, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);


    vector<vector<Point>>::iterator iter = contours.begin();


    vector<Rect> rect;

    Rect backup(0,0,0,0);
    double backupArea = 0;

    for (; iter != contours.end();)
    {
        double area = contourArea(*iter);

        if (area > 0.05 * dstImage.size().width * dstImage.size().height)
        {
            contours.erase(iter);
        }
        else if(iter->at(0).y < 0.3 * dstImage.size().height ||
                iter->at(0).y > 0.9 * dstImage.size().height)
        {
            contours.erase(iter);
        }
        else
        {
//            drawContours(result, contours, -1, Scalar(0), 1);
            Rect r = boundingRect(*iter);

            if(r.size().width > 2.5 * r.size().height && (r.size().width > 0.1 * dstImage.size().width ||
                                                          r.size().height > 0.1 * dstImage.size().height)){
                double ratio = area / (r.size().width * r.size().height);
                if(ratio > 0.8){
                    rect.push_back(r);
                }else if(area > backupArea){
                    backupArea = area;
                    backup = r;
                }
            }
            ++iter;
        }
    }

    if(rect.size() == 1){
        Rect r = rect.front();
//        rectangle(result, r, Scalar(0,0,255), 2);

        return r;
    }

    double r_area = 0;
    Rect r_result;

    for(Rect r : rect){
        if(r.width * r.height > r_area){
            r_area = r.width * r.height;
            r_result = r;
        }
    }


//        double distance = 99999;
//        Rect r_result;
//
//        for(Rect r : rect){
//            int x = abs(r.x + r.width/2 - result.size().width/2);
//            int y = abs(r.y + r.height/2 - result.size().height/2);
//            double d = sqrt(x * x + y * y);
//            if(d < distance){
//                distance = d;
//                r_result = r;
//            }
//        }


    if(r_result.width > 0){
//        rectangle(result, r_result, Scalar(0,0,255), 2);
        return r_result;
    }

//    cout << "Wrong" << endl;
    if(backup.width > 3.2 * backup.height){
        double l = (backup.width - 3.2 * backup.height) / 2;
        backup.x += l;
        backup.width -= 2 * l;
    }else{
        double l = (backup.height - backup.width / 3.2) / 2;
        backup.y += l;
        backup.height -= 2 * l;
    }
//        rectangle(result, backup, Scalar(0,0,255), 2);

    return backup;
}


Mat PlateDetection::process(Mat srcImage) {
    Rect r = plateSelect(srcImage);

    Mat grayImage = srcImage(r);
    cvtColor(grayImage, grayImage, COLOR_BGR2GRAY);

    Canny(grayImage, grayImage, 50, 200, 3);


    Size size = grayImage.size();

    // 需要的是黑色背景
    vector<Vec4i> lines;
    HoughLinesP(grayImage, lines, 1, CV_PI/180, 50, 10, 5);


    Mat disp_lines(size, CV_8UC1, Scalar(0, 0, 0));
    double angle = 0.;
    for (unsigned long i = 0; i < lines.size(); ++i)
    {
        line(disp_lines, Point(lines[i][0], lines[i][1]), Point(lines[i][2], lines[i][3]), Scalar(255, 255, 255));
        angle += atan2((double)lines[i][3] - lines[i][1], (double)lines[i][2] - lines[i][0]);
    }


    if(lines.size() < 3){

        lines.clear();
        HoughLinesP(grayImage, lines, 1, CV_PI/180, 30, 20, 10);

        disp_lines = Mat(size, CV_8UC1, Scalar(0, 0, 0));
        double angle = 0.;
        for (unsigned long i = 0; i < lines.size(); ++i)
        {
            line(disp_lines, Point(lines[i][0], lines[i][1]), Point(lines[i][2], lines[i][3]), Scalar(255, 255, 255));
            angle += atan2((double)lines[i][3] - lines[i][1], (double)lines[i][2] - lines[i][0]);
        }
        angle /= lines.size(); // mean angle, in radians.
        angle = angle * 180 / CV_PI;
    }else{
        angle /= lines.size(); // mean angle, in radians.
        angle = angle * 180 / CV_PI;
    }


//    imshow("Src2", grayImage);
//    waitKey(0);
//
//    imshow("Src", disp_lines);
//    waitKey(0);

    if(angle > 10 || angle < -10){
        angle = 0;
    }

//    cout << angle << endl;

    Point center = Point(r.x + 1/2*r.width, r.y + 1/2*r.height);

    Mat rot_mat = getRotationMatrix2D(center, angle, 1.0);

    cv::warpAffine(srcImage, srcImage, rot_mat, srcImage.size(), INTER_CUBIC);

    srcImage = srcImage(r);

    return srcImage;
}


Mat PlateDetection::on_GaussianBlur(Mat midImage)
{

    int g_kernelWidthValue = 1;
    int g_kernelHeightValue = 1;
    int g_nsigmaXValue = 1;
    int g_nsigmaYValue = 1;

    //高斯滤波
    GaussianBlur(midImage, midImage, Size(g_kernelWidthValue, g_kernelHeightValue),
                 g_nsigmaXValue, g_nsigmaYValue);

    return midImage;
}

Mat PlateDetection::on_Sobel(Mat midImage){

    Mat sobelGradient_X, sobelGradient_Y;
    Mat sobelAbsGradient_X, sobelAbsGradient_Y;
    int sobelKernelSize = 0; // TrackerBar position parameter


    Sobel(midImage, sobelGradient_X, CV_16S, 2, 0,
          2 * sobelKernelSize + 1, 1, 1, BORDER_DEFAULT);
    convertScaleAbs(sobelGradient_X, sobelAbsGradient_X);

//    Sobel(srcGrayImage, sobelGradient_Y, CV_16S, 0, 1,
//          2 * sobelKernelSize + 1, 1, 1, BORDER_DEFAULT);
//    convertScaleAbs(sobelGradient_Y, sobelAbsGradient_Y);

//    addWeighted(sobelAbsGradient_X, 0.5, sobelAbsGradient_Y, 0.5, 0, dstImage);
    midImage = sobelAbsGradient_X;

    return midImage;

}
