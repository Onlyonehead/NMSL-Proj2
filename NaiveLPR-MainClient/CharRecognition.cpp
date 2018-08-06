#include "CharRecognition.h"

vector<string> CharRecognition::process(Mat charMat, string dir){

    Ptr<ANN_MLP> ann = ANN_MLP::create();

    ann = Algorithm::load<ANN_MLP>(dir + "/ANN-Model.xml");

    string chars[CLASS_NUM1] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
                                "A", "B", "C", "D", "E", "F", "G", "H", "J", "K",
                                "L", "M", "N", "P", "Q", "R", "S", "T", "U", "V",
                                "W", "X", "Y", "Z"};

    if(charMat.rows > 3 * charMat.cols){
        vector<string> ret;
        ret.push_back("1");
        ret.push_back(to_string(100));
        return ret;
    }


    Mat sampleMat;
//
//    vector<float> feat;
//
//    calcGradientFeat(charMat, feat);
//    calcGrayFeat(charMat, feat);

    cvtColor(charMat, charMat, CV_BGR2GRAY);
    threshold(charMat, charMat, 120, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);



    int sum = 0;
    for (int i = 0; i < charMat.rows; i++)
    {
        for(int j = 0; j < charMat.cols; j++){
            sum += charMat.at<uchar>(i, j) / 255;
        }
    }

    if (sum > 0.5 * charMat.rows * charMat.cols){
        bitwise_not(charMat, charMat);
    }

    int h = charMat.rows;
    int w = charMat.cols;
    Mat transformMat = Mat::eye(2, 3, CV_32F); // 创建对角阵
    int m = max(w, h);
    transformMat.at<float>(0, 2) = m / 2 - w / 2;
    transformMat.at<float>(1, 2) = m / 2 - h / 2;

    // 仿射变换
    Mat warpImage(m, m, charMat.type());
    warpAffine(charMat, warpImage, transformMat, warpImage.size(), INTER_LINEAR, BORDER_CONSTANT, Scalar(0));

    Mat out;
    resize(warpImage, out, Size(MSIZE, MSIZE));
//    imshow("sb", out);
//    waitKey(0);

//            calcGradientFeat(srcImage, feat);
//            calcGrayFeat(srcImage, feat);
    Mat feat = features(out, SSIZE);


    float data[DIM];
    for(int k = 0; k < DIM; k++){
        data[k] = feat.at<float>(k);
    }

    // Set up training data Mat
    sampleMat = Mat(1, DIM, CV_32F, data);

    Mat responseMat;
    ann->predict(sampleMat, responseMat);
    Point maxLoc;
    double maxVal = 0;
    minMaxLoc(responseMat, nullptr, &maxVal, nullptr, &maxLoc);
//    cout << responseMat;

//    cout << "识别结果：" << chars[maxLoc.x] << "    相似度:" << maxVal * 100 << "%" << endl;

    vector<string> ret;
    ret.push_back(chars[maxLoc.x]);
    ret.push_back(to_string(maxVal * 100 / 1.404));
    return ret;
}

