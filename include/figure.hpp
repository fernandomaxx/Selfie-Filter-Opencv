#pragma once

#include <opencv2/opencv.hpp>

class Figure 
{
public:

    Figure( cv::Mat figure, int x, int y );

    cv::Mat getFigure();

    int getX();

    int getY();

private:

    int x, y;
    cv::Mat figure;

};