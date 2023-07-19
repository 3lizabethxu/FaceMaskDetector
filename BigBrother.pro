QT += core gui widgets
TARGET = BigBrother
TEMPLATE = app

HEADERS = environment.hpp opencv.hpp MaskDetector.hpp mainwindow.hpp FaceDetector.hpp Face.hpp Report.hpp
SOURCES = main.cpp mainwindow.cpp MaskDetector.cpp Face.cpp FaceDetector.cpp Report.cpp

CONFIG += link_pkgconfig
PKGCONFIG += opencv4 tensorflow
