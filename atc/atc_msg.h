#pragma once

#include "msgDef.pb.h"
#include <string>
#include <vector>
#include "router.h"

namespace msg_generator {

        // //these methods are from the drone side, and need to be changed for the atc side
        // std::string generate_heartbeat(int lng, int lat, int alt, int bat_percentage);
        // std::string generate_emergency(const std::string& drone_name);
        // std::string generate_land_request(const std::string& drone_name);

        std::string generate_route(const std::vector<waypoint> &route);

        std::string serialize(const ascend::msg& to_send);
        ascend::msg deserialize(const std::string& recv);

}