#include <iostream>
#include <string>
#include <zmq.hpp>
#include "drone_msg.h"
#include "ascend_zmq.h"
#include "constants.h"
#include <chrono>

//temp
#include <chrono>
#include <thread>


int main(){

    //variables
    auto last_heartbeat = std::chrono::system_clock::now();
    
    //set up comms
    zmq::context_t context (1);

    //send
    zmq::socket_t atc_socket (context, ZMQ_REQ);
    zmq::socket_t recv_socket (context, ZMQ_ROUTER);

    zmq::pollitem_t items [] = {
        {static_cast<void*>(recv_socket),0,ZMQ_POLLIN,0}
    };


    while(true){

        zmq::poll(&items[0], 1, 500);

        if(items[0].revents & ZMQ_POLLIN){
            std::string identity;
            std::cout <<"Recieved: " << comm::recv(recv_socket,identity) << std::endl;
        }



        //heartbeat
        auto now = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_time = now-last_heartbeat;
        std::cout<<elapsed_time.count()<<std::endl;
        if(elapsed_time.count() > 5.0){
            last_heartbeat=now;
            //send heartbeat
            std::cout<<"sending beat"<<std::endl;
            comm::connect(atc_socket,"tcp://localhost:" + constants::from_drone, "drone1");
            comm::send(atc_socket,msg_generator::generate_heartbeat(1,2,3,4));
        }


    }
    
    return 0;



}
