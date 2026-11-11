#pragma once
#include <cmath>
#include <cstdint>
#include <cstring>
#include <initializer_list>
#include "game_build.hpp"
#include "minimap_layout.hpp"
namespace crimson_nav {
struct MinimapGeometry {float canvas_width{},canvas_height{},x{},y{},width{},height{};bool valid=false;};
struct ScreenCircle {float x{},y{},radius{};bool valid=false;};
inline ScreenCircle project_minimap(const MinimapGeometry &g,float screen_width,float screen_height){
    for(float v:{g.canvas_width,g.canvas_height,g.x,g.y,g.width,g.height,screen_width,screen_height})if(!std::isfinite(v))return {};
    if(!g.valid||g.canvas_width<=0||g.canvas_height<=0||screen_width<=0||screen_height<=0||g.width<8||g.height<8||
       g.x<0||g.y<0||g.x+g.width>g.canvas_width||g.y+g.height>g.canvas_height||std::abs(g.width-g.height)>1.f)return {};
    const float sx=screen_width/g.canvas_width,sy=screen_height/g.canvas_height;
    // Canvas comes from this game state, not a fixed 1080p reference. Reject stale/aspect-mismatched layouts.
    if(std::abs(sx-sy)>0.005f*sy)return {};
    return {(g.x+g.width*.5f)*sx,(g.y+g.height*.5f)*sy,g.width*.5f*sx,true};
}
template<class Memory> MinimapGeometry read_minimap_geometry(Memory &m,std::uintptr_t base,std::uintptr_t root,const GameBuild &build=game_build_2760,
    const MinimapLayout &layout=minimap_layout_legacy){
    auto get=[&](std::uintptr_t a,auto &v){return a>=0x10000&&a<0x7fffffff0000ull-sizeof(v)&&m.read(a,&v,sizeof(v));};
    std::uintptr_t vt{},body{},hud{};
    if(!get(root,vt)||vt!=base+build.minimap_root||!get(root+8,body)||!get(root+layout.hud,hud))return {};
    auto named=[&](std::uintptr_t node,const char *expected){std::uintptr_t ref{},text{};std::uint32_t size{};char name[64]{};
        return get(node+0x60,ref)&&get(ref,text)&&get(ref+8,size)&&size==std::strlen(expected)&&size<64&&text>=0x10000&&text<0x7fffffff0000ull-size&&m.read(text,name,size)&&!std::memcmp(name,expected,size);};
    if(!named(body,"MinimapHudBody")||!named(hud,layout.hud_name))return {};
    float canvas[4]{},rect[4]{};
    if(!get(body+0x70,canvas)||!get(hud+0x70,rect))return {};
    for(float v:canvas)if(!std::isfinite(v))return {};
    for(float v:rect)if(!std::isfinite(v))return {};
    if(canvas[0]!=0||canvas[1]!=0)return {};
    std::uintptr_t again{};
    if(!get(root+8,again)||again!=body||!get(root+layout.hud,again)||again!=hud)return {};
    MinimapGeometry out{canvas[2],canvas[3],rect[0],rect[1],rect[2],rect[3],true};
    return out;
}
}
