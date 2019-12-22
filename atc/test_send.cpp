#include <iostream>
#include <string>
#include <zmq.hpp>
#include "drone_msg.h"
#include "ascend_zmq.h"


int main(){

    std::string serial_msg = msg_generator::generate_heartbeat(1,2,3,4);

    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REQ);

    std::cout << "Connecting to server…" << std::endl;
    socket.setsockopt( ZMQ_IDENTITY, "PEER2", 5);
    socket.connect ("tcp://localhost:5555");

    //  Do 10 requests, waiting each time for a response
    for (int request_nbr = 0; request_nbr != 2; request_nbr++) {
        
        //send the data
        comm::send(socket,serial_msg);

        //  Get the reply.
        std::string response;
        comm::recv(socket,response);
        std::cout<< "response: " <<response <<std::endl;

        //change the message (for testing purposes)
        serial_msg = msg_generator::generate_emergency();
    }
    
    return 0;



}
