#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#include "game_layout_detection.hpp"
#include "map_reader.hpp"
#include "map_acquisition.hpp"
#include <atomic>
#include <array>
#include <chrono>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
namespace crimson_nav {
struct ProcessMemory {
    HANDLE process;
    bool read(std::uintptr_t address,void *out,std::size_t bytes) const {
        SIZE_T got=0;
        return bytes<=65536 && address>=0x10000 && address<0x7fffffff0000-bytes &&
            ReadProcessMemory(process,reinterpret_cast<void*>(address),out,bytes,&got)&&got==bytes;
    }
};
inline Discovery discover_map(ProcessMemory &memory,std::uintptr_t base,const std::atomic<bool> &stop,const GameBuild &build=game_build_2760) {
    Discovery result{};std::array<unsigned char,65536> buffer{};std::uintptr_t address=0;
    const auto deadline=std::chrono::steady_clock::now()+std::chrono::seconds(90);
    while(address<0x7fffffff0000&&!stop.load()) {
        if(result.bytes>=8ull*1024*1024*1024||std::chrono::steady_clock::now()>deadline)return result;
        MEMORY_BASIC_INFORMATION region{};
        if(!VirtualQueryEx(memory.process,reinterpret_cast<void*>(address),&region,sizeof(region))) {result.complete=true;break;}
        auto low=reinterpret_cast<std::uintptr_t>(region.BaseAddress);auto end=low+region.RegionSize;
        if(end<=address)return result;
        if(region.State==MEM_COMMIT&&region.Type==MEM_PRIVATE&&
           (region.Protect==PAGE_READWRITE||region.Protect==PAGE_WRITECOPY||region.Protect==PAGE_EXECUTE_READWRITE||region.Protect==PAGE_EXECUTE_WRITECOPY)) {
            for(auto pos=low;pos<end&&!stop.load();pos+=buffer.size()) {
                if(result.bytes>=8ull*1024*1024*1024||std::chrono::steady_clock::now()>deadline)return result;
                auto n=static_cast<std::size_t>((std::min)(end-pos,std::uintptr_t(buffer.size())));result.bytes+=n;
                if(!memory.read(pos,buffer.data(),n))continue;
                for(std::size_t i=0;i+8<=n;i+=8) {
                    std::uintptr_t value{};std::memcpy(&value,buffer.data()+i,8);
                    if(value==base+build.worldmap_root){
                        std::uintptr_t body{},ref{},text{};std::uint32_t length{};char name[12]{};
                        if(memory.read(pos+i+8,&body,8)&&memory.read(body+0x60,&ref,8)&&memory.read(ref,&text,8)&&memory.read(ref+8,&length,4)&&length==12&&memory.read(text,name,12)&&!std::memcmp(name,"WorldMapBody",12)){++result.world_candidates;result.world_root=pos+i;}
                    }
                    if(value!=base+build.minimap_root)continue;
                    MapState state{};const auto layout=select_minimap_layout(memory,base,pos+i,build);
                    if(layout&&read_map(memory,base,pos+i,state,build,*layout)) {++result.candidates;result.root=pos+i;}
                }
                // Yield periodically; scanner never runs on the render thread.
                if((result.bytes%(8*1024*1024))==0)Sleep(1);
            }
        }
        address=end;
    }
    if(address>=0x7fffffff0000)result.complete=true;
    if(stop.load())result.complete=false;
    if(result.candidates!=1||!result.complete)result.root=0;
    if(result.world_candidates!=1||!result.complete)result.world_root=0;
    return result;
}
}
