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
    zmq::socket_t socket (context, ZMQ_REQ);

    std::cout << "Connecting to server…" << std::endl;
    socket.setsockopt( ZMQ_IDENTITY, "WorkerID", 8);
    socket.connect ("tcp://localhost:5556");


    //receive something from the atc

    //wait for a hot sec

    //send a response back
    
    return 0;



}
