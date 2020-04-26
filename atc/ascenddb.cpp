#include "ascenddb.h"
#include <iostream>


ascendDB::ascendDB(){
    try{
        this->database = std::make_shared<pqxx::connection>(connection_uri);

    }catch (const std::exception &e){
        std::cerr <<"Error in connecting db: " << e.what() << std::endl;
    }
}

pqxx::result ascendDB::execute(std::string cmd){
    std::shared_ptr<pqxx::work> conn = std::make_shared<pqxx::work>(*database);
    pqxx::result result;
    if(connected()){
        try{
            result = conn->exec(cmd);
        }catch (const std::exception &e){
            std::cerr <<"Error in executing db: " << e.what() << std::endl;
        }
    }

    conn->commit();
    return result;
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

    std::string retVal = "";
    std::shared_ptr<pqxx::work> conn = std::make_shared<pqxx::work>(*database);
    try{
        std::string find_cmd = "SELECT \"connectInfo\" FROM \"Drone\" WHERE \"model\"='" + droneName + "'";
        pqxx::row result = conn->exec1(find_cmd);

        for(auto data: result){
            retVal = data.as<std::string>();
        }
    }
    catch(...){}
    
    return retVal;
}

void ascendDB::storeConnectionInfo(const std::string& droneName,const std::string& connectionInfo){
    
    std::shared_ptr<pqxx::work> conn = std::make_shared<pqxx::work>(*database);
    std::string insert_cmd = "INSERT INTO \"Drone\" (\"model\",\"connectInfo\") values ('"
        + droneName + "','"
        + connectionInfo
        + "') ON CONFLICT (model)"
        + " DO UPDATE SET \"connectInfo\"='" + connectionInfo + "'"
        + " WHERE \"Drone\".model='" + droneName + "'";

    conn->exec0(insert_cmd);
    conn->commit();

}




