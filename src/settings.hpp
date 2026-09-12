#pragma once
#include <array>
#include <string>
#include <sstream>
#include <algorithm>
#include <cmath>
namespace crimson_nav {
struct Settings { std::array<float,4> color{177.f/255.f,124.f/255.f,1.f/255.f,1.f}; float width=6.f; bool visible=true; bool route_enabled=true; bool minimap_pulse=true; int minimap_style=1; bool auto_start=true; };
inline Settings parse_settings(const std::string &text) {
    Settings result;
    if(text.size()>4096) return result;
    bool explicit_style=false;
    std::istringstream input(text); std::string line;
    while(std::getline(input,line)) {
        const auto eq=line.find('='); if(eq==std::string::npos) continue;
        const auto key=line.substr(0,eq); std::istringstream value(line.substr(eq+1));
        if(key=="color") {
            std::array<float,4> c{};
            if(value>>c[0]>>c[1]>>c[2]>>c[3]) {
                value>>std::ws;
                if(value.eof() && std::all_of(c.begin(),c.end(),[](float v){return std::isfinite(v);})) {
                    for(auto &v:c) v=std::clamp(v,0.f,1.f);
                    result.color=c;
                }
            }
        } else if(key=="width") {
            float w; if(value>>w) {value>>std::ws;if(value.eof() && std::isfinite(w)) result.width=std::clamp(w,1.f,12.f);}
        } else if(key=="auto_start") {
            int v; if(value>>v) {value>>std::ws;if(value.eof() && (v==0||v==1)) result.auto_start=v!=0;}
        } else if(key=="minimap_style") {
            int v; if(value>>v) {value>>std::ws;if(value.eof() && v>=0 && v<=3) {result.minimap_style=v;explicit_style=true;}}
        } else if(key=="minimap_pulse") {
            int v; if(value>>v) {value>>std::ws;if(value.eof() && (v==0||v==1)) result.minimap_pulse=v!=0;}
        } else if(key=="route_enabled") {
            int v; if(value>>v) {value>>std::ws;if(value.eof() && (v==0||v==1)) result.route_enabled=v!=0;}
        } else if(key=="visible") {
            int v; if(value>>v) {value>>std::ws;if(value.eof() && (v==0||v==1)) result.visible=v!=0;}
        }
    }
    if(!explicit_style)result.minimap_style=result.minimap_pulse?1:0;
    result.minimap_pulse=result.minimap_style==1;
    return result;
}
inline std::string serialize_settings(const Settings &s) {
    std::ostringstream out; out.precision(9);
    out<<"color="<<s.color[0]<<' '<<s.color[1]<<' '<<s.color[2]<<' '<<s.color[3]<<"\nwidth="<<s.width<<"\nvisible="<<(s.visible?1:0)<<'\n';
    out<<"route_enabled="<<(s.route_enabled?1:0)<<'\n';
    out<<"minimap_style="<<s.minimap_style<<'\n';
    out<<"minimap_pulse="<<(s.minimap_style==1?1:0)<<'\n';
    out<<"auto_start="<<(s.auto_start?1:0)<<'\n';
    return out.str();
}
}
