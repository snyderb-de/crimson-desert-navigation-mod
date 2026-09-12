#pragma once
#include <cmath>
#include <algorithm>
namespace crimson_nav {
// Distance along the full route anchors the animation across segment boundaries
// and minimap clipping. Increasing phase moves toward the destination.
template<class Draw> void route_pulse(double start,double end,double seconds,Draw draw){
    if(!std::isfinite(start)||!std::isfinite(end)||!std::isfinite(seconds)||end<=start)return;
    constexpr double spacing=18,length=6;
    const double phase=std::fmod(std::max(0.,seconds)*10.,spacing);
    double head=std::floor((start-phase-length)/spacing)*spacing+phase;
    for(;head<end;head+=spacing){
        for(int band=0;band<4;++band){
            const double a=std::max(start,head+band*length/4),b=std::min(end,head+(band+1)*length/4);
            if(b>a)draw((a-start)/(end-start),(b-start)/(end-start),.25+.25*band);
        }
    }
}
}
