#pragma once
#include <cstdint>
namespace crimson_nav {
struct Discovery {std::uintptr_t root{};std::uint64_t bytes{};unsigned candidates{};bool complete{};std::uintptr_t world_root{};unsigned world_candidates{};};
// Old addresses are retry candidates, never permission to render stale data.
// Every use still passes the normal live identity/value validation.
inline Discovery merge_discovery(Discovery previous,const Discovery &scan){
    if(scan.complete){
        if(scan.candidates==1&&scan.root)previous.root=scan.root;
        if(scan.world_candidates==1&&scan.world_root)previous.world_root=scan.world_root;
    }
    previous.bytes=scan.bytes;previous.candidates=scan.candidates;
    previous.world_candidates=scan.world_candidates;previous.complete=scan.complete;
    return previous;
}
}
