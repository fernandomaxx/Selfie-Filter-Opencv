#include "draw.hpp"

int Draw::drawing( cv::Mat *frame, int scale )
{   
    double fx = 1 / scale;
    cv::Rect r, nr;
    std::vector<cv::Rect> faces;
    Detect *detectface = new DetectFace();
    //DetectFace detectface;
    //std::unique_ptr< Detect > detectface = std::make_unique< DetectFace >();
    cv::Scalar color = cv::Scalar(255,0,0);
    cv::Mat gray, small_img, small_imgROI, png;

    cvtColor( *frame, gray, cv::COLOR_BGR2GRAY );
    cv::resize( gray, small_img, cv::Size(), fx, fx, cv::INTER_LINEAR );
    cv::equalizeHist( small_img, small_img );
    faces = detectface->run( small_img );

    delete detectface;

    for ( size_t i = 0; i < faces.size(); i++ )
    {
            int radius;
            r = faces[i];
            cv::Point center;

            printf("xy face = %d x %d\n", r.x, r.y);

            rectangle( *frame, cv::Point(cvRound(r.x*scale), cvRound(r.y*scale)),
                        cv::Point(cvRound((r.x + r.width-1)*scale), cvRound((r.y + r.height-1)*scale)),
                        color, 3, 8, 0);
                    
            //std::cout << cvRound((r.x + r.width-1)*scale) << " " << cvRound((r.y + r.height-1)*scale) << std::endl;

            Figure *hat = new Hat();
            //Hat hat;
            png = hat->getFigure();

            if ( !hat->getFigure().empty() and r.y - r.height/2 > 0 )
            {
                std::cout << cvRound(( r.width-1)*scale) << " " << cvRound(( r.height-1)*scale) << std::endl;
                resize( png, png, cv::Size( r.width, r.height - r.width/4 ), 0, 0, CV_INTER_LANCZOS4);
                this->drawImage( *frame, png, r.x + 0, r.y - r.height/1.5 + 15 );
            }
            delete hat;
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