#pragma once

#include <string>
#include <memory>
#include <pqxx/pqxx>

//const std::string connection_uri = "postgresql://ascend:ZachLikesAnal917@ascend-atc.duckdns.org:5432/ascend_db"; //needed when on remote network
const std::string connection_uri = "postgresql://ascend:ZachLikesAnal917@localhost:5432/ascend_db"; //needed when on local network


class ascendDB{
    public:
        ascendDB();
        bool connected();
        void test();
        pqxx::result execute(std::string cmd);
        std::string getConnectionInfo(const std::string& droneName);
        void storeConnectionInfo(const std::string& droneName,const std::string& connectionInfo);

    private:
        std::shared_ptr<pqxx::connection> database;
        

};









