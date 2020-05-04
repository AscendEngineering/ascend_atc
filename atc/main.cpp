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
#include "drone_info.h"
#include "utilities.h"

//temp
#include <chrono>
#include <thread>


int main(){

    utilities::setup_logging();
    exit(0);

    //listening
    zmq::context_t context(1);
    zmq::socket_t recv_socket(context, ZMQ_PULL);
    recv_socket.bind("tcp://*:" + constants::from_drone);
    drone_info drone_dict;
    ledger msg_tracker;

    //sending
    zmq::socket_t send_socket(context, ZMQ_PUSH);

    //variables
    int counter = 0;
    zmq::pollitem_t items [] = {
        {static_cast<void*>(recv_socket),0,ZMQ_POLLIN,0}
    };

    std::cout << "Listening..." << std::endl;
    while(true){

        zmq::poll(&items[0], 1, 3000);

        //from drone
        if(items[0].revents & ZMQ_POLLIN){
            
            //header
            std::string sender;
            std::string operation;
            std::string data;
            comm::get_msg_header(recv_socket,sender,operation);

            //operation
            if(operation == "A"){
                msg_tracker.msg_ack(sender);
            }
            else if(operation == "O"){
                data = comm::get_msg_data(recv_socket);
                comm::send_ack(send_socket,"atc", drone_dict.get_endpoint(sender));
                ascend::msg recvd_msg = msg_generator::deserialize(data);

                //landing request
                if(recvd_msg.has_landing_request()){
                    //comm::send_msg(send_socket,"atc",drone_dict.get_endpoint(sender));
                }
                else if(recvd_msg.has_status()){
                    drone_dict.handle_status_change(sender,recvd_msg);
                }
                else if(recvd_msg.has_heartbeat()){
                    drone_dict.handle_heartbeat(sender,recvd_msg);
                }

            }
            else{
                throw std::runtime_error("Error in msg operation" + operation);
            }
        }

        //notify of unresponsive drones
        std::vector<std::string> expired_drones = msg_tracker.ttl_exceeded();
        if(expired_drones.size() > 0){
            std::cerr <<"\nERROR - Unresponsive drones" << std::endl;
            for(auto name: expired_drones){
                std::cerr<<"\t"<<name<<"\n";
            }
            std::cerr<<std::endl;
        }
    } 
}

