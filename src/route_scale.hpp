#pragma once
#include <algorithm>
#include <cmath>
namespace crimson_nav {
// Route widths and on-screen spacing are authored in 1920x1080 pixels. The
// game's UI grows with the display (its native full-map UI scale is 2.0 at
// 3840x2160), so the route scales by the same uniform fit to stay
// proportional to the maps. Settings keep storing the 1080p value.
inline float route_pixel_scale(float width,float height){
    if(!std::isfinite(width)||!std::isfinite(height)||width<=0||height<=0)return 1.f;
    return std::clamp(std::min(width/1920.f,height/1080.f),.5f,4.f);
}
// Dotted-route dot radius for a scaled route width. ImGui's AddCircleFilled
// draws nothing below a 0.5 px radius, so thin routes on small displays keep
// a minimum visible dot instead of disappearing.
inline float route_dot_radius(float scaled_width){
    if(!std::isfinite(scaled_width))return .5f;
    return std::max(scaled_width*.5f,.5f);
}
}
