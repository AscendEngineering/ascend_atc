#include <zmq.hpp>


namespace comm {

    //send
    bool send(zmq::socket_t & socket, const std::string & data, int flags = 0);
    bool send(zmq::socket_t & socket, const std::string & to, const std::string & data, int flags = 0);
    bool sendmore (zmq::socket_t & socket, const std::string & data);

    //recv
    bool recv(zmq::socket_t & socket, std::string & ostring, int flags = 0);
    bool recv(zmq::socket_t & socket, std::string & from, std::string & ostring, int flags = 0);





}



