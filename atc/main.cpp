#include <iostream>
#include <zmq.hpp>
#include <pqxx/pqxx>
#include <string>
#include "ascenddb.h"
#include "status.pb.h"
#include "ascend_zmq.h"
#include "constants.h"
#include "drone_msg.h"
#include <vector>
#include "ledger.h"

//temp
#include <chrono>
#include <thread>


int main(){

    //listening
    zmq::context_t context(1);
    zmq::socket_t recv_socket(context, ZMQ_PULL);
    recv_socket.bind("tcp://*:" + constants::from_drone);
    ledger msg_tracker;

    //sending
    zmq::socket_t send_socket(context, ZMQ_PUSH);

    //variables
    int counter = 0;
    zmq::pollitem_t items [] = {
        {static_cast<void*>(recv_socket),0,ZMQ_POLLIN,0}

    };

    std::cout << "ATC started..." << std::endl;

    while(true){

        zmq::poll(&items[0], 1, 3000);


        //from drone
        if(items[0].revents & ZMQ_POLLIN){
            
            //get the header
            std::string sender;
            std::string operation;
            std::string data;
            comm::get_msg_header(recv_socket,sender,operation);

            if(operation == "A"){
                msg_tracker.msg_ack(sender);
            }
            else if(operation == "O"){
                data = comm::get_msg_data(recv_socket);
                comm::send_ack(send_socket,"atc","tcp://localhost:" + constants::to_drone);

                //deserialize
                ascend::msg recvd_msg = msg_generator::deserialize(data);

                if(recvd_msg.has_landing_request()){
                    std::cout<<"Landing request"<<std::endl;
                }
            }
            else{
                throw std::runtime_error("Error in msg operation" + operation);
            }
            
        }
        else{
            std::cout << "Sending request to drone" << std::endl;
            
            //simulates something sent from worker

            //add drone to mail ledger
            std::string drone_name = "drone1";
            
            //mark in ledger
            msg_tracker.msg_sent(drone_name);

            //translate name to ip
            std::string ip_address = ascendDB().getIP(drone_name);
            comm::send_msg(send_socket,"atc","Hello Drone" + std::to_string(counter++),ip_address + constants::to_drone);
        }

        //check what messages have not gotten returned
        std::vector<std::string> expired_drones = msg_tracker.ttl_exceeded();
        if(expired_drones.size() > 0){
            std::cout <<"\nERROR - Unresponsive drones" << std::endl;
            for(auto name: expired_drones){
                std::cout<<"\t"<<name<<"\n";
            }
            std::cout<<std::endl;
        }

    }
    

    


    return 0;   
}