vector<string> CharRecognition::process_sp(Mat charMat, string dir){

    Ptr<ANN_MLP> ann = ANN_MLP::create();

    ann = Algorithm::load<ANN_MLP>(dir + "/ANN-Model-SP.xml");

    string chars[CLASS_NUM3] = {"A", "B", "C", "D", "E", "F", "G", "H", "J", "K",
                                "L", "M", "N", "P", "Q", "R", "S", "T", "U", "V",
                                "W", "X", "Y", "Z"};


    Mat sampleMat;
//
//    vector<float> feat;
//
//    calcGradientFeat(charMat, feat);
//    calcGrayFeat(charMat, feat);

    cvtColor(charMat, charMat, CV_BGR2GRAY);
    threshold(charMat, charMat, 120, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);



    int sum = 0;
    for (int i = 0; i < charMat.rows; i++)
    {
        for(int j = 0; j < charMat.cols; j++){
            sum += charMat.at<uchar>(i, j) / 255;
        }
    }

    if (sum > 0.5 * charMat.rows * charMat.cols){
        bitwise_not(charMat, charMat);
    }

    int h = charMat.rows;
    int w = charMat.cols;
    Mat transformMat = Mat::eye(2, 3, CV_32F); // 创建对角阵
    int m = max(w, h);
    transformMat.at<float>(0, 2) = m / 2 - w / 2;
    transformMat.at<float>(1, 2) = m / 2 - h / 2;

    // 仿射变换
    Mat warpImage(m, m, charMat.type());
    warpAffine(charMat, warpImage, transformMat, warpImage.size(), INTER_LINEAR, BORDER_CONSTANT, Scalar(0));

    Mat out;
    resize(warpImage, out, Size(MSIZE, MSIZE));
//    imshow("sb", out);
//    waitKey(0);

//            calcGradientFeat(srcImage, feat);
//            calcGrayFeat(srcImage, feat);
    Mat feat = features(out, SSIZE);


    float data[DIM];
    for(int k = 0; k < DIM; k++){
        data[k] = feat.at<float>(k);
    }

    // Set up training data Mat
    sampleMat = Mat(1, DIM, CV_32F, data);

    Mat responseMat;
    ann->predict(sampleMat, responseMat);
    Point maxLoc;
    double maxVal = 0;
    minMaxLoc(responseMat, nullptr, &maxVal, nullptr, &maxLoc);
//    cout << responseMat;

//    cout << "识别结果：" << chars[maxLoc.x] << "    相似度:" << maxVal * 100 << "%" << endl;

    vector<string> ret;
    ret.push_back(chars[maxLoc.x]);
    ret.push_back(to_string(maxVal * 100 / 1.404));
    return ret;
}

vector<string> CharRecognition::process_ch(Mat charMat, string dir){

    Ptr<ANN_MLP> ann = ANN_MLP::create();

    ann = Algorithm::load<ANN_MLP>(dir + "/ANN-Model-CH.xml");

    string chars[CLASS_NUM2] = {"云", "京", "冀", "吉", "宁", "川", "新", "晋", "桂", "沪",
                                "津", "浙", "渝", "湘", "琼", "甘", "皖", "粤", "苏", "蒙",
                                "豫", "贵", "赣", "辽", "鄂", "闽", "陕", "青", "鲁", "黑"};


    Mat sampleMat;
//
//    vector<float> feat;
//
//    calcGradientFeat(charMat, feat);
//    calcGrayFeat(charMat, feat);

    cvtColor(charMat, charMat, CV_BGR2GRAY);
    threshold(charMat, charMat, 120, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);



    int sum = 0;
    for (int i = 0; i < charMat.rows; i++)
    {
        for(int j = 0; j < charMat.cols; j++){
            sum += charMat.at<uchar>(i, j) / 255;
        }
    }

    if (sum > 0.5 * charMat.rows * charMat.cols){
        bitwise_not(charMat, charMat);
    }

    int h = charMat.rows;
    int w = charMat.cols;
    Mat transformMat = Mat::eye(2, 3, CV_32F); // 创建对角阵
    int m = max(w, h);
    transformMat.at<float>(0, 2) = m / 2 - w / 2;
    transformMat.at<float>(1, 2) = m / 2 - h / 2;

    // 仿射变换
    Mat warpImage(m, m, charMat.type());
    warpAffine(charMat, warpImage, transformMat, warpImage.size(), INTER_LINEAR, BORDER_CONSTANT, Scalar(0));

    Mat out;
    resize(warpImage, out, Size(MSIZE, MSIZE));
//    imshow("sb", out);
//    waitKey(0);

//            calcGradientFeat(srcImage, feat);
//            calcGrayFeat(srcImage, feat);
    Mat feat = features(out, SSIZE);


    float data[DIM];
    for(int k = 0; k < DIM; k++){
        data[k] = feat.at<float>(k);
    }

    // Set up training data Mat
    sampleMat = Mat(1, DIM, CV_32F, data);

    Mat responseMat;
    ann->predict(sampleMat, responseMat);

    Point maxLoc;
    double maxVal = 0;
    minMaxLoc(responseMat, nullptr, &maxVal, nullptr, &maxLoc);
//    cout << responseMat;

//    cout << "识别结果：" << chars[maxLoc.x] << "    相似度:" << maxVal * 100 << "%" << endl;

    vector<string> ret;
    ret.push_back(chars[maxLoc.x]);
    ret.push_back(to_string(maxVal * 100 / 1.404));
    return ret;
}

