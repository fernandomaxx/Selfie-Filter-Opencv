#include "draw.hpp"

int Draw::drawing( cv::Mat *frame, int scale )
{   
    cv::Mat gray, smallImg;
    cvtColor( *frame, gray, cv::COLOR_BGR2GRAY );
    double fx = 1 / scale;
    cv::resize( gray, smallImg, cv::Size(), fx, fx, cv::INTER_LINEAR );
    cv::equalizeHist( smallImg, smallImg );

    cv::CascadeClassifier cascade, nestedCascade;
    std::string imhpath = "cb1.png";
    //cv::Mat img = cv::imread(imhpath, cv::IMREAD_UNCHANGED);
    //if ( img.empty() )
        //printf( "Error opening file %s\n", imhpath );
    std::string cascadeName       = "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml";
    nestedCascade.load( "/usr/share/opencv/haarcascades/haarcascade_eye_tree_eyeglasses.xml" ) ;
    if( !cascade.load( cascadeName ) )
    {
        std::cerr << "ERROR: Could not load classifier cascade:\n " << cascadeName << std::endl;
        return 0;
    }
    std::vector<cv::Rect> faces;
    cv::Scalar color = cv::Scalar(255,0,0);
    cascade.detectMultiScale( smallImg, faces,
                        1.3, 2, 0
                        //|CASCADE_FIND_BIGGEST_OBJECT
                        //|CASCADE_DO_ROUGH_SEARCH
                        |cv::CASCADE_SCALE_IMAGE,
                        cv::Size(60, 60) );
                
    cv::Rect r, nr;
    for ( size_t i = 0; i < faces.size(); i++ )
    {
            int radius;
            cv::Mat smallImgROI;
            r = faces[i];
            cv::Point center;

            printf("xy face = %d x %d\n", r.x, r.y);

            rectangle( *frame, cv::Point(cvRound(r.x*scale), cvRound(r.y*scale)),
                        cv::Point(cvRound((r.x + r.width-1)*scale), cvRound((r.y + r.height-1)*scale)),
                        color, 3, 8, 0);
                    
            std::cout << cvRound((r.x + r.width-1)*scale) << " " << cvRound((r.y + r.height-1)*scale) << std::endl;
    }

    return 1;
}

void Draw::drawImage( cv::Mat frame, cv::Mat transp, int xPos, int yPos ) 
{
    cv::Mat mask;
    std::vector< cv::Mat > layers;

    split( transp, layers ); // seperate channels
    cv::Mat rgb[3] = { layers[0], layers[1], layers[2] };
    mask = layers[3]; // png's alpha channel used as mask
    merge( rgb, 3, transp );  // put together the RGB channels, now transp insn't transparent
    transp.copyTo( frame.rowRange( yPos, yPos + transp.rows ).colRange( xPos, xPos + transp.cols ), mask ); //write image
}

void Draw::drawImageTransparency( cv::Mat frame, cv::Mat transp, int xPos, int yPos ) 
{
    cv::Mat mask;
    std::vector< cv::Mat > layers;
    
    split( transp, layers ); // seperate channels
    cv::Mat rgb[3] = { layers[0], layers[1], layers[2] };
    mask = layers[3]; // png's alpha channel used as mask
    merge( rgb, 3, transp );  // put together the RGB channels, now transp insn't transparent 
    cv::Mat roi1 = frame( cv::Rect( xPos, yPos, transp.cols, transp.rows ));
    cv::Mat roi2 = roi1.clone();
    transp.copyTo( roi2.rowRange( 0, transp.rows ).colRange( 0, transp.cols ), mask );
    printf( "%p, %p\n", roi1.data, roi2.data );
    double alpha = 0.2;
    addWeighted( roi2, alpha, roi1, 1.0 - alpha, 0.0, roi1 );
}