// Patch 2.04 minimap layout: members moved non-uniformly, the HUD rectangle is
// carried by MinimapDisplay, and the destination icon kind changed from 21 to 22.
#include "reader_core.hpp"
#include "support/map_fixture.hpp"
#include "support/worldmap_layout_fixture.hpp"
#include <iostream>

namespace {
// Rewrites the legacy fixture into the measured patch 2.04 member layout.
map_fixture::Memory patch_2_04_fixture(){
    using namespace map_fixture;
    auto m=fixture();
    const auto &legacy=crimson_nav::minimap_layout_legacy;
    const auto &next=crimson_nav::minimap_layout_2_04;
    struct Field {std::size_t from,to,size;};
    const Field fields[]={{legacy.player,next.player,12},{legacy.hud,next.hud,8},{legacy.terrain,next.terrain,8},
        {legacy.destination_icon,next.destination_icon,8},{legacy.canvas,next.canvas,8},
        {legacy.origin,next.origin,12},{legacy.distance,next.distance,4}};
    std::vector<std::vector<unsigned char>> saved;
    for(const auto &f:fields)saved.emplace_back(m.bytes.begin()+root+f.from,m.bytes.begin()+root+f.from+f.size);
    for(const auto &f:fields)std::memset(m.bytes.data()+root+f.from,0,f.size);
    for(std::size_t i=0;i<std::size(fields);++i)std::memcpy(m.bytes.data()+root+fields[i].to,saved[i].data(),fields[i].size);
    m.name(0x25000,"MinimapDisplay");
    // Every UI element gained an 8-byte member before its visibility flags.
    for(auto view:{mini_view_address,world_view}){
        const auto flags=m.bytes[view+0xb0];m.bytes[view+0xb0]=0;m.put(view+next.ui_flags,flags);
    }
    for(unsigned c=0;c<2;++c)m.put(root+0x1000+c*0x1000+8,std::uint64_t(next.destination_kind));
    for(auto a:{target_icon,retained_icon})m.put(a+0xd0,next.destination_kind);
    return m;
}
}

