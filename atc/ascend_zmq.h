#pragma once

#include <zmq.hpp>


namespace comm {

    //send
    bool connect(zmq::socket_t & socket, const std::string & ip_address, const std::string & from="");
    std::string send(zmq::socket_t & socket, const std::string& data,const std::string& identity="");

    //receive
    std::string recv(zmq::socket_t & socket, std::string& identity);

    //helpers
    int get_identity(zmq::socket_t & socket);




}



