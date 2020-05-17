#include "ascenddb.h"
#include "constants.h"
#include <boost/algorithm/string/predicate.hpp>
#include "utilities.h"


namespace {

    int string_to_status(const std::string& status)
    {
        int retval = constants::RETIRED;
        if(boost::iequals(status, "available")){
            retval = constants::AVAILABLE;
        }
        else if(boost::iequals(status, "in_use")){
            retval = constants::IN_USE;
        }
        else if(boost::iequals(status, "repair")){
            retval = constants::REPAIR;
        }
        else if(boost::iequals(status, "retired")){
            retval = constants::RETIRED;
        }
        else{
            spdlog::error("Invalid string status: "+status);
        }
        return retval;
    }


    std::string status_to_string(int status)
    {
        std::string retval = "";
        if(status == constants::AVAILABLE){
            return "available";
        }
        else if(status == constants::IN_USE){
            return "in_use";
        }
        else if (status == constants::REPAIR){
            return "repair";
        }
        else if(status == constants::RETIRED){
            return "retired";
        }
        else{
            spdlog::error( "Invalid enum status: "+status);
        }
        return retval;
    }

}

ascendDB::ascendDB(){
    try{
        this->database = std::make_shared<pqxx::connection>(connection_uri);

    }catch (const std::exception &e){
        spdlog::error("Error in connecting db: "+std::string(e.what()));
    }
}

void ascendDB::test(){
    std::shared_ptr<pqxx::work> conn = std::make_shared<pqxx::work>(*database);
    pqxx::result result = conn->exec("SELECT * FROM \"Battery\"");
    for(auto row: result){
        for(auto data: row){
            std::cout<< data <<std::endl;
        }
    }
}

bool ascendDB::connected(){
    return database->is_open();
}

std::string ascendDB::getConnectionInfo(const std::string& droneName){

    std::string find_cmd = "SELECT \"connectInfo\" FROM \"Drone\" WHERE \"model\"='" + droneName + "'";
    std::string retval = read1<std::string>(find_cmd);
    return retval;
}

void ascendDB::storeConnectionInfo(const std::string& droneName,const std::string& connectionInfo){
    
    std::string insert_cmd = "INSERT INTO \"Drone\" (\"model\",\"connectInfo\") values ('"
        + droneName + "','"
        + connectionInfo
        + "') ON CONFLICT (model)"
        + " DO UPDATE SET \"connectInfo\"='" + connectionInfo + "'"
        + " WHERE \"Drone\".model='" + droneName + "'";

    write1(insert_cmd);
}

int ascendDB::getStatus(const std::string& droneName){

    std::string cmd = "SELECT \"status\" FROM \"Drone\" WHERE \"model\"='" + droneName + "'";;
    int retval = string_to_status(read1<std::string>(cmd));
    return retval;

}

void ascendDB::setStatus(const std::string& droneName, int status ){

    std::string update_cmd = "UPDATE \"Drone\" SET status = \'" + status_to_string(status)
        + "\' WHERE \"Drone\".model='" + droneName + "'";

    write1(update_cmd);
}

pqxx::result ascendDB::write1(const std::string cmd){
    std::shared_ptr<pqxx::work> conn = std::make_shared<pqxx::work>(*database);
    pqxx::result result;
    if(connected()){
        try{
            result = conn->exec(cmd);
        }catch (const std::exception &e){
            spdlog::error("Error in executing db: "+std::string(e.what()));
        }
    }

    conn->commit();
    return result;
}

