#include "drone_info.h"
#include "ascenddb.h"
#include "constants.h"

drone_info::drone_info(){
    

}


std::string drone_info::get_endpoint(const std::string& drone_name){
    return drone_connection.get_endpoint(drone_name);
}

void drone_info::handle_status_change(const std::string& drone_name, const ascend::msg& msg){

    //sanity
    if(!msg.has_status()){
        std::cerr << "no status in msg" << std::endl;
        return;
    }

    //store
    int status = msg.status().status();
    database.setStatus(drone_name,status);
}

void drone_info::handle_heartbeat(const std::string& drone_name, const ascend::msg& msg){
    


}











