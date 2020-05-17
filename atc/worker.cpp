#include "worker.h"
#include "ascenddb.h"
#include "constants.h"


void worker::handle_status_change(const std::string& drone_name, const ascend::msg& msg){

    //sanity
    if(!msg.has_status()){
        std::cerr << "no status in msg" << std::endl;
        return;
    }

    //store
    int status = msg.status().status();
    ascendDB database;
    database.setStatus(drone_name,status);
}

void worker::handle_heartbeat(const std::string& drone_name, const ascend::msg& msg){
    


}











