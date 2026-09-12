#include "worldmap_roads.hpp"
#include <iostream>
int main(int argc,char **argv){
 if(argc!=2)return 2;
 try{auto g=crimson_nav::load_worldmap_roads(argv[1]);
 auto r=crimson_nav::road_route(g,{-4857.43,371.65,-3361.8},{-5342.77,0,-2420.16});
 std::cout<<"Native archive loader: "<<g.points.size()<<" nodes "<<g.edges.size()<<" segments. Route "<<r.valid<<" points "<<r.points.size()<<" length "<<r.length<<"\n";
 return r.valid?0:3;
 }catch(const std::exception &e){std::cerr<<e.what();return 1;}
}
