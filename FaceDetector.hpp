/**
 * @file FaceDetector.hpp
 * @brief Header file for the FaceDetector class. The header is used to import libraries and set up variables/methods for use in the FaceDetector class
 */

#ifndef FaceDetector_hpp
#define FaceDetector_hpp

#include <stdio.h>
#include <vector>

#include "environment.hpp"
#include "opencv.hpp"
#include "MaskDetector.hpp"

using namespace cv;
using namespace std;

class FaceDetector
{
    
private:
    
    // helper to store the faces that were detected
    void setFaces(vector<Rect> facesVec);
    // store our face classifier
    CascadeClassifier faceCascade;
    // helper func to resize frames
    Mat processMat(Mat imageToResize);
    
public:
    static FaceDetector *instance;
    
    // constructor
    FaceDetector();
    // destructor
    ~FaceDetector();
    
    // singleton getter
    static FaceDetector *getInstance();
    
    // get the faces in the current image
    vector<Rect> getFaces(Mat image);
    
};

#endif /* FaceDetector_hpp */

