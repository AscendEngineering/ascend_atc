#include <iostream>
#include <string>
#include <zmq.hpp>
#include "drone_msg.h"
#include "ascend_zmq.h"

//temp
#include <chrono>
#include <thread>


int main(){

    std::string serial_msg = msg_generator::generate_heartbeat(1,2,3,4);

    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REQ);

    std::cout << "Connecting to server…" << std::endl;
    socket.setsockopt( ZMQ_IDENTITY, "WorkerID", 8);
    socket.connect ("tcp://localhost:5556");

    //  Do 10 requests, waiting each time for a response
    while(true) {
        
        //send the data
        comm::send(socket,serial_msg);

        //  Get the reply.
        std::string response;
        comm::recv(socket,response);
        std::cout<< "response: " <<response <<std::endl;

        std::cout<<"Sleeping...\n" <<std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        
    }
    
    return 0;



}
