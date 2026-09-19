#include "map_projection.hpp"
#include <cassert>
#include <iostream>
#include <limits>
struct Memory {
 std::vector<unsigned char> bytes=std::vector<unsigned char>(1024*1024);
 bool read(std::uintptr_t p,void *v,size_t n){if(p>bytes.size()||n>bytes.size()-p)return false;std::memcpy(v,bytes.data()+p,n);return true;}
 template<class T>void put(size_t p,const T &v){std::memcpy(bytes.data()+p,&v,sizeof(v));}
};
int main(){
 Memory m;constexpr std::uintptr_t root=0x10000,body=0x20000,parent=0x21000;
 m.put(root,std::uintptr_t(89508976));m.put(root+8,body);m.put(body+0x38,parent);m.put(parent+0xb0,(unsigned char)0x40);
 float canvas[2]={1920,1080},anchor[5]={960,540,-1000,-2000,2};m.put(root+0xa58,canvas);m.put(root+0xa78,anchor);
 auto p=crimson_nav::read_worldmap_projection(m,0,root);assert(p.valid&&p.ax==.5&&p.bz==-.5&&p.tx==1460&&p.ty==-460);
 // Moving the cursor without panning must leave the projection unchanged.
 anchor[0]=1060;anchor[2]=-800;m.put(root+0xa78,anchor);
 auto moved=crimson_nav::read_worldmap_projection(m,0,root);assert(moved.valid&&moved.tx==p.tx&&moved.ty==p.ty);
 // Pan while cursor remains at the same UI position.
 anchor[2]=-600;m.put(root+0xa78,anchor);assert(crimson_nav::read_worldmap_projection(m,0,root).tx==1360);
 anchor[4]=1;m.put(root+0xa78,anchor);assert(crimson_nav::read_worldmap_projection(m,0,root).ax==1);
 m.put(parent+0xb0,(unsigned char)0);assert(!crimson_nav::read_worldmap_projection(m,0,root).valid);
 m.put(parent+0xb0,(unsigned char)0x40);anchor[4]=0;m.put(root+0xa78,anchor);assert(!crimson_nav::read_worldmap_projection(m,0,root).valid);
 anchor[4]=1;anchor[0]=32769;m.put(root+0xa78,anchor);assert(!crimson_nav::read_worldmap_projection(m,0,root).valid);
 anchor[0]=2000;m.put(root+0xa78,anchor);assert(crimson_nav::read_worldmap_projection(m,0,root).valid);
 canvas[0]=2560;m.put(root+0xa58,canvas);assert(crimson_nav::read_worldmap_projection(m,0,root).valid);
 // Patch 2.03 inserted fields before the full-map projection block. Locate the
 // structurally validated canvas/anchor pair instead of binding to one build's
 // exact member offsets.
 Memory shifted;shifted.put(root,std::uintptr_t(89508976));
 shifted.put(root+8,body);shifted.put(body+0x38,parent);shifted.put(parent+0xb0,(unsigned char)0x40);
 const float repurposed_canvas[2]={-10139.9f,-6028.83f};
 const float repurposed_anchor[5]={0,0,0,0,960};
 const float shifted_canvas[2]={1920,1080};
 const float shifted_anchor[5]={960,540,-10139.9f,-6028.83f,1.47744f};
 shifted.put(root+0xa58,repurposed_canvas);shifted.put(root+0xa78,repurposed_anchor);
 shifted.put(root+0xa68,shifted_canvas);shifted.put(root+0xa88,shifted_anchor);
 const auto patch203=crimson_nav::read_worldmap_projection(shifted,0,root);
 if(!patch203.valid||std::abs(patch203.ax-1./shifted_anchor[4])>.000001||
    patch203.canvas_width!=1920||patch203.canvas_height!=1080){
     std::cerr<<"FAIL: shifted patch 2.03 full-map projection block\n";return 1;
 }
 Memory later=shifted;const float no_canvas[2]={0,0},later_canvas[2]={1280,720};
 const float later_anchor[5]={640,360,-500,-750,2};
 later.put(root+0xa68,no_canvas);later.put(root+0xab0,later_canvas);later.put(root+0xad0,later_anchor);
 if(!crimson_nav::read_worldmap_projection(later,0,root).valid){
     std::cerr<<"FAIL: bounded additive full-map projection shift\n";return 1;
 }
 Memory ambiguous=shifted;ambiguous.put(root+0xab0,later_canvas);ambiguous.put(root+0xad0,later_anchor);
 if(crimson_nav::read_worldmap_projection(ambiguous,0,root).valid){
     std::cerr<<"FAIL: ambiguous full-map projection blocks must be rejected\n";return 1;
 }
 constexpr std::uintptr_t icon=0x30000,matrix=0x31000;
 m.put(root+0x398,icon);m.put(icon+0x50,matrix);
 float identity[16]={1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};m.put(matrix,identity);
 float scale[2]={3.333333f,3.333333f};m.put(root+0x53c,scale);
 crimson_nav::MapState state{};crimson_nav::MinimapGeometry geometry{1920,1080,50,845,200,200,true};
 // Terrain canvas includes both zoom stages; icon projection does not.
 m.put(root+0x3f8,icon);scale[0]=3.333333f;m.put(root+0x53c,scale);
 float origin[3]={0,0,0};m.put(root+0x4bc,origin);
 float terrain[16]={2.7306667f,0,0,0,0,2.7306667f,0,0,0,0,1,0,0,0,0,1};
 terrain[12]=150-150*terrain[0];terrain[13]=945-945*terrain[5];m.put(matrix,terrain);
 auto locked=crimson_nav::read_minimap_projection(m,root,state,geometry);
 if(!locked.valid||std::abs(locked.ax-2.1333333)>.001){std::cerr<<"FAIL: terrain zoom must drive road projection\n";return 1;}
 assert(std::abs(locked.tx-150)<.001&&std::abs(locked.ty-945)<.001);
 origin[0]=10;m.put(root+0x4bc,origin);
 locked=crimson_nav::read_minimap_projection(m,root,state,geometry);
 assert(locked.valid&&std::abs(locked.tx-(150-10*2.1333333))<.001);
 terrain[0]=std::numeric_limits<float>::quiet_NaN();m.put(matrix,terrain);
 assert(!crimson_nav::read_minimap_projection(m,root,state,geometry).valid);
 std::cout<<"World map and terrain projection cases passed\n";
}
