#pragma once

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

class Detect 
{
public:

    virtual ~Detect(){}

    virtual std::vector< cv::Rect > run( cv::Mat small_img ) = 0;
};