void CharRecognition::ANN_Train_SP(string dir, int hiddenLayerNum, int iterNum,
                                   double thresholdValue)
{
    const string perFileReadPath = dir;

    const int maxNum = 130;

    string chars[CLASS_NUM3] = {"A", "B", "C", "D", "E", "F", "G", "H", "J", "K",
                                "L", "M", "N", "P", "Q", "R", "S", "T", "U", "V",
                                "W", "X", "Y", "Z"};

//    vector<float> feat;

    float trainingData[TOTALNUM3][DIM];

    float labels[TOTALNUM3][CLASS_NUM3];//训练样本标签


    int count = 0;

    for (int i = 0; i < CLASS_NUM3; i++)//不同类
    {
        int sampleNumPerClass = 0;//训练字符每类数量

        string temp = chars[i];

        for(int j = 0; j < maxNum; j++){

            Mat srcImage = imread(perFileReadPath + "/" + temp + "/I" + to_string(j+1) + ".png");

            if(srcImage.data == NULL){
                sampleNumPerClass = j;
                break;
            }

        }

        for(int j = 0; j < sampleNumPerClass; j++){

            Mat srcImage = imread(perFileReadPath + "/" + temp + "/I" + to_string(j+1) + ".png");

            if(srcImage.data != NULL){
                cvtColor(srcImage, srcImage, CV_BGR2GRAY);
                threshold(srcImage, srcImage, 120, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

                int h = srcImage.rows;
                int w = srcImage.cols;
                Mat transformMat = Mat::eye(2, 3, CV_32F); // 创建对角阵
                int m = max(w, h);
                transformMat.at<float>(0, 2) = m / 2 - w / 2;
                transformMat.at<float>(1, 2) = m / 2 - h / 2;

                // 仿射变换
                Mat warpImage(m, m, srcImage.type());
                warpAffine(srcImage, warpImage, transformMat, warpImage.size(), INTER_LINEAR, BORDER_CONSTANT, Scalar(0));

                Mat out;
                resize(warpImage, out, Size(MSIZE, MSIZE));

//            calcGradientFeat(srcImage, feat);
//            calcGrayFeat(srcImage, feat);
                Mat feat = features(out, SSIZE);

                for(int k = 0; k < DIM; k++){
                    trainingData[count][k] = feat.at<float>(k);

                }

                // Set up label data
                for (int k = 0; k < CLASS_NUM3; ++k)
                {
                    if (k == i)
                        labels[count][k] = 1;
                    else
                        labels[count][k] = 0;
                }

                for(int k = 0; k < DIM; k++){
                    cout << trainingData[count][k] << " ";
                }
                cout << endl;



                count++;
            }

        }

    }

//
//
//    if (!feat.empty())
//    {
//        memcpy(trainingData, &feat[0], feat.size()*sizeof(float));
//    }

    // Set up training data Mat
    Mat trainingDataMat(TOTALNUM3, DIM, CV_32F, trainingData);
    cout << "trainingDataMat——OK！" << endl;


    Mat labelsMat(TOTALNUM3, CLASS_NUM3, CV_32F, labels);
    cout << "labelsMat——OK！" << endl;

    //训练代码
    cout << "training start...." << endl;

    int ar[]={DIM, hiddenLayerNum, CLASS_NUM3};
    Mat layerSizes(1,3,CV_32S,ar);

    Ptr<ANN_MLP> ann = ANN_MLP::create();
    ann->setLayerSizes(layerSizes);
    ann->setActivationFunction(ANN_MLP::SIGMOID_SYM);

    ann->setTrainMethod(ANN_MLP::BACKPROP);
    ann->setBackpropMomentumScale(0.1);
    ann->setBackpropWeightScale(0.1);
    ann->setTermCriteria(TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, iterNum, thresholdValue));

    Ptr<TrainData> tData = TrainData::create(trainingDataMat, ROW_SAMPLE, labelsMat);

    ann->train(tData);

    ann->save(dir+ "/ANN-Model-SP.xml"); //save classifier

    cout << "training finish...Model saved " << endl;

}

void CharRecognition::ANN_Train(string dir, int hiddenLayerNum, int iterNum,
                                double thresholdValue)
{

    const string perFileReadPath = dir;

    const int maxNum = 130;

    string chars[CLASS_NUM1] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
                                "A", "B", "C", "D", "E", "F", "G", "H", "J", "K",
                                "L", "M", "N", "P", "Q", "R", "S", "T", "U", "V",
                                "W", "X", "Y", "Z"};

