#pragma once
#include "road_route.hpp"
namespace crimson_nav {
struct RouteProgress { RoadPoint point{};size_t next{};double distance{};bool valid=false; };
// Trim only the displayed prefix. Keep the solver's road path and destination.
inline RouteProgress route_progress(const RoadRoute &route,RoadPoint player){
    RouteProgress result;
    if(!route.valid||route.points.empty()||!finite(player))return result;
    double best=std::numeric_limits<double>::infinity(),along=0;
    if(route.points.size()==1)return {route.points.front(),1,0,true};
    for(size_t i=1;i<route.points.size();++i){
        auto a=route.points[i-1],b=route.points[i];
        const double dx=b.x-a.x,dz=b.z-a.z,den=dx*dx+dz*dz;
        const double t=den>0?std::clamp(((player.x-a.x)*dx+(player.z-a.z)*dz)/den,0.,1.):0;
        RoadPoint q{a.x+t*dx,a.y+t*(b.y-a.y),a.z+t*dz};
        const double error=std::hypot(player.x-q.x,player.z-q.z),length=road_distance(a,b);
        if(error<best){best=error;result={q,i,along+t*length,true};}
        along+=length;
    }
    return result;
}
}
