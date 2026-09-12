#include "route_session.hpp"
#include <cassert>
#include <iostream>
using namespace crimson_nav;
int main(){
 RoadGraph graph{{{0,0,0},{10,0,0},{10,0,10},{0,0,10}},{{0,1},{1,2},{2,3}}};
 RouteSession session;auto initial=session.update(graph,{2,0,0},{2,0,10});assert(initial->valid&&initial->length==26);
 auto moved=session.update(graph,{10,0,9},{2,0,10});assert(moved==initial&&moved->length==26);
 auto changed=session.update(graph,{10,0,9},{5,0,10});assert(changed!=initial&&changed->length==6);
 auto held=session.update(graph,{10,0,5},{5,0,10});assert(held==changed&&held->length==6);
 session.clear();auto reset=session.update(graph,{10,0,9},{5,0,10});assert(reset!=held&&reset->length==6);
 RoadGraph fork{{{0,0,0},{100,0,0},{100,0,100},{0,0,100}},{{0,1},{1,2},{2,3},{3,0}}};
 RouteSession detour;auto plan=detour.update(fork,{10,0,0},{100,0,90},100);
 assert(detour.update(fork,{0,0,60},{100,0,90},6000)==plan);
 assert(detour.update(fork,{0,0,70},{100,0,90},7999)==plan);
 auto replanned=detour.update(fork,{0,0,70},{100,0,90},8000);
 assert(replanned!=plan&&replanned->valid);
 assert(detour.update(fork,{0,0,72},{100,0,90},9000)==replanned);
 std::cout<<"5 destination session and 4 off-course cases passed\n";
}
