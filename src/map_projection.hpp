#pragma once
#include "map_reader.hpp"
#include "minimap_geometry.hpp"
#include "worldmap_layout.hpp"
#include <array>
#include <vector>
namespace crimson_nav {
// Affine world-XZ -> map-canvas transform, with the dimensions of THAT canvas.
// The two maps need not use the same UI coordinate system.
struct MapProjection {double ax{},az{},bx{},bz{},tx{},ty{};bool valid=false;double canvas_width{},canvas_height{};std::uintptr_t identity_root{},identity_body{},identity_parent{};};
inline MapProjection map_to_display(MapProjection p,double width,double height){
    for(double v:{p.ax,p.az,p.bx,p.bz,p.tx,p.ty,p.canvas_width,p.canvas_height,width,height})
        if(!std::isfinite(v))return {};
    if(!p.valid||p.canvas_width<=0||p.canvas_height<=0||width<=0||height<=0||
       p.canvas_width>32768||p.canvas_height>32768||width>32768||height>32768)return {};
    const double sx=width/p.canvas_width,sy=height/p.canvas_height;
    // Matching aspects support any resolution without presets. A mismatched
    // aspect may be a stale layout or letterboxing; no viewport offset is known.
    if(std::abs(sx-sy)>.005*sy)return {};
    p.ax*=sx;p.az*=sx;p.tx*=sx;p.bx*=sy;p.bz*=sy;p.ty*=sy;
    p.canvas_width=width;p.canvas_height=height;return p;
}
// UISDF canvas uses a 256-world-unit tile. Its final UI transform contains
// rotation, both zoom stages and tile translation. Do not substitute the icon
// matrix or current player position for the canvas's cached world origin.
template<class Memory> MapProjection read_minimap_projection(Memory &m,std::uintptr_t root,const MapState &,const MinimapGeometry &g){
    std::uintptr_t canvas{},matrix{},again{};float v[16]{},origin[3]{},check[3]{},verify[16]{};
    if(!g.valid||!std::isfinite(g.width)||!std::isfinite(g.height)||g.width<8||g.height<8)return {};
    if(!m.read(root+0x3f8,&canvas,8)||!m.read(canvas+0x50,&matrix,8)||
       !m.read(root+0x4bc,origin,sizeof(origin))||!m.read(matrix,v,sizeof(v)))return {};
    for(float f:v)if(!std::isfinite(f))return {};
    for(float f:origin)if(!std::isfinite(f)||std::abs(f)>1.e7f)return {};
    const double mx=std::hypot(v[0],v[1]),my=std::hypot(v[4],v[5]);
    if(mx<.01||mx>100||my<.01||my>100||std::abs(mx-my)>.001*mx)return {};
    if(!m.read(root+0x3f8,&again,8)||again!=canvas||!m.read(canvas+0x50,&again,8)||again!=matrix||
       !m.read(root+0x4bc,check,sizeof(check))||std::memcmp(origin,check,sizeof(origin))||
       !m.read(matrix,verify,sizeof(verify))||std::memcmp(v,verify,sizeof(v)))return {};
    const double cx=g.x+g.width/2,cy=g.y+g.height/2,sx=g.width/256.,sy=g.height/256.;
    MapProjection p{sx*v[0],-sy*v[4],sx*v[1],-sy*v[5],0,0,true,g.canvas_width,g.canvas_height};
    p.tx=cx*v[0]+cy*v[4]+v[12]-p.ax*origin[0]-p.az*origin[2];
    p.ty=cx*v[1]+cy*v[5]+v[13]-p.bx*origin[0]-p.bz*origin[2];return p;
}
// The map stores a cursor UI/world pair and world-units-per-UI-pixel together.
// This is independent of culled map icons, whose positions can remain stale.
template<class Memory> MapProjection read_worldmap_projection(Memory &m,std::uintptr_t base,std::uintptr_t root,const GameBuild &build=game_build_2760){
    std::uintptr_t vt{},body{},parent{};unsigned char flags{};
    if(!root||!m.read(root,&vt,8)||vt!=base+build.worldmap_root||!m.read(root+8,&body,8)||
       !m.read(body+0x38,&parent,8)||!m.read(parent+0xb0,&flags,1)||(flags&0x40)==0)return {};
    float canvas[2]{},anchor[5]{},again[5]{},canvas_again[2]{};
    std::size_t block_offset{};unsigned matches=0;
    // This small block moved by 0x10 in patch 2.03. Locate it by its paired
    // canvas and cursor/world transform instead of treating its old offset as
    // a game-build identity. Ambiguous or implausible memory remains rejected.
    for(std::size_t offset=0xa40;offset<=0xac0;offset+=4){
        float candidate_canvas[2]{},candidate_anchor[5]{};
        if(!m.read(root+offset,candidate_canvas,sizeof(candidate_canvas))||
           !m.read(root+offset+0x20,candidate_anchor,sizeof(candidate_anchor)))continue;
        bool valid=true;
        for(float f:candidate_canvas)if(!std::isfinite(f)||f<=8||f>32768)valid=false;
        for(float f:candidate_anchor)if(!std::isfinite(f))valid=false;
        // This is a centered reference canvas, not the visible body's bounds.
        // Wider bodies legitimately store negative / beyond-reference cursor values.
        if(std::abs(candidate_anchor[0])>32768||std::abs(candidate_anchor[1])>32768||
           std::abs(candidate_anchor[2])>100000||std::abs(candidate_anchor[3])>100000||
           candidate_anchor[4]<.01||candidate_anchor[4]>1000)valid=false;
        if(valid){
            ++matches;block_offset=offset;
            std::memcpy(canvas,candidate_canvas,sizeof(canvas));
            std::memcpy(anchor,candidate_anchor,sizeof(anchor));
        }
    }
    if(matches!=1)return {};
    std::uintptr_t link{};unsigned char flags_again{};
    if(!m.read(root+block_offset,canvas_again,sizeof(canvas_again))||std::memcmp(canvas,canvas_again,sizeof(canvas))||
       !m.read(root+block_offset+0x20,again,sizeof(again))||std::memcmp(anchor,again,sizeof(anchor))||
       !m.read(root,&link,8)||link!=vt||!m.read(root+8,&link,8)||link!=body||
       !m.read(body+0x38,&link,8)||link!=parent||!m.read(parent+0xb0,&flags_again,1)||flags_again!=flags)return {};
    const double scale=1./anchor[4];
    return {scale,0,0,-scale,anchor[0]-scale*anchor[2],anchor[1]+scale*anchor[3],true,canvas[0],canvas[1],root,body,parent};
}
template<class Memory> MapProjection read_worldmap_display_projection(Memory &m,std::uintptr_t base,
    std::uintptr_t root,double width,double height,const GameBuild &build){
    std::uintptr_t body{};
    if(!root||!m.read(root+8,&body,8))return {};
    const auto layout=read_worldmap_layout(m,body);
    auto p=read_worldmap_projection(m,base,root,build);
    if(!layout.valid||!p.valid||p.identity_body!=body||!(layout==read_worldmap_layout(m,body)))return {};
    // Native world-to-UI code centers on the actual logical body size.
    // Our cursor pair uses the centered reference at root+a58 instead.
    // Convert through the game's uniform scale and measured physical body.
    p.tx=p.tx*layout.scale+(layout.rect[2]-p.canvas_width*layout.scale)/2;
    p.ty=p.ty*layout.scale+(layout.rect[3]-p.canvas_height*layout.scale)/2;
    p.ax*=layout.scale;p.az*=layout.scale;p.bx*=layout.scale;p.bz*=layout.scale;
    p.canvas_width=layout.rect[2];p.canvas_height=layout.rect[3];
    return map_to_display(p,width,height);
}
// Presentation boundary: select the map first, then scale its own coordinates.
// Full-map drawing must not depend on the hidden minimap's layout or clip circle.
template<class Memory> MapProjection read_route_display_projection(Memory &m,std::uintptr_t base,
    std::uintptr_t mini_root,std::uintptr_t world_root,const MapState &state,const MinimapGeometry &geometry,
    double width,double height,const GameBuild &build=game_build_2760){
    if(state.map_hidden_candidate)return read_worldmap_display_projection(m,base,world_root,width,height,build);
    return map_to_display(read_minimap_projection(m,mini_root,state,geometry),width,height);
}
}
