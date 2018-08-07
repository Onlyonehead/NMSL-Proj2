//
// Created by chenchang on 8/1/18.
//

#include "VideoDetect.h"

int VideoDetect::bwLabel(Mat & src, Mat & dst, vector<Feather> & featherList,
                         const Rect &rect, int minarea)
{
    Point p = rect.tl();
    int x=p.x, y=p.y;
    int height = rect.height+y;
    int width = rect.width+x;

    //行列最大值
    int rows = height;
    int cols = width;

    int labelValue = 0;
    Point seed, neighbor;
    stack<Point> pointStack;    // 堆栈

    int area = 0;               // 用于计算连通域的面积
    int leftBoundary = 0;       // 连通域的左边界，即外接最小矩形的左边框，横坐标值，依此类推
    int rightBoundary = 0;
    int topBoundary = 0;
    int bottomBoundary = 0;
    Rect box;                   // 外接矩形框
    Feather feather;

    featherList.clear();    // 清除数组

    dst.release();
    dst = src.clone();
    for( int i = y; i < rows; i++)
    {
        uchar *pRow = dst.ptr<uchar>(i);
        for( int j = x; j < cols; j++)
        {
            if(pRow[j] == 255)
            {
                area = 0;
                labelValue++;           // labelValue最大为254，最小为1.
                seed = Point(j, i);     // Point（横坐标，纵坐标）
                dst.at<uchar>(seed) = labelValue;
                pointStack.push(seed);

                area++;
                leftBoundary = seed.x;
                rightBoundary = seed.x;
                topBoundary = seed.y;
                bottomBoundary = seed.y;

                while(!pointStack.empty())
                {
                    neighbor = Point(seed.x+1, seed.y);
                    if((seed.x != (cols-1)) && (dst.at<uchar>(neighbor) == 255))
                    {
                        dst.at<uchar>(neighbor) = labelValue;
                        pointStack.push(neighbor);

                        area++;
                        if(rightBoundary < neighbor.x)
                            rightBoundary = neighbor.x;
                    }

                    neighbor = Point(seed.x, seed.y+1);
                    if((seed.y != (rows-1)) && (dst.at<uchar>(neighbor) == 255))
                    {
                        dst.at<uchar>(neighbor) = labelValue;
                        pointStack.push(neighbor);

                        area++;
                        if(bottomBoundary < neighbor.y)
                            bottomBoundary = neighbor.y;

                    }

                    neighbor = Point(seed.x-1, seed.y);
                    if((seed.x != 0) && (dst.at<uchar>(neighbor) == 255))
                    {
                        dst.at<uchar>(neighbor) = labelValue;
                        pointStack.push(neighbor);

                        area++;
                        if(leftBoundary > neighbor.x)
                            leftBoundary = neighbor.x;
                    }

                    neighbor = Point(seed.x, seed.y-1);
                    if((seed.y != 0) && (dst.at<uchar>(neighbor) == 255))
                    {
                        dst.at<uchar>(neighbor) = labelValue;
                        pointStack.push(neighbor);

                        area++;
                        if(topBoundary > neighbor.y)
                            topBoundary = neighbor.y;
                    }

                    seed = pointStack.top();
                    pointStack.pop();
                }

                //储存符合条件的连通区域边框
                if(area > minarea)
                    box = Rect(leftBoundary, topBoundary, rightBoundary-leftBoundary, bottomBoundary-topBoundary);
                rectangle(src, box, 255);//在原图上画框
                feather.area = area;
                feather.boundingbox = box;
                feather.label = labelValue;
                if(area > minarea){
                    featherList.push_back(feather);
                }
                else{
                    --labelValue;
                }
            }
        }
    }
    return labelValue;
}

void VideoDetect::backgroundInit(Mat &bgm, VideoCapture& v, int totalCount)
{
    int frameCount = 0;
    Mat imgNum, frame;
    while(v.read(frame)&&frameCount < totalCount){

        if(imgNum.empty()){
            imgNum = Mat::zeros(frame.size(),CV_32FC3);
        }
        accumulate(frame,imgNum);
        ++frameCount;
        if (waitKey(2) >= 0)
            break;

    }
    imgNum = imgNum/totalCount;
    imgNum.convertTo(imgNum,CV_8UC3);

    waitKey(10);

    bgm = imgNum.clone();
}

