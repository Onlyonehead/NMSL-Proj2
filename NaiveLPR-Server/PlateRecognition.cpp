//
// Created by 杭咨存 on 2018/7/25.
//

#include "PlateRecognition.h"


int PlateRecognition::getRowSum(Mat& bimg, int row)
{
    int width = bimg.cols;
    int sum = 0;
    for (int i = 0; i < width; i++)
    {
        sum += bimg.at<uchar>(row, i) / 255;
    }
//    cout << sum << endl;
    return sum;
}

int PlateRecognition::getColSum(Mat& img, int col)
{
    int height = img.rows;
    int sum = 0;
    for (int i = 0; i < height; i++)
    {
        sum += img.at<uchar>(i, col) / 255;
    }
//    cout << sum << endl;
    return sum;
}

void PlateRecognition::removeRivet (Mat& img){
    vector<float> fJump;
    int whiteCount = 0;
    const int x = 6;
    Mat jump = Mat::zeros(1, img.rows, CV_32F);
    for (int i = 0; i < img.rows; i++) {
        int jumpCount = 0;

        for (int j = 0; j < img.cols - 1; j++) {
            if (img.at<char>(i, j) != img.at<char>(i, j + 1)) jumpCount++;

            if (img.at<uchar>(i, j) == 255) {
                whiteCount++;
            }
        }

        jump.at<float>(i) = (float)jumpCount;
    }

    for (int i = 0; i < img.rows; i++) {
        if (jump.at<float>(i) <= x) {
            for (int j = 0; j < img.cols; j++) {
                img.at<char>(i, j) = 0;
            }
        }
    }
}

string PlateRecognition::carType(Mat result){
    cvtColor(result, result, COLOR_BGR2GRAY);

    threshold(result, result, 120, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

    //如果二值化图颜色颠倒，则反色
    int sum = 0;
    for (int i = 0; i < result.rows; i++)
    {
        for(int j = 0; j < result.cols; j++){
            sum += result.at<uchar>(i, j) / 255;
        }
    }

    if (sum > 0.5 * result.rows * result.cols){
        return "yellow";
    }else{
        return "blue";
    }
}

Mat PlateRecognition::process(Mat result){

    cvtColor(result, result, COLOR_BGR2GRAY);

    threshold(result, result, 120, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

    //如果二值化图颜色颠倒，则反色
    int sum = 0;
    for (int i = 0; i < result.rows; i++)
    {
        for(int j = 0; j < result.cols; j++){
            sum += result.at<uchar>(i, j) / 255;
        }
    }

    if (sum > 0.5 * result.rows * result.cols){
        bitwise_not(result, result);
    }

    double ratio;

//    imshow("s", result);
//    waitKey(0);

    //    去除横向无用信息


    ratio = 0.9 * result.rows;

    int col_min = 0;
    int col_max = result.cols;

    for (int i = 0; i < result.cols; i++)
    {
//        cout << i << " -- " <<  getRowSum(result, i) << endl;
        if(getColSum(result, i) > 0.1 * ratio){
            break;
        }
        col_min = i;

    }

    for (int i = result.cols; i > 0; i--)
    {
//        cout << i << " -- " <<  getRowSum(result, i) << endl;
        if(getColSum(result, i) > 0.1 * ratio){
            break;
        }
        col_max = i;
    }

    for (int i = col_min; i < result.cols * 0.2; i++)
    {
        if(getColSum(result, i) > ratio){
            col_min = i;
//            cout << col_min << endl;

        }

    }

    for (int i = col_max; i > result.cols * 0.8; i--)
    {
//        cout << i << " " << getColSum(result, i) << endl;
        if(getColSum(result, i) > ratio){
            col_max = i;
        }
    }

    //去除纵向无用信息

    ratio = 0.9 * result.cols;

    int row_min = 0;
    int row_max = result.rows;

    for (int i = 0; i < result.rows; i++)
    {
//        cout << i << " -- " <<  getRowSum(result, i) << endl;
        if(getRowSum(result, i) > 0.1 * ratio){
            break;
        }
        row_min = i;
    }

    for (int i = result.rows; i > 0; i--)
    {
//        cout << i << " -- " <<  getRowSum(result, i) << endl;
        if(getRowSum(result, i) > 0.1 * ratio){
            break;
        }
        row_max = i;

    }

    for (int i = row_min; i < result.rows * 0.2; i++)
    {
//        cout << getRowSum(result, i) << endl;
        if(getRowSum(result, i) > ratio){
            row_min = i;
        }
//        if(getRowSum(result, i) < 0.3 * ratio){
//            row_min = i;
//        }
    }

    for (int i = row_max; i > result.rows * 0.8; i--)
    {
        if(getRowSum(result, i) > ratio){
            row_max = i;
        }

//        if(getRowSum(result, i) < 0.3 * ratio){
//            row_max = i;
//        }
    }

//    row_min = row_min - 1 > 0 ? row_min - 1 : 0;
//    row_max = row_max + 1 < result.rows ? row_max + 1 : result.rows;


//
//    for(int i = 0; i < col_min; i++){
//        for(int j = row_min; j < row_max; ++j){
//            if(result.at<char>(i, j) != 0){
//                result.at<char>(i, j) = 0;
//            }
//        }
//    }
//
//    for(int i = col_max; i < result.cols; ++i){
//        for(int j = row_min; j < row_max; ++j){
//            if(result.at<char>(i, j) != 0){
//                result.at<char>(i, j) = 0;
//            }
//        }
//    }
//
//
//    for(int i = 0; i < row_min; i++){
//        for(int j = col_min; j < col_max; ++j){
//            if(result.at<char>(i, j) != 0){
//                result.at<char>(i, j) = 0;
//            }
//        }
//    }
//
//    for(int i = row_max; i < result.rows; ++i){
//        for(int j = col_min; j < col_max; ++j){
//            if(result.at<char>(i, j) != 0){
//                result.at<char>(i, j) = 0;
//            }
//        }
//    }



    Mat element_x = getStructuringElement(MORPH_RECT,
                                          Size(2, 1));

    Mat element_y = getStructuringElement(MORPH_RECT,
                                          Size(1, 2));

    erode(result, result, element_y, Point(-1, -1), 1);

    dilate(result, result, element_y, Point(-1, -1), 1);

    erode(result, result, element_x, Point(-1, -1), 1);

    dilate(result, result, element_x, Point(-1, -1), 1);


    removeRivet(result);

//    Mat resultResized;
//    resultResized.create(80, 300, CV_8UC1);
//    resize(result, resultResized, resultResized.size(), 0, 0, INTER_CUBIC);

    return result;
}
