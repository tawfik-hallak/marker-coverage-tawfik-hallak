# ==============================
# Marker Coverage Estimator Dockerfile
# ==============================

# Base image
FROM ubuntu:22.04

# Install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libopencv-dev \
    && rm -rf /var/lib/apt/lists/*

# Working directory
WORKDIR /app

# Copy source code
COPY . .

# Build
RUN mkdir -p build && cd build && cmake .. && make -j$(nproc)

# Default command (example run with test mode)
CMD ["./build/marker_coverage", "--test"]