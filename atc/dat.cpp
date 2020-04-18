#include "dat.h"


dat::get_url(std::string drone_name){

    auto drone_url = drone_dictionary.find(drone_name);
    if(drone_url != drone_dictionary.end()){
        return "";
    }
    return drone_url.second;
}


dat::add_url(std::string drone_name, std::string drone_url){
    drone_dictionary[drone_name] = drone_url;
}


