#pragma once

#include "detect.hpp"

class DetectFace : public Detect
{
public:

    DetectFace();

    ~DetectFace();
    
    std::vector< cv::Rect > run( cv::Mat small_img ) override;

private:

    cv::CascadeClassifier cascade;
    std::vector< cv::Rect > faces;
};