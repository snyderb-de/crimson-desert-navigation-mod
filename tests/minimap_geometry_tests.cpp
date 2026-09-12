#include "minimap_geometry.hpp"
#include <iostream>
#include <limits>
int main(){int failures=0;auto check=[&](bool ok,const char *s){if(!ok){++failures;std::cerr<<"FAIL "<<s<<'\n';}};
    crimson_nav::MinimapGeometry g{1920,1080,50,845,200,200,true};
    auto c=crimson_nav::project_minimap(g,1920,1080);check(c.valid&&c.x==150&&c.y==945&&c.radius==100,"reference canvas");
    c=crimson_nav::project_minimap(g,3840,2160);check(c.valid&&c.x==300&&c.y==1890&&c.radius==200,"uniform 4K scaling");
    check(!crimson_nav::project_minimap(g,2560,1080).valid,"reject unknown aspect mapping");
    auto wide=g;wide.canvas_width=2560;c=crimson_nav::project_minimap(wide,2560,1080);check(c.valid&&c.x==150&&c.y==945,"live ultrawide canvas without fixed reference");
    g.width=0;check(!crimson_nav::project_minimap(g,1920,1080).valid,"zero size");
    g.width=200;g.x=-10;check(!crimson_nav::project_minimap(g,1920,1080).valid,"offscreen rectangle");
    g.x=50;g.height=400;check(!crimson_nav::project_minimap(g,1920,1080).valid,"noncircular region");
    g.height=200;g.canvas_height=std::numeric_limits<float>::quiet_NaN();check(!crimson_nav::project_minimap(g,1920,1080).valid,"invalid layout");
    if(!failures)std::cout<<"PASS: 8 minimap projection cases\n";return failures?1:0;
}
