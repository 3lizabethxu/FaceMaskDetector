/**
 * @file Face.hpp
 * @brief Header file for the Face class. The header is used to import libraries and set up variables/methods for use in the Face class
 */ 

#ifndef Face_hpp
#define Face_hpp

#include <stdio.h>
#include "opencv.hpp"
#include <vector>

#include "environment.hpp"

#include "MaskDetector.hpp"

using namespace cv;
using namespace std;

class Face
{
    
private:
    // user entered command
    Mat faceImage;
    Rect area;
    Range cols;
    Range rows;
    Point topLeftPoint;
    Point bottomRightPoint;
    float maskProb;
    
public:
    // constructor
    Face(Mat image, Rect area);
    // destructor
    ~Face();
    bool detectMask();
    int getProbabilityOfMask();
    
    // getters
    Rect getArea();
    Range getCols();
    Range getRows();
    Point getTopLeftPoint();
    Point getBottomRightPoint();
    
};

#endif /* Face_hpp */
