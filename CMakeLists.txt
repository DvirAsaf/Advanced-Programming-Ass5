cmake_minimum_required(VERSION 3.16)
project(Ex5)

set(CMAKE_CXX_STANDARD 14)

add_executable(Ex5 MainTrain.cpp timeseries.cpp timeseries.h SimpleAnomalyDetector.cpp SimpleAnomalyDetector.h minCircle.cpp minCircle.h HybridAnomalyDetector.cpp HybridAnomalyDetector.h CLI.cpp CLI.h anomaly_detection_util.cpp anomaly_detection_util.h AnomalyDetector.h commands.h)