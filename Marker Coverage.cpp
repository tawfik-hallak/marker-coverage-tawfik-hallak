#include "marker_detector.h"
#include <cmath>
#include <iostream>
#include "marker_detector.h"
#include <cmath>
#include <iostream>
#include <map>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


// ====== Canonical marker colors in BGR (approximate) ======
static const std::map<std::string, cv::Vec3b> MARKER_COLORS_BGR = {
    {"red",     cv::Vec3b(0, 0, 255)},
    {"green",   cv::Vec3b(0, 255, 0)},
    {"blue",    cv::Vec3b(255, 0, 0)},
    {"yellow",  cv::Vec3b(0, 255, 255)},
    {"magenta", cv::Vec3b(255, 0, 255)},
    {"cyan",    cv::Vec3b(255, 255, 0)}
};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// Convert BGR color to Lab for comparison
static cv::Vec3f bgr2labf(const cv::Vec3b& bgr) {
    cv::Mat tmp(1, 1, CV_8UC3, cv::Scalar(bgr[0], bgr[1], bgr[2]));
    cv::Mat lab;
    cv::cvtColor(tmp, lab, cv::COLOR_BGR2Lab);
    cv::Vec3b lab8 = lab.at<cv::Vec3b>(0, 0);
    return cv::Vec3f(lab8[0], lab8[1], lab8[2]);
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

cv::Mat MarkerDetector::preprocessImage(const cv::Mat& image) const {
    cv::Mat blurred;
    cv::GaussianBlur(image, blurred, cv::Size(3, 3), 0);
    return blurred;
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

std::vector<std::vector<cv::Point>> MarkerDetector::segmentColorPatches(const cv::Mat& image) const {
    cv::Mat hsv;
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);

    // Define HSV ranges for 6 marker colors
    // (values can be adjusted depending on your marker images)
    struct ColorRange {
        std::string name;
        cv::Scalar lower;
        cv::Scalar upper;
    };

    std::vector<ColorRange> ranges = {
        {"red1",    {0, 100, 100}, {10, 255, 255}},
        {"red2",    {160, 100, 100}, {179, 255, 255}}, // red wraps around
        {"green",   {40, 70, 70}, {80, 255, 255}},
        {"blue",    {100, 150, 50}, {130, 255, 255}},
        {"yellow",  {20, 100, 100}, {30, 255, 255}},
        {"cyan",    {85, 100, 100}, {95, 255, 255}},
        {"magenta", {140, 100, 100}, {160, 255, 255}}
    };

    std::vector<std::vector<cv::Point>> allContours;

    for (auto& cr : ranges) {
        cv::Mat mask;
        cv::inRange(hsv, cr.lower, cr.upper, mask);

        // Clean up noise
        cv::morphologyEx(mask, mask, cv::MORPH_OPEN, cv::Mat(), cv::Point(-1, -1), 1);
        cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, cv::Mat(), cv::Point(-1, -1), 1);

        // Find contours
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        for (auto& c : contours) {
            double area = cv::contourArea(c);
            if (area > 5 && area < 50000) {
                allContours.push_back(c);
            }
        }

        // Debug save
        cv::imwrite("mask_" + cr.name + ".png", mask);
    }

    return allContours;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// Step 4 & 5: Find marker polygon from patches
std::vector<cv::Point2f> MarkerDetector::findMarkerPolygon(
    const std::vector<std::vector<cv::Point>>& patchContours) const
{
    if (patchContours.size() < 9) {
        return {};
    }

    // Get all points from contours
    std::vector<cv::Point> allPoints;
    for (const auto& c : patchContours) {
        allPoints.insert(allPoints.end(), c.begin(), c.end());
    }

    // Convex hull around all patches
    std::vector<cv::Point> hull;
    cv::convexHull(allPoints, hull);

    // Approximate to reduce points — aim for quadrilateral
    std::vector<cv::Point> approx;
    cv::approxPolyDP(hull, approx, 10.0, true);

    // If not 4 points, keep hull as polygon
    std::vector<cv::Point2f> polygon;
    for (auto& p : (approx.size() >= 3 ? approx : hull)) {
        polygon.emplace_back(p.x, p.y);
    }

    return polygon;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


int MarkerDetector::computeCoveragePercent(const std::vector<cv::Point2f>& polygon, const cv::Size& imageSize) const {
    if (polygon.size() < 3) return 0;

    double markerArea = cv::contourArea(polygon);
    double imageArea = static_cast<double>(imageSize.width) * imageSize.height;
    if (imageArea <= 0) return 0;

    double ratio = (markerArea / imageArea) * 100.0;
    return static_cast<int>(std::round(ratio));
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void MarkerDetector::drawDebugOutput(cv::Mat& image, const std::vector<cv::Point2f>& polygon) const {
    if (polygon.size() >= 3) {
        std::vector<std::vector<cv::Point>> polyInt(1);
        for (const auto& pt : polygon) {
            polyInt[0].emplace_back(static_cast<int>(pt.x), static_cast<int>(pt.y));
        }
        cv::polylines(image, polyInt, true, cv::Scalar(0, 0, 255), 2);
    }
}



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
MarkerDetectionResult MarkerDetector::detectAndComputeCoverage(const cv::Mat& image, bool debugMode) {
    MarkerDetectionResult result;
    if (image.empty()) {
        std::cerr << "Error: empty image provided\n";
        return result;
    }

    cv::Mat preprocessed = preprocessImage(image);
    auto patchContours = segmentColorPatches(preprocessed);
    auto polygon = findMarkerPolygon(patchContours);

    if (polygon.empty()) {
        result.found = false;
        return result;
    }

    result.found = true;
    result.polygon = polygon;
    result.coveragePercent = computeCoveragePercent(polygon, image.size());

    if (debugMode) {
        cv::Mat dbgImg = image.clone();
        drawDebugOutput(dbgImg, polygon);
        cv::imwrite("debug_output.png", dbgImg);
    }

    return result;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
