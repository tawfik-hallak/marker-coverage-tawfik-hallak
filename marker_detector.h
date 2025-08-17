#pragma once
#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

struct MarkerDetectionResult {
    bool found = false;
    std::vector<cv::Point2f> polygon; // Bounding polygon points (clockwise)
    int coveragePercent = 0;          // Rounded 0–100%
};

class MarkerDetector {
public:
    MarkerDetector() = default;

    // Main entry: detect marker polygon and compute coverage
    MarkerDetectionResult detectAndComputeCoverage(const cv::Mat& image, bool debugMode = false);

private:
    // --- Internal helper functions ---
    cv::Mat preprocessImage(const cv::Mat& image) const;

    std::vector<std::vector<cv::Point>> segmentColorPatches(const cv::Mat& image) const;

    std::vector<cv::Point2f> findMarkerPolygon(const std::vector<std::vector<cv::Point>>& patchContours) const;

    int computeCoveragePercent(const std::vector<cv::Point2f>& polygon, const cv::Size& imageSize) const;

    void drawDebugOutput(cv::Mat& image, const std::vector<cv::Point2f>& polygon) const;
};
int run_unit_tests();