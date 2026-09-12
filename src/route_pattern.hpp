#pragma once
#include <algorithm>
#include <cmath>
namespace crimson_nav {
// World-distance phase remains fixed through clipping and player progress.
// Half-open intervals ensure dots at shared vertices are drawn only once.
template<class Draw> void route_pattern(double start,double end,bool dotted,Draw draw){
    if(!std::isfinite(start)||!std::isfinite(end)||start<0||end<=start)return;
    const double spacing=dotted?5.:12.;
    if(dotted){
        double at=std::ceil(start/spacing)*spacing;
        for(unsigned n=0;at<end&&n<4096;++n,at+=spacing){
            const double t=(at-start)/(end-start);draw(t,t);
        }
    }else{
        double at=std::floor(start/spacing)*spacing;
        for(unsigned n=0;at<end&&n<4096;++n,at+=spacing){
            const double lo=std::max(start,at),hi=std::min(end,at+7.);
            if(hi>lo)draw((lo-start)/(end-start),(hi-start)/(end-start));
        }
    }
}
}
