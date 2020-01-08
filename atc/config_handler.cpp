#include "config_handler.h"
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <algorithm>


config_handler::config_handler(std::string filename){
    this->filename = filename;
    load_config();
}


std::string config_handler::operator[](const std::string& key){
    return get(key);
}

std::string config_handler::get(const std::string& key){
    
    if(stale()){
        load_config();
    }

    auto setting = settings.find(key);
    if(setting == settings.end()){
        return "";
    }
    else{
        return setting->second;
    }

}

void config_handler::load_config(){

    std::unordered_map<std::string, std::string> temp_settings;

    //open config file
    std::ifstream configFile (filename);

    if(configFile.is_open()){
        std::string line;
        while(getline(configFile,line)){
            
            //junk line
            if(line == ""){
                continue;
            }
            
            //split key and value
            std::string key = line.substr(0,line.find(':'));
            std::string val = line.substr(line.find(':')+1);

            //key format
            std::remove_if(key.begin(), key.end(), isspace);

            //value format
            while(isspace(val[0])){
                val = val.substr(1);
            }
            
            //check if comment
            if(key[0] == '#'){
                continue;
            }

            temp_settings[key] = val;
        }
    }
    else{
        //throw error
        throw std::runtime_error("Config file cannot be opened");   
    }

    settings = temp_settings; //this is an issue, this should be atomic
    lastUpdated = get_last_modified_timestamp();
}

bool config_handler::stale(){
    return  get_last_modified_timestamp() > lastUpdated;
}

long int config_handler::get_last_modified_timestamp(){
    struct stat file_stat;
    stat(filename.c_str(), &file_stat);
    return file_stat.st_mtime;
}




