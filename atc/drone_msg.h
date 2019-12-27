#pragma once

#include "messaging/libs/status.pb.h"
#include <string>

namespace msg_generator {

        std::string generate_heartbeat(int lng, int lat, int alt, int bat_percentage);
        std::string generate_emergency();

        std::string serialize(const ascend::msg& to_send);

}