void VideoDetect::getFirstFrame(string videoPath, Mat &fFrame)
{
    VideoCapture capture(videoPath);
    if (!capture.isOpened())
    {
        return;
    }
    if (!capture.read(fFrame))
    {
        return;
    }
    capture.release();
}

bool fullInclude(Rect& r, Rect& roi){
    return (r.x+2<roi.x) && (r.y+2<roi.y) && (r.x+r.width-2)>(roi.x+roi.width) && (r.y+r.height-2)>(roi.y+roi.height);
}

bool VideoDetect::peccancyDetect(Rect &r, int* line) {
    int rx1, rx2, ry1, ry2;
    int lx1, lx2, ly1, ly2;
    int height, width;
    bool contain, result = false;

    rx1 = r.x;
    ry1 = r.y;
    rx2 = r.x+r.width;
    ry2 = r.y+r.height;

    lx1 = line[0];
    ly1 = line[1];
    lx2 = line[2];
    ly2 = line[3];
    contain = rx1<=lx1 && ry1<=ly1 && rx2>=lx2 && ry2>=ly2;

    if(contain){
        result = true;
    }else{
        if(lx1 == lx2){
            result = rx1<=lx1 && rx2>=lx2 && ly2>=ry1 && ly1<=ry2;
        }else if(ly1 == ly2){
            result = ry1<=ly1 && ry2>=ly2 && lx2>=rx1 && lx1<=rx2;
        }
    }

    return result;
}

