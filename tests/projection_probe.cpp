#include "native_map_reader.hpp"
#include "map_projection.hpp"
#include <iostream>
int main(int argc,char **argv){if(argc!=4)return 2;
 HANDLE h=OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,std::stoul(argv[1]));if(!h)return 3;
 crimson_nav::ProcessMemory m{h};auto mini=std::stoull(argv[2],nullptr,0),world=std::stoull(argv[3],nullptr,0);crimson_nav::MapState s{};
 crimson_nav::read_map(m,0x140000000,mini,s);auto g=crimson_nav::read_minimap_geometry(m,0x140000000,mini);
 for(auto p:{crimson_nav::read_minimap_projection(m,mini,s,g),crimson_nav::read_worldmap_projection(m,0x140000000,world)})
 std::cout<<p.valid<<" "<<p.ax<<" "<<p.az<<" "<<p.bx<<" "<<p.bz<<" "<<p.tx<<" "<<p.ty<<" player="<<p.ax*s.xyz[0]+p.az*s.xyz[2]+p.tx<<","<<p.bx*s.xyz[0]+p.bz*s.xyz[2]+p.ty<<"\n";
 CloseHandle(h);
}
