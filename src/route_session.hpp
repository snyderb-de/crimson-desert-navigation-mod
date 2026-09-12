#pragma once
#include "road_route.hpp"
#include <memory>
#include <cstdint>
namespace crimson_nav {
class RouteSession {
    RoadPoint destination{};
    std::shared_ptr<const RoadRoute> route;
    RoadPoint planned_from{};
    std::uint64_t planned_at{},deviated_at{};
    bool deviating=false;
public:
    void clear(){route.reset();deviating=false;}
    std::shared_ptr<const RoadRoute> update(const RoadGraph &graph,RoadPoint player,RoadPoint target,std::uint64_t now=0){
        bool replan=!route||road_distance(destination,target)>.05;
        if(!replan){
            double distance=std::numeric_limits<double>::infinity();
            for(size_t i=1;i<route->points.size();++i){
                auto a=route->points[i-1],b=route->points[i];
                double dx=b.x-a.x,dz=b.z-a.z,den=dx*dx+dz*dz;
                double t=den>0?std::clamp(((player.x-a.x)*dx+(player.z-a.z)*dz)/den,0.,1.):0;
                distance=std::min(distance,std::hypot(player.x-a.x-t*dx,player.z-a.z-t*dz));
            }
            const bool off=distance>15&&std::hypot(player.x-planned_from.x,player.z-planned_from.z)>=10;
            if(!off)deviating=false;
            else if(!deviating){deviating=true;deviated_at=now;}
            else replan=now-deviated_at>=2000&&now-planned_at>=5000;
        }
        if(replan){
            route=std::make_shared<RoadRoute>(road_route(graph,player,target));destination=target;
            planned_from=player;planned_at=now;deviating=false;
        }
        return route;
    }
};
}
