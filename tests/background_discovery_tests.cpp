#include "background_discovery.hpp"
#include "reader_core.hpp"
#include "support/map_fixture.hpp"
#include <iostream>
#include <stdexcept>
#include <thread>
int main(){
    using namespace crimson_nav;
    int checks=0,failures=0;auto check=[&](bool ok,const char *name){++checks;if(!ok){++failures;std::cerr<<"FAIL: "<<name<<'\n';}};
    BackgroundDiscovery scan;std::promise<void> entered,release;auto gate=release.get_future().share();
    scan.start([&](const std::atomic<bool> &){entered.set_value();gate.wait();Discovery d;d.complete=true;d.root=123;return d;});
    check(entered.get_future().wait_for(std::chrono::seconds(2))==std::future_status::ready,"background scan starts");
    Discovery result;
    check(!scan.take(result)&&scan.running(),"pending scan can be polled without blocking");
    check(!scan.start([](const std::atomic<bool> &){return Discovery{};}),"only one scan runs at once");
    ReaderCore core;Discovery roots;roots.complete=true;roots.candidates=roots.world_candidates=1;
    roots.root=map_fixture::root;roots.world_root=map_fixture::world;core.accept_scan(roots);
    auto m=map_fixture::fixture();RoadGraph graph{{{0,0,0},{100,0,0}},{{0,1}}};
    auto sample=core.sample(m,0,graph,100);
    check(sample.valid&&sample.route&&sample.route->valid&&scan.running(),"actual reader keeps producing route during blocked scan");
    release.set_value();
    for(int i=0;i<2000&&!scan.take(result);++i)std::this_thread::sleep_for(std::chrono::milliseconds(1));
    check(result.complete&&result.root==123&&!scan.running(),"finished result is delivered once");
    check(!scan.take(result),"finished result is not replayed");
    scan.start([](const std::atomic<bool> &)->Discovery{throw std::runtime_error("test scan failure");});
    for(int i=0;i<2000&&!scan.take(result);++i)std::this_thread::sleep_for(std::chrono::milliseconds(1));
    check(!scan.running()&&!result.complete&&!result.root,"scan exception becomes unsuccessful result");
    std::atomic<bool> cancelled{false};std::promise<void> cancel_entered;
    {
        BackgroundDiscovery pending;
        pending.start([&](const std::atomic<bool> &stop){
            cancel_entered.set_value();while(!stop.load())std::this_thread::sleep_for(std::chrono::milliseconds(1));
            cancelled=true;return Discovery{};
        });
        check(cancel_entered.get_future().wait_for(std::chrono::seconds(2))==std::future_status::ready,"cancellable scan starts");
    }
    check(cancelled,"destruction cancels and joins pending scan");
    if(!failures)std::cout<<"PASS: "<<checks<<" background discovery cases\n";
    return failures?1:0;
}
