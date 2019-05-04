#include "figure.hpp"

Figure::Figure( cv::Mat figure, int x, int y ):
        figure{ figure },
        x{ x },
        y{ y }
{}

cv::Mat Figure::getFigure()
{
    return figure;
}

int Figure::getX()
{
    return x;
}

int Figure::getY()
{
    return y;
}