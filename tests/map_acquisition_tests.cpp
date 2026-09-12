#include "map_acquisition.hpp"
#include <iostream>
int main(){
    using namespace crimson_nav;
    int failures=0;auto check=[&](bool ok,const char *name){if(!ok){++failures;std::cerr<<"FAIL: "<<name<<'\n';}};
    Discovery known{};known.root=0x10000;known.world_root=0x20000;
    Discovery missing{};missing.complete=true;
    auto result=merge_discovery(known,missing);
    check(result.root==0x10000&&result.world_root==0x20000,"failed scan retains old addresses for validated retry");
    Discovery fresh{};fresh.root=0x30000;fresh.world_root=0x40000;fresh.candidates=fresh.world_candidates=1;
    result=merge_discovery(known,fresh);
    check(result.root==0x10000&&result.world_root==0x20000,"incomplete scan cannot replace either root");
    fresh.complete=true;fresh.candidates=2;
    result=merge_discovery(known,fresh);
    check(result.root==0x10000&&result.world_root==0x40000,"world root recovers independently of ambiguous minimap scan");
    fresh.candidates=1;fresh.world_candidates=2;
    result=merge_discovery(known,fresh);
    check(result.root==0x30000&&result.world_root==0x20000,"minimap recovers independently of ambiguous world map");
    fresh.world_candidates=1;result=merge_discovery(known,fresh);
    check(result.root==0x30000&&result.world_root==0x40000,"complete unique scan replaces both addresses");
    result=merge_discovery({},missing);
    check(!result.root&&!result.world_root,"failed first scan cannot invent roots");
    fresh.root=fresh.world_root=0;result=merge_discovery(known,fresh);
    check(result.root==0x10000&&result.world_root==0x20000,"null addresses cannot replace known roots");
    if(!failures)std::cout<<"PASS: 7 map acquisition cases\n";
    return failures?1:0;
}
