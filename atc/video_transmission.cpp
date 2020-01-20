#include "video_transmission.h"

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <zmq.hpp>


video_transmission::video_transmission(const std::string worker_address){
    this->worker_address=worker_address;
}

void video_transmission::start_transmission(){
 
    std::cout<<"TODO: start transmission"<<std::endl;
    if(!Camera.open()){
        //TODO throw error
        std::cerr <<"Error opening camera" << std::endl;
        return;
    }

    std::cout<<"Connected to camera ="<<Camera.getId() << std::endl;
    cv::Mat image;

    while(true){
        Camera.grab();
        Camera.retrieve ( image );

		  //cv::namedWindow( "Image", cv::WINDOW_AUTOSIZE );
        //cv::imshow("Image", image);
        //cv::waitKey(0);

    }



}

void video_transmission::stop_transmission(){
    
    std::cout<<"TODO: stop transmission"<<std::endl;

}



