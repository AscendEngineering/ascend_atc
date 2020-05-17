#pragma once

#include <string>
#include <map>
#include <unordered_map>
#include "nlohmann/json.hpp"
#include "ascenddb.h"


class dat{

    public:
        dat();
        std::string get_endpoint(const std::string& drone_name);
        

    private:

        //functions
        void remoteit_authenticate();
        void remoteit_refresh_devices();
        bool request_and_store_endpoint(const std::string& drone_name);
        std::string get_token();

        //variables
        std::unordered_map<std::string,nlohmann::json> drone_metadata;
        nlohmann::json remoteit_ticket;
        ascendDB database;
        const std::string remoteit_base_url = "api.remot3.it";
        const std::string remoteit_auth_url = "/apv/v27/user/login";
        const std::string remoteit_device_url = "/apv/v27/device/list/all";
        const std::string remoteit_connect_url = "/apv/v27/device/connect";

        //remoteit credentials
        const std::string remoteit_username = "wilkinsaf@gmail.com";
        const std::string remoteit_password = "ZachLikesAnal917";
        const std::string remoteit_developer_key = "N0QxMkU1NUEtOEU4My00QjRCLUI3RDgtMDcyQTUyQkQ4MTE0";

};