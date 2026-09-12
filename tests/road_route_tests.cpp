#include "road_route.hpp"
#include <cassert>
#include <iostream>
using namespace crimson_nav;
int main() {
    RoadGraph g{{{0,0,0},{10,0,0},{10,0,10},{0,0,10}},{{0,1},{1,2},{2,3}}};
    auto r=road_route(g,{2,0,-3},{7,0,14});
    assert(r.valid && r.points.front().x==2 && r.points.back().x==7 && r.points.back().z==10);
    assert(std::abs(r.length-21)<1e-8); // Must follow the U, never shortcut across its gap.
    r=road_route(g,{2,0,1},{8,0,-2});
    assert(r.valid && std::abs(r.length-6)<1e-8 && r.points.size()==2);
    r=road_route(g,{8,0,1},{2,0,-2});
    assert(r.valid && std::abs(r.length-6)<1e-8);
    RoadGraph disconnected{{{0,0,0},{10,0,0},{0,0,10},{10,0,10}},{{0,1},{2,3}}};
    assert(!road_route(disconnected,{1,0,0},{1,0,10}).valid);
    assert(!road_route(RoadGraph{}, {}, {}).valid);
    RoadGraph broken=g;broken.edges.push_back({0,99});
    assert(!road_route(broken,{},{}).valid);
    auto bad=RoadPoint{std::numeric_limits<double>::quiet_NaN(),0,0};
    assert(!road_route(g,bad,{}).valid);
    r=road_route(g,{2,0,0},{2,0,0});assert(r.valid && r.length==0);
    std::cout<<"8 road route cases passed\n";
}
