/**
 * @file MaskDetector.cpp
 * @brief Comprises the FaceDetector class, which takes instances of faces and determines whether the face is wearing a mask
 * @bug no known bugs
 */

/** -- Includes -- **/
#include "MaskDetector.hpp"

cppflow::model model(MASK_MODEL_LOCATION);

MaskDetector* MaskDetector::instance = nullptr;

/**
 * @brief Constructor for MaskDetector class, required as every instance needs to have a command to execute
 */ 
MaskDetector::MaskDetector(){
    
    this->maskSensitivity = 0.2;
    
}

/** @brief destroys MaskDetector.
 *
 *  this just destroys the MaskDetector
 *
 */
MaskDetector::~MaskDetector(){
    
}

MaskDetector* MaskDetector::getInstance() {
    
    if(!MaskDetector::instance){
        MaskDetector::instance = new MaskDetector();
    }
    
    return MaskDetector::instance;
}

/**
 * @brief Calculates the probability of mask compliance
 * 
 * @param faceIn Input of face object/array, to be extracted
 * @return Value of mask probability as a float
 */ 
float MaskDetector::maskProbability(Mat faceIn){
    
    Mat face = faceIn.clone();
    
    // extract the matrix data which comes as a unsigned char
    std::vector<uchar> array;
    if (face.isContinuous()) {
      array.assign(face.data, face.data + face.total()*face.channels());
    } else {
      for (int i = 0; i < face.rows; ++i) {
        array.insert(array.end(), face.ptr<uchar>(i), face.ptr<uchar>(i)+face.cols*face.channels());
      }
    }
    
    // Put image in Tensor
    std::vector<uchar> temp; // This is a flat vector
    
    // setup the tensor with a flat version of the matrix
    temp.assign(face.data, face.data + face.total() * face.channels()); // Fill the flat std::vector with data

    
    // convert everything to final img_data matrix that is normalized to floats for channels
    std::vector<float> img_data; // This is a flat vector
    for(uchar i : temp)
        img_data.push_back((float)i/255.f);
    
    // create a 4D tensor and feed in the shape of the image 150x150 pixels with 3 channels of RBG
    auto input = cppflow::tensor(img_data, {1, 150, 150, 3});
    
    auto output = model({{"serving_default_conv2d_input", input}},{"StatefulPartitionedCall:0"});
    
    float probWithMask = output[0].get_data<float>()[1];
    
    return probWithMask;
    
}

/**
 * @brief Calculates the probabilty that a mask is worn based on the sensitivity which is set by the user
 */ 
bool MaskDetector::hasMask(Mat faceIn){
    
    float probWithMask = MaskDetector::maskProbability(faceIn);

    if(probWithMask > maskSensitivity){
        return true;
    }else{
        return false;
    }
    
}

/**
 * @brief Allows the user to set the sensitivity (or threshold) of what percentage is needed for mask compiance to be accepted
 */ 
void MaskDetector::setMaskSensitivity(float sensitivity){
    this->maskSensitivity = sensitivity;
}

/** 
 * @brief Getter method for the current sensitivty set by the user
 */ 
float MaskDetector::getMaskSensitivity(){
    return this->maskSensitivity;
}
