#include <iostream>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "marker_detector.h"

int main() {

    //std::cout << "Working dir: " << std::filesystem::current_path() << "\n";
    cv::Mat img = cv::imread("test.png");
    if (img.empty()) {
        std::cerr << "Image not found!\n";
        return 1;
    }
    
    bool debugMode = false;

    MarkerDetector detector;
    auto result = detector.detectAndComputeCoverage(img, debugMode);


    if (!result.found) {
        std::cerr << "Marker not found.\n";
        return 1;
    }

    std::cout << "waka waka: " << result.coveragePercent << "%\n";


    return 0;
}
