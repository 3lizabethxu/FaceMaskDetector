/**
 * @file FaceDetector.cpp
 * @brief Comprises the FaceDetector class, which receives an image that contains of faces and isolates them
 */

/** -- Includes -- **/
#include "FaceDetector.hpp"

FaceDetector* FaceDetector::instance = nullptr;

/**
 * @brief Constructor for FaceDetector class, required as every instance needs to have a command to execute
 */ 
FaceDetector::FaceDetector(){

    faceCascade.load(FACE_MODEL_LOCATION);
}

/** @brief destroys FaceDetector.
 *
 *  this just destroys the face detector
 *
 */
FaceDetector::~FaceDetector(){
    
}
/**
 * @brief Returns the singleton class object for FaceDetector
 *
 * @return FaceDetector Singleton
 */
FaceDetector* FaceDetector::getInstance() {
    
    if(!FaceDetector::instance){
        FaceDetector::instance = new FaceDetector();
    }
    
    return FaceDetector::instance;
}

Mat FaceDetector::processMat(Mat image){
    
    double scale = 1.0;
    
    Mat resized;
    resize(image, resized, Size(image.cols/RESIZE_SCALE, image.rows/RESIZE_SCALE));
    
    // recolor the frame to speed up face detection
    Mat grayscale;
    cvtColor(resized, grayscale, COLOR_BGR2GRAY);
    resize(grayscale, grayscale, Size(grayscale.size().width / scale, grayscale.size().height / scale));
    
    return grayscale;
}

/**
 * @brief Gets all faces currently in frame and stores them in a vector array of rectangle objects
 * 
 * @param image Image feed to analyze
 * @return Array of faces as a vector
 */ 
vector<Rect> FaceDetector::getFaces(Mat image){
    
    Mat preprocess = processMat(image);
    
    // do face detection and store it in faces array
    vector<Rect> faces;
    FaceDetector::faceCascade.detectMultiScale(preprocess, faces, 1.1, 3, 0, Size(30, 30));
    
    return faces;
    
}
