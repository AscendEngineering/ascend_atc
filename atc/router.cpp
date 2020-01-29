#include "router.cpp"



router::router(){}

std::vector<waypoint> router::getRoute(const waypoint& start, const waypoint& end){

    std::vector<waypoint> retVal;
    
    //Dijkstras algorithm in future on many waypoints

    retVal.push_back(start);
    retVal.push_back(end);

    return retVal;

}












