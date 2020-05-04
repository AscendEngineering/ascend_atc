#pragma once

#include <iostream>
#include "dat.h"
#include "constants.h"
#include "msgDef.pb.h"



//
class drone_info{
    public:
        drone_info();
        std::string get_endpoint(const std::string& drone_name);
        void handle_status_change(const std::string& drone_name, const ascend::msg& msg);
        void handle_heartbeat(const std::string& drone_name, const ascend::msg& msg);

    private:
        dat drone_connection;
        ascendDB database;

};



