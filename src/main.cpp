//
// Created by fernando on 02/05/19.
//

#include "main.h"

std::mutex image_mutex;
Glib::Dispatcher dispatcher;
volatile bool capture_video = false;
cv::VideoCapture camera;
cv::Mat frame_BGR, frame;
CameraWindow *camera_window = nullptr;

int main( int argc, char *argv[] )
{
    Gtk::Main app( argc, argv );

    Glib::RefPtr< Gtk::Builder > builder = Gtk::Builder::create_from_file( "../main_window.glade" );

    builder->get_widget_derived( "MainWindow", camera_window );
    if ( camera_window )
    {
        dispatcher.connect( [&]() {
            image_mutex.lock();
            camera_window->updateImage( frame );
            std::cout << "oi" << '\n';
            image_mutex.unlock();
        });

        if ( camera.open(0) )
        {
            capture_video = true;
            std::cout << "Everything sounds fine lets rock!" << std::endl;
            std::thread camera_thread = std::thread( &cameraLoop );
            Gtk::Main::run( *camera_window );

            capture_video = false;
            camera_thread.join();
        }
        else
            std::cout << "Failed to initialize the camera" << std::endl;
    }
    else
        std::cout << "Failed to initialize the GUI" << std::endl;

    if ( camera.isOpened() )
    {
        camera.release();
        std::cout << "Camera released success!" << std::endl;
    }
    
}