#include <iostream>
#include <zmq.hpp>
#include <pqxx/pqxx>
#include <string>
#include "ascenddb.h"
#include "messaging/libs/status.pb.h"
#include "ascend_zmq.h"
#include "constants.h"

//temp
#include <chrono>
#include <thread>

int main(){

    //coutner
    int counter = 0;

    //declare sockets
    zmq::context_t context(1);
    zmq::socket_t from_worker(context, ZMQ_ROUTER);
    from_worker.bind("tcp://*:" + constants::from_worker);
    zmq::socket_t from_drone(context, ZMQ_ROUTER);
    from_drone.bind("tcp://*:" + constants::from_drone);
    zmq::socket_t to_drone(context, ZMQ_REQ);

    zmq::pollitem_t items [] = {
        {static_cast<void*>(from_drone),0,ZMQ_POLLIN,0},
        {static_cast<void*>(to_drone),0,ZMQ_POLLIN,0},
        {static_cast<void*>(from_worker),0,ZMQ_POLLIN,0}

    };

    std::cout << "Listening..." << std::endl;
    while(true){ 

        //try to send msg
        comm::connect(to_drone,"atc","tcp://localhost:" + constants::to_drone);
        std::string resp = comm::send(to_drone,"Does this work?");

        std::cout << "Response: " << resp << std::endl;

        // //poll for messages
        // zmq::poll (&items [0], 2, -1);  

        // //from drone
        // if(items[0].revents & ZMQ_POLLIN){

        //     //recieve data
        //     std::string msg;
        //     std::string identity;
        //     comm::recv(drone_socket,identity, msg);
            
        //     std::cout<<"...recieved, sending response"<< std::endl;
        //     comm::send(drone_socket,identity,"Recieved " + std::to_string(counter++));
        // }

        // //to drone
        // if(items[1].revents & ZMQ_POLLIN){
        //     std::cout << "Message to drone" << std::endl;

        //     //recieve
        //     std::string msg;
        //     std::string identity;
        //     comm::recv(worker_socket,identity, msg);

        //     //send to frontend (make sure that this is done asynchronously)


        // }

        // //worker
        // if(items[2].revents & ZMQ_POLLIN){

        //     std::cout<< "Message from worker" << std::endl;

        //     //recieve data
        //     std::string msg;
        //     std::string identity;
        //     comm::recv(worker_socket,identity, msg);

        // }
    }

    


    return 0;   
}

