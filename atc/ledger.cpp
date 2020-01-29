
#include "ledger.h"
#include <chrono>
#include <iostream>
#include <climits>



//public

    ledger::ledger(unsigned int ttl){
        this->ttl = ttl;
    }

    void ledger::msg_sent(const std::string& drone_name){
        
        //if we are not already waiting on ack
        if(drone_list.find(drone_name) == drone_list.end()){
            drone_list[drone_name] = now();
        }
    }

    void ledger::msg_ack(const std::string& drone_name){
        drone_list.erase(drone_name);
    }

    void ledger::set_ttl(unsigned int ttl){
        this->ttl = ttl;
    }

    std::vector<std::string> ledger::ttl_exceeded(){
        std::vector<std::string> retval;
        unsigned int current_timestamp = now();

        for(auto itr = drone_list.begin(); itr != drone_list.end(); itr++){
            if(current_timestamp - itr->second > ttl){
                retval.push_back(itr->first);
            }
        }

        return retval;
    }

    unsigned int ledger::lastAck(const std::string& drone_name){
        auto result = drone_list.find(drone_name);

        if(result == drone_list.end()){
            return 0;
        }

        return result->second;
    }


    void ledger::listLedger(){

        std::cout << "\nLedger\n_____________" << std::endl;
        for(auto itr = drone_list.begin(); itr != drone_list.end(); itr++){
            std::cout << "Key: " << itr->first << " Val: " << itr->second << std::endl;
        }
    }

//private

    unsigned int ledger::now(){
    
        //set up the chrono
        std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
        std::chrono::system_clock::duration dtn = tp.time_since_epoch();

        unsigned int retval = dtn.count() * std::chrono::system_clock::period::num / std::chrono::system_clock::period::den;
        return retval;
    }



