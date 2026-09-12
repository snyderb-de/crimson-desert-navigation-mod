// Development-only external host for the same reader used by the ReShade add-on.
#include "native_map_reader.hpp"
#include "target_reader.hpp"
#include "minimap_geometry.hpp"
#include <Psapi.h>
#include <iostream>
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
    crimson_nav::MapState state{};bool ok=found.root&&crimson_nav::read_map(memory,base,found.root,state,*build);
    if(ok)std::cout<<"Player="<<state.xyz[0]<<","<<state.xyz[1]<<","<<state.xyz[2]<<" destination_candidate="<<state.destination_candidate<<" map_hidden_candidate="<<state.map_hidden_candidate<<" distance="<<state.distance<<"\n";
    if(ok){const auto target=crimson_nav::read_target(memory,base,found.root,*build);
        std::cout<<"Collection target status="<<static_cast<int>(target.status)<<" xyz="<<target.xyz[0]<<","<<target.xyz[1]<<","<<target.xyz[2]<<"\n";}
    if(ok){const auto g=crimson_nav::read_minimap_geometry(memory,base,found.root,*build);
        std::cout<<"Geometry valid="<<g.valid<<" canvas="<<g.canvas_width<<","<<g.canvas_height<<" rect="<<g.x<<","<<g.y<<","<<g.width<<","<<g.height<<"\n";}
    CloseHandle(process);return ok?0:5;
}
