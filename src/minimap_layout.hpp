#pragma once
#include "game_build.hpp"
#include <cstddef>
#include <cstdint>
namespace crimson_nav {
// Minimap root member offsets. Patch 2.04 inserted members at several points,
// so one base shift cannot describe it. A profile is selected only when the
// minimap canvas class identity sits at exactly one profile's canvas member.
// Every reader still validates names, links and values before using a field.
struct MinimapLayout {
    const char *name;
    std::size_t collections,player,hud,terrain,destination_icon,canvas,origin,distance;
    std::uint32_t destination_kind;
    const char *hud_name;
    // Exact destination-category icon flag value measured as a non-destination
    // presentation icon; zero when none has been observed.
    std::uint16_t presentation_flags;
    // Visibility flag byte of generic UI elements (map views). Patch 2.04 added
    // an 8-byte member to every UI element before this field. The minimap root
    // layout and this element layout changed in the same build.
    std::size_t ui_flags;
};
// Builds 1.0.0.2760 through 1.0.0.2944 (patch 2.03).
inline constexpr MinimapLayout minimap_layout_legacy{"legacy",
    0x238,0x330,0x380,0x3f8,0x418,0x448,0x4bc,0x558,21,"MinimapHud",0,0xb0};
// Build 1.0.0.3576 (patch 2.04). MinimapHud no longer occupies a root member;
// MinimapDisplay carries the same circle rectangle, as both did before.
inline constexpr MinimapLayout minimap_layout_2_04{"2.04",
    0x238,0x340,0x398,0x418,0x438,0x470,0x4fc,0x598,22,"MinimapDisplay",0x23a,0xb8};
inline constexpr const MinimapLayout *minimap_layouts[]={&minimap_layout_legacy,&minimap_layout_2_04};
template<class Memory> const MinimapLayout *select_minimap_layout(Memory &m,std::uintptr_t base,
    std::uintptr_t root,const GameBuild &build){
    std::uintptr_t vt{};
    if(!root||!m.read(root,&vt,8)||vt!=base+build.minimap_root)return nullptr;
    const MinimapLayout *selected=nullptr;
    for(const auto *layout:minimap_layouts){
        std::uintptr_t canvas{},canvas_vt{};
        if(!m.read(root+layout->canvas,&canvas,8)||canvas<0x10000||canvas>=0x7fffffff0000ull||
           !m.read(canvas,&canvas_vt,8)||canvas_vt!=base+build.minimap_canvas)continue;
        if(selected)return nullptr; // Ambiguous layouts never choose the first match.
        selected=layout;
    }
    return selected;
}
}