//    vector<float> feat;

    float trainingData[TOTALNUM1][DIM];

    float labels[TOTALNUM1][CLASS_NUM1];//训练样本标签


    int count = 0;

    for (int i = 0; i < CLASS_NUM1; i++)//不同类
    {
        if(count > TOTALNUM1){
            break;
        }
        int sampleNumPerClass = 0;//训练字符每类数量

        string temp = chars[i];

        for(int j = 0; j < maxNum; j++){

            Mat srcImage = imread(perFileReadPath + "/" + temp + "/I" + to_string(j+1) + ".png");

            if(srcImage.data == NULL){
                sampleNumPerClass = j;
                break;
            }

        }

        for(int j = 0; j < sampleNumPerClass; j++){

            Mat srcImage = imread(perFileReadPath + "/" + temp + "/I" + to_string(j+1) + ".png");

            if(srcImage.data != NULL){
                cvtColor(srcImage, srcImage, CV_BGR2GRAY);
                threshold(srcImage, srcImage, 120, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

                int h = srcImage.rows;
                int w = srcImage.cols;
                Mat transformMat = Mat::eye(2, 3, CV_32F); // 创建对角阵
                int m = max(w, h);
                transformMat.at<float>(0, 2) = m / 2 - w / 2;
                transformMat.at<float>(1, 2) = m / 2 - h / 2;

                // 仿射变换
                Mat warpImage(m, m, srcImage.type());
                warpAffine(srcImage, warpImage, transformMat, warpImage.size(), INTER_LINEAR, BORDER_CONSTANT, Scalar(0));

                Mat out;
                resize(warpImage, out, Size(MSIZE, MSIZE));

//            calcGradientFeat(srcImage, feat);
//            calcGrayFeat(srcImage, feat);
                Mat feat = features(out, SSIZE);

                for(int k = 0; k < DIM; k++){
                    trainingData[count][k] = feat.at<float>(k);

                }

                // Set up label data
                for (int k = 0; k < CLASS_NUM1; ++k)
                {
                    if (k == i)
                        labels[count][k] = 1;
                    else
                        labels[count][k] = 0;
                }

                for(int k = 0; k < DIM; k++){
                    cout << trainingData[count][k] << " ";
                }
                cout << endl;



                count++;
            }

        }

    }

//
//
//    if (!feat.empty())
//    {
//        memcpy(trainingData, &feat[0], feat.size()*sizeof(float));
//    }

    // Set up training data Mat
    Mat trainingDataMat(TOTALNUM1, DIM, CV_32F, trainingData);
    cout << "trainingDataMat——OK！" << endl;


    Mat labelsMat(TOTALNUM1, CLASS_NUM1, CV_32F, labels);
    cout << "labelsMat——OK！" << endl;

    //训练代码

    cout << "training start...." << endl;

    int ar[]={DIM, hiddenLayerNum, CLASS_NUM1};
    Mat layerSizes(1,3,CV_32S,ar);

    Ptr<ANN_MLP> ann = ANN_MLP::create();
    ann->setLayerSizes(layerSizes);
    ann->setActivationFunction(ANN_MLP::SIGMOID_SYM);

    ann->setTrainMethod(ANN_MLP::BACKPROP);
    ann->setBackpropMomentumScale(0.1);
    ann->setBackpropWeightScale(0.1);
    ann->setTermCriteria(TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, iterNum, thresholdValue));

    Ptr<TrainData> tData = TrainData::create(trainingDataMat, ROW_SAMPLE, labelsMat);

    ann->train(tData);

    ann->save(perFileReadPath + "/ANN-Model.xml"); //save classifier

    cout << "training finish...Model saved " << endl;

}


