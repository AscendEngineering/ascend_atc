#include "video_transmission.h"
#include <raspicam/raspicam.h>
#include <zmq.hpp>
#include <iostream>



video_transmission::video_transmission(const std::string worker_address){

    raspicam::RaspiCam Camera; 
    this->worker_address=worker_address;

}

void video_transmission::start_transmission(){
 
    std::cout<<"TODO: start transmission"<<std::endl;

}

void video_transmission::stop_transmission(){
    
    std::cout<<"TODO: stop transmission"<<std::endl;

}



