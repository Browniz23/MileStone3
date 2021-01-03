cmake_minimum_required(VERSION 3.17)
project(MileStone3)

set(CMAKE_CXX_STANDARD 11)

add_executable(MileStone3 anomaly_detection_util.cpp anomaly_detection_util.h AnomalyDetector.h timeseries.h SimpleAnomalyDetector.h timeseries.cpp SimpleAnomalyDetector.cpp HybridAnomalyDetector.h HybridAnomalyDetector.cpp CLI.cpp CLI.h minCircle.cpp minCircle.h commands.h StandardIO.cpp StandardIO.h MainTrain.cpp)