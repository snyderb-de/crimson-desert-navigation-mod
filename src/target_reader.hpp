#pragma once
#include <cstdint>
#include <vector>
#include <array>
#include <cstring>
#include <cmath>
#include <algorithm>
#include "game_build.hpp"
namespace crimson_nav {
enum class TargetStatus { unavailable, no_candidate, candidate, ambiguous };
struct TargetState { TargetStatus status=TargetStatus::unavailable; std::uintptr_t address{};float xyz[3]{}; };
template<class Memory> TargetState read_target(Memory &m,std::uintptr_t base,std::uintptr_t root,const GameBuild &build=game_build_2760){
    struct Block {std::uintptr_t address;std::vector<unsigned char> bytes;};
    std::vector<Block> evidence;
    auto read=[&](std::uintptr_t a,std::size_t n,std::vector<unsigned char> &b){
        if(!n){b.clear();return true;}
        if(n>4096||a<0x10000||a>=0x7fffffff0000ull-n||evidence.size()>=4096)return false;
        b.resize(n);if(!m.read(a,b.data(),n))return false;evidence.push_back({a,b});return true;
    };
    auto u64=[](const auto &b,std::size_t off){std::uint64_t v{};std::memcpy(&v,b.data()+off,8);return v;};
    auto u32=[](const auto &b,std::size_t off){std::uint32_t v{};std::memcpy(&v,b.data()+off,4);return v;};
    std::vector<unsigned char> type;
    if(!read(root,8,type)||u64(type,0)!=base+build.minimap_root)return {};
    std::array<std::vector<TargetState>,2> candidates;unsigned total_icons=0;
    for(unsigned c=0;c<2;++c){
        std::vector<unsigned char> header;
        if(!read(root+0x238+16*c,16,header))return {};
        auto count=u32(header,8),capacity=u32(header,12);
        if(count>capacity||capacity>128)return {};
        auto rows=u64(header,0);
        if(count&&(rows<0x10000||rows>=0x7fffffff0000ull-count*32))return {};
        for(unsigned i=0;i<count;++i){
            auto offset=rows+i*32;std::vector<unsigned char> tag,items;
            // Keep every row's tag stable, but ignore keys and other categories'
            // presentation data. Those do not select our destination.
            if(!read(offset+8,8,tag))return {};
            if(u64(tag,0)!=21)continue;
            if(!read(offset+16,16,items))return {};
            auto n=u32(items,8),cap=u32(items,12);
            if(n>cap||cap>128||total_icons+n>512)return {};
            total_icons+=n;std::vector<unsigned char> pointers;
            if(!read(u64(items,0),n*8,pointers))return {};
            for(unsigned j=0;j<n;++j){
                auto address=u64(pointers,j*8);std::vector<unsigned char> kind,owner,flags_bytes,xyz;
                if(address<0x10000||address>=0x7fffffff0000ull-0x120||
                   !read(address,8,type)||u64(type,0)!=base+build.minimap_icon||
                   !read(address+0xd0,4,kind)||u32(kind,0)!=21||
                   !read(address+0x110,8,owner)||u64(owner,0)!=root||!read(address+0xde,2,flags_bytes))return {};
                std::uint16_t flags{};std::memcpy(&flags,flags_bytes.data(),2);
                if(flags!=1&&flags!=3)return {}; // Other kinds have not been investigated.
                if(flags==1)continue;
                if(!read(address+0xe0,12,xyz))return {};
                TargetState target;target.status=TargetStatus::candidate;target.address=address;
                std::memcpy(target.xyz,xyz.data(),12);
                for(float v:target.xyz)if(!std::isfinite(v)||std::abs(v)>1.e7f)return {};
                if(std::any_of(candidates[c].begin(),candidates[c].end(),[&](const auto &v){return v.address==address;}))return {};
                candidates[c].push_back(target);
            }
        }
    }
    // Recheck semantic membership, identity, owner, flags and destination XYZ.
    // Unrelated animated icon bytes no longer invalidate navigation.
    for(const auto &block:evidence){std::vector<unsigned char> now(block.bytes.size());
        if(!m.read(block.address,now.data(),now.size())||now!=block.bytes)return {};
    }
    auto order=[](const auto &a,const auto &b){return a.address<b.address;};
    for(auto &v:candidates)std::sort(v.begin(),v.end(),order);
    if(candidates[0].size()!=candidates[1].size())return {};
    for(std::size_t i=0;i<candidates[0].size();++i)
        if(candidates[0][i].address!=candidates[1][i].address||std::memcmp(candidates[0][i].xyz,candidates[1][i].xyz,12))return {};
    if(candidates[0].empty()){TargetState result;result.status=TargetStatus::no_candidate;return result;}
    if(candidates[0].size()>1){TargetState result;result.status=TargetStatus::ambiguous;return result;}
    return candidates[0][0];
}
}
