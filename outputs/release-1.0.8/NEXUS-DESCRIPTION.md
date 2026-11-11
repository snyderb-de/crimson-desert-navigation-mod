# Crimson Navigation

Follow a road route to your active navigation destination on the minimap and full-screen map. Customize the route color and width, choose a minimap style, and keep the route's start with your character as you move.

## What's changed in 1.0.8

- Fixed the game patch 2.04 failure where no route appeared and the panel showed "Reader stopped after an internal error."
- Newer road data formats no longer block startup when all structural checks pass.
- The changed patch 2.04 minimap layout, destination icon and map visibility flags are detected automatically.
- The full-map projection is found again after patch 2.04 moved it.
- Route width now scales with your resolution: unchanged at 1080p, twice as thick at 4K, thinner below 1080p.
- Reader errors now show their reason in the ReShade panel.

## Known issues and bug reports

Route placement was checked at 1920x1080 and 3840x2160. Other resolutions or display layouts may still have issues. Please report them with your resolution, game/mod/ReShade versions, a screenshot, and what you were doing when it happened.

DLC areas use the world-map roads from the base game. Many DLC islands have no world-map roads, so a route to a destination there is expected to end at the nearest road point. This has not been checked in game yet.

Some interactions, including combat, may temporarily make the route disappear. It should return automatically as long as your active navigation destination remains set on the map. If it does not return, please report it.

Minimap drift or lag can still be noticeable when turning. Tightening that alignment is still being worked on.

## Coming next

Planned additions:

- Routing to user-placed map markers.
- Pulse animation speed.
- Pulse blur.
- Route glow.

These features are not included in this release.

## Features

- Automatic startup and route calculation when you set an active navigation destination.
- Solid, pulse, dashed and dotted minimap route styles; solid routes on the full map.
- Saved color, opacity, width and route-style settings in ReShade.
- Route width that scales with display resolution.
- Moving route start and rerouting after sustained deviation.
- Dashed bearing from your position toward the road route.
- Off-road destinations end at the nearest road point.

Fresh-install defaults are gold #B17C01, width 6 and the minimap pulse style. Existing saved settings are preserved. A saved width now draws thicker above 1920x1080 and thinner below it; adjust it if needed. Reset appearance restores all settings, including automatic startup.

## Requirements

Windows x64. There is no fixed game-version requirement. The add-on detects the current map layout at startup and checks live map data before drawing. Updates that change the required game structures may still need a mod update. Linux/Proton remains unverified.

**ReShade with full add-on support** is the only runtime dependency. Tested with ReShade 6.8.0.2155. Download ReShade separately from [reshade.me](https://reshade.me/).

This is a **ReShade add-on (.addon64)**. No ASI loader is required.

## Installation and updates

1. Close the game.
2. Extract the download and copy **only CrimsonNavigationLab.addon64** into **Crimson Desert/bin64**, beside CrimsonDesert.exe.
3. When updating, replace only this mod's previous same-named add-on. Keep the CrimsonNavigationLab settings folder.
4. Launch and load gameplay. Use the game's **Set Destination** action. User-placed map markers alone do not select an active route.
5. Open **Home > Add-ons > Crimson Navigation Lab** to adjust settings, then use **Save settings**.

Allow the reader to finish discovery after loading. Use Start / restart navigation if needed. Reset appearance > Save settings restores the defaults.

The menu displays **1.0.8-test3**. This build was tested on game patch 2.04 / executable 1.0.0.3576 / Steam build 26480954. Regression and native host checks pass, and the production road loader reads the untouched updated archive. The previous test build showed routes on the minimap and full-screen map, including full-map pan/zoom, at 1080p and correct placement at 4K. This build adds only the resolution-scaled width and was run and approved at 4K.

Manual installation is supported. Do not replace original game files, ReShade files/configuration, saves or other mods. Mod-manager deployment is unverified.

## Road guidance

Routes follow geometric road distance, not guaranteed fastest travel time. Conditional road connections are excluded; disconnected or overlapping roads can affect routing. Dashed player-to-road connectors are directional guides and do not check terrain obstacles. No automatic character movement is performed.

The add-on reads installed game data locally. The mod download contains no game assets, extracted map data, ReShade installation or other authors' mods. Game updates may require a compatible mod update.

## Removal

Close the game and remove only CrimsonNavigationLab.addon64. Optionally remove this mod's CrimsonNavigationLab/settings.txt. Leave ReShade, original game files and other mods in place.

## Credits and disclosure

Developed using OpenAI Codex and Anthropic Claude Code with user-directed design and in-game testing. Most of the code and this description were AI-generated. ReShade and Dear ImGui license notices are included.

The existing header is AI-edited promotional artwork based on an official Pearl Abyss screenshot. Its highlighted world-space road is illustrative; actual routes appear on the minimap and full-screen map. Existing gallery images are genuine user gameplay captures from version 1.0.0; the current panel and route width differ. They are not new validation images.

This is unofficial content which contains copyrighted materials and IP from Pearl Abyss, and is not official/endorsed content.
