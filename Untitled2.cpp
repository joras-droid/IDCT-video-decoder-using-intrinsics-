#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cmath>

// Block matching: finds best match in reference frame for a block
// Returns motion vector (dx, dy) and minimum SAD
struct MotionVector {
    int dx, dy;
    int sad;
};

MotionVector blockMatch(const cv::Mat& current, const cv::Mat& reference, 
                        int block_x, int block_y, int block_size, int search_range) {
    MotionVector mv = {0, 0, INT_MAX};
    
    // Extract current block
    cv::Rect block_rect(block_x, block_y, block_size, block_size);
    if (block_x + block_size > current.cols || block_y + block_size > current.rows)
        return mv;
    
    cv::Mat curr_block = current(block_rect);
    
    // Search in reference frame within search range
    for (int dy = -search_range; dy <= search_range; ++dy) {
        for (int dx = -search_range; dx <= search_range; ++dx) {
            int ref_x = block_x + dx;
            int ref_y = block_y + dy;
            
            // Check boundaries
            if (ref_x < 0 || ref_y < 0 || 
                ref_x + block_size > reference.cols || 
                ref_y + block_size > reference.rows)
                continue;
            
            cv::Mat ref_block = reference(cv::Rect(ref_x, ref_y, block_size, block_size));
            
            // Calculate SAD (Sum of Absolute Differences)
            int sad = 0;
            for (int r = 0; r < block_size; ++r) {
                for (int c = 0; c < block_size; ++c) {
                    sad += std::abs(curr_block.at<uchar>(r, c) - ref_block.at<uchar>(r, c));
                }
            }
            
            // Update best match
            if (sad < mv.sad) {
                mv.dx = dx;
                mv.dy = dy;
                mv.sad = sad;
            }
        }
    }
    return mv;
}

// Visualize motion vectors as arrows
void drawMotionVectors(cv::Mat& display, const std::vector<MotionVector>& mvs, 
                       int block_size, int cols_blocks) {
    for (size_t i = 0; i < mvs.size(); ++i) {
        int block_y = (i / cols_blocks) * block_size;
        int block_x = (i % cols_blocks) * block_size;
        
        cv::Point start(block_x + block_size/2, block_y + block_size/2);
        cv::Point end(start.x + mvs[i].dx * 2, start.y + mvs[i].dy * 2); // scale for visibility
        
        // Draw arrow (skip zero vectors for clarity)
        if (mvs[i].dx != 0 || mvs[i].dy != 0) {
            cv::arrowedLine(display, start, end, cv::Scalar(0, 255, 0), 2, cv::LINE_AA, 0, 0.3);
        }
        
        // Draw block boundary
        cv::rectangle(display, cv::Rect(block_x, block_y, block_size, block_size), 
                     cv::Scalar(255, 0, 0), 1);
    }
}

// Create motion compensated (predicted) frame
cv::Mat motionCompensate(const cv::Mat& reference, const std::vector<MotionVector>& mvs,
                        int block_size, int rows, int cols) {
    cv::Mat predicted = cv::Mat::zeros(rows, cols, reference.type());
    int cols_blocks = cols / block_size;
    
    for (size_t i = 0; i < mvs.size(); ++i) {
        int block_y = (i / cols_blocks) * block_size;
        int block_x = (i % cols_blocks) * block_size;
        
        int ref_x = block_x + mvs[i].dx;
        int ref_y = block_y + mvs[i].dy;
        
        // Copy predicted block
        if (ref_x >= 0 && ref_y >= 0 && 
            ref_x + block_size <= reference.cols && 
            ref_y + block_size <= reference.rows) {
            reference(cv::Rect(ref_x, ref_y, block_size, block_size))
                .copyTo(predicted(cv::Rect(block_x, block_y, block_size, block_size)));
        }
    }
    return predicted;
}

int main() {
    // Load image
    cv::Mat img = cv::imread("C:/input.jpg", cv::IMREAD_GRAYSCALE);
    if (img.empty()) {
        std::cerr << "Could not load image." << std::endl;
        return -1;
    }
    
    // Resize to reasonable size
    cv::Mat frame1;
    cv::resize(img, frame1, cv::Size(320, 240));
    
    // Create "frame 2" by shifting frame 1 (simulate motion)
    int shift_x = 10, shift_y = 5;
    cv::Mat frame2 = cv::Mat::zeros(frame1.size(), frame1.type());
    frame1(cv::Rect(0, 0, frame1.cols - shift_x, frame1.rows - shift_y))
        .copyTo(frame2(cv::Rect(shift_x, shift_y, frame1.cols - shift_x, frame1.rows - shift_y)));
    
    // Motion estimation parameters
    int block_size = 16;
    int search_range = 16;
    
    // Perform block matching
    std::vector<MotionVector> motion_vectors;
    std::cout << "Computing motion vectors..." << std::endl;
    
    for (int y = 0; y < frame2.rows; y += block_size) {
        for (int x = 0; x < frame2.cols; x += block_size) {
            MotionVector mv = blockMatch(frame2, frame1, x, y, block_size, search_range);
            motion_vectors.push_back(mv);
            std::cout << "Block(" << x << "," << y << ") -> MV(" << mv.dx << "," << mv.dy 
                     << ") SAD=" << mv.sad << std::endl;
        }
    }
    
    // Create predicted frame using motion vectors
    cv::Mat predicted = motionCompensate(frame1, motion_vectors, block_size, 
                                         frame1.rows, frame1.cols);
    
    // Calculate residual (difference between actual and predicted)
    cv::Mat residual;
    cv::absdiff(frame2, predicted, residual);
    
    // Visualize motion vectors
    cv::Mat frame2_color;
    cv::cvtColor(frame2, frame2_color, cv::COLOR_GRAY2BGR);
    drawMotionVectors(frame2_color, motion_vectors, block_size, frame2.cols / block_size);
    
    // Display results
    cv::imshow("Frame 1 (Reference)", frame1);
    cv::imshow("Frame 2 (Current)", frame2);
    cv::imshow("Motion Vectors", frame2_color);
    cv::imshow("Predicted Frame", predicted);
    cv::imshow("Residual (Difference)", residual * 5); // amplified for visibility
    
    cv::waitKey(0);
    return 0;
}
