#pragma once

#include <string>
#include <unordered_map>

class config_handler {

    public:
        static config_handler& instance(){
            static config_handler instance("config.txt");
            return instance;
        }
        
        std::string get(const std::string& key);
        std::string operator[](const std::string& key);

    private:
        //vars
        std::string filename;
        std::unordered_map<std::string,std::string> settings;
        long int lastUpdated;

        //function
        void load_config();
        bool stale();
        long int get_last_modified_timestamp();
        config_handler(){};
        config_handler(std::string filename="config.txt");
        config_handler(const config_handler&);
        void operator=(config_handler const&);
 
};