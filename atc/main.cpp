#include <iostream>
#include <zmq.hpp>
#include <pqxx/pqxx>
#include <string>
#include "ascenddb.h"
#include "msgDef.pb.h"
#include "ascend_zmq.h"
#include "constants.h"
#include <vector>
#include "ledger.h"
#include "atc_msg.h"
#include "router.h"
#include "utilities.h"
#include "logging.h"
#include "dat.h"

//temp
#include <chrono>
#include <thread>



void process_request(const std::string& sender,const ascend::msg& recvd_msg){

    if(recvd_msg.has_heartbeat()){
       //TODO
       spdlog::info("Sender " + sender + " sent heartbeat");
    }
    else if(recvd_msg.has_emergency()){
        //TODO
        spdlog::info("Sender " + sender + " sent emergency");
    }
    else if(recvd_msg.has_takeoff_request()){
        //TODO
        spdlog::info("Sender " + sender + " sent takeoff request");
    }
    else if(recvd_msg.has_landing_request()){
        //TODO
        spdlog::info("Sender " + sender + " sent landing request");
    }
    else if(recvd_msg.has_status()){
        //TODO
        spdlog::info("Sender " + sender + " sent status request");
    }
    


}



int main(){

    logging::setup_logging();

    //listening
    zmq::context_t context(1);
    zmq::socket_t recv_socket(context, ZMQ_PULL);
    zmq::socket_t send_socket(context, ZMQ_PUSH);
    recv_socket.bind("tcp://*:" + constants::from_drone);

    //variables
    dat drone_connection;
    ledger msg_tracker;
    zmq::pollitem_t items [] = {
        {static_cast<void*>(recv_socket),0,ZMQ_POLLIN,0}
    };

    spdlog::info("Listening...");
    while(true){

        zmq::poll(&items[0], 1, 3000);

        //from drone
        if(items[0].revents & ZMQ_POLLIN){

            std::cout << "giot it" << std::endl;
            
            //header
            std::string sender;
            std::string operation;
            std::string data;
            comm::get_msg_header(recv_socket,sender,operation);

            //message
            if(operation == "A"){
                msg_tracker.msg_ack(sender);
            }
            else if(operation == "O"){
                data = comm::get_msg_data(recv_socket);
                std::cout << "Sending ack to: " << drone_connection.get_endpoint(sender) << std::endl;
                bool suc = comm::send_ack(send_socket,"atc", drone_connection.get_endpoint(sender));
                ascend::msg recvd_msg = msg_generator::deserialize(data);
                process_request(sender,recvd_msg);
            }
            else{
                throw std::runtime_error("Error in msg operation" + operation);
            }
        }

        //unresponsive drones
        std::vector<std::string> expired_drones = msg_tracker.ttl_exceeded();
        if(expired_drones.size() > 0){
            spdlog::error("\nERROR - Unresponsive drones");
            for(auto name: expired_drones){
                std::cerr<<"\t"<<name<<"\n";
            }
            std::cerr<<std::endl;
        }
    } 
}

