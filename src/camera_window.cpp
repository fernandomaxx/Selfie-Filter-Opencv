//
// Created by fernando on 02/05/19.
//

#include "camera_window.h"

CameraWindow::CameraWindow( BaseObjectType* cobject, const Glib::RefPtr< Gtk::Builder >& ref_glade ):
        Gtk::Window{ cobject },
        builder{ ref_glade }
{
    this->paused = false;
    this->builder->get_widget( "DrawingImage", this->drawing_image );
    this->builder->get_widget( "closeButton", this->close_button );
    this->builder->get_widget( "resumePauseButton", this->resume_pause_button );
    this->builder->get_widget( "filter1Button", this->filter1_button );
    this->filter1_button->signal_clicked().connect( sigc::mem_fun( *this, &CameraWindow::filterOne ));
    this->close_button->signal_clicked().connect( sigc::mem_fun( *this, &CameraWindow::stopCamera ));
    this->resume_pause_button->signal_clicked().connect( sigc::mem_fun( *this, &CameraWindow::pauseResumeCamera ));
    this->drawing_image->set( "image_not_fount.png" );
}

CameraWindow::~CameraWindow()
{}

void CameraWindow::pauseResumeCamera()
{
    this->paused = !this->paused;
    if ( this->paused )
        this->resume_pause_button->set_label( "resume" );
    else
        this->resume_pause_button->set_label( "pause" );
}

void CameraWindow::stopCamera()
{
    Window::close();
}

void CameraWindow::updateImage( cv::Mat& frame )
{
    if ( !frame.empty() )
    {
        this->drawing_image->set( Gdk::Pixbuf::create_from_data( frame.data,
                                                                 Gdk::COLORSPACE_RGB,
                                                                 false,
                                                                 8,
                                                                 frame.cols,
                                                                 frame.rows,
                                                                 frame.step ) );
        this->drawing_image->queue_draw();
    }
}

void CameraWindow::filterOne()
{

}

bool CameraWindow::isPaused()
{
    return this->paused;
}

void cameraLoop()
{

    while ( capture_video )
    {
        std::vector<cv::Rect> faces;
        bool continue_tograbe = true;
        int scale = 1;
        cv::Scalar color = cv::Scalar(255,0,0);
        Draw draw;
        bool paused = camera_window->isPaused();
        if ( !paused )
        {
            continue_tograbe = camera.read( frame_BGR );
            if ( continue_tograbe )
            {
                image_mutex.lock();
                cv::cvtColor( frame_BGR, frame, CV_BGR2RGB );
                draw.drawing( &frame, 1 );
                //imshow( "title", frame_BGR );
                image_mutex.unlock();
                dispatcher.emit();
            }      
        }
        if ( !continue_tograbe )
        {
            capture_video = false;
            std::cerr << "Faleid to retrieve frame from the device. The camera was stopped." << std::endl;
        }
        else if ( paused ) 
        {
            std::this_thread::sleep_for( std::chrono::milliseconds( 30 ));
        }
    }
}