int VideoDetect::vOperate_flow(string *path, double *rat, int minarea, int mindetect,
                                int initFrame, int fMax, double shadow, int mCore)
{
    string videoPath = path[0];

    vector<Trace> tracing;

    int height, width;

    int varThreshold = 30;

    Mat frame, blu;
    Mat foreground, background;        // 前景图片

//    VideoCapture capture("/home/chenchang/Documents/cc/1.0/vtest.avi");
    VideoCapture capture(videoPath);
    if (!capture.isOpened())
    {
        return -1;
    }

    VideoWriter writer=VideoWriter(path[1],//输出路径以及文件名
                                   CV_FOURCC('D', 'I', 'V', 'X'),//根据输入视频得到其编解码器
//                                   CV_FOURCC('U', '2', '6', '3'),//根据输入视频得到其编解码器
                                   (int)capture.get(CV_CAP_PROP_FPS),//根据输入视频得到帧率
                                   Size((int)capture.get( CV_CAP_PROP_FRAME_WIDTH ),//根据输入视频得到视频宽度
                                        (int)capture.get(CV_CAP_PROP_FRAME_HEIGHT)),//根据输入视频得到视频高度
                                   true//输出为彩色
    );

    if(!writer.isOpened())
        return -1;

    // 混合高斯物体
    Ptr<BackgroundSubtractorMOG2> bgsubtractor = createBackgroundSubtractorMOG2();
    bgsubtractor->setVarThreshold(varThreshold);//模型匹配阈值

    bgsubtractor->setVarInit(15);

    bgsubtractor->setShadowThreshold(shadow);

    backgroundInit(background, capture, initFrame);

    height = background.rows;
    width = background.cols;

    //允许追踪的最大区域面积（界面大小的四分之一）
    int maxarea = width*height/4;

    GaussianBlur(background, blu, Size(3, 3), 1);

    bgsubtractor->apply(blu, foreground, 0.02);

    capture.open(videoPath);

    //自定义检测区域
    Rect rs((int)(width*rat[0]), (int)(height*rat[1]), (int)(width*rat[2]), (int)(height*rat[3]));
//    cout << rat[0] << rat[1] << rat[2] << rat[3] << endl;

    int fCount = 0;
    while (1)
    {

        if (!capture.read(frame))
        {
            break;
        }

        Mat frame2 = frame.clone();

        GaussianBlur(frame, blu, Size(3, 3), 1);

        if(fCount<fMax){
            bgsubtractor->apply(blu, foreground, 0.025);
            ++fCount;
        } else{
            bgsubtractor->apply(blu, foreground, 0.005);
        }

        // 输出的前景图片并不是2值图片，要处理一下显示
        threshold(foreground, foreground, 127, 255, THRESH_BINARY);

        //中值滤波去除椒盐噪声
        medianBlur(foreground, foreground, 5);

        //形态学滤波------
        Mat element1 = getStructuringElement(MORPH_RECT, Size(mCore, mCore));


        //闭运算——填充物体内细小空洞、连接邻近物体、平滑其边界的同时并不明显改变其面积，同时抑制比结构元小的暗细节
        Mat closeImage;
        morphologyEx(foreground, closeImage, MORPH_CLOSE, element1);


        vector<Feather> featherList;
        Mat dst;

        bwLabel(closeImage, dst, featherList, rs, minarea);
        // 为了方便观察，可以将label“放大”
        for( int i = 0; i < dst.rows; i++)
        {
            uchar *p = dst.ptr<uchar>(i);
            for( int j = 0; j < dst.cols; j++)
            {
                p[j] = 30*p[j];
            }
        }

        if(fCount >= fMax){
            for(vector<Feather>::iterator it = featherList.begin(); it < featherList.end(); it++)
            {
                rectangle(dst, it->boundingbox, 255);
                rectangle(frame, it->boundingbox, Scalar(255,0,0), 1);
            }
        }



        //车辆追踪
        if(fCount >= fMax){

            for(vector<Trace>::iterator itTrace = tracing.begin(); itTrace < tracing.end(); ++itTrace){
                int _vmin = vmin, _vmax = vmax;
                Mat hsv, hue, mask,backproj;

                cvtColor(frame, hsv, COLOR_BGR2HSV);
                inRange(hsv, Scalar(0, smin, MIN(_vmin,_vmax)),
                        Scalar(180, 256, MAX(_vmin, _vmax)), mask);
                int ch[] = {0, 0};
                hue.create(hsv.size(), hsv.depth());
                mixChannels(&hsv, 1, &hue, 1, ch, 1);


                calcBackProject(&hue, 1, 0, itTrace->hist, backproj, &phranges);
                backproj &= mask;
                RotatedRect trackBox = CamShift(backproj, itTrace->trackWindow,
                                                TermCriteria( TermCriteria::EPS | TermCriteria::COUNT, 10, 1 ));
                if( itTrace->trackWindow.area() <= 1 )
                {
                    int cols = backproj.cols, rows = backproj.rows, r = (MIN(cols, rows) + 5)/6;
                    itTrace->trackWindow = Rect(itTrace->trackWindow.x - r, itTrace->trackWindow.y - r,
                                                itTrace->trackWindow.x + r, itTrace->trackWindow.y + r) &
                                           Rect(0, 0, cols, rows);
                }

                itTrace->flag = 0;

                double width = itTrace->trackWindow.width, height = itTrace->trackWindow.height;
                double ratio = width/height;
                if((ratio>10||ratio<0.1) || itTrace->trackWindow.area()>maxarea){
                    tracing.erase(itTrace);
                    --itTrace;
                }
            }

            for(vector<Feather>::iterator it = featherList.begin(); it < featherList.end(); it++){
                vector<Trace>::iterator itTrace;

                for(itTrace = tracing.begin(); itTrace < tracing.end(); ++itTrace){
                    Rect intersection = it->boundingbox & itTrace->trackWindow;

                    //匹配成功
                    if(intersection.area()>it->boundingbox.area()*0.3 && fullInclude(rs, it->boundingbox)){

                        int _vmin = vmin, _vmax = vmax;
                        Mat hsv, hue, mask,backproj;

                        cvtColor(frame, hsv, COLOR_BGR2HSV);
                        inRange(hsv, Scalar(0, smin, MIN(_vmin,_vmax)),
                                Scalar(180, 256, MAX(_vmin, _vmax)), mask);
                        int ch[] = {0, 0};
                        hue.create(hsv.size(), hsv.depth());
                        mixChannels(&hsv, 1, &hue, 1, ch, 1);

                        Rect r = it->boundingbox;
//                        Rect selection(r.x+r.width/10, r.y+r.height/5, r.width*4/5, r.height*4/5);
                        Rect selection(r.x+r.width/5, r.y+r.height/5, r.width*3/5, r.height*3/5);
//                          Rect selection = it->boundingbox;

                        Mat roi(hue, selection), maskroi(mask, selection);
                        calcHist(&roi, 1, 0, maskroi, itTrace->hist, 1, &hsize, &phranges);
                        normalize(itTrace->hist, itTrace->hist, 0, 255, NORM_MINMAX);

                        itTrace->trackWindow = selection;

                        itTrace->flag = 1;

                        Rect rt = it->boundingbox;
                        if(rt.x>=rs.x+2 && rt.y+rt.height<=rs.y+rs.height-2){
                            if(itTrace->pic){
                                itTrace->p.x = rt.x+rt.width/2;
                                itTrace->p.y = rt.y+rt.height/2;
                                itTrace->pic = false;

                                itTrace->record = ++carCount;

                                Mat ROI = frame2(it->boundingbox);
                                string p = path[2]+
                                              to_string(itTrace->record)+".jpg";
                                imwrite(p, ROI);
                            } else{
                                Point pIndex(itTrace->p.x, itTrace->p.y);
                                itTrace->p.x = rt.x+rt.width/2;
                                itTrace->p.y = rt.y+rt.height/2;
                                double dis = powf((pIndex.x-itTrace->p.x), 2)+powf((pIndex.y-itTrace->p.y), 2);
                                dis = sqrt(dis);
                                itTrace->distance = itTrace->distance + dis;
                                ++(itTrace->frame);
                            }
                        }


                        Point end(it->boundingbox.x+it->boundingbox.width, it->boundingbox.y+it->boundingbox.height);
                        string str = to_string(itTrace->record);
                        putText(frame, str, end, cv::FONT_HERSHEY_DUPLEX, 0.5, cv::Scalar(0, 0, 255), 2);

                        break;
                    }
                }

                //首次捕获到的运动物体(面积大于给定值)
                if(itTrace==tracing.end() && fullInclude(rs, it->boundingbox) && it->boundingbox.area()>mindetect){

                    Trace trace;
                    int _vmin = vmin, _vmax = vmax;
                    Mat hsv, hue, mask,backproj;

                    cvtColor(frame, hsv, COLOR_BGR2HSV);
                    inRange(hsv, Scalar(0, smin, MIN(_vmin,_vmax)),
                            Scalar(180, 256, MAX(_vmin, _vmax)), mask);
                    int ch[] = {0, 0};
                    hue.create(hsv.size(), hsv.depth());
                    mixChannels(&hsv, 1, &hue, 1, ch, 1);


                    //获取ROI
                    Rect r = it->boundingbox;
//                    Rect selection(r.x+r.width/10, r.y+r.height/5, r.width*4/5, r.height*4/5);
                    Rect selection(r.x+r.width/5, r.y+r.height/5, r.width*3/5, r.height*3/5);
//                    Rect selection = it->boundingbox;

                    Mat roi(hue, selection), maskroi(mask, selection);
                    calcHist(&roi, 1, 0, maskroi, trace.hist, 1, &hsize, &phranges);
                    normalize(trace.hist, trace.hist, 0, 255, NORM_MINMAX);

                    trace.trackWindow = selection;




                    calcBackProject(&hue, 1, 0, trace.hist, backproj, &phranges);
                    backproj &= mask;
//                    imshow("back", backproj);
                    RotatedRect trackBox = CamShift(backproj, trace.trackWindow,
                                                    TermCriteria( TermCriteria::EPS | TermCriteria::COUNT, 10, 1 ));
                    if( trace.trackWindow.area() <= 1 )
                    {
                        int cols = backproj.cols, rows = backproj.rows, r = (MIN(cols, rows) + 5)/6;
                        trace.trackWindow = Rect(trace.trackWindow.x - r, trace.trackWindow.y - r,
                                                 trace.trackWindow.x + r, trace.trackWindow.y + r) &
                                            Rect(0, 0, cols, rows);
                    }
                    trace.flag = 1;

                    tracing.push_back(trace);
                }
            }

            for(vector<Trace>::iterator itTrace = tracing.begin(); itTrace < tracing.end(); ++itTrace){
                if(itTrace->flag == 0){
                    string info = to_string(itTrace->record)+" "+to_string(itTrace->frame)
                                  +" "+to_string(itTrace->distance);
                    ofstream out(path[3], ofstream::app);
                    out << info << endl;
                    out.close();

                    tracing.erase(itTrace);
                    --itTrace;
                }
            }
        }

        rectangle(frame, rs, Scalar(255,255,255), 1);
        writer << frame;

        if (waitKey(10) >= 0)
            break;
    }
    bgsubtractor->getBackgroundImage(background);
    imwrite(path[4], background);

    return carCount;
}

