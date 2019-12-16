#include <iostream>
#include <zmq.hpp>
#include <pqxx/pqxx>
#include <string>
#include "ascenddb.h"

int main(){

	// try{
    //     std::string connection_uri = "postgresql://ascend:ZachLikesAnal917@localhost:5432/ascend_db";
    //     pqxx::connection database(connection_uri);
    //     std::cout << "Connected to " << database.dbname() << std::endl;
    //     pqxx::work conn(database);

    //     pqxx::result result = conn.exec("SELECT * FROM \"Battery\"");

    //     for(auto row: result){
    //         for(auto data: row){
    //             std::cout<< data <<std::endl;
    //         }
    //     }

    // }catch (const std::exception &e){
    //     std::cout <<"Error in db: " << e.what() << std::endl;
    // }

    ascendDB database;
    
    std::cout << "Connected:" << database.connected() << std::endl;
    pqxx::result result = database.execute("SELECT * FROM \"Battery\"");

    // zmq::context_t context(1);
    // zmq::socket_t socket(context, ZMQ_REP);
    // socket.bind("tcp://*:5555");

    // while(true){
    //     zmq::message_t request;

    //     socket.recv(request);
    //     std::string cmd = (char*)(request.data());
    //     std::cout << "Msg recieved: " << cmd <<std::endl;

    //     socket.send(zmq::str_buffer("recieved"), zmq::send_flags::dontwait);
    // }

    return 0;   
}

