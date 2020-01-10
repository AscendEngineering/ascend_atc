#include <iostream>
#include <string>
#include "drone.h"
#include "drone_msg.h"
#include "constants.h"
#include <chrono>
#include "config_handler.h"

//temp
#include <chrono>
#include <thread>


int main(){

    drone ascendDrone;
    
    while(true){
        std::cout<<config_handler::instance()["name"]<<std::endl;
        std::cout<<config_handler::instance()["ip_address"]<<std::endl;
    }


    while(true){

        //receive messages
        std::vector<std::string> messages = ascendDrone.collect_messages();
        if(messages.size() > 0){
            std::cout<<"Messages"<<std::endl;
            for(auto msg: messages){
                std::cout<<"\t"<<msg<<std::endl;
            }
        }
    }

    //     //if time, send heartbeat
        



    //     // //heartbeat
    //     // auto now = std::chrono::system_clock::now();
    //     // std::chrono::duration<double> elapsed_time = now-last_heartbeat;
    //     // std::cout<<elapsed_time.count()<<std::endl;
    //     // if(elapsed_time.count() > 5.0){
    //     //     last_heartbeat=now;
    //     //     //send heartbeat
    //     //     std::cout<<"sending beat"<<std::endl;
    //     //     comm::connect(atc_socket,"tcp://localhost:" + constants::from_drone, "drone1");
    //     //     comm::send(atc_socket,msg_generator::generate_heartbeat(1,2,3,4));
    //     // }


    // }
    
    return 0;



}
