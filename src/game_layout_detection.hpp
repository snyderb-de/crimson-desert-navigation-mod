#pragma once
#include "game_build.hpp"
#include <algorithm>
#include <array>
#include <atomic>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <optional>
#include <vector>
namespace crimson_nav {
namespace game_layout_detail {
template<class T> T number(const unsigned char *p){T v{};std::memcpy(&v,p,sizeof(v));return v;}
struct Section {
    std::uint32_t rva{},span{},raw{},size{},flags{};
    std::vector<unsigned char> data;
};
// Only read initialized, non-executable data. Never load or execute the PE.
// Bounds are resource limits, not game-version or hash allowlists.
class Image {
public:
    std::uint64_t base{};
    std::vector<Section> sections;
    bool load(const std::filesystem::path &path,const std::atomic<bool> &stop){
        if(stop.load())return false;
        std::ifstream file(path,std::ios::binary|std::ios::ate);
        if(!file)return false;
        const auto end=file.tellg();if(end<64||end>0xffffffffull)return false;
        const auto size=static_cast<std::uint64_t>(end);
        auto read=[&](std::uint64_t offset,void *out,std::size_t bytes){
            if(stop.load()||offset>size||bytes>size-offset)return false;
            file.seekg(static_cast<std::streamoff>(offset));file.read(static_cast<char*>(out),bytes);return bool(file);
        };
        std::array<unsigned char,264> h{};
        if(!read(0,h.data(),64)||number<std::uint16_t>(h.data())!=0x5a4d)return false;
        const auto pe=number<std::uint32_t>(h.data()+0x3c);
        if(pe<64||!read(pe,h.data(),h.size())||number<std::uint32_t>(h.data())!=0x4550||
            number<std::uint16_t>(h.data()+4)!=0x8664||number<std::uint16_t>(h.data()+24)!=0x20b)return false;
        const auto count=number<std::uint16_t>(h.data()+6),optional=number<std::uint16_t>(h.data()+20);
        const auto image_size=number<std::uint32_t>(h.data()+80),headers=number<std::uint32_t>(h.data()+84);
        base=number<std::uint64_t>(h.data()+48);
        const std::uint64_t table=std::uint64_t(pe)+24+optional,table_end=table+std::uint64_t(count)*40;
        if(!count||count>96||optional<240||base<0x10000||base>0x7fffffff0000ull-image_size||
            !image_size||headers>size||headers>image_size||table_end>headers)return false;
        std::uint64_t budget=0;
        for(unsigned i=0;i<count;++i){
            if(!read(table+i*40,h.data(),40))return false;
            Section s{};
            s.rva=number<std::uint32_t>(h.data()+12);s.raw=number<std::uint32_t>(h.data()+20);
            s.size=number<std::uint32_t>(h.data()+16);s.flags=number<std::uint32_t>(h.data()+36);
            s.span=(std::max)(s.size,number<std::uint32_t>(h.data()+8));
            if(s.rva<headers||std::uint64_t(s.rva)+s.span>image_size||
                (s.size&&(s.raw<headers||std::uint64_t(s.raw)+s.size>size)))return false;
            for(const auto &other:sections){
                if(s.span&&other.span&&s.rva<std::uint64_t(other.rva)+other.span&&other.rva<std::uint64_t(s.rva)+s.span)return false;
                if(s.size&&other.size&&s.raw<std::uint64_t(other.raw)+other.size&&other.raw<std::uint64_t(s.raw)+s.size)return false;
            }
            if((s.flags&0x40000000)&&!(s.flags&0x20000000)){
                budget+=s.size;if(budget>128ull*1024*1024)return false;
                s.data.resize(s.size);
                for(std::size_t p=0;p<s.data.size();p+=65536){
                    const auto n=(std::min)(std::size_t(65536),s.data.size()-p);
                    if(!read(std::uint64_t(s.raw)+p,s.data.data()+p,n))return false;
                }
            }
            sections.push_back(std::move(s));
        }
        return !stop.load();
    }
    const unsigned char *data(std::uint64_t rva,std::size_t n) const {
        for(const auto &s:sections)if(rva>=s.rva&&rva-s.rva<=s.data.size()&&n<=s.data.size()-(rva-s.rva))return s.data.data()+(rva-s.rva);
        return nullptr;
    }
    bool code(std::uint64_t va) const {
        if(va<base)return false;const auto rva=va-base;
        for(const auto &s:sections)if((s.flags&0x20000000)&&rva>=s.rva&&rva-s.rva<s.size)return true;
        return false;
    }
    bool hierarchy(std::uint32_t rva,std::uint32_t type) const {
        const auto h=data(rva,16);
        if(!h||number<std::uint32_t>(h)!=0||number<std::uint32_t>(h+4)>7)return false;
        const auto count=number<std::uint32_t>(h+8);
        if(!count||count>256)return false;
        const auto bases=data(number<std::uint32_t>(h+12),count*4);
        if(!bases)return false;
        const auto first=data(number<std::uint32_t>(bases),28);
        return first&&number<std::uint32_t>(first)==type&&number<std::uint32_t>(first+4)==count-1&&
            number<std::uint32_t>(first+8)==0&&number<std::uint32_t>(first+12)==0xffffffff&&
            number<std::uint32_t>(first+16)==0&&number<std::uint32_t>(first+24)==rva;
    }
};
}
// MSVC x64 RTTI identifies the four map classes even when a patch moves their
// code/data. These are identities only: all existing live object, name, pointer,
// collection and projection checks still have to pass before a route is drawn.
inline std::optional<GameBuild> identify_game_build(const std::filesystem::path &path,const std::atomic<bool> &stop){
    using namespace game_layout_detail;
    Image image;if(!image.load(path,stop))return std::nullopt;
    constexpr const char *names[]={".?AVUIGamePlayControlRootMiniMap@uiCommonScript@pa@@",
        ".?AVUIGamePlayControlRootWorldMap@uiCommonScript@pa@@",
        ".?AVUIGamePlayControlCommon_MiniMapIconCanvas@uiCommonScript@pa@@",
        ".?AVUIGamePlayControlCommon_MiniMapIcon@uiCommonScript@pa@@"};
    std::array<std::uint32_t,4> types{},locators{},tables{};
    for(const auto &s:image.sections)for(std::size_t p=16;p<s.data.size();++p){
        if((p&65535)==0&&stop.load())return std::nullopt;
        if(s.data[p]!='.'||((s.rva+p-16)&7))continue;
        for(std::size_t i=0;i<4;++i){
            const auto length=std::strlen(names[i])+1;
            if(length>s.data.size()-p||std::memcmp(s.data.data()+p,names[i],length))continue;
            const auto header=s.data.data()+p-16;const auto vt=number<std::uint64_t>(header);
            if(vt<image.base||!image.data(vt-image.base,8)||number<std::uint64_t>(header+8)!=0)return std::nullopt;
            if(types[i])return std::nullopt; // Ambiguous identity: never select the first match.
            types[i]=s.rva+static_cast<std::uint32_t>(p)-16;
        }
    }
    for(auto type:types)if(!type)return std::nullopt;
    for(const auto &s:image.sections)for(std::size_t p=0;p+24<=s.data.size();p+=4){
        if((p&65535)==0&&stop.load())return std::nullopt;
        const auto c=s.data.data()+p;
        if(number<std::uint32_t>(c)!=1||number<std::uint32_t>(c+4)||number<std::uint32_t>(c+8))continue;
        for(std::size_t i=0;i<4;++i)if(number<std::uint32_t>(c+12)==types[i]&&
            number<std::uint32_t>(c+20)==s.rva+p&&image.hierarchy(number<std::uint32_t>(c+16),types[i])){
            if(locators[i])return std::nullopt;
            locators[i]=s.rva+static_cast<std::uint32_t>(p);
        }
    }
    for(auto locator:locators)if(!locator)return std::nullopt;
    for(const auto &s:image.sections)for(std::size_t p=0;p+104<=s.data.size();p+=8){
        if((p&65535)==0&&stop.load())return std::nullopt;
        const auto t=s.data.data()+p;
        for(std::size_t i=0;i<4;++i)if(number<std::uint64_t>(t)==image.base+locators[i]){
            bool valid=true;
            for(unsigned m=0;m<12;++m)if(!image.code(number<std::uint64_t>(t+8+m*8))){valid=false;break;}
            if(!valid)continue;
            if(tables[i])return std::nullopt;
            tables[i]=s.rva+static_cast<std::uint32_t>(p)+8;
        }
    }
    for(auto table:tables)if(!table)return std::nullopt;
    if(stop.load())return std::nullopt;
    return GameBuild{"Auto-detected","",tables[0],tables[1],tables[2],tables[3]};
}
}
