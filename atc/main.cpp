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

//temp
#include <chrono>
#include <thread>

int main(){

    //variables
    int counter = 0;
    std::vector<std::string> outgoing;

    //listening
    zmq::context_t context(1);
    zmq::socket_t from_worker(context, ZMQ_ROUTER);
    from_worker.bind("tcp://*:" + constants::from_worker);
    zmq::socket_t from_drone(context, ZMQ_ROUTER);
    from_drone.bind("tcp://*:" + constants::from_drone);

    //sending
    zmq::socket_t to_drone(context, ZMQ_REQ);

    zmq::pollitem_t items [] = {
        {static_cast<void*>(from_drone),0,ZMQ_POLLIN,0},
        {static_cast<void*>(from_worker),0,ZMQ_POLLIN,0}

    };

    std::cout << "Listening..." << std::endl;

    

    while(true){

        zmq::poll(&items[0], 2, -1);


        //from drone
        if(items[0].revents & ZMQ_POLLIN){
            
            //get the msg
            std::string identity;
            std::string raw_msg = comm::recv(from_drone,identity);

            //deserialize
            ascend::msg msg = msg_generator::deserialize(raw_msg);

            //check which worker it needs to go to
            if(msg.has_emergency()){
                std::cout << "goto emergency" << std::endl;
            }
            else if(msg.has_heartbeat()){
                std::cout << "goto heartbeat" << std::endl;
            }
            

        }

        //from worker
        if(items[1].revents & ZMQ_POLLIN){
            std::string identity;
            std::string msg = comm::recv(from_worker,identity);
            std::cout <<"Recieved: " << msg << std::endl;

            //worker establishing connection
            if(identity.substr(0,7) == "outgoing"){
                
                //get ip address
                std::string address = identity.substr(identity.find('|')+1);

                //send
                comm::connect(to_drone,address,"atc");
                comm::send(to_drone,msg);
            }
            //reply
            else{
                comm::send(from_drone,msg,identity);
            }

        }





    }
    

    


    return 0;   
}

