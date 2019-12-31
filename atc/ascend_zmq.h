#pragma once

#include <zmq.hpp>


namespace comm {

    bool send_ack(zmq::socket_t & socket, const std::string& our_name, const std::string& address);
    bool send_msg(zmq::socket_t & socket, const std::string& our_name, const std::string& data, const std::string& address);

    void get_msg_header(zmq::socket_t & socket, std::string& sender, std::string& op);
    std::string get_msg_data(zmq::socket_t & socket);


}



