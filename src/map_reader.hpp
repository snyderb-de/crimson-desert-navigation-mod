#pragma once
#include <cstdint>
#include <cstddef>
#include <cmath>
#include <cstring>
#include "game_build.hpp"
namespace crimson_nav {
struct MapState { float xyz[3]{}; std::uint32_t distance{}; bool destination_candidate{}, map_hidden_candidate{},destination_ui_valid{},distance_valid{}; };
template<class Memory> bool read_map(Memory &m, std::uintptr_t base, std::uintptr_t root, MapState &out,const GameBuild &build=game_build_2760) {
    out={}; MapState next{};
    auto get=[&](std::uintptr_t a,auto &v){return a>=0x10000 && a<0x7fffffff0000-sizeof(v) && m.read(a,&v,sizeof(v));};
    std::uintptr_t vt{},body{},canvas{},cv{};
    if(!get(root,vt)||vt!=base+build.minimap_root || !get(root+8,body)||!get(root+0x448,canvas)||!get(canvas,cv)||cv!=base+build.minimap_canvas)return false;
    auto named=[&](std::uintptr_t e,const char *expected){
        std::uintptr_t ref{},text{};std::uint32_t length{};char name[64]{};
        return get(e+0x60,ref)&&get(ref,text)&&get(ref+8,length)&&length==std::strlen(expected)&&length<sizeof(name)&&
            text>=0x10000&&text<0x7fffffff0000ull-length&&m.read(text,name,length)&&std::memcmp(name,expected,length)==0;
    };
    if(!named(body,"MinimapHudBody"))return false;
    std::uintptr_t view{};std::uint8_t view_flags{};
    // Child layout state can remain visible when its parent view is hidden.
    if(!get(body+0x38,view)||!named(view,"MinimapView")||!get(view+0xb0,view_flags))return false;
    for(unsigned i=0;i<3;++i)if(!get(root+0x330+4*i,next.xyz[i])||!std::isfinite(next.xyz[i])||std::abs(next.xyz[i])>1.e7f)return false;
    // Destination label and distance are optional corroborating observations.
    // Their animation/lifetime must not make the whole map unreadable.
    std::uintptr_t icon{},ip{},link{};std::uint8_t active{},active_again{};
    next.destination_ui_valid=get(root+0x418,icon)&&named(icon,"DestinationDistanceIcon")&&
        get(icon+0x18,ip)&&get(ip+0x11,active)&&active<=1&&
        get(root+0x418,link)&&link==icon&&get(icon+0x18,link)&&link==ip&&
        get(ip+0x11,active_again)&&active_again==active;
    next.destination_candidate=next.destination_ui_valid&&active!=0;
    std::uint32_t distance_again{};
    next.distance_valid=get(root+0x558,next.distance)&&next.distance<=10000000&&
        get(root+0x558,distance_again)&&distance_again==next.distance;
    if(!next.distance_valid)next.distance=0;
    // Re-read identity links and values to reject a replaced or transitioning snapshot.
    std::uintptr_t again{};std::uint8_t flag{};
    if(!get(root,again)||again!=vt||!get(root+8,again)||again!=body||
       !get(root+0x448,again)||again!=canvas||!get(body+0x38,again)||again!=view||
       !get(view+0xb0,flag)||flag!=view_flags)return false;
    next.map_hidden_candidate=(view_flags&0x40)==0;out=next;return true;
}
}
