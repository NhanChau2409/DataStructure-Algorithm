// Datastructures.hh
//
// Student name:
// Student email:
// Student number:

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <limits>
#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <algorithm>
#include <cmath>
#include <set>

// Types for IDs
using StationID = std::string;
using TrainID = std::string;
using RegionID = unsigned long long int;
using Name = std::string;
using Time = unsigned short int;

// Return values for cases where required thing was not found
StationID const NO_STATION = "---";
TrainID const NO_TRAIN = "---";
RegionID const NO_REGION = -1;
Name const NO_NAME = "!NO_NAME!";
Time const NO_TIME = 9999;

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();


// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary



struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    unsigned long long dis_a = c1.x*c1.x + c1.y*c1.y;
    unsigned long long dis_b = c2.x*c2.x + c2.y*c2.y;

    if ( dis_a == dis_b) {
        return c1.y < c2.y;
        }
    return dis_a < dis_b;
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};


// This is the class you are supposed to implement


struct Station_Info {
    Name name = NO_NAME;
    Coord xy = NO_COORD;
    RegionID regionParent = NO_REGION;
    std::vector<std::pair<Time, TrainID>> schedule;
};

struct Region_Info {
    Name name = NO_NAME;
    std::vector<Coord> xy_vec;
    RegionID parentID = NO_REGION;
    std::vector<RegionID> childrendID;
};

using Station = std::unordered_map<StationID, Station_Info>;

using Region = std::unordered_map<RegionID, Region_Info>;



class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: O(1)
    // Short rationale for estimate:
    // return size of unordermap station
    unsigned int station_count();

    // Estimate of performance: O(1)
    // Short rationale for estimate:
    // Clear all data inside unordermap station, unordermap region, map station_coord_map
    void clear_all();

    // Estimate of performance: O(n)
    // Short rationale for estimate: loop through n element of unordermap staion to get StationID
    std::vector<StationID> all_stations();

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: find & insert in unordermap cost constant, same with assign, but insert in map cost log(n)
    bool add_station(StationID id, Name const& name, Coord xy);

    // Estimate of performance: O(1)
    // Short rationale for estimate: find & access unordermap station id cost constant
    Name get_station_name(StationID id);

    // Estimate of performance: O(1)
    // Short rationale for estimate: find & access unordermap station id cost constant
    Coord get_station_coordinates(StationID id);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n)
    // Short rationale for estimate: loop through n element twice
    std::vector<StationID> stations_alphabetically();

    // Estimate of performance: O(n) 
    // Short rationale for estimate: loop through n element once
    std::vector<StationID> stations_distance_increasing();

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: find & access element in map cost log(n)
    StationID find_station_with_coord(Coord xy);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: the most affective one here is map erase
    bool change_station_coord(StationID id, Coord newcoord);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: find & push back cost constant
    bool add_departure(StationID stationid, TrainID trainid, Time time);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: find & erase in map cost log(n)
    bool remove_departure(StationID stationid, TrainID trainid, Time time);

    // Estimate of performance: O(n)
    // Short rationale for estimate: copy_if cost O(n)
    std::vector<std::pair<Time, TrainID>> station_departures_after(StationID stationid, Time time);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(1)
    // Short rationale for estimate: find & assign in unordermap & vector is constant
    bool add_region(RegionID id, Name const &name, std::vector<Coord> coords);

    // Estimate of performance: O(1)
    // Short rationale for estimate: return vector added before
    std::vector<RegionID> all_regions();

    // Estimate of performance: O(1)
    // Short rationale for estimate: find in unordermap cost constant 
    Name get_region_name(RegionID id);

    // Estimate of performance: O(1)
    // Short rationale for estimate: find & assign in unordermap cost constant
    std::vector<Coord> get_region_coords(RegionID id);

    // Estimate of performance: O(1)
    // Short rationale for estimate: find in unordermap cost constant
    bool add_subregion_to_region(RegionID id, RegionID parentid);

    // Estimate of performance: O(1)
    // Short rationale for estimate: find & access in unordermap cost constant
    bool add_station_to_region(StationID id, RegionID parentid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: depend on recursive
    std::vector<RegionID> station_in_regions(StationID id);

    // Non-compulsory operations

    // Estimate of performance: O(n)
    // Short rationale for estimate: it denpend on recursive times 
    std::vector<RegionID> all_subregions_of_region(RegionID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: loop through map once
    std::vector<StationID> stations_closest_to(Coord xy);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: erase map cost log(n)
    bool remove_station(StationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: depend on recusive times 
    RegionID common_parent_of_regions(RegionID id1, RegionID id2);

private:
    Station station;
    std::map<Coord, StationID> station_coord_map;
    std::map<Name, StationID> station_name_map;

    Region region;
    std::vector<RegionID> region_id_vec;

    void recursive_add_subregion(std::vector<RegionID> &subregion_Id, std::vector<RegionID>&r);
    void recursive_add_parent_region(RegionID &parent_Id, std::vector<RegionID>&r);

    bool cache_station_distance_increasing = false;
    bool cache_station_alphabeltically = false;

    std::vector<StationID> result_distance_increasing;
    std::vector<StationID> result_alphabeltically;
    void find_all_parent_region(RegionID id, std::unordered_set<RegionID> &r);

};

#endif // DATASTRUCTURES_HH
