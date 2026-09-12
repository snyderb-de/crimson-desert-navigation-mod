#pragma once
#include <vector>
#include <functional>
#include <cstring>
#include <cstdint>
#include <string>
namespace map_fixture {
constexpr std::uintptr_t root=0x10000,mini_body_address=0x20000,mini_view_address=0x21000,
    distance_icon=0x22000,mini_canvas_address=0x23000,property=0x24000,
    target_icon=0x30000,retained_icon=0x31000,terrain=0x32000,terrain_matrix_address=0x33000,
    world=0x40000,world_body=0x42000,world_view=0x43000,
    replacement_world=0x50000;
struct Memory {
    std::vector<unsigned char> bytes=std::vector<unsigned char>(0x100000);
    std::function<void(std::uintptr_t,std::size_t)> before_read;
    bool read(std::uintptr_t a,void *out,std::size_t n){
        if(a>bytes.size()||n>bytes.size()-a)return false;
        if(before_read)before_read(a,n);
        std::memcpy(out,bytes.data()+a,n);return true;
    }
    template<class T>void put(std::uintptr_t a,const T &value){std::memcpy(bytes.data()+a,&value,sizeof(value));}
    void name(std::uintptr_t a,const std::string &value){
        put(a+0x60,a+0x400);put(a+0x400,a+0x500);put(a+0x408,std::uint32_t(value.size()));
        std::memcpy(bytes.data()+a+0x500,value.data(),value.size());
    }
};
Memory fixture(){
    Memory m;
    m.put(root,std::uintptr_t(89513840));m.put(root+8,mini_body_address);
    m.put(root+0x418,distance_icon);m.put(root+0x448,mini_canvas_address);m.put(mini_canvas_address,std::uintptr_t(89515328));
    m.name(mini_body_address,"MinimapHudBody");m.name(mini_view_address,"MinimapView");m.name(distance_icon,"DestinationDistanceIcon");
    m.put(mini_body_address+0x38,mini_view_address);m.put(mini_view_address+0xb0,std::uint8_t(0xd0));
    m.put(distance_icon+0x18,property);m.put(property+0x11,std::uint8_t(1));
    constexpr std::uintptr_t hud_address=0x25000;
    m.put(root+0x380,hud_address);m.name(hud_address,"MinimapHud");
    float canvas_rect[4]={0,0,1920,1080},hud_rect[4]={50,845,200,200};
    m.put(mini_body_address+0x70,canvas_rect);m.put(hud_address+0x70,hud_rect);
    float player[3]={0,0,0};m.put(root+0x330,player);m.put(root+0x558,std::uint32_t(100));
    for(auto a:{target_icon,retained_icon}){
        m.put(a,std::uintptr_t(89518944));m.put(a+0xd0,std::uint32_t(21));
        m.put(a+0xde,std::uint16_t(a==target_icon?3:1));
        float xyz[3]={100,0,0};m.put(a+0xe0,xyz);m.put(a+0x110,root);
    }
    for(unsigned c=0;c<2;++c){
        auto header=root+0x238+c*16,rows=root+0x1000+c*0x1000,list=rows+0x100;
        m.put(header,rows);m.put(header+8,std::uint32_t(2));m.put(header+12,std::uint32_t(2));
        m.put(rows+8,std::uint64_t(21));m.put(rows+16,list);m.put(rows+24,std::uint32_t(2));m.put(rows+28,std::uint32_t(2));
        m.put(list,retained_icon);m.put(list+8,target_icon);
        m.put(rows+32+8,std::uint64_t(99)); // Unrelated icon category; no target data.
    }
    m.put(root+0x3f8,terrain);m.put(terrain+0x50,terrain_matrix_address);
    float identity[16]={1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};m.put(terrain_matrix_address,identity);
    float origin[3]={0,0,0};m.put(root+0x4bc,origin);
    m.put(world,std::uintptr_t(89508976));m.put(world+8,world_body);m.put(world_body+0x38,world_view);
    m.put(world_view+0xb0,std::uint8_t(0x90));
    float size[2]={1920,1080},anchor[5]={960,540,0,0,2};m.put(world+0xa58,size);m.put(world+0xa78,anchor);
    return m;
}
}
