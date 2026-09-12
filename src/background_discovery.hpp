#pragma once
#include "map_acquisition.hpp"
#include <atomic>
#include <chrono>
#include <future>
#include <utility>
namespace crimson_nav {
class BackgroundDiscovery {
    std::atomic<bool> cancel{false};
    std::future<Discovery> work;
public:
    ~BackgroundDiscovery(){cancel=true;if(work.valid())work.wait();}
    bool running()const{return work.valid();}
    template<class Scan>bool start(Scan scan){
        if(work.valid())return false;
        cancel=false;
        work=std::async(std::launch::async,[this,scan=std::move(scan)]() mutable {return scan(cancel);});
        return true;
    }
    bool take(Discovery &result){
        if(!work.valid()||work.wait_for(std::chrono::milliseconds(0))!=std::future_status::ready)return false;
        try {result=work.get();}catch(...){result={};}
        return true;
    }
};
}
