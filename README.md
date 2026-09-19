<p align="center">
  <img src="docs/images/crimson-navigation-header.png" alt="Crimson Navigation promotional illustration" width="100%">
</p>

# Crimson Navigation

Road navigation for **Crimson Desert** through a native ReShade add-on. Set an active navigation destination and the mod draws a road-following route on the minimap and full-screen map, with custom color, width, and minimap styles.

**Current version:** 1.0.7 (`1.0.7-test2` internally)<br>
**Runtime requirement:** Windows x64 and [ReShade](https://reshade.me/) with full add-on support<br>
**Download:** [Nexus Mods](https://www.nexusmods.com/crimsondesert/mods/3447)<br>
**Release record:** [1.0.7 notes and verification](outputs/release-1.0.7/RELEASE-INDEX.md)

> The header is an AI-edited promotional illustration based on an official Pearl Abyss screenshot. The highlighted world-space line illustrates the concept; the actual add-on draws routes on the game maps.

## In game

| Full-screen map route | Traveling-pulse minimap route |
|---|---|
| ![Road route on the full-screen map](docs/images/full-map-route.png) | ![Traveling-pulse route on the minimap](docs/images/minimap-pulse.png) |

<p align="center">
  <img src="docs/images/reshade-addon-menu.png" alt="Crimson Navigation controls in the ReShade add-on menu" width="663">
</p>

These are genuine user gameplay captures from version 1.0.0 and illustrate the route presentation. The menu image shows that older version and saved custom settings. Current 1.0.7 behavior was separately verified on patch 2.03.00. See [image sources and use](docs/images/README.md).

## Features

- Road routes to the active navigation destination on the minimap and full-screen map.
- Solid, traveling-pulse, dashed, and dotted minimap styles; solid full-map routes.
- Saved route color, opacity, width, and style settings.
- Moving route start and recalculation after sustained deviation.
- Directional player-to-road guide for off-road starts.
- Automatic executable map-class detection without an exact version/hash allowlist.
- Fail-closed validation for map identity, projection data, destinations, and road records.

Routes minimize geometric road distance rather than measured travel time. Conditional road connections are excluded, and off-road destination endpoints stop at the nearest road point. The directional connector does not test terrain obstacles or move the character automatically.

## Patch 2.03 compatibility

Game patch 2.03.00 exposed two brittle assumptions in version 1.0.6: the road archive gained a fourth top-level section, and the full-map projection block moved by 16 bytes. Version 1.0.7 replaces both exact-build assumptions with bounded structural validation:

- The archive loader accepts bounded additive sections only while the required `worldmaproad` identity and every consumed record/boundary check remain valid.
- The full-map reader finds exactly one plausible paired canvas/transform block inside a narrow validated object window, then re-reads the data and object identity before drawing.

The complete regression suite, untouched patch 2.03 archive probe, read-only live probes, deployment checks, and installed gameplay validation pass. The user confirmed the minimap, full-screen map, full-map pan/zoom, and ReShade health on Steam build 25381195 / executable 1.0.0.2944. Details are in the [patch 2.03 compatibility review](outputs/Update-2.03.00-compatibility-review.md).

Small additive executable or archive changes should no longer require a new exact offset or count. Semantic changes to required game data can still need a mod update and are rejected rather than interpreted speculatively.

## Install or update

1. Install ReShade with full add-on support separately.
2. Close Crimson Desert.
3. Copy **only** `CrimsonNavigationLab.addon64` into `Crimson Desert/bin64`, beside `CrimsonDesert.exe`.
4. When updating, replace only this mod's positively identified same-named add-on. Keep `CrimsonNavigationLab/settings.txt`.
5. Launch the game, load gameplay, and use the game's **Set Destination** action. User-placed map markers are distinct and do not select the active navigation destination.
6. Open **Home → Add-ons → Crimson Navigation Lab** to adjust or save appearance settings.

No installer is included. Do not replace original game files, saves, Steam configuration, ReShade files/configuration, or other mods.

## Development

Source is under [`src/`](src/) and regression tests and read-only research probes are under [`tests/`](tests/). ReShade is the only runtime dependency; local build and research tools are development-only. No game archives, extracted map data, ReShade distribution, or other authors' mod content are included.

`build.cmd` uses the Windows Visual Studio 2026 Build Tools installation and expects separately obtained ReShade SDK headers at `work/reference/reshade/include` with matching Dear ImGui headers under `work/reference/reshade/deps/imgui` (API 20 / ImGui 1.92.5).

```powershell
cmd /c build.cmd
```

The command runs the regression and native-host checks and writes the development add-on under `outputs/local-test-1.0.7/build/`. The verified 1.0.7 release archive is recorded under [`outputs/release-1.0.7/`](outputs/release-1.0.7/RELEASE-INDEX.md); generated binaries and ZIP files remain local and are excluded from source control.

## Known limitations and reports

- Minor minimap drift or turn lag can remain visible.
- Overlapping or disconnected road data can produce ambiguous or unavailable routes.
- Combat and other interactions can temporarily hide the route; it should return while the active navigation destination remains set.
- Linux/Proton remains unverified.

When reporting a problem, include the mod, game, and ReShade versions; OS or Proton/Wine version; resolution and UI scale; route style; whether the issue changes during pan, zoom, movement, or destination clearing; and a screenshot or relevant sanitized log excerpt. See [known issues](docs/known-issues.md).

## Credits, images, and licensing

Developed with OpenAI Codex and iterative user-directed testing. Substantial code and documentation were AI-generated. Crimson Route was research context only; none of its files are included or required.

ReShade and Dear ImGui retain their respective licenses; see [`THIRD-PARTY-NOTICES.txt`](THIRD-PARTY-NOTICES.txt). No project-wide open-source license has been selected, so public source visibility does not itself grant redistribution rights.

Header source: official Pearl Abyss promotional screenshot, modified with an AI-edited gold route highlight and used under the recorded fan-content policy review. Gameplay images were supplied by the user. This is unofficial fan content and is not endorsed by Pearl Abyss.