void CharRecognition::ANN_Train_CH(string dir, int hiddenLayerNum, int iterNum,
                                   double thresholdValue)
{
    const string perFileReadPath = dir;

    const int maxNum = 130;

    string chars[CLASS_NUM2] = {"云", "京", "冀", "吉", "宁", "川", "新", "晋", "桂", "沪",
                                "津", "浙", "渝", "湘", "琼", "甘", "皖", "粤", "苏", "蒙",
                                "豫", "贵", "赣", "辽", "鄂", "闽", "陕", "青", "鲁", "黑"};

//    vector<float> feat;

    float trainingData[TOTALNUM2][DIM];

    float labels[TOTALNUM2][CLASS_NUM2];//训练样本标签

    int count = 0;

    for (int i = 0; i < CLASS_NUM2; i++)//不同类
    {
        int sampleNumPerClass = 0;//训练字符每类数量

        string temp = chars[i];

        for(int j = 0; j < maxNum; j++){

            Mat srcImage = imread(perFileReadPath + "/" + temp + "/I" + to_string(j+1) + ".png");

            if(srcImage.data == NULL){
                srcImage = imread(perFileReadPath + "/" + temp + "/I" + to_string(j+1) + ".bmp");
            }

            if(srcImage.data == NULL){
                sampleNumPerClass = j;
                break;
            }

        }

//        cout << sampleNumPerClass << " ";

        for(int j = 0; j < sampleNumPerClass; j++){

            Mat srcImage = imread(perFileReadPath + "/" + temp + "/I" + to_string(j+1) + ".png");

            if(srcImage.data == NULL){
                srcImage = imread(perFileReadPath + "/" + temp + "/I" + to_string(j+1) + ".bmp");
            }

            if(srcImage.data != NULL){
                cvtColor(srcImage, srcImage, CV_BGR2GRAY);
                threshold(srcImage, srcImage, 120, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

                int h = srcImage.rows;
                int w = srcImage.cols;
                Mat transformMat = Mat::eye(2, 3, CV_32F); // 创建对角阵
                int m = max(w, h);
                transformMat.at<float>(0, 2) = m / 2 - w / 2;
                transformMat.at<float>(1, 2) = m / 2 - h / 2;

                // 仿射变换
                Mat warpImage(m, m, srcImage.type());
                warpAffine(srcImage, warpImage, transformMat, warpImage.size(), INTER_LINEAR, BORDER_CONSTANT, Scalar(0));

                Mat out;
                resize(warpImage, out, Size(MSIZE, MSIZE));

//            calcGradientFeat(srcImage, feat);
//            calcGrayFeat(srcImage, feat);
                Mat feat = features(out, SSIZE);

                for(int k = 0; k < DIM; k++){
                    trainingData[count][k] = feat.at<float>(k);

                }

                // Set up label data
                for (int k = 0; k < CLASS_NUM2; ++k)
                {
                    if (k == i)
                        labels[count][k] = 1;
                    else
                        labels[count][k] = 0;
                }

                for(int k = 0; k < DIM; k++){
                    cout << trainingData[count][k] << " ";
                }
                cout << endl;


                count++;
            }

        }

    }

//
//
//    if (!feat.empty())
//    {
//        memcpy(trainingData, &feat[0], feat.size()*sizeof(float));
//    }

    // Set up training data Mat
    Mat trainingDataMat(TOTALNUM2, DIM, CV_32F, trainingData);
    cout << "trainingDataMat——OK！" << endl;


    Mat labelsMat(TOTALNUM2, CLASS_NUM2, CV_32F, labels);
    cout << "labelsMat——OK！" << endl;

    //训练代码

    cout << "training start...." << endl;

    int ar[]={DIM, hiddenLayerNum, CLASS_NUM2};
    Mat layerSizes(1,3,CV_32S,ar);

    Ptr<ANN_MLP> ann = ANN_MLP::create();
    ann->setLayerSizes(layerSizes);
    ann->setActivationFunction(ANN_MLP::SIGMOID_SYM);

    ann->setTrainMethod(ANN_MLP::BACKPROP);
    ann->setBackpropMomentumScale(0.1);
    ann->setBackpropWeightScale(0.1);
    ann->setTermCriteria(TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, iterNum, thresholdValue));

    Ptr<TrainData> tData = TrainData::create(trainingDataMat, ROW_SAMPLE, labelsMat);

    ann->train(tData);

    ann->save(dir + "/ANN-Model-CH.xml"); //save classifier

    cout << "training finish...Model saved " << endl;

}

