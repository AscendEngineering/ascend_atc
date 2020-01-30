#include "atc_msg.h"


std::string msg_generator::generate_route(const std::vector<waypoint> &route){
    ascend::msg msg;
    ascend::waypointList_msg ret_route;

    for(auto waypoint: route){
        ascend::waypoint_point* new_waypoint = ret_route.add_waypoint();
        new_waypoint->set_lat(waypoint.get_lat());
        new_waypoint->set_lng(waypoint.get_lng());
        new_waypoint->set_alt(waypoint.get_alt());
    }

    *msg.mutable_waypoints() = ret_route;
    return serialize(msg);
}


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





