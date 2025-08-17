#include <iostream>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "marker_detector.h"
#include <opencv2/core/utils/logger.hpp>


int main(int argc, char** argv) {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);

    if (argc >= 2 && std::string(argv[1]) == "--test") {
        return run_unit_tests();
    }

    if (argc < 2) {
        std::cerr << "Usage: marker_coverage <image_path> [--debug]\n";
        return 1;
    }

    bool debugMode = (argc > 2 && std::string(argv[2]) == "--debug");

    cv::Mat img = cv::imread(argv[1]);
    MarkerDetector detector;
    auto result = detector.detectAndComputeCoverage(img, debugMode);

    if (!result.found) {
        return 1; // Marker not found
    }

    std::cout << argv[1] << " " << result.coveragePercent << "%\n";
    return 0;
}