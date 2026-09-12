#include "map_reader.hpp"
#include <map>
#include <vector>
#include <cstring>
#include <iostream>
#include <limits>
struct Memory {
    bool replace_root=false;unsigned root_reads=0;
    std::map<std::uintptr_t,std::vector<unsigned char>> blocks;
    template<class T> void put(std::uintptr_t a,T v) { for(std::size_t i=0;i<sizeof v;++i) blocks[a+i]={reinterpret_cast<unsigned char*>(&v)[i]}; }
    bool read(std::uintptr_t a,void *out,std::size_t n) {
        if(a==0x200000000&&replace_root&&++root_reads==2){std::memset(out,0,n);return true;}
        for(std::size_t i=0;i<n;++i) {auto it=blocks.find(a+i);if(it==blocks.end())return false;static_cast<unsigned char*>(out)[i]=it->second[0];}return true; }
};
constexpr std::uintptr_t base=0x140000000,root=0x200000000,body_address=0x200010000,icon_address=0x200020000,canvas_address=0x200030000,view_address=0x200040000;
Memory fixture() {
    Memory m;
    m.put(root,base+89513840);m.put(root+8,body_address);m.put(root+0x418,icon_address);m.put(root+0x448,canvas_address);m.put(canvas_address,base+89515328);
    m.put(root+0x330,12.f);m.put(root+0x334,30.f);m.put(root+0x338,42.f);m.put(root+0x558,std::uint32_t(383));
    m.put(body_address+0x38,view_address);m.put(view_address+0xb0,std::uint8_t(0xd0));
    for(auto pair:{std::pair{body_address,"MinimapHudBody"},std::pair{icon_address,"DestinationDistanceIcon"},std::pair{view_address,"MinimapView"}}) {
        auto e=pair.first;auto text=pair.second;auto length=std::uint32_t(std::strlen(text));
        m.put(e+0x60,e+0x100);m.put(e+0x100,e+0x200);m.put(e+0x108,length);
        for(std::size_t i=0;i<length;++i)m.put(e+0x200+i,text[i]);
        m.put(e+0x18,e+0x300);m.put(e+0x311,std::uint8_t(e==icon_address?1:0));
    }
    return m;
}
int main() {
    int failures=0;auto check=[&](bool ok,const char *s){if(!ok){++failures;std::cerr<<"FAIL "<<s<<'\n';}};
    crimson_nav::MapState s{};auto m=fixture();
    check(crimson_nav::read_map(m,base,root,s)&&s.destination_candidate&&!s.map_hidden_candidate&&s.distance==383,"active valid fixture");
    m.put(view_address+0xb0,std::uint8_t(0x90));check(crimson_nav::read_map(m,base,root,s)&&s.map_hidden_candidate&&s.destination_candidate,"hidden parent with unchanged visible child: full-map leak regression");
    m.put(view_address+0xb0,std::uint8_t(0xd0));check(crimson_nav::read_map(m,base,root,s)&&!s.map_hidden_candidate,"parent visible again after closing map");
    m.put(icon_address+0x311,std::uint8_t(0));check(crimson_nav::read_map(m,base,root,s)&&!s.destination_candidate&&s.distance==383,"cached distance is not validity");
    m.put(icon_address+0x311,std::uint8_t(2));check(crimson_nav::read_map(m,base,root,s)&&!s.destination_ui_valid,"unsupported optional UI byte does not invalidate map");
    m=fixture();m.put(root,base+1);check(!crimson_nav::read_map(m,base,root,s),"reject stale vtable");
    m=fixture();m.put(icon_address+0x108,std::uint32_t(4096));check(crimson_nav::read_map(m,base,root,s)&&!s.destination_ui_valid,"reject optional oversized name without dropping map");
    m=fixture();m.put(icon_address+0x200,'X');check(crimson_nav::read_map(m,base,root,s)&&!s.destination_ui_valid,"reject optional wrong UI name without dropping map");
    m=fixture();m.put(root+0x330,std::numeric_limits<float>::quiet_NaN());check(!crimson_nav::read_map(m,base,root,s),"reject invalid coordinates");
    m=fixture();m.blocks.erase(view_address+0xb0);check(!crimson_nav::read_map(m,base,root,s),"reject unreadable parent visibility");
    m=fixture();m.put(view_address+0x200,'X');check(!crimson_nav::read_map(m,base,root,s),"reject wrong parent identity");
    m=fixture();m.replace_root=true;check(!crimson_nav::read_map(m,base,root,s)&&!s.destination_candidate&&s.distance==0,"reject replacement during read and clear previous output");
    m=fixture();m.blocks.erase(icon_address+0x311);check(crimson_nav::read_map(m,base,root,s)&&!s.destination_ui_valid,"missing optional destination label preserves map");
    m=fixture();m.put(root+0x558,std::uint32_t(0xffffffff));check(crimson_nav::read_map(m,base,root,s)&&!s.distance_valid,"invalid cached distance preserves map");
    if(!failures)std::cout<<"PASS: 14 map-reader cases\n";
    return failures?1:0;
}
