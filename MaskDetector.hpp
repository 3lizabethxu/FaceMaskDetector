/**
 * @file MaskDetector.hpp
 * @brief Header file for the MaskDetector class. The header is used to import libraries and set up variables/methods for use in the MaskDetector class
 */

#ifndef MaskDetector_hpp
#define MaskDetector_hpp

#include <stdio.h>
#include "cppflow/cppflow.h"
#include "opencv.hpp"
#include "environment.hpp"

using namespace std;
//using namespace cppflow;
using namespace cv;

class MaskDetector
{
private:

    float maskSensitivity;


public:
    // singleton instance
    static MaskDetector *instance;
    // constructor
    MaskDetector();
    // destructor
    ~MaskDetector();
    
    static MaskDetector *getInstance();

    bool hasMask(Mat);
    float maskProbability(Mat);
    
    void setMaskSensitivity(float sensitivity);
    float getMaskSensitivity();

};


#endif /* MaskDetector_hpp */
