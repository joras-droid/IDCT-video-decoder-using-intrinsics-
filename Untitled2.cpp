#include <opencv2/opencv.hpp>
#include <vector>
#include "idctutils.h"

const int num_frames = 300;
const int blocks_w = 8, blocks_h = 8;
const int width = N * blocks_w;
const int height = N * blocks_h;
const int channels = 3; // BGR

int main() {
    std::vector<std::vector<std::vector<std::vector<std::vector<double>>>>> encoded_data(num_frames);
    generate_encoded_video(encoded_data, num_frames, blocks_h, blocks_w, channels);

    cv::namedWindow("Video Demo", cv::WINDOW_AUTOSIZE);

    for (int f = 0; f < num_frames; ++f) {
        cv::Mat img(height, width, CV_8UC3);
        for (int by = 0; by < blocks_h; ++by)
            for (int bx = 0; bx < blocks_w; ++bx) {
                double block[N][N], pix[N][N];
                for (int ch = 0; ch < channels; ++ch) {
                    for (int i = 0; i < N * N; ++i)
                        block[i / N][i % N] = encoded_data[f][by][bx][ch][i] * 1.2;
                    idct8x8(block, pix);
                    for (int x = 0; x < N; ++x)
                        for (int y = 0; y < N; ++y) {
                            int val = my_round(pix[x][y] + 127);
                            if (val < 0) val = 0;
                            if (val > 255) val = 255;
                            img.at<cv::Vec3b>(by * N + y, bx * N + x)[ch] = static_cast<uchar>(val);
                        }
                }
            }
        cv::imshow("Video Demo", img);
        if (cv::waitKey(33) >= 0) break;
    }
    return 0;
}
 