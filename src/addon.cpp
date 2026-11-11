// Independent local rendering prototype. No game hooks or gameplay automation.
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <imgui.h>
#include <reshade.hpp>
#include "settings.hpp"
#include "route_pulse.hpp"
#include "route_scale.hpp"
#include "route_pattern.hpp"
#include "route_progress.hpp"
#include "reader_service.hpp"
#include <filesystem>
#include <fstream>
#include <mutex>

namespace {
crimson_nav::Settings settings;
crimson_nav::ReaderService reader;
std::filesystem::path config_path;
std::mutex state_mutex;
const char *status="Defaults loaded. Use Save settings to keep changes.";
const char *render_status="No frame sampled yet.";
std::string last_logged_state;
ULONGLONG last_log_at{};
bool startup_pending=true;

void save_settings() noexcept {
    try {
        std::filesystem::create_directories(config_path.parent_path());
        std::ofstream output(config_path,std::ios::binary|std::ios::trunc);
        output<<crimson_nav::serialize_settings(settings); output.flush();
        status=output.good()?"Settings saved.":"Could not save settings. Check folder write access.";
    } catch(...) {status="Could not save settings. Check folder write access.";}
}
void settings_panel(reshade::api::effect_runtime *) {
    std::lock_guard<std::mutex> lock(state_mutex);
    ImGui::TextUnformatted("Crimson Navigation 1.0.9-test1");
    ImGui::TextWrapped("Experimental road navigation on both maps. Off-road destinations end at the nearest road point. Conditional road transitions are excluded.");
    ImGui::Checkbox("Start navigation automatically",&settings.auto_start);
    if(ImGui::Button("Start / restart navigation")){startup_pending=false;reader.start();}
    ImGui::SameLine();if(ImGui::Button("Stop navigation")){startup_pending=false;reader.stop();}
    const auto diagnostic=reader.snapshot();
    ImGui::TextWrapped("%s",diagnostic.status.c_str());
    ImGui::TextWrapped("Display: %s",render_status);
    ImGui::TextWrapped("Acquisition: %s",diagnostic.acquisition.c_str());
    if(diagnostic.navigation.active)ImGui::Text("Navigation: %s",diagnostic.navigation.retained?"retained destination":"current game destination");
    if(diagnostic.valid) {
        ImGui::Text("Game map layout: %s | minimap layout %s",diagnostic.build.version,diagnostic.layout->name);
        ImGui::Text("Player: %.2f, %.2f, %.2f",diagnostic.state.xyz[0],diagnostic.state.xyz[1],diagnostic.state.xyz[2]);
    ImGui::Text("Destination label: %s (corroborating UI signal)",!diagnostic.state.destination_ui_valid?"unavailable":diagnostic.state.destination_candidate?"active":"inactive");
        switch(diagnostic.target.status) {
        case crimson_nav::TargetStatus::candidate:
            ImGui::TextUnformatted("Target: matching collection candidate (experimental)");
            ImGui::Text("Target XYZ: %.2f, %.2f, %.2f",diagnostic.target.xyz[0],diagnostic.target.xyz[1],diagnostic.target.xyz[2]);break;
        case crimson_nav::TargetStatus::no_candidate:ImGui::TextUnformatted("Target: no supported candidate in collections");break;
        case crimson_nav::TargetStatus::ambiguous:ImGui::TextUnformatted("Target observation: ambiguous; previous destination retained if available");break;
        default:ImGui::TextUnformatted("Target observation: unavailable or changing; previous destination retained if available");break;
        }
        ImGui::Text("Minimap view signal: %s",diagnostic.state.map_hidden_candidate?"hidden candidate":"visible candidate");
        if(diagnostic.state.distance_valid)ImGui::Text("Cached distance: %u (not target validity)",diagnostic.state.distance);
        else ImGui::TextUnformatted("Cached distance: unavailable");
    }
    ImGui::Separator();
    ImGui::Checkbox("Show road route",&settings.route_enabled);
    ImGui::TextWrapped("Dashed line: direction from you to the road route; terrain obstacles are not checked.");
    if(diagnostic.route&&diagnostic.route->valid)ImGui::Text("Road distance: %.0f | Route points: %zu",diagnostic.route->length,diagnostic.route->points.size());
    ImGui::Text("Projection: minimap %s | full map %s",diagnostic.mini_projection.valid?"ready":"unavailable",diagnostic.world_projection.valid?"ready":"hidden/unavailable");
    const auto display=ImGui::GetIO().DisplaySize;
    ImGui::Text("Overlay size: %.0f x %.0f",display.x,display.y);
    if(diagnostic.geometry.valid)ImGui::Text("Minimap canvas: %.0f x %.0f",diagnostic.geometry.canvas_width,diagnostic.geometry.canvas_height);
    if(diagnostic.world_projection.valid){
        const auto &world=diagnostic.world_projection;
        ImGui::Text("Full-map logical reference: %.0f x %.0f",world.canvas_width,world.canvas_height);
    }
    ImGui::ColorEdit4("Route color",settings.color.data());
    ImGui::SliderFloat("Route width",&settings.width,1.f,12.f,"%.1f px at 1080p");
    ImGui::Text("Display width scale: %.2fx",crimson_nav::route_pixel_scale(display.x,display.y));
    int mini_style=settings.minimap_style;
    if(ImGui::Combo("Minimap route style",&mini_style,"Solid line\0Pulse (recommended)\0Dashed line\0Dotted line\0")){settings.minimap_style=mini_style;settings.minimap_pulse=mini_style==1;}
    if(settings.minimap_pulse)ImGui::TextWrapped("Highlights travel toward the destination over a faint route. Uses your route color and width.");
    if(ImGui::Button("Save settings")) save_settings();
    ImGui::SameLine();
    if(ImGui::Button("Reset appearance")) {settings=crimson_nav::Settings{};status="Defaults restored. Save to keep them.";}
    ImGui::TextWrapped("%s",status);
}

void draw_route(reshade::api::effect_runtime *) {
    std::lock_guard<std::mutex> lock(state_mutex);
    // Start only from the first presentation callback, after host initialization.
    if(startup_pending){startup_pending=false;if(settings.auto_start)reader.start();}
    if(!settings.route_enabled)return;
    auto d=reader.snapshot();
    const std::string report=d.status+" | "+render_status+" | "+d.acquisition+" | target="+
        std::to_string(static_cast<int>(d.target.status))+" retained="+std::to_string(d.navigation.retained);
    if(report!=last_logged_state&&GetTickCount64()-last_log_at>=1000){
        reshade::log::message(reshade::log::level::info,("Crimson Navigation: "+report).c_str());
        last_logged_state=report;last_log_at=GetTickCount64();
    }
    if(!d.valid){render_status="Waiting for map reader.";return;}
    if(!d.route||!d.route->valid){render_status="No valid road route available.";return;}
    if(GetTickCount64()-d.sampled_at>250){render_status="Reader sample expired; route hidden.";return;}
    // Route solving stays on the worker. Read only the small UI projection at
    // presentation time so it follows the current frame instead of a 30 ms sample.
    crimson_nav::ProcessMemory memory{GetCurrentProcess()};
    const auto base=reinterpret_cast<std::uintptr_t>(GetModuleHandleW(nullptr));
    if(!crimson_nav::read_map(memory,base,d.root,d.state,d.build,*d.layout)){render_status="Map changing this frame; retrying next frame.";return;}
    const bool full=d.state.map_hidden_candidate;
    const auto screen=ImGui::GetIO().DisplaySize;
    // Widths and spacing are 1080p pixels; scale them with the game UI.
    const float px=crimson_nav::route_pixel_scale(screen.x,screen.y),width=settings.width*px,inset=3.f*px;
    crimson_nav::ScreenCircle circle;
    if(!full){
        d.geometry=crimson_nav::read_minimap_geometry(memory,base,d.root,d.build,*d.layout);
        circle=crimson_nav::project_minimap(d.geometry,screen.x,screen.y);
        if(!circle.valid){render_status="Minimap canvas geometry unavailable.";return;}
    }
    const auto p=crimson_nav::read_route_display_projection(memory,base,d.root,d.world_root,d.state,d.geometry,screen.x,screen.y,d.build,*d.layout);
    if(!p.valid){render_status=full?"Full map hidden, changing, or UI layout unavailable.":"Minimap projection unavailable.";return;}
    auto project=[&](crimson_nav::RoadPoint q){return ImVec2(float(p.ax*q.x+p.az*q.z+p.tx),float(p.bx*q.x+p.bz*q.z+p.ty));};
    auto *draw=ImGui::GetBackgroundDrawList();const auto &c=settings.color;
    const auto color=ImGui::ColorConvertFloat4ToU32(ImVec4(c[0],c[1],c[2],c[3]));
    draw->PushClipRect(ImVec2(0,full?screen.y*.12f:0),ImVec2(screen.x,full?screen.y*.9f:screen.y),true);
    unsigned drawn=0;
    const crimson_nav::RoadPoint player{d.state.xyz[0],d.state.xyz[1],d.state.xyz[2]};
    const auto progress=crimson_nav::route_progress(*d.route,player);
    if(!progress.valid){draw->PopClipRect();return;}
    double distance_along=progress.distance;
    const double pulse_time=double(GetTickCount64()%1800000)/1000.;
    for(size_t i=progress.next-1;i<d.route->points.size();++i){
        const bool connector=i==progress.next-1;
        const auto from=connector?player:(i==progress.next?progress.point:d.route->points[i-1]);
        const auto to=connector?progress.point:d.route->points[i];
        const double segment_start=distance_along,segment_length=crimson_nav::road_distance(from,to);
        if(!connector)distance_along+=segment_length;
        double clip_start=0,clip_end=1;
        auto a=project(from),b=project(to);
        if(!full){
            const double dx=b.x-a.x,dy=b.y-a.y,x=a.x-circle.x,y=a.y-circle.y;
            const double aa=dx*dx+dy*dy,bb=2*(x*dx+y*dy),cc=x*x+y*y-(circle.radius-inset)*(circle.radius-inset);
            if(aa<1e-10)continue;const double disc=bb*bb-4*aa*cc;if(disc<0)continue;
            const double lo=std::max(0.,(-bb-std::sqrt(disc))/(2*aa)),hi=std::min(1.,(-bb+std::sqrt(disc))/(2*aa));
            if(lo>hi)continue;
            clip_start=lo;clip_end=hi;
            b=ImVec2(float(a.x+hi*dx),float(a.y+hi*dy));a=ImVec2(float(a.x+lo*dx),float(a.y+lo*dy));
        }
        if(connector){
            // A dashed bearing to the road, not a terrain-validated walking path.
            const double length=std::hypot(b.x-a.x,b.y-a.y);
            for(double at=0;at<length;at+=10.*px){
                const double lo=at/length,hi=std::min(at+6.*px,length)/length;
                ImVec2 x(float(a.x+(b.x-a.x)*lo),float(a.y+(b.y-a.y)*lo));
                ImVec2 y(float(a.x+(b.x-a.x)*hi),float(a.y+(b.y-a.y)*hi));
                draw->AddLine(x,y,ImGui::ColorConvertFloat4ToU32(ImVec4(c[0],c[1],c[2],c[3]*.65f)),width);
            }
        }else if(!full&&settings.minimap_style>=2){
            const bool dotted=settings.minimap_style==3;
            const float dot_radius=crimson_nav::route_dot_radius(width);
            crimson_nav::route_pattern(segment_start+segment_length*clip_start,segment_start+segment_length*clip_end,dotted,
                [&](double lo,double hi){
                    ImVec2 x(float(a.x+(b.x-a.x)*lo),float(a.y+(b.y-a.y)*lo));
                    ImVec2 y(float(a.x+(b.x-a.x)*hi),float(a.y+(b.y-a.y)*hi));
                    if(dotted){
                        if(std::hypot(x.x-circle.x,x.y-circle.y)+dot_radius<=circle.radius-inset)
                            draw->AddCircleFilled(x,dot_radius,color);
                    }else draw->AddLine(x,y,color,width);
                });
        }else if(!full&&settings.minimap_style==1){
            draw->AddLine(a,b,ImGui::ColorConvertFloat4ToU32(ImVec4(c[0],c[1],c[2],c[3]*.15f)),width);
            crimson_nav::route_pulse(segment_start+segment_length*clip_start,segment_start+segment_length*clip_end,pulse_time,
                [&](double lo,double hi,double opacity){
                    ImVec2 x(float(a.x+(b.x-a.x)*lo),float(a.y+(b.y-a.y)*lo));
                    ImVec2 y(float(a.x+(b.x-a.x)*hi),float(a.y+(b.y-a.y)*hi));
                    draw->AddLine(x,y,ImGui::ColorConvertFloat4ToU32(ImVec4(c[0],c[1],c[2],float(c[3]*opacity*.18))),width+4.f*px);
                    draw->AddLine(x,y,ImGui::ColorConvertFloat4ToU32(ImVec4(c[0],c[1],c[2],float(c[3]*opacity))),width);
                });
        }else{
            draw->AddLine(a,b,IM_COL32(0,0,0,210),width+2.f*px);draw->AddLine(a,b,color,width);
        }
        ++drawn;
    }
    draw->PopClipRect();
    render_status=full?"Full map route submitted.":drawn?"Minimap route submitted.":"Route lies outside minimap; destination remains tracked.";
}

}
extern "C" __declspec(dllexport) const char *NAME="Crimson Navigation Lab";
extern "C" __declspec(dllexport) const char *DESCRIPTION="Road navigation 1.0.9-test1. Patch 2.04 road format, minimap layout and full-map projection compatibility; automatic layout detection; route width scales with display resolution; dotted minimap dots keep a 0.5 px minimum radius.";
extern "C" __declspec(dllexport) bool AddonInit(HMODULE module,HMODULE host) {
    try {
        wchar_t filename[32768]; const DWORD count=GetModuleFileNameW(module,filename,32768);
        if(!count || count>=32768) return false;
        config_path=std::filesystem::path(filename).parent_path()/L"CrimsonNavigationLab"/L"settings.txt";
        if(std::filesystem::exists(config_path) && std::filesystem::file_size(config_path)<=4096) {
            std::ifstream input(config_path,std::ios::binary);
            if(input) {settings=crimson_nav::parse_settings(std::string(std::istreambuf_iterator<char>(input),{}));status="Saved settings loaded.";}
        }
    } catch(...) {status="Could not read settings; using defaults.";}
    if(!reshade::register_addon(module,host)) {reshade::unregister_addon(module);return false;}
    startup_pending=true;
    reshade::register_overlay(nullptr,settings_panel);
    reshade::register_event<reshade::addon_event::reshade_overlay>(draw_route);
    reshade::log::message(reshade::log::level::info,"Crimson Navigation Lab 1.0.9-test1 loaded: patch 2.04 road format, minimap layout and full-map projection compatibility; automatic layout detection; route width scales with display resolution; dotted minimap dots keep a 0.5 px minimum radius.");
    return true;
}
extern "C" __declspec(dllexport) void AddonUninit(HMODULE module,HMODULE) {
    reader.stop();
    reshade::unregister_event<reshade::addon_event::reshade_overlay>(draw_route);
    reshade::unregister_overlay(nullptr,settings_panel);
    reshade::unregister_addon(module);
}
