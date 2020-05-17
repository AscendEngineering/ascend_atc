#include "dat.h"
#include "httplib.h"
#include "utilities.h"
#include "nlohmann/json.hpp"
#include <iostream>
#include "utilities.h"


dat::dat(){
    remoteit_authenticate();
}

std::string dat::get_endpoint(const std::string& drone_name){

    //if found, return endpoint
    std::string endpoint = database.getConnectionInfo(drone_name);

    nlohmann::json endpoint_info = nlohmann::json::parse(endpoint, nullptr, false);
    if(!endpoint_info.is_discarded()){
        if(endpoint_info["expiresOn"].get<int>() > utilities::now_epoch()){
            return endpoint_info["proxy"].get<std::string>();
        }
    }
    
    //add new connection
    remoteit_refresh_devices();
    auto metadata = drone_metadata.find(drone_name);
    if(metadata != drone_metadata.end()){
        request_and_store_endpoint(drone_name);
        std::string endpoint = database.getConnectionInfo(drone_name);
        endpoint_info = nlohmann::json::parse(endpoint, nullptr, false);
        if(!endpoint_info.is_discarded()){
            return endpoint_info["proxy"].get<std::string>();
        }
    }

    spdlog::error("Drone "+drone_name+" does not exist in remoteit");
    return "";
}

void dat::remoteit_authenticate(){

    //fill in request
    httplib::Headers headers = {
        { "developerkey", remoteit_developer_key }
    };
    nlohmann::json auth_data;
    auth_data["username"] = remoteit_username;
    auth_data["password"] = remoteit_password;

    //send request
    httplib::Client cli(remoteit_base_url, 80);
    cli.set_follow_location(true);
    auto res = cli.Post(remoteit_auth_url.c_str(),headers,auth_data.dump(),"application/json");
    
    //process the result
    if (res) {
        remoteit_ticket = nlohmann::json::parse(res->body);
    }
    else{
        spdlog::error("UNABLE TO AUTHENTICATE WITH REMOTEIT");
    }
}

void dat::remoteit_refresh_devices(){

    //send request
    httplib::Headers headers = {
        { "developerkey", remoteit_developer_key },
        { "token", remoteit_ticket["token"]}
    };
    httplib::Client cli(remoteit_base_url, 80);
    cli.set_follow_location(true);
    auto res = cli.Get(remoteit_device_url.c_str(),headers);

    //update drone metadata
    if(res) {
        nlohmann::json device_list = nlohmann::json::parse(res->body)["devices"];

        for(nlohmann::json::iterator itr = device_list.begin(); itr != device_list.end(); itr++){
            std::string drone_alias = (*itr)["devicealias"].get<std::string>();
            if(drone_alias.find("to_drone") == std::string::npos){
                continue;
            }
            std::string drone_name = drone_alias.substr(3);
            drone_metadata[drone_name] = *itr;
        }
    }
    else{
        spdlog::error("REMOTEIT DEVICE FETCH FAILED");
    }
}

bool dat::request_and_store_endpoint(const std::string& drone_name){

    //sanity check
    if(drone_metadata.find(drone_name) == drone_metadata.end()){
        return false;
    }

    //form request
    nlohmann::json metadata = drone_metadata[drone_name];
    httplib::Headers headers = {
        { "developerkey", remoteit_developer_key },
        { "token", remoteit_ticket["token"]}
    };
    nlohmann::json device_details;
    device_details["deviceaddress"] = metadata["deviceaddress"];
    device_details["wait"] = true;
    device_details["hostip"] = metadata["devicelastip"];

    //send request
    httplib::Client cli(remoteit_base_url, 80);
    cli.set_follow_location(true);
    auto res = cli.Post(remoteit_connect_url.c_str(),headers, device_details.dump(),"application/json");

    //modify
    if(res){

        //expiration
        nlohmann::json returned_connection = nlohmann::json::parse(res->body)["connection"];
        long expires_on = std::stol(returned_connection["expirationsec"].get<std::string>()) 
            + utilities::now_epoch();
        returned_connection["expiresOn"] = expires_on;

        //endpoint
        std::string endpoint = returned_connection["proxy"].get<std::string>();
        returned_connection["proxy"] = endpoint.substr(endpoint.find("//")+2);

        //store
        database.storeConnectionInfo(drone_name, returned_connection.dump());

        return true;
    }
    else{
        spdlog::error("ERROR IN RETREIVING ENDPOINT ");
        return false;
    }

}


