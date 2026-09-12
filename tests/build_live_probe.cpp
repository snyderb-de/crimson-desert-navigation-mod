// Development-only, read-only live compatibility evidence. No hooks or writes
// to the game. The caller supplies current PID/roots and a workspace output file.
#include "native_map_reader.hpp"
#include "reader_core.hpp"
#include "worldmap_roads.hpp"
#include <Psapi.h>
#include <iostream>
#include <iomanip>
int wmain(int argc,wchar_t **argv){
    if(argc!=5&&argc!=7)return 2;
    const auto output=std::filesystem::weakly_canonical(argv[4]);
    const auto relative=output.lexically_relative(std::filesystem::current_path());
    if(relative.empty()||relative.is_absolute()||*relative.begin()==L"..")return 6;
    HANDLE process=OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,std::stoul(argv[1]));
    if(!process)return 3;
    struct Close {HANDLE h;~Close(){CloseHandle(h);}} close{process};
    std::array<HMODULE,2048> modules{};DWORD needed{};std::uintptr_t base{};std::filesystem::path exe;
    if(EnumProcessModulesEx(process,modules.data(),sizeof(modules),&needed,LIST_MODULES_64BIT)&&needed<=sizeof(modules))
        for(unsigned i=0;i<needed/sizeof(HMODULE);++i){wchar_t path[32768]{};
            if(GetModuleFileNameExW(process,modules[i],path,32768)&&std::filesystem::path(path).filename()==L"CrimsonDesert.exe"){
                base=reinterpret_cast<std::uintptr_t>(modules[i]);exe=path;break;}}
    std::atomic<bool> stop{false};const auto build=base?crimson_nav::identify_game_build(exe,stop):std::nullopt;
    if(!build)return 4;
    try{
        const auto mini=std::stoull(argv[2],nullptr,0),world=std::stoull(argv[3],nullptr,0);
        crimson_nav::ProcessMemory memory{process};
        const auto roads=crimson_nav::load_worldmap_roads(exe.parent_path().parent_path());
        crimson_nav::ReaderCore core(*build);core.accept_scan({mini,0,1,true,world,1});
        std::ofstream out(output);if(!out)return 7;
        out<<std::setprecision(12)<<"{\n\"build\":\""<<build->version<<"\",\"base\":"<<base
            <<",\"mini\":"<<mini<<",\"world\":"<<world<<",\"road_nodes\":"<<roads.points.size()
            <<",\"road_segments\":"<<roads.edges.size()<<",\n\"samples\":[\n";
        const double display_width=argc==7?std::stod(argv[5]):0,display_height=argc==7?std::stod(argv[6]):0;
        unsigned valid=0,targets=0,full=0,mini_projections=0,display_projections=0;
        for(int i=0;i<10;++i){
            auto view=core.sample(memory,base,roads,GetTickCount64());
            valid+=view.valid;targets+=view.target.status==crimson_nav::TargetStatus::candidate;
            full+=view.world_projection.valid;mini_projections+=view.mini_projection.valid;
            auto p=[&](const auto &v){out<<"["<<v.ax<<","<<v.az<<","<<v.bx<<","<<v.bz<<","<<v.tx<<","<<v.ty<<"]";};
            if(i)out<<",\n";
            out<<"{\"valid\":"<<view.valid<<",\"target_status\":"<<int(view.target.status)
                <<",\"player\":["<<view.state.xyz[0]<<","<<view.state.xyz[1]<<","<<view.state.xyz[2]
                <<"],\"target\":["<<view.target.xyz[0]<<","<<view.target.xyz[1]<<","<<view.target.xyz[2]
                <<"],\"distance\":"<<view.state.distance<<",\"distance_valid\":"<<view.state.distance_valid
                <<",\"destination_ui_valid\":"<<view.state.destination_ui_valid<<",\"destination_ui_active\":"<<view.state.destination_candidate
                <<",\"mini_hidden\":"<<view.state.map_hidden_candidate<<",\"mini_geometry_valid\":"<<view.geometry.valid
                <<",\"mini_canvas\":["<<view.geometry.canvas_width<<","<<view.geometry.canvas_height
                <<"],\"mini_projection_valid\":"<<view.mini_projection.valid<<",\"mini_projection\":";p(view.mini_projection);
            out<<",\"full_projection_valid\":"<<view.world_projection.valid<<",\"full_canvas\":["<<view.world_projection.canvas_width<<","<<view.world_projection.canvas_height
                <<"],\"full_projection\":";p(view.world_projection);
            if(argc==7){
                const auto screen=crimson_nav::read_route_display_projection(memory,base,mini,world,view.state,view.geometry,display_width,display_height,*build);
                display_projections+=screen.valid;
                out<<",\"display_size\":["<<display_width<<","<<display_height<<"],\"display_projection_valid\":"<<screen.valid<<",\"display_projection\":";p(screen);
            }
            out<<",\"navigation_active\":"<<view.navigation.active<<",\"route_valid\":"<<bool(view.route&&view.route->valid)
                <<",\"route_length\":"<<(view.route?view.route->length:0)<<"}";
            Sleep(50);
        }
        out<<"\n]}\n";out.flush();if(!out)return 7;
        std::cout<<"Build "<<build->version<<": "<<roads.points.size()<<" road nodes, "<<roads.edges.size()<<" segments; "
            <<valid<<"/10 valid map samples, "<<targets<<" destinations, "<<full<<" full projections, "<<mini_projections<<" minimap projections\n";
        if(argc==7)std::cout<<display_projections<<"/10 display projections at "<<display_width<<"x"<<display_height<<'\n';
        return argc==7?(display_projections==10?0:5):(valid?0:5);
    }catch(const std::exception &e){std::cerr<<e.what()<<'\n';return 8;}
}
