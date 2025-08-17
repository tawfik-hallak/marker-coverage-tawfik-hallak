#include "marker_detector.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdexcept>
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define ASSERT_TRUE(cond) \
    if (!(cond)) throw std::runtime_error("ASSERT_TRUE failed: " #cond);

#define ASSERT_EQ(a,b) \
    if (!((a) == (b))) throw std::runtime_error("ASSERT_EQ failed: " #a " != " #b);

#define ASSERT_GT(a,b) \
    if (!((a) > (b))) throw std::runtime_error("ASSERT_GT failed: " #a " <= " #b);




/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
cv::Mat createSyntheticMarker(int size = 300) {
    cv::Mat img = cv::Mat::zeros(size, size, CV_8UC3);

    std::vector<cv::Scalar> colors = {
        {0,0,255},   // red
        {0,255,0},   // green
        {255,0,0},   // blue
        {0,255,255}, // yellow
        {255,0,255}, // magenta
        {255,255,0}  // cyan
    };

    int cell = size / 3;
    int colorIndex = 0;
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            cv::Scalar color = colors[colorIndex % colors.size()];
            cv::rectangle(img, cv::Rect(col * cell, row * cell, cell, cell), color, -1);
            colorIndex++;
        }
    }
    return img;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


int run_unit_tests() {
    int passed = 0;
    int failed = 0;

    auto run = [&](auto name, auto fn) {
        try {
            fn();
            std::cout << "[ OK ] " << name << "\n";
            passed++;
        }
        catch (const std::exception& ex) {
            std::cerr << "[FAIL] " << name << " : " << ex.what() << "\n";
            failed++;
        }
        };

    run("EmptyImage", [] {
        MarkerDetector d;
        cv::Mat empty;
        auto res = d.detectAndComputeCoverage(empty, false);
        ASSERT_TRUE(!res.found);
        ASSERT_EQ(res.coveragePercent, 0);
        });

    run("Synthetic3x3Marker", [] {
        cv::Mat img = createSyntheticMarker();

        MarkerDetector d;
        auto res = d.detectAndComputeCoverage(img, false);

        ASSERT_TRUE(res.found);
        ASSERT_GT(res.coveragePercent, 20);
        });

    std::cout << "\nSummary: " << passed << " passed, " << failed << " failed\n";
    return failed > 0 ? 1 : 0;
}