float CharRecognition::calcMatValue(const Mat &image)
{
    float sumValue = 0;
    int r = image.rows;
    int c = image.cols;
    if (image.isContinuous())
    {
        c = r*c;
        r = 1;
    }
    for (int i = 0; i < r; i++)
    {
        const uchar* linePtr = image.ptr<uchar>(i);
        for (int j = 0; j < c; j++)
        {
            sumValue += linePtr[j];
        }
    }
    return sumValue;
}

void CharRecognition::calcGradientFeat(const Mat& imgSrc, vector<float>& feat)
{
    Mat image;
    cvtColor(imgSrc,image,CV_BGR2GRAY);
    resize(image,image,Size(8,16));

    // 计算x方向和y方向上的滤波
    float mask[3][3] = { { 1, 2, 1 }, { 0, 0, 0 }, { -1, -2, -1 } };
    Mat y_mask = Mat(3, 3, CV_32F, mask) / 8;
    Mat x_mask = y_mask.t(); // 转置
    Mat sobelX, sobelY;

    filter2D(image, sobelX, CV_32F, x_mask);
    filter2D(image, sobelY, CV_32F, y_mask);

    sobelX = abs(sobelX);
    sobelY = abs(sobelY);

    float totalValueX = calcMatValue(sobelX);
    float totalValueY = calcMatValue(sobelY);

// 将图像划分为4*2共8个格子，计算每个格子里灰度值总和的百分比
    for (int i = 0; i < image.rows; i = i + 4)
    {
        for (int j = 0; j < image.cols; j = j + 4)
        {
            Mat subImageX = sobelX(Rect(j, i, 4, 4));
            feat.push_back(calcMatValue(subImageX) / totalValueX);
            Mat subImageY= sobelY(Rect(j, i, 4, 4));
            feat.push_back(calcMatValue(subImageY) / totalValueY);
        }
    }
}

void CharRecognition::calcGrayFeat(const Mat& imgSrc, vector<float>& feat)
{
    Mat image;
    cvtColor(imgSrc,image,CV_BGR2GRAY);
    resize(image,image,Size(4,8));

    for (int i = 0; i < image.rows; i++)
    {
        for (int j = 0; j < image.cols; j++)
        {

            feat.push_back((float)image.at<char>(i, j));
//            cout << (float)image.at<char>(i, j) << " ";

        }
    }
//    cout << endl;
}

/* 累积直方图 */
Mat CharRecognition::ProjectHistogram(Mat img, int t) {

    // 如果是竖直方向，t = 0，sz = img.cols
    // 总之，选取长宽的最大值，以便创建完全包含数字图像的矩阵
    int sz = (t) ? img.rows : img.cols;
    Mat mhist = Mat::zeros(1, sz, CV_32F); // mhist是一个1 * sz的矩阵

    // 按行/列 统计非零像素值的个数，并保存在mhist中
    for (int j = 0; j < sz; j++) {
        Mat data = (t) ? img.row(j) : img.col(j);
        mhist.at<float>(j) = countNonZero(data);
    }

    double min, max;
    minMaxLoc(mhist, &min, &max); // 找到矩阵中的最大值，以便归一化

    if (max > 0) {
        // 矩阵的每一个元素都除以最大值，这正是归一化操作
        mhist.convertTo(mhist, -1, 1.0f/max, 0);
        return mhist;
    }
    return mhist;
}

