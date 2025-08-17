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



\### Windows (MSVC / Visual Studio)

1\. Install \[OpenCV](https://opencv.org/) and ensure its `bin/` is on your PATH.

2\. Clone this repository:

&nbsp;  ```bash

&nbsp;  git clone https://github.com/<your-username>/marker-coverage-<your-full-name>.git

&nbsp;  cd marker-coverage-<your-full-name>



3\. Open in Visual Studio (with CMake integration enabled) or run:



mkdir build \&\& cd build

cmake ..

cmake --build . --config Release



\#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#



pacman -S mingw-w64-x86\_64-gcc mingw-w64-x86\_64-cmake mingw-w64-x86\_64-opencv

git clone https://github.com/<your-username>/marker-coverage-<your-full-name>.git

cd marker-coverage-<your-full-name>

mkdir build \&\& cd build

cmake -G "MinGW Makefiles" ..

mingw32-make



\#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#



macOS (clang + Homebrew):



brew install opencv cmake

mkdir build \&\& cd build

cmake ..

make



\#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#



Linux (Ubuntu/Debian):



sudo apt-get install build-essential cmake libopencv-dev

mkdir build \&\& cd build

cmake ..

make



\#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#



Usage Examples



./marker\_coverage marker01.png marker02.jpg --debug

\# Example output

marker01.png 83%

marker02.jpg 12%



Run in test mode:

./marker\_coverage --test



\[ OK ] EmptyImage

\[ OK ] Synthetic3x3Marker

Summary: 2 passed, 0 failed

\#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

Static Analysis



You can run static analysis using tools like:



Windows (MSVC)



Run Code Analysis from Visual Studio (Analyze → Run Code Analysis).



Linux/macOS

clang-tidy src/\*.cpp -- -Iinclude -std=c++17

cppcheck --enable=all --inconclusive --std=c++17 src/

\#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

Dockerfile Usage:



\# Build image

docker build -t marker-coverage .



\# Run in test mode

docker run --rm marker-coverage



\# Run on a real image (mount a host folder with images)

docker run --rm -v $(pwd):/data marker-coverage ./build/marker\_coverage /data/test3.png



