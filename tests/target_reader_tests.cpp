#include "target_reader.hpp"
#include <map>
#include <cstring>
#include <iostream>
#include <limits>
#include <fstream>
#include <functional>
struct Memory {
    std::map<std::uintptr_t,unsigned char> bytes;
    std::uintptr_t change_address{};int reads{};
    std::function<void(std::uintptr_t,std::size_t)> before_read;
    template<class T> void put(std::uintptr_t a,const T &v){for(std::size_t i=0;i<sizeof(v);++i)bytes[a+i]=reinterpret_cast<const unsigned char*>(&v)[i];}
    void zero(std::uintptr_t a,std::size_t n){for(std::size_t i=0;i<n;++i)bytes[a+i]=0;}
    bool read(std::uintptr_t a,void *out,std::size_t n){
        if(before_read)before_read(a,n);
        if(a==change_address&&++reads==2){std::memset(out,0,n);return true;}
        for(std::size_t i=0;i<n;++i){auto it=bytes.find(a+i);if(it==bytes.end())return false;static_cast<unsigned char*>(out)[i]=it->second;}return true;
    }
};
constexpr std::uintptr_t base=0x140000000,root=0x200000000,target_address=0x200100000,retained=0x200200000;
Memory fixture(bool present=true){
    Memory m;m.put(root,base+89513840);
    for(auto a:{target_address,retained}){m.zero(a,0x120);m.put(a,base+89518944);m.put(a+0xd0,std::uint32_t(21));m.put(a+0xde,std::uint16_t(a==target_address?3:1));m.put(a+0xe0,-5329.f);m.put(a+0xe4,483.f);m.put(a+0xe8,-2337.f);m.put(a+0x110,root);}
    for(unsigned n=0;n<2;++n){auto header=root+0x238+n*16;auto entries=root+0x1000+n*0x1000;auto list=entries+0x100;
        m.zero(header,16);m.put(header,entries);m.put(header+8,std::uint32_t(1));m.put(header+12,std::uint32_t(1));
        m.zero(entries,32);m.put(entries+8,std::uint64_t(21));m.put(entries+16,list);m.put(entries+24,std::uint32_t(present?2:1));m.put(entries+28,std::uint32_t(2));
        m.put(list,retained);m.put(list+8,target_address);
    }return m;
}
int main(int argc,char **argv){
    if(argc==2){
        std::ifstream input(argv[1],std::ios::binary);Memory replay;std::uint64_t image{},object{};std::uint32_t expected{},blocks{};
        input.read(reinterpret_cast<char*>(&image),8);input.read(reinterpret_cast<char*>(&object),8);
        input.read(reinterpret_cast<char*>(&expected),4);input.read(reinterpret_cast<char*>(&blocks),4);
        if(!input||blocks>1024)return 2;
        for(unsigned i=0;i<blocks;++i){std::uint64_t address{};std::uint32_t size{};input.read(reinterpret_cast<char*>(&address),8);input.read(reinterpret_cast<char*>(&size),4);
            if(!input||size>65536)return 3;
            for(unsigned j=0;j<size;++j){char value{};input.get(value);replay.bytes[address+j]=static_cast<unsigned char>(value);}
        }
        if(!input)return 4;
        const auto state=crimson_nav::read_target(replay,image,object);
        if(static_cast<unsigned>(state.status)!=expected)return 5;
        std::cout<<"PASS: captured icon fixture "<<argv[1]<<" status="<<expected<<'\n';return 0;
    }
    int failures=0;auto check=[&](bool ok,const char *name){if(!ok){++failures;std::cerr<<"FAIL "<<name<<'\n';}};
    using S=crimson_nav::TargetStatus;auto m=fixture();auto s=crimson_nav::read_target(m,base,root);
    check(s.status==S::candidate&&s.address==target_address&&s.xyz[1]==483,"target_address in both collections");
    m=fixture(false);check(crimson_nav::read_target(m,base,root).status==S::no_candidate,"retained icons do not imply target_address");
    m=fixture();m.put(root+0x2000+24,std::uint32_t(1));check(crimson_nav::read_target(m,base,root).status==S::unavailable,"disagreeing collections");
    m=fixture();m.put(target_address+0xde,std::uint16_t(7));check(crimson_nav::read_target(m,base,root).status==S::unavailable,"unknown flags");
    m=fixture();m.put(target_address+0x110,root+8);check(crimson_nav::read_target(m,base,root).status==S::unavailable,"wrong owner");
    m=fixture();m.put(target_address,base+1);check(crimson_nav::read_target(m,base,root).status==S::unavailable,"wrong type");
    m=fixture();m.put(target_address+0xe0,std::numeric_limits<float>::infinity());check(crimson_nav::read_target(m,base,root).status==S::unavailable,"invalid coordinates");
    m=fixture();m.put(root+0x240,std::uint32_t(100000));check(crimson_nav::read_target(m,base,root).status==S::unavailable,"oversized collection");
    m=fixture();m.bytes.erase(target_address+0xe4);check(crimson_nav::read_target(m,base,root).status==S::unavailable,"unreadable object");
    m=fixture();m.change_address=target_address;check(crimson_nav::read_target(m,base,root).status==S::unavailable,"object changes during capture");
    m=fixture();m.put(retained+0xde,std::uint16_t(3));check(crimson_nav::read_target(m,base,root).status==S::ambiguous,"multiple targets never choose first");
    m=fixture();m.put(target_address+0xe0,0.f);m.put(target_address+0xe4,0.f);m.put(target_address+0xe8,0.f);check(crimson_nav::read_target(m,base,root).status==S::candidate,"zero coordinates are not absence");
    for(auto address:{target_address,retained}){
        m=fixture();unsigned count=0;
        m.before_read=[&](std::uintptr_t a,std::size_t n){if(a<=address+0x40&&address+0x40<a+n&&++count==2)m.bytes[address+0x40]^=1;};
        check(crimson_nav::read_target(m,base,root).status==S::candidate,"irrelevant icon animation bytes do not hide destination");
    }
    m=fixture();unsigned row_count=0;
    m.before_read=[&](std::uintptr_t a,std::size_t n){if(a<=root+0x1000&&root+0x1000<a+n&&++row_count==2)m.bytes[root+0x1000]^=1;};
    check(crimson_nav::read_target(m,base,root).status==S::candidate,"unused row key changes do not hide destination");
    m=fixture();unsigned coordinate_reads=0;
    m.before_read=[&](std::uintptr_t a,std::size_t n){if(a<=target_address+0xe0&&target_address+0xe0<a+n&&++coordinate_reads==2)m.put(target_address+0xe0,100.f);};
    check(crimson_nav::read_target(m,base,root).status==S::unavailable,"changing target coordinates are still rejected");
    if(!failures)std::cout<<"PASS: 16 target-reader cases\n";return failures?1:0;
}
