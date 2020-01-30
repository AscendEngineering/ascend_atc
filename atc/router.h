#pragma once

#include <vector>


struct waypoint{
    waypoint(double lat, double lng, double alt);
    double get_lat();
    double get_lng();
    double get_alt();

    private:
        double lat;
        double lng;
        double alt;
};


class router{

    public:
        router();

        //methods
        //will have more in future, after demo
        std::vector<waypoint> getRoute();
        void start(double lat,double lng);
        void end(double lat,double lng);

    private:
        waypoint start_waypoint;
        waypoint end_waypoint;

        bool start_is_set;
        bool end_is_set;


};














