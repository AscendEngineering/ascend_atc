#include <iostream>
#include <string>
#include <zmq.hpp>
#include "drone_msg.h"
#include "ascend_zmq.h"

//temp
#include <chrono>
#include <thread>


int main(){

    //temp
    std::string serial_msg = msg_generator::generate_heartbeat(1,2,3,4);

    //set up comms
    zmq::context_t context (1);
    zmq::socket_t atc_socket (context, ZMQ_DEALER);
    atc_socket.setsockopt( ZMQ_IDENTITY, "DroneID", 7);
    atc_socket.connect("tcp://*:5555");
    zmq::socket_t recv_socket (context, ZMQ_ROUTER);
    recv_socket.setsockopt( ZMQ_IDENTITY, "DroneID", 7);
    recv_socket.connect("tcp://*:5556");

    zmq::pollitem_t items [] = {
        {static_cast<void*>(recv_socket),0,ZMQ_POLLIN,0}
    };

    while(true){

        //poll for messages
        zmq::poll (&items [0], 1, -1);

        //ATC
        if(items[0].revents & ZMQ_POLLIN){
            std::string data;
            std::string identity;
            comm::recv(atc_socket,identity,data);

            std::cout << "Data from ATC: " << data << std::endl;
        }
        else{
            std::this_thread::sleep_for(std::chrono::milliseconds(5000));

            //send the data
            comm::send(atc_socket,serial_msg);

            //Get the reply.
            std::string response;
            std::string identity;
            comm::recv(atc_socket,identity,response);
            std::cout<< "response: " <<response <<std::endl;

        }
            
    }

    return 0;



}
