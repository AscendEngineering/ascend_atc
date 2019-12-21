#include <iostream>
#include <string>
#include <zmq.hpp>
#include "drone_msg.h"


int main(){

    std::string serial_msg = msg_generator::generate_heartbeat(1,2,3,4);

    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REQ);

    std::cout << "Connecting to server…" << std::endl;
    socket.connect ("tcp://localhost:5555");

    //  Do 10 requests, waiting each time for a response
    for (int request_nbr = 0; request_nbr != 2; request_nbr++) {
        zmq::message_t request (serial_msg.size());
        memcpy (request.data (), serial_msg.c_str(), serial_msg.size());
        socket.send(request,zmq::send_flags::dontwait);

        //  Get the reply.
        zmq::message_t reply;
        socket.recv (&reply);
        std::cout << "Received " << request_nbr << std::endl;

        serial_msg = msg_generator::generate_emergency();
    }
    
    return 0;



}