/*
  创建特征矩阵
  水平方向累积直方图 + 竖直方向累积直方图 + 低分辨率图像
*/
Mat CharRecognition::features(Mat in, int sizeData) {

    const int HORIZONTAL = 1; // 水平方向，在创建累积直方图时，需要用到
    const int VERTICAL = 0; // 竖直方向，在创建累积直方图时，需要用到
    // 分别在水平方向和垂直方向上 创建累积直方图

    Mat vhist = ProjectHistogram(in, VERTICAL);
    Mat hhist = ProjectHistogram(in, HORIZONTAL);

    // 低分辨率图像
    // 低分辨率图像中的每一个像素都将被保存在特征矩阵中
    Mat lowData;
    resize(in, lowData, Size(sizeData, sizeData));

    // 特征矩阵的列数
    int numCols = vhist.cols + hhist.cols + lowData.cols * lowData.cols;
    Mat out = Mat::zeros(1, numCols, CV_32F); // 创建特征矩阵

    // 向特征矩阵赋值
    int j = 0;
    // 首先把水平方向累积直方图的值，存到特征矩阵中
    for (int i = 0; i < vhist.cols; i++) {
        out.at<float>(j) = vhist.at<float>(i);
        j++;
    }
    // 然后把竖直方向累积直方图的值，存到特征矩阵中
    for (int i = 0; i < hhist.cols; i++) {
        out.at<float>(j) = hhist.at<float>(i);
        j++;
    }
    // 最后把低分辨率图像的像素值，存到特征矩阵中
    for (int x = 0; x < lowData.cols; x++) {
        for (int y = 0; y < lowData.rows; y++){
            out.at<float>(j) = (float)lowData.at<unsigned char>(x, y);
            j++;
        }
    }
    return out;
}
//
///*
//  训练和识别
//  注：为了测试方便，我把训练和识别写到一个函数里了
//     正常情况下，应该单独封装为函数
//*/
//int classificationANN(Mat TrainingData, Mat classes, int nlayers, Mat f){
//
//    // step1. 生成训练数据
//    Mat trainClasses;
//    trainClasses.create(TrainingData.rows, numCharacters, CV_32FC1);
//    for (int i = 0; i < trainClasses.rows; i++) {
//        for (int j = 0; j < trainClasses.cols; j++) {
//            if (j == classes.at<int>(i))
//                trainClasses.at<float>(i, j) = 1;
//            else
//                trainClasses.at<float>(i, j) = 0;
//        }
//    }
//    Ptr<TrainData> trainingData = TrainData::create(TrainingData, ROW_SAMPLE, trainClasses);
//
//    // step2. 创建分类器
//    Mat layers(1, 3, CV_32SC1);
//    layers.at<int>(0) = TrainingData.cols;
//    layers.at<int>(1) = nlayers;
//    layers.at<int>(2) = numCharacters;
//
//    Ptr<ANN_MLP> ann = ANN_MLP::create();
//    ann->setLayerSizes(layers); // 设置层数
//    ann->setActivationFunction(ANN_MLP::SIGMOID_SYM, 1, 1); // 设置激励函数
//
//    // step3. 训练
//    ann->train(trainingData);
//
//    // step4. 预测
//    // 处理输入的特征Mat f
//    Mat src;
//    src.create(45, 77, CV_32FC1);
//    resize(f, src, src.size(), 0, 0, INTER_CUBIC);
//    src.convertTo(src, CV_32FC1);
//    src = src.reshape(1, 1);
//
//    Mat output(1, numCharacters, CV_32FC1);
//    ann->predict(f, output); // 开始预测
//
//
//    Point maxLoc;
//    double maxVal;
//    // output为一个向量，向量的每一个元素反映了输入样本属于每个类别的概率
//    minMaxLoc(output, 0, &maxVal, 0, &maxLoc); // 找到最大概率
//
//    // 返回字符在strCharacters[]数组中的索引
//    return maxLoc.x;
//}
//
//
//int main() {
//    // Step0. 预处理数字图像
//    Mat src = imread("7.jpg", 0); // 读取灰度图
//
//    int h = src.rows;
//    int w = src.cols;
//    Mat transformMat = Mat::eye(2, 3, CV_32F); // 创建对角阵
//    int m = max(w, h);
//    transformMat.at<float>(0, 2) = m / 2 - w / 2;
//    transformMat.at<float>(1, 2) = m / 2 - h / 2;
//
//    // 仿射变换
//    Mat warpImage(m, m, src.type());
//    warpAffine(src, warpImage, transformMat, warpImage.size(), INTER_LINEAR, BORDER_CONSTANT, Scalar(0));
//
//    Mat out;
//    resize(warpImage, out, Size(charSize, charSize)); // 重新调整大小
//    imshow("out", out);
//
//    // Step1. 读取训练数据OCR.xml
//    FileStorage fs;
//    fs.open("OCR.xml", FileStorage::READ);
//    Mat TrainingData;
//    Mat Classes;
//    fs["TrainingDataF5"] >> TrainingData;
//    fs["classes"] >> Classes;
//
//    // Step2. 创建特征矩阵
//    Mat f = features(out, 5);
//
//    // Step3. 训练 + 测试（写到一个函数里了）
//    int index = classificationANN(TrainingData, Classes, 10, f);
//    cout << strCharacters[index] << endl;
//
//    waitKey();
//    return 0;
//}

