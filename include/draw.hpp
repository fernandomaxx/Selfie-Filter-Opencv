#pragma once

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

class Draw {

public:

    int drawing( cv::Mat *frame, int scale );

    void drawImage( cv::Mat frame, cv::Mat transp, int xPos, int yPos );

    void drawImageTransparency( cv::Mat frame, cv::Mat transp, int xPos, int yPos ); 
};