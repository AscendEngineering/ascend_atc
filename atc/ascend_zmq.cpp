#include "ascend_zmq.h"
#include <string>


namespace comm {

    bool send(zmq::socket_t & socket, const std::string & data, int flags){
        
        zmq::message_t message(data.size());
        memcpy (message.data(), data.data(), data.size());

        bool rc = socket.send (message, flags);
        return (rc);
    }

    bool sendmore (zmq::socket_t & socket, const std::string & data) {

        zmq::message_t message(data.size());
        memcpy (message.data(), data.data(), data.size());

        bool rc = socket.send (message, ZMQ_SNDMORE);
        return (rc);
    }

    bool send(zmq::socket_t & socket, const std::string & to, const std::string & data, int flags){
        bool rc = true;

        //socket.setsockopt( ZMQ_IDENTITY, to);
        rc &= sendmore(socket,to);
        rc &= sendmore(socket,"");
        rc &= send(socket,data);

        return rc;
    }


    bool recv(zmq::socket_t & socket, std::string & ostring, int flags){
        zmq::message_t message;
        bool rc = socket.recv(&message, flags);

        if (rc) {
            ostring = std::string(static_cast<char*>(message.data()), message.size());
        }
        
        return (rc);
    }

    bool recv(zmq::socket_t & socket, std::string & from, std::string & ostring, int flags){
        bool rc = true;
        std::string delimeter;

        //socket.setsockopt( ZMQ_IDENTITY, to);
        rc &= recv(socket,from);
        rc &= recv(socket,delimeter);
        rc &= recv(socket,ostring);

        return rc;
    }
}







