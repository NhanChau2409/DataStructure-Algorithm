// Datastructures.cc
//
// Student name:
// Student email:
// Student number:

#include "datastructures.hh"

#include <random>

#include <cmath>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{

}

Datastructures::~Datastructures()
{
    // Write any cleanup you need here
}

/**
 * @brief station_count lists all the number of stations currently 
 * in the data structure
 * @return Returns the number of stations currently 
 * in the data structure
*/
unsigned int Datastructures::station_count()
{
    return station.size();
}

/**
 * @brief clear_all clears out the data structures.
 * all_stations() and all_regions() return empty vectors
*/
void Datastructures::clear_all()
{
    station.clear();
    region.clear();
    station_coord_map.clear();
    station_name_map.clear();
    result_distance_increasing.clear();
    result_alphabeltically.clear();
}

/**
 * @brief all_stations lists all the stations in station id order
 * @return all the stations in any (arbitrary) order
*/
std::vector<StationID> Datastructures::all_stations()
{
    std::vector<StationID> result;
    for (auto& i: station){
        result.push_back(i.first);
    }
    return result;
}

/**
 * @brief add_station adds a station to the data structure with 
 * the given unique id, name, type, and coordinates
 * @param id station id
 * @param name name of station id
 * @param xy coordinate of station id
 * @return If there already is a station with the given id, nothing 
 * is done and false is returned, otherwise true is returned
*/
bool Datastructures::add_station(StationID id, const Name& name, Coord xy)
{
    auto found_station = station.find(id);
    if (found_station == station.end()) {
        
        Station_Info info;
        info.name = name;
        info.xy = xy;

        station.insert({id, info});
        station_coord_map[xy] = id;
        station_name_map[name] = id;

        cache_station_alphabeltically = true;
        cache_station_distance_increasing = true;
        return true;
    }
    return false;
}

/**
 * @brief get_station_name gets the station name based on the given station id
 * @param id station id
 * @return Returns the name and type of the station with the given
 * ID, or NO_NAME if such a station doesn't exist.
*/
Name Datastructures::get_station_name(StationID id)
{
    auto founded_station = station.find(id);
    if (founded_station == station.end()){
        return NO_NAME;
    }

    return (*founded_station).second.name;
}

/**
 * @brief get_station_coordinates gets the station coordinates based 
 * on the given station id
 * @param id station id
 * @return Returns the name and type of the station with the given
 * ID, or NO_NAME if such a station doesn't exist.
*/
Coord Datastructures::get_station_coordinates(StationID id)
{
    auto founded_station = station.find(id);
    if (founded_station == station.end()){
        return NO_COORD;
    }

    return (*founded_station).second.xy;
}

/**
 * @brief stations_alphabetically sorts according to alphabetical 
 * order of station names
 * @return Returns vector station IDs sorted according to 
 * alphabetical order of station name
*/
std::vector<StationID> Datastructures::stations_alphabetically()
{
    if (cache_station_alphabeltically == true) {
        std::vector<StationID> r;
        r.reserve(station.size());
        for (auto &i : station_name_map){
        r.push_back(i.second);
        }
        result_alphabeltically = r;
    }
    cache_station_alphabeltically = false;
    return result_alphabeltically;
}

/**
 * @brief stations_distance_increasing lists station IDs sorted 
 * according to their coordinates. 
 * comparison is based on the “normal” euclidean distance from 
 * the origin. If the distance to the origin is the same, the 
 * coordinate with the smaller y-value comes firs
 * @return Returns vector station IDs sorted according to their coordinates
*/
std::vector<StationID> Datastructures::stations_distance_increasing()
{
    if (cache_station_distance_increasing == true) {
        std::vector<StationID> r;
        r.reserve(station.size());
        for (auto &i : station_coord_map){
        r.push_back(i.second);
        }
        result_distance_increasing = r;
        return r;
    }

    cache_station_distance_increasing = false;
    return result_distance_increasing;
}

/**
 * @brief find_station_with_coord get the station coordinates based
 *  on the given station id
 * @param xy coordinate of station
 * @return Returns a station with the given coordinate, or
 * NO_STATION, if no such station exists
*/
StationID Datastructures::find_station_with_coord(Coord xy)
{
    const auto foundStation = station_coord_map.find(xy);
    if (foundStation == station_coord_map.end()){
        return NO_STATION;
    }

    return foundStation->second;
}

/**
 * @brief change_station_coord changes the location of the station 
 * with given ID.
 * @param id station id
 * @param newcoord new coordinate of station
 * @return If such station doesn't exist, returns false, otherwise true.
*/
bool Datastructures::change_station_coord(StationID id, Coord newcoord)
{
    auto found_station = station.find(id);
    if (found_station == station.end()){
        return false;
    }
    auto temp = (*found_station).second.xy;
    (*found_station).second.xy = newcoord;
    station_coord_map.erase(temp);
    station_coord_map[newcoord] = id;
    return true;
}

