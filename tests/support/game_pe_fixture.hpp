#pragma once
#include <array>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <vector>
// An independently assembled PE32+ image: code, read-only RTTI/vtables, and
// writable type descriptors. It has no version resource or known game hash.
struct GamePeFixture {
    std::vector<unsigned char> bytes=std::vector<unsigned char>(0x2400);
    std::uint64_t image_base;
    std::uint32_t shift;
    template<class T> void put(std::size_t offset,T value){std::memcpy(bytes.data()+offset,&value,sizeof(value));}
    std::uint32_t rva(std::size_t offset) const {
        return shift+static_cast<std::uint32_t>(offset<0x400?0x1000+offset-0x200:offset<0x1400?0x2000+offset-0x400:0x4000+offset-0x1400);
    }
    explicit GamePeFixture(std::uint32_t relocation=0,std::uint64_t base=0x140000000ull):image_base(base),shift(relocation){
        put(0,std::uint16_t(0x5a4d));put(0x3c,std::uint32_t(0x80));put(0x80,std::uint32_t(0x4550));
        put(0x84,std::uint16_t(0x8664));put(0x86,std::uint16_t(3));put(0x94,std::uint16_t(240));
        put(0x98,std::uint16_t(0x20b));put(0xb0,image_base);put(0xd0,std::uint32_t(shift+0x5000));
        put(0xd4,std::uint32_t(0x200));
        const auto section=[&](std::size_t p,std::uint32_t va,std::uint32_t raw,std::uint32_t size,std::uint32_t flags){
            put(p+8,size);put(p+12,shift+va);put(p+16,size);put(p+20,raw);put(p+36,flags);
        };
        section(0x188,0x1000,0x200,0x200,0x60000020);
        section(0x1b0,0x2000,0x400,0x1000,0x40000040);
        section(0x1d8,0x4000,0x1400,0x1000,0xc0000040);
        const char *names[]={".?AVUIGamePlayControlRootMiniMap@uiCommonScript@pa@@",
            ".?AVUIGamePlayControlRootWorldMap@uiCommonScript@pa@@",
            ".?AVUIGamePlayControlCommon_MiniMapIconCanvas@uiCommonScript@pa@@",
            ".?AVUIGamePlayControlCommon_MiniMapIcon@uiCommonScript@pa@@"};
        for(std::size_t i=0;i<4;++i){
            const auto type=0x1400+i*0x100,col=0x400+i*0x200,hierarchy=col+0x20,array=col+0x30,base_desc=col+0x40,table=col+0x80;
            put(type,image_base+rva(0x1300));std::memcpy(bytes.data()+type+16,names[i],std::strlen(names[i])+1);
            put(col,std::uint32_t(1));put(col+12,rva(type));put(col+16,rva(hierarchy));put(col+20,rva(col));
            put(hierarchy+8,std::uint32_t(1));put(hierarchy+12,rva(array));put(array,rva(base_desc));
            put(base_desc,rva(type));put(base_desc+12,std::uint32_t(0xffffffff));put(base_desc+20,std::uint32_t(64));put(base_desc+24,rva(hierarchy));
            put(table,image_base+rva(col));
            for(std::size_t m=0;m<12;++m)put(table+8+m*8,image_base+rva(0x200+m*8));
        }
    }
    void save(const char *path) const {std::ofstream out(path,std::ios::binary);out.write(reinterpret_cast<const char*>(bytes.data()),bytes.size());}
};
