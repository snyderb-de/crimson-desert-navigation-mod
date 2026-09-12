// Development-only probe; extracted research data is not a release dependency.
#include "road_route.hpp"
#include <fstream>
#include <iostream>
#include <cstdint>
#include <cstring>
#include <string>
int main(int argc,char **argv) {
    if(argc!=6)return 2;
    std::ifstream f(argv[1],std::ios::binary);char magic[8]{};std::uint32_t n{},m{};
    f.read(magic,8);f.read(reinterpret_cast<char*>(&n),4);f.read(reinterpret_cast<char*>(&m),4);
    if(!f||std::memcmp(magic,"CNROAD01",8)||n>10000000||m>20000000)return 3;
    crimson_nav::RoadGraph g;g.points.reserve(n);g.edges.reserve(m);
    for(std::uint32_t i=0;i<n;++i){float p[3]{};f.read(reinterpret_cast<char*>(p),12);g.points.push_back({p[0],p[1],p[2]});}
    for(std::uint32_t i=0;i<m;++i){std::uint32_t e[2]{};f.read(reinterpret_cast<char*>(e),8);g.edges.push_back({e[0],e[1]});}
    if(!f||f.peek()!=std::char_traits<char>::eof())return 4;
    auto r=crimson_nav::road_route(g,{std::stod(argv[2]),0,std::stod(argv[3])},{std::stod(argv[4]),0,std::stod(argv[5])});
    std::cout<<"Graph: "<<n<<" nodes, "<<m<<" segments. Offline route valid="<<r.valid<<" points="<<r.points.size()<<" geometric length="<<r.length<<"\n";
    if(r.valid)std::cout<<"Start "<<r.points.front().x<<","<<r.points.front().z<<" end "<<r.points.back().x<<","<<r.points.back().z<<"\n";
    return r.valid?0:5;
}
