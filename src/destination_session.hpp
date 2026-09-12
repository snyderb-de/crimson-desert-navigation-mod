#pragma once
#include "target_reader.hpp"
#include "map_reader.hpp"
#include <cstdint>
namespace crimson_nav {
struct DestinationView {TargetState target{};bool active=false,retained=false,clear_pending=false;};
class DestinationSession {
    DestinationView view;
    bool seen_on_full_map=false,confirming=false,full_map_clear_pending=false;
    std::uint64_t clear_since{},last_clear{},last_sample{};
    bool sampled=false;
public:
    DestinationView update(const TargetState &observed,const MapState &state,
        bool minimap_ready,bool worldmap_ready,std::uint64_t now){
        if(sampled&&(now<last_sample||now-last_sample>250)){
            seen_on_full_map=false;confirming=false;full_map_clear_pending=false;
        }
        sampled=true;last_sample=now;
        if(!worldmap_ready)seen_on_full_map=false;
        if(observed.status==TargetStatus::candidate){
            view={observed,true,false,false};confirming=false;full_map_clear_pending=false;
            if(worldmap_ready)seen_on_full_map=true;
            return view;
        }
        view.retained=view.active;view.clear_pending=false;
        // The game removes the destination on arrival. This proximity filter
        // only corroborates removal; it NEVER completes a present target.
        // Five world units is an experimental corroboration tolerance, not an
        // arrival trigger. UI destination Y==0 may be unspecified.
        const auto *target=view.target.xyz;
        const bool near_removed_destination=minimap_ready&&state.distance_valid&&state.distance==0&&
            std::isfinite(state.xyz[0])&&std::isfinite(state.xyz[1])&&std::isfinite(state.xyz[2])&&
            std::hypot(double(state.xyz[0])-target[0],double(state.xyz[2])-target[2])<=5.&&
            (target[1]==0.f||std::abs(double(state.xyz[1])-target[1])<=5.);
        const bool clear=view.active&&observed.status==TargetStatus::no_candidate&&
            state.destination_ui_valid&&!state.destination_candidate&&
            ((worldmap_ready&&seen_on_full_map)||near_removed_destination||
             (full_map_clear_pending&&minimap_ready));
        if(!clear){confirming=false;full_map_clear_pending=false;return view;}
        view.clear_pending=true;
        if(worldmap_ready&&seen_on_full_map)full_map_clear_pending=true;
        if(!confirming||now<last_clear||now-last_clear>250){confirming=true;clear_since=now;}
        last_clear=now;
        if(now-clear_since>=200){view={};confirming=false;seen_on_full_map=false;full_map_clear_pending=false;}
        return view;
    }
};
}
