#pragma once
#include "map_acquisition.hpp"
#include "destination_session.hpp"
#include "map_projection.hpp"
#include "route_session.hpp"
#include <string>
namespace crimson_nav {
struct ReaderView {
    GameBuild build=game_build_2760;
    MapState state{};TargetState target{};DestinationView navigation{};
    MinimapGeometry geometry{};MapProjection mini_projection{},world_projection{};
    std::shared_ptr<const RoadRoute> route;
    std::uintptr_t root{},world_root{};std::uint64_t sampled_at{};
    bool valid=false,world_missing=false;
    std::string status="Diagnostics stopped.",acquisition;
};
class ReaderCore {
    GameBuild build;
    Discovery roots;
    RouteSession route_session;
    DestinationSession destination;
    std::shared_ptr<const RoadRoute> route;
    std::uintptr_t last_world_root{},last_world_body{},last_world_parent{};
    struct WorldIdentity {std::uintptr_t root{},body{},parent{};};
    template<class Memory>WorldIdentity read_world_identity(Memory &m,std::uintptr_t base)const{
        std::uintptr_t vt{},body{},parent{},again{};std::uint8_t flags{};
        const bool valid=roots.world_root&&m.read(roots.world_root,&vt,8)&&vt==base+build.worldmap_root&&
            m.read(roots.world_root+8,&body,8)&&body>=0x10000&&body<0x7fffffff0000ull-0x40&&
            m.read(body+0x38,&parent,8)&&parent>=0x10000&&parent<0x7fffffff0000ull-0xb1&&
            m.read(parent+0xb0,&flags,1)&&m.read(body+0x38,&again,8)&&again==parent&&
            m.read(roots.world_root,&again,8)&&again==vt&&
            m.read(roots.world_root+8,&again,8)&&again==body;
        return valid?WorldIdentity{roots.world_root,body,parent}:WorldIdentity{};
    }
public:
    explicit ReaderCore(const GameBuild &profile=game_build_2760):build(profile){}
    void accept_scan(const Discovery &scan){
        const auto previous=roots;roots=merge_discovery(roots,scan);
        if(previous.root!=roots.root||previous.world_root!=roots.world_root)
            destination.update({}, {},false,false,0); // Reset clear evidence, retain intent.
    }
    template<class Memory>ReaderView sample(Memory &memory,std::uintptr_t base,const RoadGraph &roads,std::uint64_t now){
        ReaderView view;view.build=build;view.root=roots.root;view.world_root=roots.world_root;view.sampled_at=now;
        view.valid=read_map(memory,base,roots.root,view.state,build);
        if(!view.valid){
            view.navigation=destination.update({}, {},false,false,now);view.route=route;
            view.status="Map state temporarily unavailable. Destination retained; retrying.";return view;
        }
        view.target=read_target(memory,base,roots.root,build);
        view.geometry=read_minimap_geometry(memory,base,roots.root,build);
        view.mini_projection=read_minimap_projection(memory,roots.root,view.state,view.geometry);
        view.world_projection=read_worldmap_projection(memory,base,roots.world_root,build);
        const auto identity=read_world_identity(memory,base);
        view.world_missing=view.state.map_hidden_candidate&&!identity.root;
        const auto &world=view.world_projection;
        if(identity.root!=last_world_root||identity.body!=last_world_body||identity.parent!=last_world_parent)
            destination.update({}, {},false,false,now);
        last_world_root=identity.root;last_world_body=identity.body;last_world_parent=identity.parent;
        if(world.valid&&(world.identity_root!=identity.root||world.identity_body!=identity.body||world.identity_parent!=identity.parent))
            view.world_projection={};
        view.navigation=destination.update(view.target,view.state,
            !view.state.map_hidden_candidate&&view.mini_projection.valid,
            view.world_projection.valid,now);
        if(view.navigation.active){
            const auto *target=view.navigation.target.xyz;
            route=route_session.update(roads,{view.state.xyz[0],view.state.xyz[1],view.state.xyz[2]},
                {target[0],target[1],target[2]},now);
            view.status=!route->valid?"No connected road route. No shortcut drawn.":
                view.navigation.clear_pending?"Confirming game destination clear; route retained.":
                view.navigation.retained?"Following retained destination; retrying game target observation.":
                "Road route ready. Off-road destination ends at nearest road point.";
        }else{
            view.status=route?"Game destination cleared; route ended.":"Set an active destination to show a road route.";
            route_session.clear();route.reset();
        }
        view.route=route;return view;
    }
};
}
