#include "map_projection.hpp"
#include "support/worldmap_layout_fixture.hpp"
#include <iostream>
#include <limits>
#include <string>

namespace {
constexpr std::uintptr_t root=0x10000,body_address=0x20000,parent_address=0x21000;
struct Memory {
    std::vector<unsigned char> bytes=std::vector<unsigned char>(0x40000);
    int transition=0;
    unsigned canvas_reads=0;
    int layout_transition=0;
    unsigned layout_reads=0;
    bool read(std::uintptr_t p,void *v,size_t n) {
        if(p>bytes.size()||n>bytes.size()-p)return false;
        if(p==body_address+0x70&&++layout_reads==2){
            if(layout_transition==1)put(0x360e0,2.f);
            if(layout_transition==2)put(body_address+0x78,4096.f);
            if(layout_transition==3)put(0x35008,std::uintptr_t(0x37000));
            if(layout_transition==4)put(0x3613b,std::uint8_t(0));
            if(layout_transition==5)return false;
        }
        if(p==root+0xa58&&++canvas_reads==2){
            if(transition==1){float changed[2]={2560,1440};put(p,changed);}
            if(transition==2)put(root+0xa80,float(-999));
            if(transition==3)put(parent_address+0xb0,(unsigned char)0);
            if(transition==4)put(root+8,std::uintptr_t(0x22000));
            if(transition==5)return false;
            if(transition==6)put(body_address+0x38,std::uintptr_t(0x23000));
        }
        std::memcpy(v,bytes.data()+p,n);return true;
    }
    template<class T>void put(size_t p,const T &v){std::memcpy(bytes.data()+p,&v,sizeof(v));}
    void world(float width,float height,float units) {
        put(root,std::uintptr_t(89508976));put(root+8,body_address);put(body_address+0x38,parent_address);put(parent_address+0xb0,(unsigned char)0x40);
        float canvas[2]={width,height},anchor[5]={width/2,height/2,-1000,-2000,units};
        put(root+0xa58,canvas);put(root+0xa78,anchor);canvas_reads=0;
        worldmap_layout_fixture(*this,body_address,0x32000,width,height);
        layout_reads=0;
    }
};
int failures=0,checks=0;
void check(bool ok,const std::string &name){++checks;if(!ok){++failures;std::cerr<<"FAIL: "<<name<<'\n';}}
void point(const crimson_nav::MapProjection &p,double x,double z,double want_x,double want_y,const std::string &name) {
    check(p.valid&&std::abs(p.ax*x+p.az*z+p.tx-want_x)<.002&&std::abs(p.bx*x+p.bz*z+p.ty-want_y)<.002,name);
}

crimson_nav::MapProjection display(Memory &m,const crimson_nav::MinimapGeometry &g,float width,float height) {
    crimson_nav::MapState state{};state.map_hidden_candidate=true;
    return crimson_nav::read_route_display_projection(m,0,0,root,state,g,width,height);
}
}
int main(){
    Memory m;const crimson_nav::MinimapGeometry mini{1920,1080,50,845,200,200,true};
    // Same visible framing in two legitimate UI coordinate systems. The point
    // 600 east / 300 south of the map center must be at (2520,1380) on a 4K display.
    m.world(1920,1080,2);
    point(display(m,mini,3840,2160),-400,-2300,2520,1380,"4K with logical 1080 canvas");
    m.world(3840,2160,1);
    point(display(m,mini,3840,2160),-1000,-2000,1920,1080,"4K center with full-map canvas different from minimap");
    point(display(m,mini,3840,2160),-400,-2300,2520,1380,"4K road point must not be scaled twice");
    point(display(m,{},3840,2160),-400,-2300,2520,1380,"full map independent of hidden minimap geometry");
    for(int transition=1;transition<=6;++transition){
        m.world(1920,1080,2);m.transition=transition;
        check(!crimson_nav::read_worldmap_projection(m,0,root).valid,"reject resize/pan/visibility/identity/read transition "+std::to_string(transition));
    }
    m.transition=0;

    // Captured 4096x2160 session: a 1920x1080 reference, 2x UI scale,
    // and cursor outside that reference but inside the real map body.
    m.world(1920,1080,2);
    worldmap_layout_fixture(m,body_address,0x32000,4096,2160,2);
    float wide_anchor[5]={-27.2f,43.5f,-9353.279296875f,-1198.94287109375f,1.2003990411758423f};
    m.put(root+0xa78,wide_anchor);
    point(display(m,{},4096,2160),wide_anchor[2],wide_anchor[3],73.6,87,"live wider-4K cursor pair");
    // 100 logical pixels east, 50 south: equal scale on BOTH axes.
    point(display(m,{},4096,2160),wide_anchor[2]+100.*wide_anchor[4],wide_anchor[3]-50.*wide_anchor[4],273.6,187,"wider-4K uniform route scale");
    check(!display(m,{},3840,2160).valid,"physical map/display mismatch still rejected during resize");
    for(const auto &screen:std::vector<std::array<float,3>>{{3840,2160,2},{4096,2160,2},{3440,1440,4.f/3},{640,480,1.f/3},{2560,1600,4.f/3}}){
        m.world(1920,1080,2);worldmap_layout_fixture(m,body_address,0x32000,screen[0],screen[1],screen[2]);
        point(display(m,{},screen[0],screen[1]),-400,-2300,screen[0]/2+300.*screen[2],screen[1]/2+150.*screen[2],"physical body and native UI scale determine centering");
    }
    for(int transition=1;transition<=5;++transition){
        m.world(1920,1080,2);m.layout_transition=transition;
        check(!display(m,{},1920,1080).valid,"reject torn UI scale/extent/identity/flags/read "+std::to_string(transition));
    }
    m.layout_transition=0;
    m.world(1920,1080,2);
    m.put(0x360d4,1.25f);m.put(0x360d8,2.f);m.put(0x3613b,std::uint8_t(0));
    point(display(m,{},1920,1080),-400,-2300,1710,915,"fixed UI scale multiplied by user scale");
    m.put(0x360dc,1.5f);m.put(0x3613b,std::uint8_t(1));m.put(0x3613c,std::uint8_t(0));
    point(display(m,{},1920,1080),-400,-2300,1522.5,821.25,"native alternate scale selection");
    m.put(body_address+0x94,std::uint8_t(0xff));
    point(display(m,{},1920,1080),-400,-2300,1260,690,"native no-group scale is one");
    for(auto offset:{body_address+0x58,std::uintptr_t(0x320b0),std::uintptr_t(0x330b0),std::uintptr_t(0x34000),std::uintptr_t(0x35008)}){
        m.world(1920,1080,2);m.put(offset,std::uintptr_t(0));
        check(!display(m,{},1920,1080).valid,"missing native UI group link rejected");
    }
    m.world(1920,1080,2);m.put(body_address+0x94,std::uint8_t(2));
    check(!display(m,{},1920,1080).valid,"UI group index outside count rejected");
    m.world(1920,1080,2);m.put(body_address+0xa0,std::uintptr_t(0x37000));m.put(0x37008,std::uintptr_t(1));
    check(!display(m,{},1920,1080).valid,"unsupported named inherited group rejected");
    m.put(body_address+0xa0,std::uintptr_t(0));
    for(float bad:{0.f,-1.f,std::numeric_limits<float>::quiet_NaN(),129.f}){
        m.world(1920,1080,2);m.put(0x360e0,bad);
        check(!display(m,{},1920,1080).valid,"invalid native UI scale rejected");
    }

    // Hand-derived road position: (1260,690) on a 1920x1080 canvas. Each row is
    // an independent expected DISPLAY position, independent of the UI canvas.
    struct Resolution {float width,height;double x,y;};
    const Resolution sizes[]={
        {640,360,420,230},{960,540,630,345},{1280,720,840,460},
        {1600,900,1050,575},{1920,1080,1260,690},{2560,1440,1680,920},
        {3200,1800,2100,1150},{3840,2160,2520,1380},{7680,4320,5040,2760}
    };
    for(const auto &screen:sizes){
        for(float canvas_width:{1280.f,1920.f,2560.f,3840.f}){
            m.world(canvas_width,canvas_width*9/16,3840/canvas_width);
            const auto p=display(m,mini,screen.width,screen.height);
            point(p,-1000,-2000,screen.width/2,screen.height/2,"center at "+std::to_string(int(screen.width)));
            point(p,-400,-2300,screen.x,screen.y,"road across independent UI/display resolutions");
        }
    }
    // Rounded 16:9 480p, 4:3 480p, 16:10, ultrawide and super-ultrawide.
    // Each has its own canvas extent; horizontal/vertical offsets remain map pixels.
    const Resolution aspects[]={{854,480,502,277.5},{640,480,395,277.5},
        {2560,1600,1580,950},{3440,1440,2020,870},{5120,1440,2860,870}};
    for(const auto &screen:aspects){
        m.world(screen.width,screen.height,screen.height==480?8.f:2.f);
        point(display(m,{},screen.width,screen.height),-400,-2300,screen.x,screen.y,"native canvas aspect "+std::to_string(int(screen.width)));
    }
    // Sweep ALL integer heights 480..4320, rounded 16:9 widths, not just named
    // resolution presets. The independent oracle is the known normalized point.
    for(int height=480;height<=4320;++height){
        const float width=float(std::round(height*16./9.));
        m.world(1920,1080,2);
        point(display(m,{},width,float(height)),-400,-2300,width*21./32.,height*23./36.,"continuous resolution sweep "+std::to_string(height));
    }

    // Repeated display changes in one session must never reuse a previous scale.
    for(const auto &screen:{sizes[4],sizes[5],sizes[7],sizes[4]}){
        m.world(1920,1080,2);
        point(display(m,mini,screen.width,screen.height),-400,-2300,screen.x,screen.y,"1080 -> 1440 -> 4K -> 1080 switch");
    }
    m.world(1920,1080,2);
    float cursor_move[5]={1060,590,-800,-2100,2};m.put(root+0xa78,cursor_move);
    point(display(m,mini,3840,2160),-400,-2300,2520,1380,"cursor motion preserves full-map projection at 4K");
    float pan[5]={960,540,-600,-1800,2};m.put(root+0xa78,pan);
    point(display(m,mini,3840,2160),-400,-2300,2120,1580,"full-map pan at 4K");
    float zoom[5]={960,540,-600,-1800,1};m.put(root+0xa78,zoom);
    point(display(m,mini,3840,2160),-400,-2300,2320,2080,"full-map zoom at 4K");
    m.put(parent_address+0xb0,(unsigned char)0);
    check(!display(m,mini,3840,2160).valid,"hidden world map never draws");

    const double nan=std::numeric_limits<double>::quiet_NaN(),inf=std::numeric_limits<double>::infinity();
    m.world(1920,1080,2);const auto raw=crimson_nav::read_worldmap_projection(m,0,root);
    for(double bad:{0.,-1.,nan,inf,32769.}){
        check(!crimson_nav::map_to_display(raw,bad,1080).valid,"invalid display width");
        check(!crimson_nav::map_to_display(raw,1920,bad).valid,"invalid display height");
        auto invalid=raw;invalid.canvas_width=bad;
        check(!crimson_nav::map_to_display(invalid,1920,1080).valid,"invalid source canvas width");
        invalid=raw;invalid.canvas_height=bad;
        check(!crimson_nav::map_to_display(invalid,1920,1080).valid,"invalid source canvas height");
    }
    check(!crimson_nav::map_to_display({},1920,1080).valid,"unavailable projection");
    for(int term=0;term<6;++term){
        auto invalid=raw;
        double *terms[]={&invalid.ax,&invalid.az,&invalid.bx,&invalid.bz,&invalid.tx,&invalid.ty};*terms[term]=nan;
        check(!crimson_nav::map_to_display(invalid,1920,1080).valid,"non-finite affine coefficient");
    }
    check(!display(m,{},3440,1440).valid,"unknown letterbox/stale aspect is rejected, not stretched");
    m.world(0,1080,2);check(!display(m,{},1920,1080).valid,"invalid live canvas");

    // Actual minimap reader: 90-degree terrain rotation, plus nonzero translation.
    // Its projection stays independent of the full map and uses its own canvas.
    constexpr std::uintptr_t mini_root=0x28000,terrain_node=0x30000,matrix_address=0x31000;
    m.put(mini_root+0x3f8,terrain_node);m.put(terrain_node+0x50,matrix_address);
    float origin[3]={0,0,0},matrix[16]={0,2,0,0,-2,0,0,0,0,0,1,0,2040,645,0,1};
    m.put(mini_root+0x4bc,origin);m.put(matrix_address,matrix);
    crimson_nav::MapState state{};
    auto rotated=crimson_nav::read_route_display_projection(m,0,mini_root,root,state,mini,3840,2160);
    point(rotated,0,0,300,1890,"minimap center at 4K");
    point(rotated,32,16,350,1990,"minimap rotation and translation scaled once");
    const auto circle=crimson_nav::project_minimap(mini,3840,2160);
    check(circle.valid&&circle.x==300&&circle.y==1890&&circle.radius==200,"minimap clipping shares its display coordinate system");
    check(!crimson_nav::read_route_display_projection(m,0,mini_root,root,state,{},3840,2160).valid,"minimap still requires its own valid geometry");
    point(crimson_nav::map_to_display({2,3,4,5,6,7,true,1920,1080},3840,2160),10,20,172,294,"all six affine terms scale");
    std::cout<<checks<<" resolution checks, "<<failures<<" failures\n";
    return failures?1:0;
}
