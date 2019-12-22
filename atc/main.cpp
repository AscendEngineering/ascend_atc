#include <iostream>
#include <zmq.hpp>
#include <pqxx/pqxx>
#include <string>
#include "ascenddb.h"
#include "messaging/libs/status.pb.h"
#include "ascend_zmq.h"

int main(){

    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_ROUTER);
    socket.bind("tcp://*:5555");

    std::cout << "Listening..." << std::endl;
    while(true){

        //indentity
        std::string identity;
        comm::recv(socket,identity);
        std::cout<<"Identity: " << identity << std::endl;

        //delimiter
        std::string del;
        comm::recv(socket,del);
        std::cout<<"delimeter: " << del << std::endl;

        //recieve data
        std::string msg;
        comm::recv(socket, msg);

        //convert to message
        ascend::msg recieved_msg;
        recieved_msg.ParseFromString(msg);

        //Process
        if(recieved_msg.has_emergency()){
            std::cout << recieved_msg.emergency().name() << std::endl;
        }
        else if(recieved_msg.has_heartbeat()){
            const ascend::heartbeat_msg& heartbeat = recieved_msg.heartbeat();
            std::cout << "Long:" << heartbeat.lng() << std::endl;
            std::cout << "Lat:" << heartbeat.lat() << std::endl;
            std::cout << "Alt:" << heartbeat.alt() << std::endl;
            std::cout << "Charge:" << heartbeat.bat_percentage() << std::endl;
        }

        std::cout<<"recieved, sending response"<< std::endl;
        socket.setsockopt( ZMQ_IDENTITY, "PEER2", 5);
        comm::sendmore(socket,identity);
        comm::sendmore(socket,"");
        comm::send(socket,"Recieved");

    }

    


    return 0;   
}

