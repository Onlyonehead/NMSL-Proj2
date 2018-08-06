//
// Created by 杭咨存 on 2018/7/25.
//

#include "CharSegment.h"


int CharSegment::count = 0;

int CharSegment::getColSum(Mat& bimg, int col)
{
    int height = bimg.rows;
    int sum = 0;
    for (int i = (int)(height * 0.1); i < height * 0.9; i++)
    {
        sum += bimg.at<uchar>(i, col);
    }
//    cout << sum << endl;
    return sum;
}

vector<int> CharSegment::cutLeft(Mat& src, int Tsum, int left, int right)//左右切割
{
    int i;

//    if(count == 0){
//        for (i = 0; i < src.cols; i++)
//        {
//            int colValue = getColSum(src, i);
////            cout << colValue << endl;
//            if (colValue < Tsum / 2)
//            {
//                if(i < 1 / 8 * src.cols){
//                    left = i;
//                }
//                break;
//            }
//        }
//    }
    for (i=left; i < src.cols; i++)
    {
        int colValue = getColSum(src, i);
//        cout << i << " " << colValue << endl;
        if (colValue > Tsum)
        {
            left = i;
            right = i;
            break;
        }
    }

    vector<int> r;

    int colValueCount = 0;

    for (; i < src.cols; i++)
    {

        int colValue = getColSum(src, i);
        colValueCount += colValue;
        if (colValue < Tsum)
        {
            right = i;

//            cout << right - left << endl;
            if(count == 0){
                if ((right - left) < (src.cols/10)){
                    continue;
                }else{
                    break;
                }
            }else if(count == 2){
                if ((right - left) < (src.cols/8)){
                    continue;
                }else{
                    break;
                }
            }else{
                if ((right - left) < (src.cols/12)){
                    continue;
                }else{
                    break;
                }
            }


        }
    }

    if(i >= src.cols){
        right = src.cols;
    }

//    cout << left << " " << right << " " << colValueCount << " " << (right-left) * Tsum << endl;

    if(colValueCount < (right-left) * Tsum){
        cout << "sb" << endl;
        r.push_back(-1);
        r.push_back(right);
        return r;
    }

    if((right - left) > (src.cols/7) && count != 2){
        cout << "sb2" << endl;
        r.push_back(-2);
        r.push_back(right);
        return r;
    }

    count++;
    r.push_back(left);
    r.push_back(right);
    return r;

}

