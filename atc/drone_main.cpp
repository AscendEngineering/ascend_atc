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
    std::string ourName = "drone1";
    
    //set up comms
    zmq::context_t context (1);

    //send
    zmq::socket_t send_socket (context, ZMQ_PUSH);
    zmq::socket_t recv_socket (context, ZMQ_PULL);
    std::string ourIP = "tcp://localhost:";
    recv_socket.bind("tcp://*:" + constants::to_drone);

    zmq::pollitem_t items [] = {
        {static_cast<void*>(recv_socket),0,ZMQ_POLLIN,0}
    };


    while(true){

        zmq::poll(&items[0], 1, -1);

        //from atc
        if(items[0].revents & ZMQ_POLLIN){
            //get the message
            std::string sender;
            std::string operation;
            std::string data;
            comm::get_msg_header(recv_socket,sender,operation);

            if(operation=="A"){
                std::cout<<"Acknowledgement" << std::endl;

                //TODO: figure out how we are going to handle mail
            }
            else if(operation=="O"){
                
                //recv and ack
                data = comm::get_msg_data(recv_socket);
                comm::send_ack(send_socket,ourName,"tcp://localhost:" + constants::from_drone);

                //send message to workers
                //TODO
            }
            else{
                throw std::runtime_error("Error in msg operation" + operation);
            }
            
            

        }
        //to atc
        else{
            std::cout<< "Contacting ATC" << std::endl;
            comm::send_msg(send_socket,ourName,"Hello atc","tcp://localhost:" + constants::from_drone);
        }



        // //heartbeat
        // auto now = std::chrono::system_clock::now();
        // std::chrono::duration<double> elapsed_time = now-last_heartbeat;
        // std::cout<<elapsed_time.count()<<std::endl;
        // if(elapsed_time.count() > 5.0){
        //     last_heartbeat=now;
        //     //send heartbeat
        //     std::cout<<"sending beat"<<std::endl;
        //     comm::connect(atc_socket,"tcp://localhost:" + constants::from_drone, "drone1");
        //     comm::send(atc_socket,msg_generator::generate_heartbeat(1,2,3,4));
        // }


    }
    
    return 0;



}
