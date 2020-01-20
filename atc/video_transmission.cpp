#include "video_transmission.h"

#include <algorithm>  
#include "ascend_zmq.h"
#include "constants.h"
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
    
    //set up camera
    Camera.set ( cv::CAP_PROP_FPS, 30);

    std::cout<<"Connected to camera ="<<Camera.getId() << std::endl;
    cv::Mat image;
    
    //start zmq
    zmq::context_t context(1);
    zmq::socket_t send_socket(context, ZMQ_PUSH);

    while(true){
        Camera.grab();
        Camera.retrieve ( image );
        std::cout<< image.rows << std::endl;
        std::cout<< image.cols << std::endl;
        
        std::string imgData(image.datastart,image.dataend);
        	
        //send over
        
        bool succ = comm::send_msg(send_socket,"drone1",imgData,"tcp://localhost:"+constants::from_drone);
        std::cout<<"send success: " << succ << std::endl;

		  //cv::namedWindow( "Image", cv::WINDOW_AUTOSIZE );
        //cv::imshow("Image", image);
        //cv::waitKey(0);

    }



}

void video_transmission::stop_transmission(){
    
    std::cout<<"TODO: stop transmission"<<std::endl;

}



