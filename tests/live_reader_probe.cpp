// Development-only external host for the same reader used by the ReShade add-on.
#include "native_map_reader.hpp"
#include "target_reader.hpp"
#include "minimap_geometry.hpp"
#include <Psapi.h>
#include <iostream>
#include <iomanip>

namespace {
struct NamedPointer {std::uintptr_t value{};std::size_t name_offset{};std::string name;};

std::string element_name(crimson_nav::ProcessMemory &memory,std::uintptr_t element,std::size_t &name_offset) {
    for(std::size_t offset=0;offset<=0x120;offset+=8) {
        std::uintptr_t ref{},text{};std::uint32_t length{};char name[65]{};
        if(!memory.read(element+offset,&ref,8)||!memory.read(ref,&text,8)||
           !memory.read(ref+8,&length,4)||!length||length>64||!memory.read(text,name,length))continue;
        bool printable=true;for(std::uint32_t i=0;i<length;++i)if(name[i]<32||name[i]>126){printable=false;break;}
        if(printable){name_offset=offset;return {name,name+length};}
    }
    return {};
}

void inspect_object(crimson_nav::ProcessMemory &memory,std::uintptr_t object,const crimson_nav::GameBuild &build,
                    std::uintptr_t base,const char *kind) {
    std::uintptr_t body{};memory.read(object+8,&body,8);std::size_t body_name_offset{};
    const auto body_name=element_name(memory,body,body_name_offset);
    std::cout<<kind<<" object=0x"<<std::hex<<object<<" body=0x"<<body<<std::dec
        <<" body_name="<<(body_name.empty()?"<none>":body_name)<<" name_offset=0x"<<std::hex<<body_name_offset<<std::dec<<'\n';
    for(std::size_t offset=8;offset<=0xb00;offset+=8) {
        std::uintptr_t value{},vt{};if(!memory.read(object+offset,&value,8)||!memory.read(value,&vt,8))continue;
        std::size_t name_offset{};const auto name=element_name(memory,value,name_offset);
        const bool identity=vt==base+build.minimap_canvas||vt==base+build.minimap_icon||
            vt==base+build.minimap_root||vt==base+build.worldmap_root;
        if(name.empty()&&!identity)continue;
        std::cout<<"  root+0x"<<std::hex<<offset<<" -> 0x"<<value<<" vt=0x"<<vt;
        if(!name.empty())std::cout<<" name="<<name<<" name+0x"<<name_offset;
        if(identity)std::cout<<" known-vtable";
        std::cout<<std::dec<<'\n';
    }
    if(body)for(std::size_t offset=0;offset<=0x200;offset+=8) {
        std::uintptr_t value{};if(!memory.read(body+offset,&value,8))continue;
        std::size_t name_offset{};const auto name=element_name(memory,value,name_offset);if(name.empty())continue;
        std::cout<<"  body+0x"<<std::hex<<offset<<" -> 0x"<<value<<" name="<<name<<" name+0x"<<name_offset<<std::dec<<'\n';
    }
}

void diagnose_identities(crimson_nav::ProcessMemory &memory,std::uintptr_t base,const crimson_nav::GameBuild &build) {
    std::array<unsigned char,65536> buffer{};std::uintptr_t address=0;unsigned mini=0,world=0;
    while(address<0x7fffffff0000ull) {
        MEMORY_BASIC_INFORMATION region{};if(!VirtualQueryEx(memory.process,reinterpret_cast<void*>(address),&region,sizeof(region)))break;
        const auto low=reinterpret_cast<std::uintptr_t>(region.BaseAddress),end=low+region.RegionSize;if(end<=address)break;
        if(region.State==MEM_COMMIT&&region.Type==MEM_PRIVATE&&
           (region.Protect==PAGE_READWRITE||region.Protect==PAGE_WRITECOPY||region.Protect==PAGE_EXECUTE_READWRITE||region.Protect==PAGE_EXECUTE_WRITECOPY))
            for(auto pos=low;pos<end;pos+=buffer.size()) {
                const auto size=static_cast<std::size_t>((std::min)(end-pos,std::uintptr_t(buffer.size())));
                if(!memory.read(pos,buffer.data(),size))continue;
                for(std::size_t i=0;i+8<=size;i+=8) {
                    std::uintptr_t value{};std::memcpy(&value,buffer.data()+i,8);
                    if(value==base+build.minimap_root){++mini;inspect_object(memory,pos+i,build,base,"minimap");}
                    if(value==base+build.worldmap_root){++world;inspect_object(memory,pos+i,build,base,"worldmap");}
                }
            }
        address=end;
    }
    std::cout<<"Identity occurrences: minimap="<<mini<<" worldmap="<<world<<'\n';
}
}
int wmain(int argc,wchar_t **argv) {
    if(argc!=2)return 2;
    HANDLE process=OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,std::stoul(argv[1]));
    if(!process)return 3;
    std::array<HMODULE,2048> modules{};DWORD needed{};std::uintptr_t base{};std::filesystem::path exe;
    if(EnumProcessModulesEx(process,modules.data(),sizeof(modules),&needed,LIST_MODULES_64BIT)&&needed<=sizeof(modules))
        for(unsigned i=0;i<needed/sizeof(HMODULE);++i) {wchar_t path[32768]{};
            if(GetModuleFileNameExW(process,modules[i],path,32768)&&std::filesystem::path(path).filename()==L"CrimsonDesert.exe") {base=reinterpret_cast<std::uintptr_t>(modules[i]);exe=path;break;}}
    std::atomic<bool> stop{false};
    const auto build=base?crimson_nav::identify_game_build(exe,stop):std::nullopt;
    if(!build){CloseHandle(process);return 4;}
    std::cout<<"Selected game profile="<<build->version<<" base=0x"<<std::hex<<base<<std::dec<<"\n";
    crimson_nav::ProcessMemory memory{process};auto found=crimson_nav::discover_map(memory,base,stop,*build);
    std::cout<<"Scan bytes="<<found.bytes<<" candidates="<<found.candidates<<" complete="<<found.complete<<" root=0x"<<std::hex<<found.root<<std::dec<<"\n";
    std::cout<<"World candidates="<<found.world_candidates<<" root=0x"<<std::hex<<found.world_root<<std::dec<<"\n";
    if(!found.root||!found.world_root)diagnose_identities(memory,base,*build);
    crimson_nav::MapState state{};bool ok=found.root&&crimson_nav::read_map(memory,base,found.root,state,*build);
    if(ok)std::cout<<"Player="<<state.xyz[0]<<","<<state.xyz[1]<<","<<state.xyz[2]<<" destination_candidate="<<state.destination_candidate<<" map_hidden_candidate="<<state.map_hidden_candidate<<" distance="<<state.distance<<"\n";
    if(ok){const auto target=crimson_nav::read_target(memory,base,found.root,*build);
        std::cout<<"Collection target status="<<static_cast<int>(target.status)<<" xyz="<<target.xyz[0]<<","<<target.xyz[1]<<","<<target.xyz[2]<<"\n";}
    if(ok){const auto g=crimson_nav::read_minimap_geometry(memory,base,found.root,*build);
        std::cout<<"Geometry valid="<<g.valid<<" canvas="<<g.canvas_width<<","<<g.canvas_height<<" rect="<<g.x<<","<<g.y<<","<<g.width<<","<<g.height<<"\n";}
    CloseHandle(process);return ok?0:5;
}
