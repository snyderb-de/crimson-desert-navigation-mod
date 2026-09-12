#pragma once
// Geometric routing only. Archive traversal flags are not yet understood.
#include <vector>
#include <cmath>
#include <limits>
#include <queue>
#include <algorithm>
#include <utility>
namespace crimson_nav {
struct RoadPoint { double x{},y{},z{}; };
struct RoadEdge { size_t a{},b{}; };
struct RoadGraph { std::vector<RoadPoint> points; std::vector<RoadEdge> edges; };
struct RoadRoute { bool valid{}; double length{}; std::vector<RoadPoint> points; };
inline bool finite(RoadPoint p) {return std::isfinite(p.x)&&std::isfinite(p.y)&&std::isfinite(p.z);}
inline double road_distance(RoadPoint a,RoadPoint b) {
    return std::hypot(std::hypot(a.x-b.x,a.z-b.z),a.y-b.y);
}
// Select the nearest point on a segment in map X/Z, not the nearest stored vertex.
// Y is interpolated from the road: destination UI Y is sometimes zero.
inline RoadRoute road_route(const RoadGraph &g,RoadPoint start,RoadPoint destination) {
    RoadRoute result;
    if(!finite(start)||!finite(destination)||g.edges.empty()||g.points.size()>10000000||g.edges.size()>20000000)return result;
    for(auto p:g.points)if(!finite(p))return result;
    for(auto e:g.edges)if(e.a>=g.points.size()||e.b>=g.points.size())return result;
    struct Snap {size_t edge{};double t{},distance=std::numeric_limits<double>::infinity();RoadPoint point;};
    auto snap=[&](RoadPoint p) {
        Snap best;
        for(size_t i=0;i<g.edges.size();++i) {
            const auto e=g.edges[i];const auto a=g.points[e.a],b=g.points[e.b];
            const double dx=b.x-a.x,dz=b.z-a.z,den=dx*dx+dz*dz;
            if(den<=0)continue;
            const double t=std::clamp(((p.x-a.x)*dx+(p.z-a.z)*dz)/den,0.,1.);
            RoadPoint q{a.x+t*dx,a.y+t*(b.y-a.y),a.z+t*dz};
            const double d=std::hypot(p.x-q.x,p.z-q.z);
            if(d<best.distance)best={i,t,d,q};
        }
        return best;
    };
    const auto s=snap(start),t=snap(destination);
    if(!std::isfinite(s.distance)||!std::isfinite(t.distance))return result;
    const size_t n=g.points.size(),source=n,target=n+1;
    std::vector<std::vector<std::pair<size_t,double>>> adj(n+2);
    auto link=[&](size_t a,size_t b,double length){adj[a].push_back({b,length});adj[b].push_back({a,length});};
    for(auto e:g.edges)link(e.a,e.b,road_distance(g.points[e.a],g.points[e.b]));
    for(auto item: {std::make_pair(source,s),std::make_pair(target,t)}) {
        const auto e=g.edges[item.second.edge];
        link(item.first,e.a,road_distance(item.second.point,g.points[e.a]));
        link(item.first,e.b,road_distance(item.second.point,g.points[e.b]));
    }
    if(s.edge==t.edge)link(source,target,road_distance(s.point,t.point));
    const double inf=std::numeric_limits<double>::infinity();
    std::vector<double> distances(n+2,inf);std::vector<size_t> previous(n+2,n+2);
    using Item=std::pair<double,size_t>;
    std::priority_queue<Item,std::vector<Item>,std::greater<Item>> queue;
    distances[source]=0;queue.push({0,source});
    while(!queue.empty()) {
        const auto current=queue.top();queue.pop();const auto u=current.second;
        if(current.first!=distances[u])continue;
        if(u==target)break;
        for(auto edge:adj[u]) {
            const auto v=edge.first;const auto next=current.first+edge.second;
            if(next<distances[v]) {distances[v]=next;previous[v]=u;queue.push({next,v});}
        }
    }
    if(!std::isfinite(distances[target]))return result; // Do not move the endpoint to another road.
    for(size_t u=target;;u=previous[u]) {
        const auto p=u==source?s.point:u==target?t.point:g.points[u];
        if(result.points.empty()||road_distance(result.points.back(),p)>1e-9)result.points.push_back(p);
        if(u==source)break;
    }
    std::reverse(result.points.begin(),result.points.end());
    result.length=distances[target];result.valid=true;return result;
}
}
