CRIMSON NAVIGATION 1.0.8

Road guidance on Crimson Desert's minimap and full-screen map.
The add-on menu displays 1.0.8-test3. This update restores compatibility with game patch 2.04 and makes the route width scale with display resolution.

REQUIREMENTS

- Windows x64 Crimson Desert. No fixed executable version or Steam build requirement.
- ReShade with full add-on support, installed and working. Tested with ReShade 6.8.0.2155. Download separately: https://reshade.me/
- ReShade is the only runtime dependency. No ASI loader, Python or Crimson Route dependency.

The add-on detects the game map classes and the minimap object layout automatically at startup. It reads the named road section by structure instead of treating the road format version or file size as an exact game-build identity. Live object, destination, road-data and projection checks remain active before anything is drawn. Game updates that change the required data may still need a mod update. Linux/Proton and other ReShade host versions remain unverified.

Tested on executable 1.0.0.3576 / Steam build 26480954 (game patch 2.04). Full regression and native host checks pass, and the production loader reads the untouched patch 2.04 road archive. The previous test build (1.0.8-test2) showed routes on the minimap and full-screen map, including full-map pan/zoom, at 1920x1080, and correct route placement at 3840x2160. This build adds only the resolution-scaled route width, which is unchanged at 1920x1080; it was run and approved at 3840x2160. Earlier tests on older builds remain historical evidence, not a fixed version requirement.

INSTALL OR UPDATE

1. Close Crimson Desert normally.
2. Extract the ZIP to a temporary folder.
3. Locate Crimson Desert/bin64, containing CrimsonDesert.exe.
4. Copy ONLY CrimsonNavigationLab.addon64 from this package into bin64.
5. For an update, replace only a positively identified previous Crimson Navigation Lab add-on. Keep the CrimsonNavigationLab/settings.txt file and folder. Do not overwrite an unrelated file with a matching name or keep duplicate active copies of this add-on.
6. Launch and load gameplay. Allow automatic discovery to finish.
7. Use the game's Set Destination action. User-placed map markers alone do not select an active navigation destination.

Manual installation is supported; mod-manager deployment is unverified. No installer is included. Do not replace original game files, ReShade DLLs/configuration, saves, Steam configuration or other mods. ReShade must be installed separately.

CONTROLS

Open Home > Add-ons > Crimson Navigation Lab.
- Start navigation automatically: controls startup on the next launch; save to persist.
- Start / restart navigation: manually start or reacquire navigation.
- Stop navigation: stop the current navigation session.
- Show road route: toggle route display.
- Route color: color and opacity.
- Route width: line width in pixels at 1920x1080. The route scales with your display, for example 2x at 3840x2160. The panel shows the current scale.
- Minimap route style: Solid line, Pulse, Dashed line or Dotted line. The full-map route is solid.
- Save settings: preserve your choices.
- Reset appearance, then Save settings: restore all settings to defaults, including automatic startup and Show road route.

Fresh-install defaults: automatic startup on, route enabled, gold #B17C01, full opacity, width 6 and Pulse minimap style. Existing saved settings take precedence. The same saved width now draws thicker above 1920x1080 and thinner below it, scaled by the smaller of width/1920 and height/1080. Adjust it if needed. Settings are saved only when requested to bin64/CrimsonNavigationLab/settings.txt. No personal settings file is included in this ZIP.

FIXES AND REMAINING ISSUES

Route placement is calculated from live map dimensions and the game's UI scale. It was checked at 1920x1080 and 3840x2160 on patch 2.04. Below 1080p with the Dotted style, a route width under about 1.5 can make the dots too small to draw; raise the width if dots disappear. Other resolutions or display layouts may still have issues; report them.

Combat and some other interactions may temporarily make the route disappear. It should return automatically as long as the active navigation destination remains set on the map. User-placed map markers are distinct from that destination. Please report a route that fails to return.

DLC areas use the world-map road network from the base game data. Many DLC islands have no world-map roads, so a route to a destination there ends at the nearest road point. This has not been checked in game.

Minimap drift and turn lag are still being tightened. Routing to user-placed map markers, pulse animation speed, pulse blur and route glow controls are planned and are NOT included in this release.

Road routes minimize geometric distance, not guaranteed fastest travel time. Conditional road connections are excluded and some roads may be disconnected. Overlapping roads can be ambiguous. Off-road destinations end at the nearest road point. Dashed player-to-road connectors show direction and do not test terrain obstacles. The mod does not move your character automatically.

TROUBLESHOOTING AND BUG REPORTS

- Add-on missing: check full add-on-support ReShade, bin64 placement and host version.
- No route after loading: check the active destination, Show road route and the panel status. Allow discovery to finish; use Start / restart navigation if needed.
- Map layout unavailable: try Start / restart navigation. If detection still fails, report the game build and the panel status so changed structures can be investigated. The game version number alone never blocks startup.
- "Reader stopped after an internal error": the panel now shows the reason after this text. Include it in your report.
- Old appearance: your saved choices were preserved. Reset appearance, then Save settings to change to defaults (this also turns automatic startup and Show road route back on).
- Report resolution/alignment bugs with the selected resolution, panel Overlay size and Display width scale, game/mod/ReShade versions, a screenshot and whether it happens during pan, zoom, movement or turning.
- Report disappearing routes with the interaction involved and whether the active navigation destination remained set. Include relevant Crimson Navigation entries from ReShade.log; omit private information.

Nexus page: https://www.nexusmods.com/crimsondesert/mods/3447

UNINSTALL

Close the game. Remove only CrimsonNavigationLab.addon64. Optionally remove this mod's CrimsonNavigationLab/settings.txt and its empty folder. Leave ReShade, game files and other mods intact.

CREDITS AND PERMISSIONS

Independent implementation developed using OpenAI Codex and Anthropic Claude Code with user-directed testing. Most of the code and documentation were AI-generated. Crimson Route was research context only; none of its files are included or required. ReShade and Dear ImGui license notices are in THIRD-PARTY-NOTICES.txt.

Installed game data is read locally without patching game archives, executable or saves. No extracted game data, game assets, other mods, ReShade distribution, networking or auto-updater is included.

This is unofficial content which contains copyrighted materials and IP from Pearl Abyss, and is not official/endorsed content. Permissions for this project's content remain those stated on its Nexus page; this README grants no additional redistribution or modification permissions. Third-party license rights remain intact.
