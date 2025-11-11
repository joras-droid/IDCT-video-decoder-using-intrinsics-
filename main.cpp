#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>

bool fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

int main() {
    // --- Argument/usage check ---
   
    // Use the provided file path
    std::string filename = "C:/output.jpeg";

    // --- File existence check ---
    if (!fileExists(filename)) {
        std::cerr << "Error: File '" << filename << "' does not exist." << std::endl;
        return -1;
    }

    // --- Reading the image ---
    cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR); // Color by default
    if (image.empty()) {
        std::cerr << "Error: Failed to load image '" << filename << "'." << std::endl;
        return -1;
    }

    // --- Displaying the image ---
    cv::imshow("Displayed Image", image); // Use a friendly window title
    cv::waitKey(0); // Wait for a key press
    cv::destroyAllWindows(); // Good practice: clean up windows
    return 0;
}
