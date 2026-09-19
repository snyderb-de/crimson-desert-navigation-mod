CRIMSON NAVIGATION 1.0.7

Road guidance on Crimson Desert's minimap and full-screen map.
The add-on menu displays 1.0.7-test2. This update restores compatibility with game patch 2.03.00, tolerates bounded additive road-archive sections, and locates the shifted full-map projection block structurally.

REQUIREMENTS

- Windows x64 Crimson Desert. No fixed executable version or Steam build requirement.
- ReShade with full add-on support, installed and working. Tested with ReShade 6.8.0.2155. Download separately: https://reshade.me/
- ReShade is the only runtime dependency. No ASI loader, Python or Crimson Route dependency.

The add-on detects the game map classes automatically at startup. It locates the required named road section and validates its structure instead of treating the archive's total section count as an exact game-build identity. It also locates exactly one plausible paired full-map canvas/transform block within a narrow bounded object window and re-reads it before drawing. Live object, destination, road-data and projection checks remain active. Semantic changes to required data may still need a mod update. Linux/Proton and other ReShade host versions remain unverified.

Tested on executable 1.0.0.2944 / Steam build 25381195. Full regression/native host checks pass, the production loader reads the untouched patch 2.03.00 road archive, and the installed build passes the minimap, full-screen map, full-map pan/zoom, and ReShade-health checks. Earlier route/scaling tests on build 25246367 remain historical evidence, not a fixed version requirement.

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
- Route width: line width.
- Minimap route style: Solid line, Pulse, Dashed line or Dotted line. The full-map route is solid.
- Save settings: preserve your choices.
- Reset appearance, then Save settings: restore defaults.

Fresh-install defaults: automatic startup on, route enabled, gold #B17C01, full opacity, 6 px and Pulse minimap style. Existing saved settings take precedence. Settings are saved only when requested to bin64/CrimsonNavigationLab/settings.txt. No personal settings file is included in this ZIP.

FIXES AND REMAINING ISSUES

Full-map resolution scaling/centering and the reported vendor/NPC interaction bugs have been fixed in the tested setup. The route calculates its placement from live map dimensions and UI scale. Some resolutions or display layouts may still have issues; report them.

Combat and some other interactions may temporarily make the route disappear. It should return automatically as long as the active navigation destination remains set on the map. User-placed map markers are distinct from that destination. Please report a route that fails to return.

Minimap drift and turn lag are still being tightened. Routing to user-placed map markers, pulse animation speed, pulse blur and route glow controls are planned and are NOT included in this release.

Road routes minimize geometric distance, not guaranteed fastest travel time. Conditional road connections are excluded and some roads may be disconnected. Overlapping roads can be ambiguous. Off-road destinations end at the nearest road point. Dashed player-to-road connectors show direction and do not test terrain obstacles. The mod does not move your character automatically.

TROUBLESHOOTING AND BUG REPORTS

- Add-on missing: check full add-on-support ReShade, bin64 placement and host version.
- No route after loading: check the active destination, Show road route and the panel status. Allow discovery to finish; use Start / restart navigation if needed.
- Map layout unavailable: try Start / restart navigation. If detection still fails, report the game build and status so changed structures can be investigated. A version number alone never blocks startup.
- Old appearance: your saved choices were preserved. Reset appearance, then Save settings to change to defaults.
- Report resolution/alignment bugs with the selected resolution, panel Overlay size/Display status, game/mod/ReShade versions, a screenshot and whether it happens during pan, zoom, movement or turning.
- Report disappearing routes with the interaction involved and whether the active navigation destination remained set. Include relevant Crimson Navigation entries from ReShade.log; omit private information.

Nexus page: https://www.nexusmods.com/crimsondesert/mods/3447

UNINSTALL

Close the game. Remove only CrimsonNavigationLab.addon64. Optionally remove this mod's CrimsonNavigationLab/settings.txt and its empty folder. Leave ReShade, game files and other mods intact.

CREDITS AND PERMISSIONS

Independent implementation developed using OpenAI Codex with user-directed testing. Substantial code and documentation were AI-generated. Crimson Route was research context only; none of its files are included or required. ReShade and Dear ImGui license notices are in THIRD-PARTY-NOTICES.txt.

Installed game data is read locally without patching game archives, executable or saves. No extracted game data, game assets, other mods, ReShade distribution, networking or auto-updater is included.

This is unofficial fan content, not endorsed by Pearl Abyss. Permissions for this project's content remain those stated on its Nexus page; this README grants no additional redistribution or modification permissions. Third-party license rights remain intact.
