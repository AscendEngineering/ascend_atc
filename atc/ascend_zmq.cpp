#include "ascend_zmq.h"
#include <string>


namespace comm {

    bool send(zmq::socket_t & socket, const std::string & string, int flags){
        
        zmq::message_t message(string.size());
        memcpy (message.data(), string.data(), string.size());

        bool rc = socket.send (message, flags);
        return (rc);
    }

    bool sendmore (zmq::socket_t & socket, const std::string & string) {

        zmq::message_t message(string.size());
        memcpy (message.data(), string.data(), string.size());

        bool rc = socket.send (message, ZMQ_SNDMORE);
        return (rc);
    }

    bool recv(zmq::socket_t & socket, std::string & ostring, int flags){
        zmq::message_t message;
        bool rc = socket.recv(&message, flags);

        if (rc) {
            ostring = std::string(static_cast<char*>(message.data()), message.size());
        }
        
        return (rc);
    }
}







