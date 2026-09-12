# Crimson Navigation

Road navigation for Crimson Desert through a native ReShade add-on. Routes appear on the minimap and full-screen map. Includes solid and traveling-pulse minimap styles, saved color and width, a player-following route start, and automatic destination routing.

[Download and installation on Nexus Mods](https://www.nexusmods.com/crimsondesert/mods/3447).

## Compatibility

The prepared 1.0.6 update has no exact executable version/hash lock. It locates the current map class identities automatically and retains live map-data validation. Compatible game patches can work without a mod update; changes to required game structures may still need one. Windows x64 and ReShade with full add-on support are required. Linux/Proton remains unverified.

Test history: prior route/scaling gameplay validation used executable 1.0.0.2850 / Steam build 25246367 with ReShade 6.8.0.2155. The new detector matches the independently recorded addresses in that executable and passes synthetic changed-address checks. Full regression/native host checks pass; startup, automatic map discovery and routes on both maps are confirmed by the game log and the user's visual check.

Routes minimize geometric road length, not measured travel time. Off-road destination endpoints remain on the road. Dashed player-to-road connectors are directional guides and do not check obstacles. Minor minimap drift and overlapping-road ambiguity remain possible.

## Prepared update

[1.0.6 upload bundle](outputs/release-1.0.6/RELEASE-INDEX.md) and [updated Nexus description](outputs/release-1.0.6/NEXUS-DESCRIPTION.md) supersede the earlier 1.0.5 preparation; startup, automatic map discovery and routes on both maps are confirmed by the game log and the user's visual check; the public page has not been changed by this preparation.

Reported vendor/NPC interaction bugs and full-map resolution scaling are fixed in the tested setup. Some resolutions may still have issues. Combat and other interactions may briefly hide the route; it should return while the active navigation destination remains set. Please report failures to recover.

Minimap drift tightening remains ongoing. Routing to user-placed map markers, pulse animation speed, pulse blur and route glow controls are planned, not included in this release.

## Development

Source is in `src/`; regression tests and research probes are in `tests/`. ReShade is the only runtime dependency. No game archives or extracted map assets are included. Installed data is read locally without patching original game files.

The current `build.cmd` is a Windows developer build using Visual Studio 2026 Build Tools at its standard installation path. It expects the ReShade SDK headers in `work/reference/reshade/include` and matching Dear ImGui headers in `work/reference/reshade/deps/imgui` (API 20 / ImGui 1.92.5). Obtain SDK dependencies separately from their official repositories. A portable dependency setup is not included yet.

Run `cmd /c build.cmd` from a configured Windows development environment. It builds and runs regression and host checks, then writes the current 1.0.6-test1 add-on under `outputs/local-test-1.0.6/build/`. This candidate reads the full map's native UI scale and body extent to center routes at wider resolutions, including 4096x2160. It detects map class addresses from the current executable without a version/hash allowlist and retains destination recovery. It does not deploy to the game. Offline tests do not establish in-game or Proton compatibility; destination-clear detection still needs live validation.

## Bug reports

Include the mod/game/ReShade versions, OS and Proton/Wine version if applicable, resolution, UI scale, graphics API/translation layer, route style, and screenshots. Describe whether the issue changes while panning, zooming, moving, or clearing the destination. Share only relevant log excerpts after removing private details.

See `docs/known-issues.md` for reports awaiting investigation.

## Credits and licensing

Developed with OpenAI Codex and iterative user-directed testing. Substantial code and documentation were AI-generated.

ReShade and Dear ImGui retain their respective licenses; see `THIRD-PARTY-NOTICES.txt`. No project-wide open-source license has been selected. Public source visibility, if enabled, does not by itself grant a redistribution license. Unofficial fan project, not endorsed by Pearl Abyss.
