\# Marker Coverage Estimator



\## Problem Overview

This app detects a 3×3 color marker (red, green, blue, yellow, magenta, cyan) in an image,

computes its bounding polygon, and reports the marker area as a percentage of the total image.

Robust to rotation, tilt, blur, and moderate color shifts. Runtime ≤200 ms at 640×480.



\## Algorithm (≤150 words)

The image is converted to HSV. Each marker color is segmented via inRange thresholds,

and small blobs are filtered. The 9 patches are detected and arranged into a grid. The

outer corners define the marker polygon. Area is computed via `cv::contourArea()`,

and the ratio `(marker\_area / image\_area) \* 100` is printed. Debug mode saves intermediate

masks and overlays. Fallback convex-hull detection is provided.



\## Setup \& Build Instructions



\### Windows (Visual Studio / MSVC)

```bash

\# Install OpenCV (prebuilt or vcpkg)

git clone https://github.com/opencv/opencv.git

mkdir build \&\& cd build

cmake -A x64 ..

cmake --build . --config Release





\#######################



build on all platforms with:



mkdir build \&\& cd build

cmake ..

cmake --build .

\####################

macOS (clang + Homebrew):



brew install opencv cmake

mkdir build \&\& cd build

cmake ..

make

\#####################

Linux (Ubuntu/Debian):



sudo apt-get install build-essential cmake libopencv-dev

mkdir build \&\& cd build

cmake ..

make

\####################



./marker\_coverage marker01.png marker02.jpg --debug

\# Example output

marker01.png 83%

marker02.jpg 12%













