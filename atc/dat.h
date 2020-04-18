#include <string>
#include <map>



class dat{

    public:
        dat(){};
        get_url(std::string drone_name);
        add_url(std::string drone_name, std::string drone_url);

    private:
        std::unordered_map<std::string,std::string> drone_dictionary;


}