vector<Mat> CharSegment::process(Mat& srcImage, Mat& inimg)
{


    Mat resultResized = srcImage;
//    resultResized.create(80, 300, CV_8UC1);
//    resize(srcImage, resultResized, resultResized.size(), 0, 0, INTER_CUBIC);

    vector<vector<Point>> contours;

    contours.clear();

    findContours(inimg, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    vector<Rect> chars;
    chars.clear();
    int left = (int)(0.2 * resultResized.cols);
    int right = (int)(0.8 * resultResized.cols);

    while(chars.size() != 6){
        if(chars.size() < 6){
            left--;
            right++;
        }else{
            left++;
            right++;
        }
        chars.clear();
        auto iter = contours.begin();

        for (; iter != contours.end(); ++iter)
        {
            double area = contourArea(*iter);

            if (area > 0.003 * resultResized.rows * resultResized.cols)
            {
//                vector<vector<Point>> p;
//                p.push_back(*iter);
//                drawContours(resultResized, p, -1, Scalar(0, 0, 255), 1);
//                imshow("Src", resultResized);
//                waitKey(0);

                Rect r = boundingRect(*iter);
                if(r.height > 1.1 * r.width &&
                   r.x > left && r.x < right){
                    r.x -= 1;
                    r.y -= 2;
                    r.width += 2;
                    r.height += 4;
                    chars.push_back(r);
                }

            }
        }
        if(left < 0 || right > resultResized.cols){
            break;
        }
    }


    Rect specRect;
    for(Rect &r : chars){
        if(r.y + r.height > resultResized.rows){
            r.height = resultResized.rows - r.y;
        }

        if(r.x + r.width > resultResized.cols){
            r.width = resultResized.cols - r.x;
        }

        if(r.x < 0){
            r.width -= (0 - r.x);
            r.x = 0;
        }

        if(r.y < 0){
            r.height -= (0 - r.y);
            r.y = 0;
        }
        if(r.x < resultResized.cols * 2 / 7){
            specRect = r;
            specRect.x -= r.width * 1.15;
            if(specRect.x < 0){
                specRect.width -= (0 - specRect.x);
                specRect.x = 0;
            }
            chars.push_back(specRect);
//            rectangle(resultResized, specRect, Scalar(0,0,255), 1);
        }
//        rectangle(resultResized, r, Scalar(0,0,255), 1);
    }

//    drawContours(resultResized, contours, -1, Scalar(0, 0, 255), 1);



//    imshow("Src", resultResized);

//    waitKey(0);

    vector<Rect> sortedChars;

    for(Rect &r : chars){
        if(sortedChars.size() == 0){
            sortedChars.push_back(r);
        }else{
            if(r.x < sortedChars.front().x){
                sortedChars.insert(sortedChars.begin(), r);
            }else if(r.x > sortedChars.back().x){
                sortedChars.push_back(r);
            }else{
                for(int i = 0; i < sortedChars.size(); i++){
                    if(r.x > sortedChars.at(i).x && r.x < sortedChars.at(i+1).x){
                        sortedChars.insert(sortedChars.begin()+ i + 1, r);
                    }
                }
            }
        }

    }


    vector<Mat> char_mat;


    for(Rect &r : sortedChars){
//        cout << r.x << endl;
//        cout << r.y << endl;
//        cout << r.width << endl;
//        cout << r.height << endl;
        Mat m;
        Mat result = inimg(r);

        m = srcImage(r);


//        m.create(40, 20, CV_8UC3);
//
//        for(int i = 0; i < m.rows; i++){
//            for(int j = 0; j < m.cols; j++){
//                m.at<char>(j, i) = 0;
//            }
//        }
//
//        cvtColor(m, m, COLOR_BGR2GRAY);
//
//        threshold(m, m, 120, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
//
//        if(result.size().height > 2 * result.size().width){
//            int width = 40 * result.size().width
//                         / result.size().height;
//            resize(result, result, Size(width, 40), 0, 0, INTER_CUBIC);
//            for(int i = 0; i < (20-width) / 2; i++){
//                for(int j = 0; j < 40; j++){
//                    m.at<char>(j, i) = 0;
//                }
//            }
//            for(int i = (20-width) / 2; i < (20+width) / 2; i++){
//                for(int j = 0; j < 40; j++){
//                    m.at<char>(j, i) = result.at<char>(j, i - (20-width) / 2);
//                }
//            }
//            for(int i = (20+width) / 2; i < 20; i++){
//                for(int j = 0; j < 40; j++){
//                    m.at<char>(j, i) = 0;
//                }
//            }
//        }else{
//            int height = 40 * result.size().height
//                        / (2 * result.size().width);
//            resize(result, result, Size(20, height), 0, 0, INTER_CUBIC);
//            for(int i = 0; i < 20; i++){
//                for(int j = 0; j < (40 - height) / 2; j++){
//                    m.at<char>(j, i) = 0;
//                }
//            }
//            for(int i = 0; i < 20; i++){
//                for(int j = (40 - height) / 2; j < (40 + height) / 2; j++){
//                    m.at<char>(j, i) = result.at<char>(j - (40 - height) / 2, i);
//                }
//            }
//            for(int i = 0; i < 20; i++){
//                for(int j = (40 + height) / 2; j < 40; j++){
//                    m.at<char>(j, i) = 0;
//                }
//            }
//        }
//
//        Mat element_x = getStructuringElement(MORPH_RECT,
//                                              Size(2, 1));
//
//        Mat element_y = getStructuringElement(MORPH_RECT,
//                                              Size(1, 2));
//
//        dilate(m, m, element_y, Point(-1, -1), 1);
//
//        erode(m, m, element_y, Point(-1, -1), 1);


        char_mat.push_back(m);
    }

    return char_mat;
}