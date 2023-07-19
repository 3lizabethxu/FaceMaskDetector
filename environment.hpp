/**
 * @file environment.hpp
 * @brief Header file for the environment variables, which is mainly used to standardize images to match output
 */ 

#ifndef environment_h
#define environment_h

// please always change this when producing a new build on a new machine
#define FACE_MODEL_LOCATION "~ /haarcascade_frontalface_alt.xml"
#define MASK_MODEL_LOCATION "~/mask-detect-009.model"

// reports and videos will go to this folder
#define OUTPUT_FOLDER "~/Downloads"

// used to standardize the face images to match the input the model is expecting
#define IMG_SIZE 150

// used to scale down images for processing to speed up since less data points are used
#define RESIZE_SCALE 4.0


#endif /* environment_h */
