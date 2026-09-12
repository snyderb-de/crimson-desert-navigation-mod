#include "native_map_reader.hpp"
#include "support/game_pe_fixture.hpp"
#include <iostream>
int wmain(int argc,wchar_t **argv){
    int checks=0,failures=0;
    auto check=[&](bool ok,const char *name){++checks;if(!ok){++failures;std::cerr<<"FAIL: "<<name<<'\n';}};
    constexpr auto path="work/build/layout-fixture.exe";
    std::atomic<bool> stop{false};
    const auto detect=[&](const GamePeFixture &fixture){fixture.save(path);return crimson_nav::identify_game_build(path,stop);};
    for(auto shift:{0u,0x10000u,0x5300000u}){
        GamePeFixture fixture(shift,0x180000000ull+shift);
        const auto result=detect(fixture);
        check(result&&result->minimap_root==shift+0x2088&&result->worldmap_root==shift+0x2288&&
            result->minimap_canvas==shift+0x2488&&result->minimap_icon==shift+0x2688,"unknown executable resolves all four moved map identities");
        fixture.bytes[0x250]=0xa9; // Unrelated code change changes the file hash.
        const auto patched=detect(fixture);
        check(patched&&patched->worldmap_root==shift+0x2288,"unrelated executable change does not block routing");
    }
    const auto rejects=[&](auto corrupt,const char *name){GamePeFixture f;corrupt(f);check(!detect(f),name);};
    rejects([](auto &f){f.bytes.resize(40);},"truncated DOS header rejected");
    rejects([](auto &f){f.bytes[0]=0;},"bad DOS signature rejected");
    rejects([](auto &f){f.put(0x3c,std::uint32_t(0xfffffff0));},"PE offset beyond file rejected");
    rejects([](auto &f){f.bytes[0x80]=0;},"bad PE signature rejected");
    rejects([](auto &f){f.put(0x84,std::uint16_t(0x14c));},"non-x64 image rejected");
    rejects([](auto &f){f.put(0x98,std::uint16_t(0x10b));},"PE32 image rejected");
    rejects([](auto &f){f.put(0x86,std::uint16_t(97));},"excess section count rejected");
    rejects([](auto &f){f.put(0x94,std::uint16_t(120));},"short optional header rejected");
    rejects([](auto &f){f.put(0xd4,std::uint32_t(0x180));},"section table outside headers rejected");
    rejects([](auto &f){f.put(0xb0,std::uint64_t(0xffffffffffff0000ull));},"overflowing image address rejected");
    rejects([](auto &f){f.put(0x1b0+12,std::uint32_t(0x1000));},"overlapping virtual sections rejected");
    rejects([](auto &f){f.put(0x1b0+20,std::uint32_t(0x200));},"overlapping raw sections rejected");
    rejects([](auto &f){f.put(0x1b0+20,std::uint32_t(0xfffffff0));},"out-of-file raw section rejected");
    rejects([](auto &f){f.put(0x1b0+12,std::uint32_t(0xfffffff0));},"overflowing virtual section rejected");
    rejects([](auto &f){f.bytes[0x1410]='X';},"missing required map class rejected");
    rejects([](auto &f){f.bytes[0x1410+std::strlen(".?AVUIGamePlayControlRootMiniMap@uiCommonScript@pa@@")]='X';},"longer type name is not an exact match");
    rejects([](auto &f){std::memcpy(f.bytes.data()+0x1800,f.bytes.data()+0x1400,0x100);},"duplicate type descriptor rejected");
    rejects([](auto &f){f.put(0x1400,std::uint64_t(0));},"invalid type-info pointer rejected");
    rejects([](auto &f){f.put(0x1408,std::uint64_t(1));},"invalid type descriptor header rejected");
    rejects([](auto &f){f.put(0x400+20,std::uint32_t(0x2004));},"incorrect locator self reference rejected");
    rejects([](auto &f){f.put(0x400+4,std::uint32_t(8));},"secondary vtable cannot stand in for a root");
    rejects([](auto &f){f.put(0x420+8,std::uint32_t(257));},"oversized hierarchy rejected");
    rejects([](auto &f){f.put(0x420+12,std::uint32_t(0xfffffffc));},"out-of-image base array rejected");
    rejects([](auto &f){f.put(0x440,std::uint32_t(0x4100));},"hierarchy for another class rejected");
    rejects([](auto &f){f.put(0x440+24,std::uint32_t(0));},"broken hierarchy back reference rejected");
    rejects([](auto &f){std::memcpy(f.bytes.data()+0x1000,f.bytes.data()+0x400,24);f.put(0x1014,f.rva(0x1000));},"duplicate primary locator rejected");
    rejects([](auto &f){std::memcpy(f.bytes.data()+0x1000,f.bytes.data()+0x480,104);},"duplicate plausible vtable rejected");
    rejects([](auto &f){f.put(0x488,f.image_base+0x4000);},"vtable method in non-executable data rejected");
    rejects([](auto &f){f.put(0x488,std::uint64_t(0xffffffffffffffffull));},"vtable method outside image rejected");
    rejects([](auto &f){f.put(0x1b0+36,std::uint32_t(0x60000020));},"RTTI-like bytes in executable code are not accepted");
    // A stray reference with invalid methods does not create a second vtable.
    GamePeFixture decoy;decoy.put(0x1000,decoy.image_base+decoy.rva(0x400));
    check(bool(detect(decoy)),"non-vtable locator reference ignored");
    stop=true;check(!detect(GamePeFixture{}),"cancelled detection rejected promptly");stop=false;
    check(!crimson_nav::identify_game_build("work/build/missing-layout-file.exe",stop),"missing file rejected");
    if(argc==2){
        const auto start=std::chrono::steady_clock::now();
        const auto current=crimson_nav::identify_game_build(argv[1],stop);
        check(current&&current->minimap_root==89530200&&current->worldmap_root==89514408&&
            current->minimap_canvas==89531688&&current->minimap_icon==89528856,"installed 2850 PE auto-detection matches independently recorded identities");
        if(current)std::cout<<"Auto-detected RVAs: "<<current->minimap_root<<", "<<current->worldmap_root<<", "<<current->minimap_canvas<<", "<<current->minimap_icon<<'\n';
        std::cout<<"Installed PE detection milliseconds: "<<std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()-start).count()<<'\n';
    }
    std::cout<<checks<<" automatic layout checks, "<<failures<<" failures\n";
    return failures?1:0;
}
