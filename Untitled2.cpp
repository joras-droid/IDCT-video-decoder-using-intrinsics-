#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    int width = 1920, height = 1080;
    cv::Mat img(height, width, CV_8UC3);

    // fill pixels with a color pattern (demo)
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            //RGB order for OPencv
            img.at<cv::Vec3b>(y, x)[0] = x % 256;
            img.at<cv::Vec3b>(y, x)[1] = y % 256;
            img.at<cv::Vec3b>(y, x)[2] = (x+y) % 256;

        }
    }
        //cv::namedWindow("Colorful Pixels", cv::WINDOW_NORMAL);
        //cv::setWindowProperty("Colorful Pixels", cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);

        cv::imshow(
            "colorful pixels", img
        );
        cv::waitKey(0);
        return 0;
    

}
 