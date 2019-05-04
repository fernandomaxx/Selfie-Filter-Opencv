#include "detect_face.hpp"

DetectFace::DetectFace()
{
    this->cascade.load( "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml" );
    //this->cascade.~CascadeClassifier();
}

DetectFace::~DetectFace()
{
    this->faces.clear();
    //this->cascade.~CascadeClassifier();
}

std::vector< cv::Rect > DetectFace::run( cv::Mat small_img )
{
    this->cascade.detectMultiScale( small_img, this->faces,
                                    1.3, 2, 0
                                    |cv::CASCADE_SCALE_IMAGE,
                                    cv::Size(30, 30) );
    return faces;
}