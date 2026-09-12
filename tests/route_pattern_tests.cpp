#include "route_pattern.hpp"
#include <vector>
#include <utility>
#include <limits>
#include <iostream>
int main(){
    int failures=0;
    auto check=[&](bool v){if(!v)++failures;};
    std::vector<std::pair<double,double>> spans;
    auto collect=[&](double start,double end,bool dots){crimson_nav::route_pattern(start,end,dots,[&](double a,double b){spans.emplace_back(start+a*(end-start),start+b*(end-start));});};
    collect(0,24,false);check(spans.size()==2&&spans[0].second==7&&spans[1].first==12);
    spans.clear();collect(5,15,false);check(spans.size()==2&&spans[0].first==5&&spans[0].second==7&&spans[1].first==12);
    spans.clear();collect(0,10,true);collect(10,20,true);check(spans.size()==4&&spans[2].first==10&&spans[3].first==15);
    spans.clear();collect(6,14,true);check(spans.size()==1&&spans[0].first==10);
    spans.clear();collect(1,1,false);collect(-1,20,true);collect(0,std::numeric_limits<double>::infinity(),false);check(spans.empty());
    if(failures){std::cerr<<failures<<" pattern failures\n";return 1;}
    std::cout<<"PASS: 5 route pattern cases\n";
}
