#include "reader_core.hpp"
#include "support/map_fixture.hpp"
#include <iostream>
int main(){
    using namespace crimson_nav;
    using namespace map_fixture;
    const RoadGraph roads{{{0,0,0},{100,0,0}},{{0,1}}};
    Discovery found{};found.complete=true;found.candidates=found.world_candidates=1;found.root=root;found.world_root=world;
    ReaderCore core;core.accept_scan(found);auto m=fixture();
    int checks=0,failures=0;auto check=[&](bool ok,const char *name){++checks;if(!ok){++failures;std::cerr<<"FAIL: "<<name<<'\n';}};
    auto first=core.sample(m,0,roads,100);
    check(first.valid&&first.route&&first.route->valid,"reader starts valid route");
    m.put(root,std::uintptr_t(0));auto missing=core.sample(m,0,roads,200);
    check(!missing.valid&&missing.navigation.active&&missing.route==first.route,"failed map read hides drawing but retains session");
    Discovery empty{};empty.complete=true;core.accept_scan(empty);
    m=fixture();auto recovered=core.sample(m,0,roads,300);
    check(recovered.valid&&recovered.root==root&&recovered.route==first.route,"valid old root resumes immediately after failed scan");
    m.put(target_icon+0xde,std::uint16_t(7));auto unknown=core.sample(m,0,roads,400);
    check(unknown.valid&&unknown.navigation.retained&&unknown.route==first.route,"unknown target state retains drawable route");
    m=fixture();m.put(property+0x11,std::uint8_t(2));auto label=core.sample(m,0,roads,500);
    check(label.valid&&label.route==first.route&&!label.state.destination_ui_valid,"animated optional label preserves route");
    m=fixture();m.put(mini_view_address+0xb0,std::uint8_t(0x90));m.put(world_view+0xb0,std::uint8_t(0xd0));
    auto full=core.sample(m,0,roads,600);check(full.world_projection.valid,"full map can arm clear observation");
    for(unsigned c=0;c<2;++c)m.put(root+0x1000+c*0x1000+24,std::uint32_t(1));
    m.put(property+0x11,std::uint8_t(0));core.sample(m,0,roads,700);
    auto cleared=core.sample(m,0,roads,900);check(!cleared.navigation.active&&!cleared.route,"coherent removal ends reader route");
    m.put(root,std::uintptr_t(0));auto later=core.sample(m,0,roads,1000);
    check(!later.route&&!later.navigation.active,"later failure cannot resurrect removed route");
    m=fixture();auto resumed=core.sample(m,0,roads,1100);check(resumed.route&&resumed.route->valid,"selecting destination again restarts route");
    m.put(world,std::uintptr_t(0));auto bad_world=core.sample(m,0,roads,1200);
    check(bad_world.valid&&!bad_world.world_missing&&bad_world.route,"unused missing full map does not cause repeated gameplay scans");
    m.put(mini_view_address+0xb0,std::uint8_t(0x90));bad_world=core.sample(m,0,roads,1300);
    check(bad_world.valid&&bad_world.world_missing&&bad_world.route,"needed full map can recover independently of readable minimap");
    m=fixture();m.put(mini_view_address+0xb0,std::uint8_t(0x90));
    m.put(world_body+0x38,std::uintptr_t(0x200000));
    check(core.sample(m,0,roads,1400).world_missing,"unreadable world parent triggers reacquisition even with surviving root and body");
    ReaderCore fresh;fresh.accept_scan(found);m=fixture();
    for(unsigned c=0;c<2;++c)m.put(root+0x1000+c*0x1000+24,std::uint32_t(1));
    check(!fresh.sample(m,0,roads,100).route,"fresh empty destination never routes to retained map markers");
    for(bool replace_body:{false,true}){
        ReaderCore rebuilt;rebuilt.accept_scan(found);m=fixture();
        m.put(mini_view_address+0xb0,std::uint8_t(0x90));m.put(world_view+0xb0,std::uint8_t(0xd0));
        rebuilt.sample(m,0,roads,100);
        const std::uintptr_t new_body=replace_body?0x60000:world_body,new_parent=0x61000;
        m.put(world+8,new_body);m.put(new_body+0x38,new_parent);m.put(new_parent+0xb0,std::uint8_t(0xd0));
        for(unsigned c=0;c<2;++c)m.put(root+0x1000+c*0x1000+24,std::uint32_t(1));
        m.put(property+0x11,std::uint8_t(0));rebuilt.sample(m,0,roads,200);
        check(rebuilt.sample(m,0,roads,400).navigation.active,"rebuilt world body or parent cannot inherit old clear evidence");
    }
    for(int transition=0;transition<3;++transition){
        ReaderCore close;close.accept_scan(found);m=fixture();
        m.put(mini_view_address+0xb0,std::uint8_t(0x90));m.put(world_view+0xb0,std::uint8_t(0xd0));
        close.sample(m,0,roads,100);
        for(unsigned c=0;c<2;++c)m.put(root+0x1000+c*0x1000+24,std::uint32_t(1));
        m.put(property+0x11,std::uint8_t(0));close.sample(m,0,roads,200);
        m.put(mini_view_address+0xb0,std::uint8_t(0xd0));m.put(world_view+0xb0,std::uint8_t(0x90));
        if(transition==1)m.put(world_body+0x38,std::uintptr_t(0x200000));
        if(transition==2){m.put(world+8,std::uintptr_t(0x60000));m.put(0x60038,std::uintptr_t(0x61000));m.put(0x610b0,std::uint8_t(0x90));}
        close.sample(m,0,roads,300);auto closed=close.sample(m,0,roads,400);
        check(closed.navigation.active==(transition!=0),"quick close only confirms removal when hidden world identity survives");
    }
    if(!failures)std::cout<<"PASS: "<<checks<<" reader integration cases\n";
    return failures?1:0;
}
