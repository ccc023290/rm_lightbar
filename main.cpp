#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
using namespace cv;
using namespace std;

int main(){
    VideoCapture cap("/mnt/d/微信聊天/xwechat_files/wxid_zwsr0ooxdqqc29_00c3/msg/file/2026-10/装甲板视频/装甲板视频/装甲板视频.avi");
    if (!cap.isOpened())
    {
        cerr << "视频打开失败！" << endl;
        return -1;
    }
    vector<RotatedRect> lightBars;
    Mat frame;
    while(cap.read(frame)){
        if (frame.empty())
        {
            cerr << "空帧！" << endl;
            break;
        }
        Mat hsv;
        cvtColor(frame, hsv, COLOR_BGR2HSV);
        Scalar lower_red1 = Scalar(0, 120, 120);
        Scalar upper_red1 = Scalar(10, 255, 255);
        Scalar lower_red2 = Scalar(160, 120, 120);
        Scalar upper_red2 = Scalar(180, 255, 255);
        Mat mask1, mask2;
        inRange(hsv, lower_red1, upper_red1, mask1);
        inRange(hsv, lower_red2, upper_red2, mask2);
        Mat mask = mask1 | mask2;

        vector<vector<Point>> contours;
        Mat hierarchy; // 修正：使用具名 Mat 变量接收 hierarchy
        findContours(mask, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        lightBars.clear();
        for (size_t i = 0; i < contours.size(); i++){
            vector<Point> &contour = contours[i];
            if(contour.size() < 2) continue;
            double area = contourArea(contour);
            if (area < 20){
                continue;
            }
            RotatedRect rrect = minAreaRect(contour);
            float w = rrect.size.width;
            float h = rrect.size.height;
            if (w < h) swap(w, h);
            float ratio = w / h;
            if (ratio > 3){
                lightBars.push_back(rrect);
            }
        }
        if(lightBars.size() >= 2){
            RotatedRect bar1 = lightBars[0];
            RotatedRect bar2 = lightBars[1];
            Point2f p1[4], p2[4];
            bar1.points(p1);
            bar2.points(p2);
            vector<Point2f> a;
            for(int k = 0; k < 4; k++) a.push_back(p1[k]);
            for(int k = 0; k < 4; k++) a.push_back(p2[k]);
            RotatedRect b = minAreaRect(a);
            Point2f armorPts[4];
            b.points(armorPts);
            for (int k = 0; k < 4; k++){
                line(frame, armorPts[k], armorPts[(k+1)%4], Scalar(0,255,0), 2);
            }
        }
        imshow("result", frame);
        int key = waitKey(30) & 0xff;
        if(key == 27) break;
    }
    cap.release();
    destroyAllWindows();
    return 0;
}