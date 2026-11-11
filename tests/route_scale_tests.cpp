#include "route_scale.hpp"
#include <cmath>
#include <iostream>
#include <limits>
int main(){
    int checks=0,failures=0;
    auto check=[&](bool ok,const char *name){++checks;if(!ok){++failures;std::cerr<<"FAIL: "<<name<<'\n';}};
    auto near=[](float a,float b){return std::abs(a-b)<1e-4f;};
    using crimson_nav::route_pixel_scale;
    check(near(route_pixel_scale(1920,1080),1.f),"1080p keeps the authored width");
    check(near(route_pixel_scale(2560,1440),4.f/3),"1440p scales by 4/3");
    check(near(route_pixel_scale(3840,2160),2.f),"native 4K doubles the width");
    check(near(route_pixel_scale(4096,2160),2.f),"DCI 4K is limited by height");
    check(near(route_pixel_scale(3440,1440),4.f/3),"ultrawide is limited by height");
    check(near(route_pixel_scale(1920,1200),1.f),"16:10 is limited by width");
    check(near(route_pixel_scale(1280,720),2.f/3),"720p thins proportionally");
    check(near(route_pixel_scale(320,200),.5f),"tiny displays clamp to half width");
    check(near(route_pixel_scale(15360,8640),4.f),"very large displays clamp to four times");
    const float nan=std::numeric_limits<float>::quiet_NaN();
    check(near(route_pixel_scale(nan,1080),1.f)&&near(route_pixel_scale(0,0),1.f)&&near(route_pixel_scale(-1,1080),1.f),
        "invalid display sizes keep the authored width");
    using crimson_nav::route_dot_radius;
    check(near(route_dot_radius(6.f),3.f),"dot radius is half the route width");
    check(near(route_dot_radius(1.f),.5f),"width 1 keeps the exact minimum radius");
    check(near(route_dot_radius(1.f*route_pixel_scale(1280,720)),.5f),"width 1 at 720p keeps a visible dot");
    check(near(route_dot_radius(1.f*route_pixel_scale(320,200)),.5f),"width 1 at the smallest scale keeps a visible dot");
    check(near(route_dot_radius(0.f),.5f)&&near(route_dot_radius(-3.f),.5f)&&near(route_dot_radius(nan),.5f),
        "invalid widths keep a visible dot");
    check(route_dot_radius(1.5f*route_pixel_scale(960,540))>=.5f,"documented TODO-006 case reaches ImGui's 0.5 px minimum");
    std::cout<<checks<<" route scale checks, "<<failures<<" failures\n";
    return failures?1:0;
}
