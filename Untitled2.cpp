#include <opencv2/opencv.hpp>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

// ---- 1. Utility: JPEG Quantization Table (Luma) ----
cv::Mat makeQuantLuma() {
    int qtab[16][16] = {
        {16,11,10,16,24,40,51,61},
        {12,12,14,19,26,58,60,55},
        {14,13,16,24,40,57,69,56},
        {14,17,22,29,51,87,80,62},
        {18,22,37,56,68,109,103,77},
        {24,35,55,64,81,104,113,92},
        {49,64,78,87,103,121,120,101},
        {72,92,95,98,112,100,103,99},
         
    };
    cv::Mat tab(8,8, CV_32F);
    for (int r = 0; r <8; ++r) for (int c = 0; c < 8; ++c) tab.at<float>(r, c) = float(qtab[r][c]);
    return tab;
}
cv::Mat idctQuantLuma() {
    int qtab[16][16] = {
      
        {18,22,37,56,68,109,103,77},
        {24,35,55,64,81,104,113,92},
        {49,64,78,87,103,121,120,101},
        {72,92,95,98,112,100,103,99},
          {16,11,10,16,24,40,51,61},
        {12,12,14,19,26,58,60,55},
        {14,13,16,24,40,57,69,56},
        {14,17,22,29,51,87,80,62},

    };
    cv::Mat tab(8, 8, CV_32F);
    for (int r = 0; r < 8; ++r) for (int c = 0; c < 8; ++c) tab.at<float>(r, c) = float(qtab[r][c]);
    return tab;
}

// ---- 2. Utility: Create Colorful BGR Gradient ------
cv::Mat colorfulGradient(int rows, int cols) {
    cv::Mat img(rows, cols, CV_8UC3);
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            uchar b = static_cast<uchar>(255 * c / (cols - 1));
            uchar g = static_cast<uchar>(255 * r / (rows - 1));
            uchar rch = static_cast<uchar>(255 * (r + c) / (rows + cols - 2));
            img.at<cv::Vec3b>(r, c) = cv::Vec3b(b, g, rch);
        }
    }
    return img;
}

// ---- 3. Blockwise DCT/Quantize/IDCT for one 8x8-channel----
void jpegBlockProcess(const cv::Mat& in, cv::Mat& out, const cv::Mat& Q, float qScale) {
    //CV_Assert(in.rows == 8 && in.cols == 8 && in.type() == CV_8U);
    cv::Mat f;
    in.convertTo(f, CV_32F);
    cv::Mat dct;
    cv::dct(f, dct);
    cv::Mat quant = dct / (Q * qScale);
    for (int y = 0; y <in.rows; ++y) for (int x = 0; x < in.cols; ++x)
        quant.at<float>(y, x) = std::round(quant.at<float>(y, x));
    cv::Mat deq = quant.mul(Q-20 * qScale);
    cv::Mat idct;
    cv::idct(deq, idct);
     
    idct.convertTo(out, CV_8U);
}

