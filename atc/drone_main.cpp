#include <iostream>
#include <string>
#include <zmq.hpp>
#include "drone_msg.h"
#include "ascend_zmq.h"
#include "constants.h"

//temp
#include <chrono>
#include <thread>


int main(){

    //temp
    std::string serial_msg = msg_generator::generate_heartbeat(1,2,3,4);

    //set up comms
    zmq::context_t context (1);
    zmq::socket_t atc_socket (context, ZMQ_REQ);
    atc_socket.setsockopt( ZMQ_IDENTITY, "DroneID", 7);
    atc_socket.connect("tcp://localhost:" + constants::from_drone);
    zmq::socket_t recv_socket (context, ZMQ_ROUTER);
    recv_socket.setsockopt( ZMQ_IDENTITY, "DroneID", 7);
    recv_socket.bind("tcp://*:" + constants::to_drone);

    zmq::pollitem_t items [] = {
        {static_cast<void*>(atc_socket),0,ZMQ_POLLIN,0},
        {static_cast<void*>(recv_socket),0,ZMQ_POLLIN,0}
    };

    while(true){

        //poll for messages
        zmq::poll (&items [0], 2, -1);

        //ATC
        if(items[0].revents & ZMQ_POLLIN){
            std::string data;
            std::string identity;
            comm::recv(atc_socket,identity,data);

            std::cout << "Data to ATC: " << data << std::endl;
        }

        if(items[1].revents & ZMQ_POLLIN){
            std::cout << "Drone received info" << std::endl;

            std::string data;
            std::string identity;
            comm::recv(recv_socket,identity,data);

            bool sent = comm::send(recv_socket,identity,"received");
            std::cout << "Successful send: " << sent << std::endl;

            std::cout << "Data from ATC: " << data << std::endl;
        }

            
    }

    return 0;



}