int main(){
    using namespace map_fixture;
    int checks=0,failures=0;
    auto check=[&](bool ok,const char *name){++checks;if(!ok){++failures;std::cerr<<"FAIL: "<<name<<'\n';}};
    const auto &build=crimson_nav::game_build_2760;

    auto legacy=fixture();
    check(crimson_nav::select_minimap_layout(legacy,0,root,build)==&crimson_nav::minimap_layout_legacy,
        "legacy member layout selected by canvas identity");

    auto m=patch_2_04_fixture();
    const auto *layout=crimson_nav::select_minimap_layout(m,0,root,build);
    check(layout==&crimson_nav::minimap_layout_2_04,"patch 2.04 member layout selected by canvas identity");
    if(!layout){std::cout<<checks<<" minimap layout checks, "<<failures<<" failures\n";return 1;}

    crimson_nav::MapState state{};
    check(crimson_nav::read_map(m,0,root,state,build,*layout)&&state.distance==100&&state.destination_candidate,
        "patch 2.04 player, distance and destination label readable");
    check(!crimson_nav::read_map(m,0,root,state,build),"patch 2.04 object rejected under legacy member layout");
    const auto target=crimson_nav::read_target(m,0,root,build,*layout);
    check(target.status==crimson_nav::TargetStatus::candidate&&target.address==target_icon&&target.xyz[0]==100,
        "patch 2.04 destination kind 22 readable");
    check(crimson_nav::read_target(m,0,root,build).status!=crimson_nav::TargetStatus::candidate,
        "legacy destination kind 21 is not assumed for patch 2.04");
    const auto geometry=crimson_nav::read_minimap_geometry(m,0,root,build,*layout);
    check(geometry.valid&&geometry.width==200&&geometry.x==50,"patch 2.04 MinimapDisplay circle readable");
    check(!crimson_nav::read_minimap_geometry(m,0,root,build).valid,"legacy MinimapHud member is not assumed for patch 2.04");
    check(crimson_nav::read_minimap_projection(m,root,state,geometry,*layout).valid,"patch 2.04 terrain canvas and origin readable");

    // The measured 0x23a presentation icon in the destination category is skipped,
    // while every uninvestigated flag value, including nearby ones, still rejects.
    auto presentation=m;presentation.put(retained_icon+0xde,std::uint16_t(0x23a));
    check(crimson_nav::read_target(presentation,0,root,build,*layout).status==crimson_nav::TargetStatus::candidate,
        "measured 0x23a presentation icon is not a destination and does not block one");
    auto unknown=m;unknown.put(retained_icon+0xde,std::uint16_t(7));
    check(crimson_nav::read_target(unknown,0,root,build,*layout).status==crimson_nav::TargetStatus::unavailable,
        "uninvestigated icon flags still reject the snapshot");
    for(std::uint16_t near:{std::uint16_t(0x23b),std::uint16_t(0x203),std::uint16_t(0x200),std::uint16_t(0x235)}){
        auto unmeasured=m;unmeasured.put(retained_icon+0xde,near);
        check(crimson_nav::read_target(unmeasured,0,root,build,*layout).status==crimson_nav::TargetStatus::unavailable,
            "unmeasured 0x200-family flag value still rejects the snapshot");
    }
    auto legacy_presentation=legacy;legacy_presentation.put(retained_icon+0xde,std::uint16_t(0x23a));
    check(crimson_nav::read_target(legacy_presentation,0,root,build).status==crimson_nav::TargetStatus::unavailable,
        "0x23a is not assumed for layouts where it was never measured");

    auto ambiguous=m;ambiguous.put(root+crimson_nav::minimap_layout_legacy.canvas,mini_canvas_address);
    check(!crimson_nav::select_minimap_layout(ambiguous,0,root,build),"canvas identity at two profile offsets is rejected");
    auto stale=m;stale.put(root,std::uintptr_t(1));
    check(!crimson_nav::select_minimap_layout(stale,0,root,build),"wrong minimap root identity selects no layout");
    auto none=m;none.put(root+crimson_nav::minimap_layout_2_04.canvas,std::uintptr_t(0));
    check(!crimson_nav::select_minimap_layout(none,0,root,build),"missing canvas identity selects no layout");

    // Visibility comes from the shifted UI element flags, never the old byte.
    check(state.map_hidden_candidate==false,"patch 2.04 visible minimap view read from +0xb8");
    auto hidden=m;hidden.put(mini_view_address+crimson_nav::minimap_layout_2_04.ui_flags,std::uint8_t(0x90));
    hidden.put(mini_view_address+0xb0,std::uint8_t(0xd0));
    check(crimson_nav::read_map(hidden,0,root,state,build,*layout)&&state.map_hidden_candidate,
        "patch 2.04 hidden minimap view ignores stale legacy flag byte");
    auto open=m;worldmap_layout_fixture(open,world_body,0x70000,1920,1080);
    open.put(world_view+crimson_nav::minimap_layout_2_04.ui_flags,std::uint8_t(0xd0));
    const auto full=crimson_nav::read_worldmap_projection(open,0,world,build,*layout);
    check(full.valid&&full.tx==960&&full.ty==540,"patch 2.04 open full map read from shifted visibility flag");
    check(!crimson_nav::read_worldmap_projection(open,0,world,build).valid,"legacy visibility offset is not assumed for patch 2.04");
    open.put(world_view+crimson_nav::minimap_layout_2_04.ui_flags,std::uint8_t(0x90));
    check(!crimson_nav::read_worldmap_projection(open,0,world,build,*layout).valid,"patch 2.04 closed full map stays hidden");

    // Integrated reader and per-frame display path select the same layout.
    worldmap_layout_fixture(m,world_body,0x70000,1920,1080);
    crimson_nav::ReaderCore core(build);core.accept_scan({root,0,1,true,world,1});
    const crimson_nav::RoadGraph roads{{{0,0,0},{100,0,0}},{{0,1}}};
    const auto view=core.sample(m,0,roads,1000);
    check(view.valid&&view.layout==layout&&view.navigation.active&&view.route&&view.route->valid,
        "patch 2.04 layout passes through integrated route reader");
    check(view.mini_projection.valid,"patch 2.04 minimap projection reaches integrated reader");
    const auto display=crimson_nav::read_route_display_projection(m,0,view.root,view.world_root,view.state,view.geometry,
        1920,1080,view.build,*view.layout);
    check(display.valid,"patch 2.04 layout reaches per-frame display projection");
    std::cout<<checks<<" minimap layout checks, "<<failures<<" failures\n";
    return failures?1:0;
}
