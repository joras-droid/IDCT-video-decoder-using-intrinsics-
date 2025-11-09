#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    int block_size = 8;

    int width = 920, height = 780; 
    cv::Mat block(block_size, block_size, CV_8UC3);
    //cv::Mat img(height, width, CV_8UC3);

    // fill pixels with a color pattern (demo)
    for (int y = 0; y < block_size; ++y) {
        for (int x = 0; x < block_size; ++x) {
            //RGB order for OPencv
            if (y % 2 == 0) {
                block.at<cv::Vec3b>(y, x) = cv::Vec3b(255, 0, 0);
            }
            else {
                block.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 255);
            }
        }
    }
    cv::Mat block_big;
    cv::resize(block, block_big, cv::Size( 320,  320), 0, 0, cv::INTER_NEAREST);
        //cv::namedWindow("Colorful Pixels", cv::WINDOW_NORMAL);
        //cv::setWindowProperty("Colorful Pixels", cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);

        cv::imshow(
            "one macroblock", block_big
        );
        cv::waitKey(0);
        return 0;
    

}
 