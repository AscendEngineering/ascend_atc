#include "atc_msg.h"

std::string msg_generator::serialize(const ascend::msg& to_send){

    //form to a string
    std::string serial_msg; 
    to_send.SerializeToString(&serial_msg);

    return serial_msg;
}


ascend::msg msg_generator::deserialize(const std::string& recv){
    ascend::msg retval;
    retval.ParseFromString(recv);

    return retval;


}





