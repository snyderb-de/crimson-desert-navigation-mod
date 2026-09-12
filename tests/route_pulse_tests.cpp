#include "route_pulse.hpp"
#include <cassert>
#include <vector>
#include <array>
#include <iostream>
int main(){
 std::vector<std::array<double,3>> spans;
 auto add=[&](double a,double b,double opacity){assert(a>=0&&b<=1&&a<b&&opacity>0&&opacity<=1);spans.push_back({a,b,opacity});};
 crimson_nav::route_pulse(0,18,0,add);assert(spans.size()==4&&spans.front()[0]==0&&spans.back()[1]==1./3);
 spans.clear();crimson_nav::route_pulse(0,18,.5,add);assert(spans.size()==4&&spans.front()[0]>0);
 spans.clear();crimson_nav::route_pulse(2,4,0,add);assert(!spans.empty()&&spans.front()[0]==0&&spans.back()[1]==1);
 spans.clear();crimson_nav::route_pulse(3,3,0,add);assert(spans.empty());
 std::cout<<"PASS: pulse direction, wrap period, clipped continuity and empty segment\n";
}