/**
 * @brief add_departure adds information that the given train leaves 
 * from the given station at the given time
 * @param stationid station id
 * @param trainid train id
 * @param time time train leaves from given station 
 * @return If such station doesn't exist or the departure has already
 * been added (train already leaves from given station at the given time),
 * returns false, otherwise true
*/
bool Datastructures::add_departure(StationID stationid, TrainID trainid, Time time)
{
    auto found_station = station.find(stationid);
    if (station.find(stationid) == station.end()) {
        return false;
    }

    std::pair<Time, TrainID> departure (time, trainid);
    (*found_station).second.schedule.push_back(departure);
    return true;
}

/**
 * @brief remove_departure removes the given train departure 
 * from the given station at the given time
 * @param stationid station id
 * @param train train id
 * @param time time train leaves from given station 
 * @return If such a station or such departure from the station 
 * doesn't exist, returns false, otherwise true.
*/
bool Datastructures::remove_departure(StationID stationid, TrainID trainid, Time time)
{
    auto found_station = station.find(stationid);
    if (found_station== station.end()) {
        return false;
    } 

    std::pair<Time, TrainID> departure (time, trainid);
    auto re = std::find((*found_station).second.schedule.begin(), (*found_station).second.schedule.end(), departure);
    if (re == (*found_station).second.schedule.end()) {
        return false;
    }
    station.at(stationid).schedule.erase(re);
    return true;

}

/**
 * @brief station_departures_after lists all train departures 
 * from the given station at or after the given time
 * @param station station id
 * @param time input time 
 * @return vector<pair<Time,TrainID> If trains have the same departure 
 * time, they should be sorted based on their ids. If the given station 
 * doesn't exist, pair {NO_TIME,NO_TRAIN} is returned.  Departures 
 * should be sorted based on departure time
*/
std::vector<std::pair<Time, TrainID>> Datastructures::station_departures_after(StationID stationid, Time time)
{
    std::vector<std::pair<Time, TrainID>> r;
    auto found_station = station.find(stationid);

    if (found_station == station.end()) {
        std::pair<Time, TrainID> departure (NO_TIME, NO_TRAIN);
        r.push_back(departure);
        return r;
    }
    std::copy_if((*found_station).second.schedule.begin(), (*found_station).second.schedule.end(), std::back_inserter(r),
                 [time](auto& departure){return departure.first >= time;});
    return r;
}

/**
 * @brief add_region Adds a region to the data structure with given 
 * unique id, name and polygon (coordinates). Initially the added
 * region is not a subregion of any region, and it doesn’t contain 
 * any subregions or stations
 * @param id region id
 * @param name region name
 * @param coords coordinates of region
 * @return If there already is a region with the given id, nothing 
 * is done and false is returned, otherwise true is returned.
*/
bool Datastructures::add_region(RegionID id, const Name &name, std::vector<Coord> coords)
{
    if (region.find(id) == region.end()) {
        Region_Info info;
        info.name = name;
        info.xy_vec = coords;
        region.insert({id, info});
        region_id_vec.push_back(id);
        return true;
    }
    return false;

}

/**
 * @brief all_regions lists all the regions in id region order 
 * @return Returns all the regions in any (arbitrary) order (the
 * main routine sorts them based on their ID)
*/
std::vector<RegionID> Datastructures::all_regions()
{
    return region_id_vec;
}

/**
 * @brief get_region_name gets the name of the region with 
 * the given ID
 * @param id region id
 * @return Returns the name of the region with the given ID, or 
 * NO_NAME if such region doesn't exist.
*/
Name Datastructures::get_region_name(RegionID id)
{
    if (region.find(id) == region.end()){
        return NO_NAME;
    }

    return region.at(id).name;
}

/**
 * @brief get_region_coords gets the coordinate vector of the 
 * region with the given ID
 * @param id region id
 * @return Returns the coordinate vector of the region with the
 * given ID, or a vector with single item NO_COORD, if
 * such region doesn't exist.
*/
std::vector<Coord> Datastructures::get_region_coords(RegionID id)
{
    auto found_region = region.find(id);
    if (found_region == region.end()){
        std::vector<Coord> r =  {NO_COORD};
        return r;
    }

    return (*found_region).second.xy_vec;
}

/**
 * @brief add_subregion_to_region adds the first given region as 
 * a subregion to the second region
 * @param id subregion id
 * @param parentid parent region id
 * @return If no regions exist with the given IDs, or if the first 
 * region is already a subregion of some region, nothing is done 
 * and false is returned, otherwise true is returned
*/
bool Datastructures::add_subregion_to_region(RegionID id, RegionID parentid)
{
    auto parent_region = region.find(parentid);
    auto child_region = region.find(id);
    if (child_region == region.end() || parent_region  == region.end()){
        return false;
    }

    if ((*child_region).second.parentID != NO_REGION) {
        return false;
    }

    (*parent_region).second.childrendID.push_back(id);
    (*child_region).second.parentID = parentid;
    return true;
}

