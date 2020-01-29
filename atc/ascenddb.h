#pragma once

#include <string>
#include <memory>
#include <pqxx/pqxx>

const std::string connection_uri = "postgresql://ascend:ZachLikesAnal917@localhost:5432/ascend_db";

class ascendDB{
    public:
        ascendDB();
        bool connected();
        void test();
        pqxx::result execute(std::string cmd);
        std::string getIP(const std::string& droneName);

    private:
        std::shared_ptr<pqxx::connection> database;
        std::shared_ptr<pqxx::work> conn;

};