int VideoDetect::vOperatePeccancy(string *path, double *rat, long *time, int minarea, int mindetect,
                                   int initFrame, int fMax, double shadow, int mCore)
{
    string videoPath = path[0];

    vector<Trace> tracing;

    int height, width;

    int varThreshold = 30;

    Mat frame, blu;
    Mat foreground, background;        // 前景图片

    VideoCapture capture(videoPath);
    if (!capture.isOpened())
    {
        return -1;
    }

    double spf = 1/capture.get(CV_CAP_PROP_FPS)*1000;
    double timeCounter = 0;

    VideoWriter writer=VideoWriter(path[1],//输出路径以及文件名
                                   CV_FOURCC('D', 'I', 'V', 'X'),//根据输入视频得到其编解码器
                                   (int)capture.get(CV_CAP_PROP_FPS),//根据输入视频得到帧率
                                   Size((int)capture.get( CV_CAP_PROP_FRAME_WIDTH ),//根据输入视频得到视频宽度
                                        (int)capture.get(CV_CAP_PROP_FRAME_HEIGHT)),//根据输入视频得到视频高度
                                   true//输出为彩色
    );

    if(!writer.isOpened())
        return -1;

    // 混合高斯物体
    Ptr<BackgroundSubtractorMOG2> bgsubtractor = createBackgroundSubtractorMOG2();
    bgsubtractor->setVarThreshold(varThreshold);//模型匹配阈值

    bgsubtractor->setVarInit(15);

    bgsubtractor->setShadowThreshold(shadow);

    backgroundInit(background, capture, initFrame);

    height = background.rows;
    width = background.cols;

    //允许追踪的最大区域面积（界面大小的四分之一）
    int maxarea = width*height/4;

    GaussianBlur(background, blu, Size(3, 3), 1);

    bgsubtractor->apply(blu, foreground, 0.02);

    capture.open(videoPath);

    //检测区域（全屏）
    Rect rs(0, 0, width, height);

    //线
    int myLine[4] = {(int)(rat[0]*width), (int)(rat[1]*height), (int)((rat[0]+rat[2])*width), (int)((rat[1]+rat[3])*height)};

    int fCount = 0;
    while (1)
    {
        if (!capture.read(frame))
        {
            break;
        }
        timeCounter += spf;

        Mat frame2 = frame.clone();

        GaussianBlur(frame, blu, Size(3, 3), 1);

        if(fCount<fMax){
            bgsubtractor->apply(blu, foreground, 0.025);
            ++fCount;
        } else{
            bgsubtractor->apply(blu, foreground, 0.005);
        }

        // 输出的前景图片并不是2值图片，要处理一下显示
        threshold(foreground, foreground, 127, 255, THRESH_BINARY);

        //中值滤波去除椒盐噪声
        medianBlur(foreground, foreground, 5);

        //形态学滤波------
        Mat element1 = getStructuringElement(MORPH_RECT, Size(mCore, mCore));


        //闭运算——填充物体内细小空洞、连接邻近物体、平滑其边界的同时并不明显改变其面积，同时抑制比结构元小的暗细节
        Mat closeImage;
        morphologyEx(foreground, closeImage, MORPH_CLOSE, element1);


        vector<Feather> featherList;
        Mat dst;

        bwLabel(closeImage, dst, featherList, rs, minarea);
        // 为了方便观察，可以将label“放大”
        for( int i = 0; i < dst.rows; i++)
        {
            uchar *p = dst.ptr<uchar>(i);
            for( int j = 0; j < dst.cols; j++)
            {
                p[j] = 30*p[j];
            }
        }


        //车辆追踪
//        cout << timeCounter << " " << time[0] << " " << time[1] << endl;
        if(fCount >= fMax && timeCounter>=time[0] && timeCounter<=time[1]){

            for(vector<Trace>::iterator itTrace = tracing.begin(); itTrace < tracing.end(); ++itTrace){
                int _vmin = vmin, _vmax = vmax;
                Mat hsv, hue, mask,backproj;

                cvtColor(frame, hsv, COLOR_BGR2HSV);
                inRange(hsv, Scalar(0, smin, MIN(_vmin,_vmax)),
                        Scalar(180, 256, MAX(_vmin, _vmax)), mask);
                int ch[] = {0, 0};
                hue.create(hsv.size(), hsv.depth());
                mixChannels(&hsv, 1, &hue, 1, ch, 1);


                calcBackProject(&hue, 1, 0, itTrace->hist, backproj, &phranges);
                backproj &= mask;
                RotatedRect trackBox = CamShift(backproj, itTrace->trackWindow,
                                                TermCriteria( TermCriteria::EPS | TermCriteria::COUNT, 10, 1 ));
                if( itTrace->trackWindow.area() <= 1 )
                {
                    int cols = backproj.cols, rows = backproj.rows, r = (MIN(cols, rows) + 5)/6;
                    itTrace->trackWindow = Rect(itTrace->trackWindow.x - r, itTrace->trackWindow.y - r,
                                                itTrace->trackWindow.x + r, itTrace->trackWindow.y + r) &
                                           Rect(0, 0, cols, rows);
                }

                itTrace->flag = 0;

                double ratio = (double)itTrace->trackWindow.width/(double)itTrace->trackWindow.height;
                if((ratio>10||ratio<0.1) || itTrace->trackWindow.area()>maxarea){
                    tracing.erase(itTrace);
                    --itTrace;
                }
            }

            for(vector<Feather>::iterator it = featherList.begin(); it < featherList.end(); it++){
                vector<Trace>::iterator itTrace;

                for(itTrace = tracing.begin(); itTrace < tracing.end(); ++itTrace){
                    Rect intersection = it->boundingbox & itTrace->trackWindow;

                    //匹配成功
//                    cout << it->boundingbox << "  " << myLine[0] << " " << myLine[1] << endl;
                    if(intersection.area()>it->boundingbox.area()*0.3 && peccancyDetect(it->boundingbox, myLine)){

                        int _vmin = vmin, _vmax = vmax;
                        Mat hsv, hue, mask,backproj;

                        cvtColor(frame, hsv, COLOR_BGR2HSV);
                        inRange(hsv, Scalar(0, smin, MIN(_vmin,_vmax)),
                                Scalar(180, 256, MAX(_vmin, _vmax)), mask);
                        int ch[] = {0, 0};
                        hue.create(hsv.size(), hsv.depth());
                        mixChannels(&hsv, 1, &hue, 1, ch, 1);

                        Rect r = it->boundingbox;
//                        Rect selection(r.x+r.width/10, r.y+r.height/5, r.width*4/5, r.height*4/5);
                        Rect selection(r.x+r.width/5, r.y+r.height/5, r.width*3/5, r.height*3/5);
//                          Rect selection = it->boundingbox;

                        Mat roi(hue, selection), maskroi(mask, selection);
                        calcHist(&roi, 1, 0, maskroi, itTrace->hist, 1, &hsize, &phranges);
                        normalize(itTrace->hist, itTrace->hist, 0, 255, NORM_MINMAX);

                        itTrace->trackWindow = selection;

                        itTrace->flag = 1;

                        if(itTrace->pic){
                            itTrace->pic = false;

                            itTrace->record = ++carCount;

                            Point end(it->boundingbox.x+it->boundingbox.width, it->boundingbox.y+it->boundingbox.height);
                            string str = to_string(itTrace->record);
                            putText(frame, str, end, cv::FONT_HERSHEY_DUPLEX, 0.5, cv::Scalar(0, 0, 255), 2);

                            Mat ROI = frame2(it->boundingbox);
                            string p = path[2]+ to_string(itTrace->record)+".jpg";
                            imwrite(p, ROI);
                        }
                        rectangle(frame, it->boundingbox, Scalar(255,0,0), 1);

                        Point end(it->boundingbox.x+it->boundingbox.width, it->boundingbox.y+it->boundingbox.height);
                        string str = to_string(itTrace->record);
                        putText(frame, str, end, cv::FONT_HERSHEY_DUPLEX, 0.5, cv::Scalar(0, 0, 255), 2);

                        break;
                    }
                }

                //首次捕获到的压线车辆(面积大于给定值)
                if(itTrace==tracing.end() && peccancyDetect(it->boundingbox, myLine) && it->boundingbox.area()>mindetect){

                    Trace trace;
                    int _vmin = vmin, _vmax = vmax;
                    Mat hsv, hue, mask,backproj;

                    cvtColor(frame, hsv, COLOR_BGR2HSV);
                    inRange(hsv, Scalar(0, smin, MIN(_vmin,_vmax)),
                            Scalar(180, 256, MAX(_vmin, _vmax)), mask);
                    int ch[] = {0, 0};
                    hue.create(hsv.size(), hsv.depth());
                    mixChannels(&hsv, 1, &hue, 1, ch, 1);


                    //获取ROI
                    Rect r = it->boundingbox;
//                    Rect selection(r.x+r.width/10, r.y+r.height/5, r.width*4/5, r.height*4/5);
                    Rect selection(r.x+r.width/5, r.y+r.height/5, r.width*3/5, r.height*3/5);
//                    Rect selection = it->boundingbox;

                    Mat roi(hue, selection), maskroi(mask, selection);
                    calcHist(&roi, 1, 0, maskroi, trace.hist, 1, &hsize, &phranges);
                    normalize(trace.hist, trace.hist, 0, 255, NORM_MINMAX);

                    trace.trackWindow = selection;




                    calcBackProject(&hue, 1, 0, trace.hist, backproj, &phranges);
                    backproj &= mask;
//                    imshow("back", backproj);
                    RotatedRect trackBox = CamShift(backproj, trace.trackWindow,
                                                    TermCriteria( TermCriteria::EPS | TermCriteria::COUNT, 10, 1 ));
                    if( trace.trackWindow.area() <= 1 )
                    {
                        int cols = backproj.cols, rows = backproj.rows, r = (MIN(cols, rows) + 5)/6;
                        trace.trackWindow = Rect(trace.trackWindow.x - r, trace.trackWindow.y - r,
                                                 trace.trackWindow.x + r, trace.trackWindow.y + r) &
                                            Rect(0, 0, cols, rows);
                    }
                    trace.flag = 1;

                    tracing.push_back(trace);
                }
            }

            for(vector<Trace>::iterator itTrace = tracing.begin(); itTrace < tracing.end(); ++itTrace){
                if(itTrace->flag == 0){
                    tracing.erase(itTrace);
                    --itTrace;
                }
            }

            putText(frame, "Red Light", Point(20, 20), cv::FONT_HERSHEY_DUPLEX, 0.5, cv::Scalar(0, 0, 255), 2);
        }else{
            putText(frame, "Green Light", Point(20, 20), cv::FONT_HERSHEY_DUPLEX, 0.5, cv::Scalar(0, 255, 0), 2);
        }

        line(frame, Point(myLine[0], myLine[1]), Point(myLine[2], myLine[3]), Scalar(0, 0, 255), 2);
        writer << frame;

        if (waitKey(10) >= 0)
            break;
    }
    bgsubtractor->getBackgroundImage(background);
    imwrite(path[3], background);

    return carCount;
}
