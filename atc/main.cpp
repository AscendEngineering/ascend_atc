#include <iostream>
#include <zmq.hpp>
#include <pqxx/pqxx>
#include <string>
#include "ascenddb.h"
#include "messaging/libs/status.pb.h"
#include "ascend_zmq.h"
#include "constants.h"
#include "drone_msg.h"
#include <vector>
#include <unordered_map>

//temp
#include <chrono>
#include <thread>


unsigned int now(){
    
    //set up the chrono
    std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
    std::chrono::system_clock::duration dtn = tp.time_since_epoch();

    unsigned int retval = dtn.count() * std::chrono::system_clock::period::num / std::chrono::system_clock::period::den;
    return retval;
}

void listLedger(const std::unordered_map<std::string,unsigned int>& ledger){

    std::cout << "\nLedger\n_____________" << std::endl;
    for(auto itr = ledger.begin(); itr != ledger.end(); itr++){
        std::cout << "Key: " << itr->first << " Val: " << itr->second << std::endl;
    }


}


int main(){

    //listening
    zmq::context_t context(1);
    zmq::socket_t recv_socket(context, ZMQ_PULL);
    recv_socket.bind("tcp://*:" + constants::from_drone);

    //sending
    zmq::socket_t send_socket(context, ZMQ_PUSH);

    //variables
    int counter = 0;
    std::unordered_map<std::string,unsigned int> ledger;
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
                std::cout<<"Acknowledgement" << std::endl;

                //remove the drone from the mail list
                ledger.erase(sender);
                listLedger(ledger);
            }
            else if(operation == "O"){
                data = comm::get_msg_data(recv_socket);
                comm::send_ack(send_socket,"atc","tcp://localhost:" + constants::to_drone);
            }
            else{
                throw std::runtime_error("Error in msg operation" + operation);
            }
            
        }
        else{
            
            //simulates something sent from worker

            //add drone to mail ledger
            std::string drone_name = "drone1";
            ledger[drone_name] = now();

            //translate name to ip
            std::string ip_address = ascendDB().getIP(drone_name);
            listLedger(ledger);

            
            comm::send_msg(send_socket,"atc","Hello Drone",ip_address);
        }

        

    }
    

    


    return 0;   
}

