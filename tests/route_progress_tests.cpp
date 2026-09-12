#include "route_progress.hpp"
#include <cassert>
#include <iostream>
using namespace crimson_nav;
int main(){
 RoadRoute r{true,20,{{0,0,0},{10,0,0},{10,0,10}}};
 auto a=route_progress(r,{4,0,0});assert(a.valid&&a.next==1&&a.point.x==4&&a.distance==4);
 auto b=route_progress(r,{10,0,6});assert(b.valid&&b.next==2&&b.point.z==6&&b.distance==16);
 auto c=route_progress(r,{13,0,6});assert(c.valid&&c.point.x==10&&c.point.z==6&&c.distance==16);
 auto end=route_progress(r,{10,0,12});assert(end.point.z==10&&end.distance==20);
 auto back=route_progress(r,{2,0,0});assert(back.distance==2);
 assert(!route_progress(RoadRoute{},{}).valid);
 std::cout<<"PASS: progress at road start, bend, off road, destination, backtracking, empty route\n";
}
