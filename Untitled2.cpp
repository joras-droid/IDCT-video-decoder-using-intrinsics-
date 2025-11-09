#include <opencv2/opencv.hpp>
#include <iostream>
#include <iomanip>
void printMat(const cv::Mat& mat, const std::string& label) {
    std::cout << label << ":\n";
    for (int r = 0; r < mat.rows; ++r) {
        for (int c = 0; c < mat.cols; ++c) {
            std::cout << std::setw(4) << (int)mat.at<uchar>(r, c) << " ";
        }
        std::cout << std::endl;
    }
}

void printChannel(const std::vector<cv::Mat>& channel,const  std::string& label) {
    for (size_t i = 0; i < channel.size(); ++i)
    {
        std::string label_i = label+ "[" + std::to_string(i) + "]";
        printChannel(channel[i], label_i);
    }
}
int main() {
    // Step 1: Create a 4x4 color patch (each pixel has unique color)
    cv::Mat rgb(4, 4, CV_8UC3);
    for (int y = 0; y < 4; ++y) for (int x = 0; x < 4; ++x) {
        rgb.at<cv::Vec3b>(y, x) = cv::Vec3b(64 * x, 64 * y, 255 - 32 * x - 32 * y); // Just for clear visualization
    }
    printMat(rgb, "formed input macroblock of 4x4");

    // Step 2: Convert to YCbCr
    cv::Mat ycbcr;
    cv::cvtColor(rgb, ycbcr, cv::COLOR_BGR2YCrCb); // OpenCV uses YCrCb (Cr then Cb!)
    printMat(ycbcr, "formed YCBCR after conversion ");

    // Step 3: Split channels
    std::vector<cv::Mat> chans;
    //std::cout << "3 ota channels" <<  chans <<std::endl;  
    cv::split(ycbcr, chans); // chans[0] = Y, [1] = Cr, [2] = Cb
    cv::Mat& Y = chans[0];
    cv::Mat& Cr = chans[1];
    cv::Mat& Cb = chans[2];

    printMat(Y, "Y channels");
    printMat(Cb, "Cb channels");
    printMat(Cr, "Cr channels");


    // Step 4: Subsample Cb and Cr (4:2:0)
    // Keep Y at 4x4, but Cb/Cr become 2x2 (average each 2x2 block)
    cv::Mat Cb_sub, Cr_sub; // downsampled

    //INTER_AREA computes the average of pixels values that falls inside the area of the output pixel, this is what we want for subsampling,ideals for images with high frequency chrom details.
    cv::resize(Cb, Cb_sub, cv::Size(2, 2), 0, 0, cv::INTER_AREA);
    cv::resize(Cr, Cr_sub, cv::Size(2, 2), 0, 0, cv::INTER_AREA);
    //printMat(Y, "Y channels");
    printMat(Cb_sub, "Cb downsampled channels");
    printMat(Cr_sub, "Cr downsampled channels");

    // (Optional: Simulate DCT/quantization per plane here)
    // We'll skip real DCT for now, focus on subsampling
    // Uncomment to see saving (store Y as 4x4, Cb/Cr as 2x2!

    // Step 5: Upsample back (for display/reconstruction)
    //while unsampling, we did INTER_LINEAR which guesses the value of new pixel based on the weighted average of it's four nearest neighbouts in the low resolution image.
    cv::Mat Cb_up, Cr_up;
    cv::resize(Cb_sub, Cb_up, cv::Size(4, 4), 0, 0, cv::INTER_LINEAR);
    cv::resize(Cr_sub, Cr_up, cv::Size(4, 4), 0, 0, cv::INTER_LINEAR);
    //printMat(Y, "Y channels"); 
    printMat(Cb_up, "Cb upsampled channels");
    printMat(Cr_up, "Cr upsampled channels");

    // Step 6: Merge and convert back to RGB
    std::vector<cv::Mat> chans_rec = { Y, Cr_up, Cb_up };
    //printChannel(chans_rec, "we combined into one vector");

    cv::Mat ycbcr_rec;

    cv::merge(chans_rec, ycbcr_rec);
     
    printMat(ycbcr_rec, "REFORMED YCBCR for conversion to RCB");
    cv::Mat rgb_rec;
    cv::cvtColor(ycbcr_rec, rgb_rec, cv::COLOR_YCrCb2BGR);

    printMat(rgb_rec, "output before rescaling");
    // Step 7: Enlarge to see each pixel
    //INTER_NEAREST is used for resizing and replicating the same value for the resized pixels. but not good for natural or smooth photos
    cv::resize(rgb, rgb, cv::Size(160, 160), 0, 0, cv::INTER_NEAREST);
    cv::resize(rgb_rec, rgb_rec, cv::Size(160, 160), 0, 0, cv::INTER_NEAREST);
    printMat(rgb_rec, "output after rescaling");

    // Show before/after
    cv::imshow("Original RGB patch", rgb);
    cv::imshow("After 4:2:0 subsample/recon", rgb_rec);
    cv::waitKey(0);
    return 0;
}
