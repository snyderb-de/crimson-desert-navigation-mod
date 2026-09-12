#pragma once
#include "native_map_reader.hpp"
#include "worldmap_roads.hpp"
#include "reader_core.hpp"
#include "read_recovery.hpp"
#include "background_discovery.hpp"
#include <memory>
#include <mutex>
#include <thread>
namespace crimson_nav {
class ReaderService {
    std::atomic<bool> stopped{false};std::thread worker;std::mutex mutex;ReaderView view;
    void publish(const std::string &status) {
        std::lock_guard<std::mutex> guard(mutex);view={};view.status=status;
    }
    void publish(ReaderView sample){std::lock_guard<std::mutex> guard(mutex);view=std::move(sample);}
public:
    ~ReaderService(){stop();}
    void stop(){stopped=true;if(worker.joinable())worker.join();publish("Diagnostics stopped.");}
    ReaderView snapshot(){std::lock_guard<std::mutex> guard(mutex);return view;}
    void start(){
        stop();stopped=false;publish("Detecting game map layout...");
        try {worker=std::thread([this]{
            try {
                wchar_t path[32768]{};DWORD n=GetModuleFileNameW(nullptr,path,32768);
                const auto build=n&&n<32768?identify_game_build(path,stopped):std::nullopt;
                if(!build){publish("Map layout unavailable or detection cancelled. Report this if it persists after an update.");return;}
                publish("Loading road network from installed game data...");
                auto roads=load_worldmap_roads(std::filesystem::path(path).parent_path().parent_path());
                ProcessMemory memory{GetCurrentProcess()};auto base=reinterpret_cast<std::uintptr_t>(GetModuleHandleW(nullptr));
                publish("Discovering minimap (bounded background scan)...");
                ReaderCore core(*build);core.accept_scan(discover_map(memory,base,stopped,*build));
                ReadRecovery recovery,world_recovery;
                BackgroundDiscovery scan;
                std::uint64_t next_scan{};
                std::string acquisition="Initial discovery finished.";
                while(!stopped.load()) {
                    const auto now=GetTickCount64();
                    Discovery result;
                    if(scan.take(result)){
                        core.accept_scan(result);
                        acquisition="Scan complete="+std::to_string(result.complete)+
                            " mini candidates="+std::to_string(result.candidates)+
                            " full candidates="+std::to_string(result.world_candidates)+
                            ". Previous addresses retained if no unique replacement.";
                        next_scan=now+30000;
                        recovery.discovery_attempted(now);world_recovery.discovery_attempted(now);
                    }
                    auto sample=core.sample(memory,base,roads,now);
                    if(sample.valid)recovery.succeeded();else recovery.failed(now);
                    if(sample.world_missing)world_recovery.failed(now);else world_recovery.succeeded();
                    if(!scan.running()&&now>=next_scan&&(recovery.needs_discovery(now)||world_recovery.needs_discovery(now))){
                        acquisition="Reacquiring map in background; validated sampling continues.";
                        scan.start([memory,base,build](const std::atomic<bool> &cancel) mutable {return discover_map(memory,base,cancel,*build);});
                    }
                    sample.acquisition=acquisition;publish(std::move(sample));
                    for(int i=0;i<3&&!stopped.load();++i)Sleep(10);
                }
            } catch(...) {publish("Reader stopped after an internal error.");}
        });} catch(...) {publish("Could not start diagnostic worker.");}
    }
};
}
