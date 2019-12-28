#include "ascend_zmq.h"
#include <string>
#include <iostream>


namespace comm {

//helpers
    int get_type(zmq::socket_t & socket){
        int type;
        size_t type_size = sizeof (type);
        zmq_getsockopt (socket, ZMQ_TYPE, &type, &type_size);
        
        return type;
    }


//sending

    bool connect(zmq::socket_t & socket, const std::string & ip_address, const std::string & from){

        if(from != ""){
            socket.setsockopt(ZMQ_IDENTITY, from.c_str(), (int)from.size());
        }

        socket.connect(ip_address);

        return true;
    }

    std::string send_packet(zmq::socket_t & socket,const std::string& data, int flags=0){
        zmq::message_t message(data.size());
        memcpy (message.data(), data.data(), data.size());
        try{
            socket.send(message,flags);
        }catch(zmq::error_t err){
            std::cerr<<err.what()<<std::endl;
        }

        return "sent";
    }

    std::string send(zmq::socket_t & socket, const std::string& data,const std::string& identity){

        //REQ
        if(get_type(socket) == ZMQ_PUSH){
            send_packet(socket,data);
        }

        //ROUTER
        else if(get_type(socket) == ZMQ_ROUTER){        
            send_packet(socket, identity, ZMQ_SNDMORE); //identity
            send_packet(socket, "", ZMQ_SNDMORE);       //delimeter
            send_packet(socket, data);                  //data
        }

        return "sent";
    }


//receiving

    std::string recv_packet(zmq::socket_t & socket){
        zmq::message_t message;
        std::string ostring = "";
        bool rc = socket.recv(&message, 0);

        if (rc) {

            ostring = std::string(static_cast<char*>(message.data()), message.size());
        }
        
        return ostring;
    }

    std::string recv(zmq::socket_t & socket, std::string& identity){
        zmq::message_t message;
        std::string ostring = "";
        bool rc = true;

        //REQ
        if(get_type(socket) == ZMQ_REQ){
            ostring = recv_packet(socket);
        }

        //ROUTER
        else if(get_type(socket) == ZMQ_ROUTER){   
            identity = recv_packet(socket);
            recv_packet(socket); //delimeter
            ostring = recv_packet(socket);
        }
        
        return ostring;

    }




   
}







