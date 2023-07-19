/**
 * @file Report.hpp
 * @brief Header file for the Report class. The header is used to import libraries and set up variables/methods for use in the Report class
 */

#ifndef Report_hpp
#define Report_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include "environment.hpp"
#include "opencv.hpp"

using namespace std;
using namespace cv;

class Report
{
    
private:
    int people;
    float compliance;
    string outputLocation;
    string dateTime;
    
public:
    // constructor
    Report(int, float);
    // destructor
    ~Report();
    void exportFile();
    
    string getOutputLocation();
    void setOutputLocation(string);
    
};

#endif /* Report_hpp */
