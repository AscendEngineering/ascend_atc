#include "drone_msg.h"

std::string msg_generator::generate_heartbeat(int lng, int lat, int alt, int bat_percentage){
    ascend::msg msg;
    ascend::heartbeat_msg heartbeat;
    heartbeat.set_lng(lng);
    heartbeat.set_lat(lat);
    heartbeat.set_alt(alt);
    heartbeat.set_bat_percentage(bat_percentage);
    *msg.mutable_heartbeat() = heartbeat;

    return serialize(msg);
}

std::string msg_generator::generate_emergency(){
    ascend::msg msg;
    ascend::emergency_msg emergency;
    emergency.set_name("this is an emergency name");
    *msg.mutable_emergency() = emergency;

    return serialize(msg);
    
}


std::string msg_generator::serialize(const ascend::msg& to_send){

    //form to a string
    std::string serial_msg; 
    to_send.SerializeToString(&serial_msg);

    return serial_msg;
}






