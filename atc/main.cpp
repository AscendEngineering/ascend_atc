#include <iostream>
#include <zmq.hpp>
#include <pqxx/pqxx>
#include <string>
#include "ascenddb.h"
#include "messaging/libs/status.pb.h"

int main(){

    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REP);
    socket.bind("tcp://*:5555");

    std::cout << "Listening..." << std::endl;
    while(true){
        zmq::message_t request;

        socket.recv(request,zmq::recv_flags::none);
        std::string msg = (char*)(request.data());
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

        socket.send(zmq::str_buffer("recieved"), zmq::send_flags::dontwait);
    }

    


    return 0;   
}

