add_executable(marker_coverage
    src/main.cpp
    src/marker_detector.cpp
    tests/unit_tests.cpp
)
target_include_directories(marker_coverage PRIVATE include tests)
target_link_libraries(marker_coverage PRIVATE ${OpenCV_LIBS})