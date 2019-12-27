#pragma once

#include <zmq.hpp>


namespace comm {

    //send
    std::string send(zmq::socket_t & socket, const std::string & data, int flags = 0);
    bool send_msg(zmq::socket_t & socket, const std::string & data, int flags = 0);
    bool connect(zmq::socket_t & socket, const std::string & to, const std::string & ip_address);
    bool sendmore (zmq::socket_t & socket, const std::string & data);

    //recv
    bool recv(zmq::socket_t & socket, std::string & ostring, int flags = 0);
    bool recv(zmq::socket_t & socket, std::string & from, std::string & ostring, int flags = 0);




}



