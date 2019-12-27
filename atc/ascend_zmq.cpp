#include "ascend_zmq.h"
#include <string>
#include <iostream>


namespace comm {

    bool connect(zmq::socket_t & socket, const std::string & to, const std::string & ip_address){
        socket.setsockopt( ZMQ_IDENTITY, to);
        socket.connect(ip_address);

        return true;

    }

    bool send_msg(zmq::socket_t & socket, const std::string & data, int flags){
        
        zmq::message_t message(data.size());
        memcpy (message.data(), data.data(), data.size());

        bool rc = socket.send(message, flags);
        return (rc);
    }

    bool sendmore (zmq::socket_t & socket, const std::string & data) {

        zmq::message_t message(data.size());
        memcpy (message.data(), data.data(), data.size());

        bool rc = socket.send (message, ZMQ_SNDMORE);
        return (rc);
    }

    std::string send(zmq::socket_t & socket, const std::string & data, int flags){
        bool rc = true;

        //if socket is not REQ, throw error here

        //if socket is not connected throw error

        //if zmq_identity is not set, throw error

        
        rc &= send_msg(socket,data);

        if(!rc){
            throw zmq::error_t();
        }

        //get the response
        std::string response;
        std::cout<< "About to receive"<< std::endl;
        recv(socket,response);
        std::cout<<"REturned" << std::endl;

        return response;
    }


    bool recv(zmq::socket_t & socket, std::string & ostring, int flags){
        zmq::message_t message;
        bool rc = socket.recv(&message, flags);
        std::cout<<"Wait" << std::endl;

        if (rc) {
            ostring = std::string(static_cast<char*>(message.data()), message.size());
        }
        
        std::cout<<"return" << std::endl;
        return (rc);
    }

    bool recv(zmq::socket_t & socket, std::string & from, std::string & ostring, int flags){
        bool rc = true;
        std::string delimeter;

        rc &= recv(socket,from);
        rc &= recv(socket,delimeter);
        rc &= recv(socket,ostring);

        return rc;
    }
}







