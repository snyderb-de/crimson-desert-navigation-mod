#pragma once
#include <cstdint>
namespace crimson_nav {
// Recorded identities are fixtures for previously validated layouts, not a
// runtime allowlist. Production discovers identities from the current PE RTTI.
// Object field offsets stay in the readers and are checked against live data.
struct GameBuild {
    const char *version;
    const char *sha256;
    std::uintptr_t minimap_root,worldmap_root,minimap_canvas,minimap_icon;
};
inline constexpr GameBuild game_build_2760{"1.0.0.2760",
    "4d99c15c58bd20a94d354d10ae395d1fac777d59ef52cba8080dc3fc8dc6f454",
    89513840,89508976,89515328,89518944};
inline constexpr GameBuild game_build_2850{"1.0.0.2850",
    "bcbf623ad5690147dc462aeaed5b4f97bd73296ba0d6ab54663586e7088b1c0e",
    89530200,89514408,89531688,89528856};
}
