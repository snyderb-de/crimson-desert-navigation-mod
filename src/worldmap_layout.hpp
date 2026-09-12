#pragma once
#include <array>
#include <cmath>
#include <cstdint>
namespace crimson_nav {
// Physical full-map body and its native UI group. The game calculates logical
// body size by dividing the physical extent by this group's uniform scale.
struct WorldMapLayout {
    std::array<std::uintptr_t,8> links{};
    std::array<float,4> rect{},scales{};
    std::array<unsigned char,2> flags{};
    unsigned char group_index{};
    std::uint32_t count{};
    double scale=1;
    bool valid=false;
    bool operator==(const WorldMapLayout &other)const{
        return valid&&other.valid&&links==other.links&&rect==other.rect&&scales==other.scales&&
            flags==other.flags&&group_index==other.group_index&&count==other.count&&scale==other.scale;
    }
};
template<class Memory> WorldMapLayout read_worldmap_layout(Memory &m,std::uintptr_t body){
    WorldMapLayout l;
    // A transformed/subpanel body requires a different viewport mapping.
    if(!body||!m.read(body+0x50,&l.links[0],8)||l.links[0]||
       !m.read(body+0x70,l.rect.data(),sizeof(l.rect)))return {};
    for(float f:l.rect)if(!std::isfinite(f))return {};
    if(l.rect[0]!=0||l.rect[1]!=0||l.rect[2]<8||l.rect[3]<8||l.rect[2]>32768||l.rect[3]>32768)return {};
    // Named inherited groups are a separate native lookup; do not guess one.
    if(!m.read(body+0xa0,&l.links[1],8)||
       (l.links[1]&&(!m.read(l.links[1]+8,&l.links[2],8)||l.links[2]))||
       !m.read(body+0x94,&l.group_index,1))return {};
    if(l.group_index!=0xff){
        if(!m.read(body+0x58,&l.links[3],8)||!l.links[3]||
           !m.read(l.links[3]+0xb0,&l.links[4],8)||!l.links[4]||
           !m.read(l.links[4]+0xb0,&l.links[5],8)||!l.links[5]||
           !m.read(l.links[5],&l.links[6],8)||!l.links[6]||
           !m.read(l.links[5]+8,&l.count,4)||l.count>256||l.group_index>=l.count||
           !m.read(l.links[6]+8*l.group_index,&l.links[7],8)||!l.links[7]||
           !m.read(l.links[7]+0xd4,l.scales.data(),sizeof(l.scales))||
           !m.read(l.links[7]+0x13b,l.flags.data(),sizeof(l.flags)))return {};
        for(float f:l.scales)if(!std::isfinite(f)||f<=0||f>128)return {};
        l.scale=double(l.scales[0])*l.scales[l.flags[0]?(l.flags[1]?3:2):1];
    }
    if(!std::isfinite(l.scale)||l.scale<.01||l.scale>128)return {};
    l.valid=true;return l;
}
}
