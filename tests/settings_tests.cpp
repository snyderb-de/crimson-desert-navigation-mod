#include "settings.hpp"
#include <iostream>
#include <cmath>
int main() {
    int failures=0;
    auto check=[&](bool ok,const char *name){ if(!ok){std::cerr<<"FAIL: "<<name<<'\n';++failures;} };
    const auto s=crimson_nav::parse_settings("color=0.1 0.2 0.3 0.4\nwidth=7\nvisible=0\n");
    check(std::abs(s.color[2]-0.3f)<1e-6f && s.width==7 && !s.visible,"read user settings");
    const auto bad=crimson_nav::parse_settings("color=nan 0 0 1\nwidth=inf\nvisible=banana\n");
    check(bad.width==6 && bad.color[0]==177.f/255.f && bad.visible,"reject nonfinite or invalid values");
    const auto bounds=crimson_nav::parse_settings("color=-2 4 0.5 2\nwidth=999\n");
    check(bounds.color[0]==0 && bounds.color[1]==1 && bounds.width==12,"bound display settings");
    const auto partial=crimson_nav::parse_settings("color=0.2 0.3\nwidth=5garbage\n");
    check(partial.color[0]==177.f/255.f && partial.width==6,"reject truncated and trailing data");
    const auto restored=crimson_nav::parse_settings(crimson_nav::serialize_settings(s));
    check(restored.color==s.color && restored.width==s.width && restored.visible==s.visible,"settings serialization round trip");
    auto pulse=crimson_nav::parse_settings("minimap_pulse=1\n");
    check(pulse.minimap_pulse,"pulse option enabled");
    check(crimson_nav::parse_settings(crimson_nav::serialize_settings(pulse)).minimap_pulse,"pulse option persists");
    check(crimson_nav::parse_settings("minimap_pulse=2\n").minimap_pulse,"invalid pulse option defaults to pulse");
    const crimson_nav::Settings defaults;
    check(defaults.auto_start&&defaults.route_enabled&&defaults.minimap_pulse&&defaults.width==6&&defaults.color[0]==177.f/255.f&&defaults.color[1]==124.f/255.f&&defaults.color[2]==1.f/255.f,"1.0 defaults");
    auto manual=crimson_nav::parse_settings("auto_start=0\nminimap_pulse=0\nwidth=3\n");
    check(!manual.auto_start&&!manual.minimap_pulse&&manual.width==3&&!crimson_nav::parse_settings(crimson_nav::serialize_settings(manual)).auto_start,"preserve opt-out and existing appearance");
    for(int style=0;style<4;++style){
        auto v=crimson_nav::parse_settings("minimap_style="+std::to_string(style)+"\n");
        check(v.minimap_style==style&&crimson_nav::parse_settings(crimson_nav::serialize_settings(v)).minimap_style==style,"all styles persist");
    }
    check(crimson_nav::parse_settings("minimap_pulse=0\n").minimap_style==0,"legacy solid migration");
    check(crimson_nav::parse_settings("minimap_style=3\nminimap_pulse=1\n").minimap_style==3,"explicit style beats legacy flag");
    check(crimson_nav::parse_settings("minimap_pulse=0\nminimap_style=99\n").minimap_style==0,"invalid style falls back to legacy");
    if(failures) return 1;
    std::cout<<"PASS: 17 settings cases\n";
}
