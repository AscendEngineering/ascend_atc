#pragma once

#include <string>
#include <memory>
#include <pqxx/pqxx>
#include <iostream>
#include "logging.h"

//const std::string connection_uri = "postgresql://ascend:ZachLikesAnal917@ascend-atc.duckdns.org:5432/ascend_db"; //needed when on remote network
const std::string connection_uri = "postgresql://ascend:ZachLikesAnal917@localhost:5432/ascend_db"; //needed when on local network


class ascendDB{
    public:
        ascendDB();
        bool connected();
        void test();
        std::string getConnectionInfo(const std::string& droneName);
        void storeConnectionInfo(const std::string& droneName,const std::string& connectionInfo);
        int getStatus(const std::string& droneName);
        void setStatus(const std::string& droneName, int status );


    private:
        std::shared_ptr<pqxx::connection> database;
        pqxx::result write1(const std::string cmd);
        
        template <class T>
        T read1(const std::string& cmd){
            if(connected()){
                std::shared_ptr<pqxx::work> conn = std::make_shared<pqxx::work>(*database);
                try{
                    return conn->exec1(cmd)[0].as<T>();
                }
                catch(const std::exception &e){
                    spdlog::error("Read: "+cmd+"did not work -> "+e.what());
                }
            }
            return T();
        }
        
};