/**
 * @brief add_station_to_region adds the given station to the given region
 * @param id station id
 * @param parentid region id
 * @return If no station or region exist with the given IDs, or if the 
 * station already belongs to some region, nothing is done and false is 
 * returned, otherwise true is returned.
*/
bool Datastructures::add_station_to_region(StationID id, RegionID parentid)
{
    auto found_station = station.find(id);
    auto found_region = region.find(parentid);
    if (found_station == station.end() || found_region == region.end()){
        return false;
    }

    if ((*found_station).second.regionParent != NO_REGION) {
        return false;
    }

    (*found_station).second.regionParent = parentid;
    return true;
}

void Datastructures::recursive_add_subregion(std::vector<RegionID> &subregion_Id, std::vector<RegionID> &r)
{
    if (subregion_Id.size() == 0) {
        return;
    }

    r.insert(r.end(), subregion_Id.begin(), subregion_Id.end());
    for (auto &i : subregion_Id) {
        recursive_add_subregion(region.at(i).childrendID, r);
    }

}

void Datastructures::recursive_add_parent_region(RegionID &parent_Id, std::vector<RegionID>&r){
    if (parent_Id == NO_REGION){
        return;
    }
    r.push_back(parent_Id);
    recursive_add_parent_region(region.at(parent_Id).parentID, r);
}

/**
 * @brief station_in_regions finds a list of regions to which the given 
 * station belongs either directly or indirectly
 * @param id station id
 * @return The returned vector first contains the region to which the 
 * given station belongs directly, then the region that this region 
 * belongs to, etc. If no station with the given ID exists, a
 * vector with a single element NO_REGION is returned. 
*/
std::vector<RegionID> Datastructures::station_in_regions(StationID id)
{
    std::vector<RegionID> r;

    auto found_station = station.find(id);
    if (found_station == station.end()){
        r = {NO_REGION};
        return r;
    }

    recursive_add_parent_region((*found_station).second.regionParent, r);
    return r;
}

std::vector<RegionID> Datastructures::all_subregions_of_region(RegionID id)
{
    std::vector<RegionID> r;
    if (region.find(id) == region.end()){
        r = {NO_REGION};
        return r;
    }
    recursive_add_subregion(region.at(id).childrendID, r);
    return r;
}

std::vector<StationID> Datastructures::stations_closest_to(Coord xy)
{
    auto MAX = std::numeric_limits<unsigned long long>::max();

    std::vector<StationID> result;
    result.reserve(50);
    auto dis1 = MAX;
    auto dis2 = MAX;
    auto dis3 = MAX;

    
    auto position = result.begin();

    for (auto &i: station_coord_map){
        unsigned long long dis = (i.first.x-xy.x)*(i.first.x-xy.x) + (i.first.y-xy.y)*(i.first.y-xy.y);

        if ( dis < dis1) {
            dis3 = dis2;
            dis2 = dis1;
            dis1 = dis;
            result.insert(position, i.second); 

        } else if (dis < dis2) {
            dis3 = dis2;
            dis2 = dis;
            result.insert(position+1, i.second);

        } else if (dis < dis3) {
            dis3 = dis;
            result.insert(position+2, i.second);
        }
        if(result.size() > 3) {
            result.pop_back();
        }
    }
    result.shrink_to_fit();
    return result;
}

bool Datastructures::remove_station(StationID id)
{
    auto found_station = station.find(id);
    if (found_station == station.end()){
        return false;
    }

    station_coord_map.erase((*found_station).second.xy);
    station_name_map.erase((*found_station).second.name);
    station.erase(id);
    cache_station_alphabeltically = true;
    cache_station_distance_increasing = true;
    return true;
}

void Datastructures::find_all_parent_region(RegionID id, std::unordered_set<RegionID> &r) {
    auto e = region.at(id).parentID;
    if (e == NO_REGION) {
        return;
    }
   r.insert(e);
   find_all_parent_region(e, r);
}

RegionID Datastructures::common_parent_of_regions(RegionID id1, RegionID id2)
{
    if (region.find(id1) == region.end() || region.find(id2) == region.end()) { 
        return NO_REGION;
    }
    std::unordered_set<RegionID> r;
    find_all_parent_region(id1, r);

    auto parent2 = region.at(id2).parentID;
    while (parent2 != NO_REGION) {
        if (r.find(parent2) != r.end()) {
            return parent2;
        }
        parent2 = region.at(parent2).parentID;
    }
    return NO_REGION;
}
