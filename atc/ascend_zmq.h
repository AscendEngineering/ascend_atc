#include <zmq.hpp>


namespace comm {

    //send
    bool send(zmq::socket_t & socket, const std::string & string, int flags = 0);
    bool sendmore (zmq::socket_t & socket, const std::string & string);

    //recv
    bool recv(zmq::socket_t & socket, std::string & ostring, int flags = 0);





}



