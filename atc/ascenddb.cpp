#include "ascenddb.h"
#include <iostream>


ascendDB::ascendDB(){
    try{
        this->database = std::make_shared<pqxx::connection>(connection_uri);
        this->conn = std::make_shared<pqxx::work>(*database);

    }catch (const std::exception &e){
        std::cout <<"Error in connecting db: " << e.what() << std::endl;
    }
}

pqxx::result ascendDB::execute(std::string cmd){
    pqxx::result result;
    if(connected()){
        try{
            result = this->conn->exec(cmd);
        }catch (const std::exception &e){
            std::cout <<"Error in executing db: " << e.what() << std::endl;
        }
    }

    this->conn->commit();
    return result;
}


void ascendDB::test(){
    pqxx::result result = this->conn->exec("SELECT * FROM \"Battery\"");
    for(auto row: result){
        for(auto data: row){
            std::cout<< data <<std::endl;
        }
    }
}

bool ascendDB::connected(){
    return this->database->is_open();
}




