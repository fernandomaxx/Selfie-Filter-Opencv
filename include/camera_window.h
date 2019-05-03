#ifndef SELFIE_FILTER_OPENCV_CAMERA_WINDOW_H
#define SELFIE_FILTER_OPENCV_CAMERA_WINDOW_H

#include <gtkmm.h>
#include <opencv2/opencv.hpp>
#include <mutex>
#include <thread>
#include <iostream>
#include <string>
#include "draw.hpp"


void cameraLoop();
bool initializeCamera( int camera_index );

class CameraWindow : public Gtk::Window
{
public:

    CameraWindow( BaseObjectType* cobject, const Glib::RefPtr< Gtk::Builder >& ref_glade );

    virtual ~CameraWindow();

    void stopCamera();

    void pauseResumeCamera();

    void filterOne();

    void updateImage( cv::Mat& frame );

    bool isPaused();

public:

    Glib::RefPtr< Gtk::Builder > builder;
    Gtk::Image *drawing_image;
    Gtk::Button *close_button;
    Gtk::ToggleButton *resume_pause_button;
    Gtk::Button *filter1_button;

    bool paused;

};

extern std::mutex image_mutex;
extern Glib::Dispatcher dispatcher;
extern volatile bool capture_video;
extern cv::VideoCapture camera;
extern cv::Mat frame_BGR, frame;
extern CameraWindow *camera_window;


#endif //SELFIE_FILTER_OPENCV_CAMERA_WINDOW_H
