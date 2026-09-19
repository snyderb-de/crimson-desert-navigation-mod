#pragma once
#include "road_route.hpp"
#include <cstdint>
#include <cstring>
#include <string>
#include <stdexcept>
#include <unordered_map>
#include <fstream>
#include <filesystem>
namespace crimson_nav {
// Supported build's uncompressed fielddefault.road begins at archive offset zero.
// Reads original data in place. No extracted asset/cache is shipped or written.
inline RoadGraph load_worldmap_roads(const std::filesystem::path &game) {
    std::ifstream input(game/"0008"/"1.paz",std::ios::binary);
    std::vector<unsigned char> bytes(3207193);input.read(reinterpret_cast<char*>(bytes.data()),bytes.size());
    if(!input)throw std::runtime_error("Road archive unavailable");
    size_t p=0;
    auto skip=[&](size_t n){if(n>bytes.size()-p)throw std::runtime_error("Invalid road bounds");p+=n;};
    auto u=[&](){if(bytes.size()-p<4)throw std::runtime_error("Truncated road data");std::uint32_t v;std::memcpy(&v,bytes.data()+p,4);p+=4;return v;};
    auto number=[&](){auto bits=u();float f;std::memcpy(&f,&bits,4);if(!std::isfinite(f))throw std::runtime_error("Invalid road coordinate");return f;};
    auto count=[&](std::uint32_t max){auto n=u();if(n>max)throw std::runtime_error("Invalid road count");return n;};
    auto text=[&](){auto n=count(64);size_t start=p;skip(n);return std::string(reinterpret_cast<char*>(bytes.data()+start),n);};
    if(u()!=46)throw std::runtime_error("Unsupported road version");skip(16);
    const auto layers=u();
    if(layers<3||layers>64||text()!="worldmaproad")throw std::runtime_error("Unexpected road layer");
    auto links=count(100000);
    for(std::uint32_t i=0;i<links;++i){u();for(int j=0;j<2;++j){auto n=count(10000);skip(size_t(n)*4);}skip(1);}
    struct Chain {std::vector<std::uint32_t> ids;bool conditional;};
    std::vector<Chain> chains;auto edge_count=count(100000);
    for(std::uint32_t i=0;i<edge_count;++i){
        u();number();number();skip(14);auto a=u(),b=u();auto n=count(10000);
        Chain c;c.ids.push_back(a);for(std::uint32_t j=0;j<n;++j)c.ids.push_back(u());c.ids.push_back(b);
        u();if(p==bytes.size())throw std::runtime_error("Truncated road condition");auto conditions=bytes[p++];
        if(conditions>1)throw std::runtime_error("Unknown road condition format");skip(size_t(conditions)*33);c.conditional=conditions!=0;
        chains.push_back(std::move(c));
    }
    RoadGraph g;std::unordered_map<std::uint32_t,size_t> ids;auto nodes=count(100000);
    for(std::uint32_t i=0;i<nodes;++i){auto id=u();RoadPoint q{number(),number(),number()};number();u();auto n=count(10000);skip(size_t(n)*4);
        if(std::abs(q.x)>100000||std::abs(q.y)>100000||std::abs(q.z)>100000||!ids.emplace(id,g.points.size()).second)throw std::runtime_error("Invalid road node");
        g.points.push_back(q);
    }
    if(text()!="wagon")throw std::runtime_error("Road section boundary mismatch");
    for(const auto &c:chains){for(auto id:c.ids)if(!ids.count(id))throw std::runtime_error("Missing road endpoint");
        if(c.conditional)continue;
        for(size_t i=1;i<c.ids.size();++i){auto a=ids.at(c.ids[i-1]),b=ids.at(c.ids[i]);if(a!=b)g.edges.push_back({a,b});}
    }
    return g;
}
}
