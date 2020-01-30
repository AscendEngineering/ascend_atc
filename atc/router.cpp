#include "router.h"
#include <stdexcept>


waypoint::waypoint(double lat, double lng, double alt){
    this->lat = lat;
    this->lng = lng;
    this->alt = alt;
}

double waypoint::get_lat(){
    return lat;
}
double waypoint::get_lng(){
    return lng;
}
double waypoint::get_alt(){
    return alt;
}

//set waypoints to invalid values for lat/long
router::router():start_waypoint(1000,1000,1000),end_waypoint(1000,1000,1000){

    start_is_set = false;
    end_is_set = false;

}

std::vector<waypoint> router::getRoute(){

    if(!start_is_set || !end_is_set){
        throw std::runtime_error("Start and/or End waypoint not set");
    }
    
    std::vector<waypoint> route;

    //TODO: Dijkstras algorithm in future on many waypoints
    route.push_back(start_waypoint);
    route.push_back(end_waypoint);

    return route;

}

void router::start(double lat,double lng){
    this->start_waypoint = waypoint(lat,lng,0.0);
    start_is_set = true;
}

void router::end(double lat,double lng){
    this->end_waypoint = waypoint(lat,lng,0.0);
    end_is_set = true;
}












