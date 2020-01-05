#pragma once
#include <unordered_map>
#include <string>
#include <vector>

class ledger{

public:

    ledger(unsigned int ttl=5);

    //print out all drones and timestamps in ledger
    void listLedger();

    //adds or deletes drones from ledger
    void msg_sent(const std::string& drone_name);
    void msg_ack(const std::string& drone_name);

    //set the ttl before considered an error
    void set_ttl(unsigned int ttl);

    /*return list of drones that have exceeded ttl
    without sending acknowledgement*/ 
    std::vector<std::string> ttl_exceeded();

    //return last time drone responded
    unsigned int lastAck(const std::string& drone_name);

private:

    unsigned int ttl;
    std::unordered_map<std::string,unsigned int> drone_list;

    //econds since epoch
    unsigned int now();
    
    

};
