#include <iostream>
#include <string>
#include <zmq.hpp>
#include "drone_msg.h"
#include "ascend_zmq.h"

//temp
#include <chrono>
#include <thread>


int main(){


    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_ROUTER);
    socket.bind("tcp://*:5556");

    zmq::pollitem_t items [] = {
        {static_cast<void*>(socket),0,ZMQ_POLLIN,0}
    };

    while(true){

        zmq::poll(&items[0], 1, 500);

        if(items[0].revents & ZMQ_POLLIN){
            std::string identity;
        }
    }

    //std::cout << "Connecting to server…" << std::endl;
    socket.setsockopt( ZMQ_IDENTITY, "WorkerID", 8);
    socket.connect ("tcp://localhost:5556");


    //receive something from the atc

    //wait for a hot sec

    //send a response back
    
    return 0;



}
