/**
 * @file Face.cpp
 * @brief Comprises the Face class, which defines what the program considers to be a face. Also handles how to draw out faces on the video feed
 * @bug no known bugs
 */

/** -- Includes -- **/
#include "Face.hpp"

/**
 * @brief Constructor for the face, as every instance needs to have a command to execute
 * 
 * @param image Feed in the image as a matrix of pixel data
 * @param area Importing a pre-defined area as a rectange
 */ 
Face::Face(Mat image, Rect area){
        
    Range cols(area.x, area.x + area.width - 1);
    Range rows(area.y, area.y + area.height - 1);
    
    faceImage = image.clone();

    faceImage = faceImage(rows, cols);
    
    this->area = area;
    this->rows = rows;
    this->cols = cols;
    this->topLeftPoint = Point(cvRound(area.x * RESIZE_SCALE), cvRound(area.y * RESIZE_SCALE));
    this->bottomRightPoint = Point(cvRound((area.x + area.width - 1) * RESIZE_SCALE), cvRound((area.y + area.height - 1) * RESIZE_SCALE));
    this->maskProb = 0.0;
    
    
}
/** @brief destroys Face.
 *
 *  this just destroys the Face
 *
 */
Face::~Face(){
    
}

/**
 * @brief Defines how to detect mask compliance
 * 
 * @return Boolean value determining whether mask is worn
 */ 
bool Face::detectMask(){
    
    Mat normalized = faceImage.clone();
    
    // convert and prep face
    
    Mat finalSize;
    resize(normalized, finalSize, Size(IMG_SIZE,IMG_SIZE));
    
    this->maskProb = MaskDetector().getInstance()->maskProbability(finalSize);
    
    return MaskDetector().getInstance()->hasMask(finalSize);
    
}

/**
 * @brief Captures the probability of whether a mask is being worn, converted to an integer
 * 
 * @return Integer mask probability
 */
int Face::getProbabilityOfMask(){
    
    int convertedToInt = Face::maskProb*100;
    
    return convertedToInt;
    
}

/**
 * @return The area as a rectange object
 */ 
Rect Face::getArea(){
    return this->area;
}

/** 
 * @return The number of columns as a range object
 */ 
Range Face::getCols(){
    return this->cols;
}

/** 
 * @return The number of rows as a range object
 */ 
Range Face::getRows(){
    return this->rows;
}

/** 
 * @return The top left of the rectange as a point
 */ 
Point Face::getTopLeftPoint(){
    return this->topLeftPoint;
}

/** 
 * @return The bottom right of the rectange as a point
 */ 
Point Face::getBottomRightPoint(){
    return this->bottomRightPoint;
}

