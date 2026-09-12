#pragma once
#include <cstdint>
// Independently supplied UI layout: physical body extent and game UI scale.
// The reference canvas at root+a58 need not have the body's aspect ratio.
template<class Memory> void worldmap_layout_fixture(Memory &m,std::uintptr_t body,
    std::uintptr_t arena,float width,float height,float scale=1.f){
    const float rect[4]={0,0,width,height};m.put(body+0x70,rect);
    m.put(body+0x94,std::uint8_t(1));m.put(body+0x58,arena);
    m.put(arena+0xb0,arena+0x1000);m.put(arena+0x10b0,arena+0x2000);
    m.put(arena+0x2000,arena+0x3000);m.put(arena+0x2008,std::uint32_t(2));
    m.put(arena+0x3008,arena+0x4000);
    const float scales[4]={1,1,scale,scale};m.put(arena+0x40d4,scales);
    m.put(arena+0x413b,std::uint8_t(1));m.put(arena+0x413c,std::uint8_t(1));
}