// ---- 4. Apply Blockwise Process to All Channels ------
cv::Mat processBlocks(const cv::Mat& img, float qScale, bool printFirstBlock = false) {
    // img: CV_8UC3
    cv::Mat ycbcr;
    cv::cvtColor(img, ycbcr, cv::COLOR_BGR2YCrCb);

    int paddedRows = (ycbcr.rows + 7) / 8 * 8, paddedCols = (ycbcr.cols + 7) / 8 * 8;
    cv::Mat padded;
    cv::copyMakeBorder(ycbcr, padded, 0, paddedRows - ycbcr.rows, 0, paddedCols - ycbcr.cols, cv::BORDER_REPLICATE);
    cv::Mat rec = padded.clone();
    cv::Mat Q = makeQuantLuma();
    cv::Mat IQ = idctQuantLuma();
    // for each 8x8 block in each channel
    for (int y = 0; y < padded.rows; y += 8) for (int x = 0; x < padded.cols; x += 8) {
        cv::Mat blk = padded(cv::Rect(x, y, 8, 8));//0,0 (8,0),
        std::vector<cv::Mat> chs(3), outChs(3), resizedChs(3);
       
        cv::split(blk, chs);//for bgr case, it splitted to b,g,r channel, now it splits to y,cb and cr channels.
        cv::Mat cb_rz, cr_rz;

        int width_rz = chs[1].cols / 2;
        int height_rz = chs[1].rows;
        cv::resize(chs[1], cr_rz,cv::Size(4,8),0,0, cv::INTER_AREA);
        cv::resize(chs[2], cb_rz, cv::Size(4, 8), 0, 0, cv::INTER_AREA);

        jpegBlockProcess(chs[0], resizedChs[0], Q, qScale);
        jpegBlockProcess(cr_rz, resizedChs[1], Q.colRange(0,4), qScale);
        jpegBlockProcess(cb_rz, resizedChs[2], Q.colRange(4, 8), qScale);
 
        cv::Mat outBlk;
        cv::resize(resizedChs[1], outChs[1], cv::Size(8, 8), 0, 0, cv::INTER_LINEAR);
        cv::resize(resizedChs[2], outChs[2], cv::Size(8, 8), 0, 0, cv::INTER_LINEAR);
        outChs[0] = chs[0];//yo missed out vako rahixa.

        cv::merge(outChs, outBlk);
        outBlk.copyTo(rec(cv::Rect(x, y, 8, 8)));
        int block_row_index = y / 8;
        int block_col_index = x / 8;

        //if ((block_row_index + block_col_index) % 2 == 0) {
        //    // Blocks where (row + col) is EVEN (e.g., top-left, 0,0) will be YELLOW
        //     cv::rectangle(rec, cv::Rect(x, y, 8, 8), cv::Scalar(0, 0, 255), 1); // YELLOW (BGR: 0, 255, 255)
        //}
        //else {
        //    // Blocks where (row + col) is ODD will be GREEN
        //    cv::rectangle(rec, cv::Rect(x, y, 8, 8), cv::Scalar(0, 255, 0), 1); // GREEN (BGR: 0, 255, 0)
        //}
        // print details on first processed block
        if (printFirstBlock && y == 0 && x == 0) {
            std::cout << "First block B, G, R before:\n";
            for (int rr = 0; rr < 8; ++rr) {
                for (int cc = 0; cc < 8; ++cc) {
                    cv::Vec3b pix = blk.at<cv::Vec3b>(rr, cc);
                    std::cout << "[" << (int)pix[0] << "," << (int)pix[1] << "," << (int)pix[2] << "] ";
                }
                std::cout << "\n";
            }
            std::cout << "First block B, G, R after (IDCT):\n";
            for (int rr = 0; rr < 8; ++rr) {
                for (int cc = 0; cc < 8; ++cc) {
                    cv::Vec3b pix = outBlk.at<cv::Vec3b>(rr, cc);
                    std::cout << "[" << (int)pix[0] << "," << (int)pix[1] << "," << (int)pix[2] << "] ";
                }
                std::cout << "\n";
            }
        }
    }
    cv::Mat bgr;
    cv::cvtColor(rec, bgr, cv::COLOR_YCrCb2BGR);
    return bgr;
}

// ---- 5. Enlarge helper for display ----
cv::Mat enlargeImage(const cv::Mat& image, int scale) {
    cv::Mat enlarged;
    cv::resize(image, enlarged, cv::Size(), scale, scale, cv::INTER_NEAREST);
    return enlarged;
}

int main() {
    // Generate and display the colorful input
    int rows = 32, cols = 32;
    cv::Mat img = cv::imread("C:/input.jpg", cv::IMREAD_COLOR);
    cv::Mat resized_image;
    cv::resize(
        img,      // Source image
        resized_image ,       // Destination image (will be allocated by OpenCV)
        cv::Size(620,620), // Target size (160x160)
        0,                   // fx (scale factor in X - set to 0 as we use target size)
        0,                   // fy (scale factor in Y - set to 0 as we use target size)
        cv::INTER_AREA    // Interpolation method (smooth for scaling down or up)
    );
    float qScale = 2.0f; // Try 1.0 for nearly lossless, 2.0+ for strong blockiness (like bad JPEG)
    // Process macroblock-wise and reconstruct
    cv::Mat rec = processBlocks(resized_image, qScale, true); // true = print first block debug
    // Enlarge both for macroblock visualization
    int scale = 7;
    cv::Mat enlarged_in = enlargeImage(resized_image, scale);
    cv::Mat enlarged_out = enlargeImage(rec, scale);

    cv::namedWindow("input", cv::WINDOW_NORMAL);
    cv::resizeWindow("input", 600,400);
    cv::imshow("input", enlarged_in);
    cv::namedWindow("output", cv::WINDOW_NORMAL);
    cv::resizeWindow("output", 600, 400);
    cv::imshow("output", enlarged_out);
    cv::waitKey(0);
    return 0;
}
