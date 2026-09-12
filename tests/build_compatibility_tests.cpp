#include "native_map_reader.hpp"
#include "reader_core.hpp"
#include "support/map_fixture.hpp"
#include "support/worldmap_layout_fixture.hpp"
#include <iostream>

int main(){
    int checks=0,failures=0;
    auto check=[&](bool ok,const char *name){++checks;if(!ok){++failures;std::cerr<<"FAIL: "<<name<<'\n';}};
    auto memory=map_fixture::fixture();
    worldmap_layout_fixture(memory,map_fixture::world_body,0x70000,1920,1080);
    const auto &build=crimson_nav::game_build_2850;
    // Independent new-build RTTI identities, measured from the 1.0.0.2850 PE.
    memory.put(map_fixture::root,std::uintptr_t(89530200));
    memory.put(map_fixture::mini_canvas_address,std::uintptr_t(89531688));
    for(auto address:{map_fixture::target_icon,map_fixture::retained_icon})memory.put(address,std::uintptr_t(89528856));
    memory.put(map_fixture::world,std::uintptr_t(89514408));
    memory.put(map_fixture::world_view+0xb0,std::uint8_t(0xd0));
    crimson_nav::MapState state{};
    check(crimson_nav::read_map(memory,0,map_fixture::root,state,build),"new-build named minimap and player readable");
    auto target=crimson_nav::read_target(memory,0,map_fixture::root,build);
    check(target.status==crimson_nav::TargetStatus::candidate&&target.xyz[0]==100,"new-build active destination readable");
    auto geometry=crimson_nav::read_minimap_geometry(memory,0,map_fixture::root,build);
    check(geometry.valid&&geometry.canvas_width==1920,"new-build minimap geometry readable");
    auto projection=crimson_nav::read_worldmap_projection(memory,0,map_fixture::world,build);
    check(projection.valid&&projection.tx==960&&projection.ty==540,"new-build full-map anchor readable");
    check(!crimson_nav::read_map(memory,0,map_fixture::root,state),"new object rejected under old profile");
    auto old=map_fixture::fixture();
    check(crimson_nav::read_map(old,0,map_fixture::root,state),"old build still readable");
    check(!crimson_nav::read_map(old,0,map_fixture::root,state,build),"old object rejected under new profile");
    memory.put(map_fixture::mini_view_address+0xb0,std::uint8_t(0x90));
    crimson_nav::ReaderCore core(build);
    core.accept_scan({map_fixture::root,0,1,true,map_fixture::world,1});
    const crimson_nav::RoadGraph roads{{{0,0,0},{100,0,0}},{{0,1}}};
    const auto view=core.sample(memory,0,roads,1000);
    check(view.valid&&view.navigation.active&&view.route&&view.route->valid,"new profile passes through integrated route reader");
    check(view.world_projection.valid&&!view.world_missing,"world identity recovery uses new profile");
    check(view.build.minimap_root==89530200,"snapshot preserves selected build for render callback");
    const auto display=crimson_nav::read_route_display_projection(memory,0,view.root,view.world_root,view.state,view.geometry,3840,2160,view.build);
    check(display.valid&&display.tx==1920&&display.ty==1080,"new profile reaches per-frame 4K projection");
    // Exercise production discovery against this test process, not game memory.
    // Relocate the synthetic object's pointers into its actual heap allocation.
    auto live_fixture=memory;
    live_fixture.name(map_fixture::world_body,"WorldMapBody");
    const auto allocation=reinterpret_cast<std::uintptr_t>(live_fixture.bytes.data());
    for(std::size_t offset=0;offset+8<=live_fixture.bytes.size();offset+=8){
        std::uintptr_t value{};std::memcpy(&value,live_fixture.bytes.data()+offset,8);
        if(value>=0x10000&&value<live_fixture.bytes.size())live_fixture.put(offset,value+allocation);
    }
    crimson_nav::ProcessMemory process_memory{GetCurrentProcess()};std::atomic<bool> scanning{false};
    const auto found=crimson_nav::discover_map(process_memory,0,scanning,build);
    check(found.complete&&found.candidates==1&&found.root==allocation+map_fixture::root,"production discovery selects new minimap profile");
    check(found.world_candidates==1&&found.world_root==allocation+map_fixture::world,"production discovery selects new full-map profile");
    crimson_nav::ReaderCore reacquired(build);reacquired.accept_scan(found);
    const auto sampled=reacquired.sample(process_memory,0,roads,2000);
    check(sampled.valid&&sampled.navigation.active&&sampled.world_projection.valid,"discovered new-build roots pass production reads");
    std::atomic<bool> cancelled{true};
    check(!crimson_nav::identify_game_build("work/build/nonexistent-game.exe",cancelled),"cancelled or unreadable executable never selects a profile");
    std::cout<<checks<<" compatibility checks, "<<failures<<" failures\n";
    return failures?1:0;
}
