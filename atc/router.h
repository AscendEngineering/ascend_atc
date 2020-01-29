#include <vector>




struct waypoint{
    double long;
    double lat;
    double alt;
};


class router{

    public:
        router();

        //methods
        //will have more in future, after demo
        std::vector<waypoint> getRoute(const waypoint& start, const waypoint& end);


};














