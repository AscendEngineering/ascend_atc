#include "ascend_zmq.h"
#include <string>
#include <iostream>
#include <stdexcept>


namespace comm {

//helpers
    int get_type(zmq::socket_t & socket){
        int type;
        size_t type_size = sizeof (type);
        zmq_getsockopt (socket, ZMQ_TYPE, &type, &type_size);
        
        return type;
    }

    bool connect(zmq::socket_t & socket, const std::string & ip_address){

        socket.connect(ip_address);

        return true;
    }

    bool send_packet(zmq::socket_t & socket,const std::string& data, int flags=0){
        zmq::message_t message(data.size());
        memcpy (message.data(), data.data(), data.size());
        try{
            socket.send(message,flags);
        }catch(zmq::error_t err){
            std::cerr<<err.what()<<std::endl;
        }

        return "sent";
    }

    std::string recv_packet(zmq::socket_t & socket){
        zmq::message_t message;
        std::string ostring = "";
        bool rc = socket.recv(&message, 0);

        if (rc) {

            ostring = std::string(static_cast<char*>(message.data()), message.size());
        }
        
        return ostring;
    }


//sending

    bool send_ack(zmq::socket_t & socket, const std::string& our_name, const std::string& address){
        bool retval = true;

        //now connect to the new address
        if(!connect(socket,address)){
            return false;
        }

        //send
        retval &= comm::send_packet(socket,our_name, ZMQ_SNDMORE);
        retval &= comm::send_packet(socket,"", ZMQ_SNDMORE);
        retval &= comm::send_packet(socket,"A");

        //disconnect
        zmq_disconnect(socket,address.c_str());

        return retval;

    }

    bool send_msg(zmq::socket_t & socket, const std::string& our_name, const std::string& data, const std::string& address){
        bool retval = true;

        //connect to new endpoint
        if(!connect(socket,address)){
            return false;
        }

        //send
        retval &= comm::send_packet(socket,our_name, ZMQ_SNDMORE);
        retval &= comm::send_packet(socket,"", ZMQ_SNDMORE);
        retval &= comm::send_packet(socket,"O",ZMQ_SNDMORE);
        retval &= comm::send_packet(socket,"",ZMQ_SNDMORE);
        retval &= comm::send_packet(socket,data);

        //disconnect
        zmq_disconnect(socket,address.c_str());
        
        return retval;
    }

//receiving

    void get_msg_header(zmq::socket_t & socket, std::string& sender, std::string& op){
        
        //sender
        sender = recv_packet(socket);

        //get the delimiter
        std::string delimit = recv_packet(socket);
        if(delimit != ""){
            throw std::runtime_error("Delimit not empty");
        }

        //get operation
        op = recv_packet(socket);

    }


    std::string get_msg_data(zmq::socket_t & socket){
        
        //get the delimeter
        std::string delimit = recv_packet(socket);
        if(delimit != ""){
            throw std::runtime_error("Delimit not empty");
        }

        //return data
        return recv_packet(socket);
    }




   
}







