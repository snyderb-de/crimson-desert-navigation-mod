#pragma once
#include <cstdint>
namespace crimson_nav {
// Failed observations hide the route; they do not terminate navigation.
// Only sustained failure permits another expensive discovery scan.
class ReadRecovery {
    bool missing=false, attempted=false;
    std::uint64_t first{},last{};
public:
    void failed(std::uint64_t now){if(!missing){missing=true;first=now;}}
    bool needs_discovery(std::uint64_t now)const{return missing&&now-first>=2000&&(!attempted||now-last>=30000);}
    void discovery_attempted(std::uint64_t now){attempted=true;last=now;}
    void succeeded(){missing=false;attempted=false;}
};
}
