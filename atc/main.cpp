#include <iostream>
#include <zmq.hpp>
#include <pqxx/pqxx>
#include <string>
#include "ascenddb.h"
#include "messaging/libs/status.pb.h"
#include "ascend_zmq.h"

//temp
#include <chrono>
#include <thread>

int main(){

    //coutner
    int counter = 0;


    zmq::context_t context(1);
    zmq::socket_t drone_socket(context, ZMQ_ROUTER);
    drone_socket.bind("tcp://*:5555");
    zmq::socket_t worker_socket(context, ZMQ_ROUTER);
    worker_socket.bind("tcp://*:5556");

    zmq::pollitem_t items [] = {
        {static_cast<void*>(drone_socket),0,ZMQ_POLLIN,0},
        {static_cast<void*>(worker_socket),0,ZMQ_POLLIN,0}

    };

    std::cout << "Listening..." << std::endl;
    while(true){ 

        //poll for messages
        zmq::poll (&items [0], 2, -1);  

        //drone
        if(items[0].revents & ZMQ_POLLIN){

            //recieve data
            std::string msg;
            std::string identity;
            comm::recv(drone_socket,identity, msg);

            //convert to message
            ascend::msg recieved_msg;
            recieved_msg.ParseFromString(msg);

            //Process
            if(recieved_msg.has_emergency()){
                std::cout << recieved_msg.emergency().name() << std::endl;
            }
            else if(recieved_msg.has_heartbeat()){
                const ascend::heartbeat_msg& heartbeat = recieved_msg.heartbeat();
                std::cout << "Long:" << heartbeat.lng();
                std::cout << " Lat:" << heartbeat.lat();
                std::cout << " Alt:" << heartbeat.alt();
                std::cout << " Charge:" << heartbeat.bat_percentage();
            }

            std::cout<<"...recieved, sending response"<< std::endl;
            comm::send(drone_socket,identity,"Recieved " + std::to_string(counter++));
            comm::send(drone_socket,identity,"Recieved " + std::to_string(counter++));
            std::cout<<"OUT"<<std::endl;
        }

        //worker
        if(items[1].revents & ZMQ_POLLIN){

            //recieve data
            std::string msg;
            std::string identity;
            comm::recv(worker_socket,identity, msg);

        }
    }

    


    return 0;   
}

