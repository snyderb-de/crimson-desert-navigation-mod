# Crimson Navigation 1.0.9-test1 - minimum dotted radius

Built and installed November 11, 2026 on top of the 1.0.8 release. Not yet run in game. Released as [1.0.9](../release-1.0.9/RELEASE-INDEX.md).

## Change

Fixes [TODO-006](../../docs/known-issues.md). Route width scales by `min(width/1920, height/1080)`, down to 0.5. Below 1080p, the Dotted minimap style could give a dot radius under 0.5 px, and ImGui draws nothing for that. Dots now keep a 0.5 px minimum radius. At 1920x1080 and above, every allowed width already gave at least 0.5 px, so output there is unchanged.

## Build

| Build | SHA256 | Result |
|---|---|---|
| 1.0.9-test1 | `C86EC6A8DADAEF8E7474EEC759C33F918A81C7104C237C8EC084316D66A20B4E` | Offline checks pass; installed; release binary; not yet run in game |

`build/CrimsonNavigationLab.addon64` (ignored by Git). MSVC builds are not byte-reproducible, so a rebuild gives a different hash.

## Verification

- Complete regression suite passes, including 16 route scale checks (6 new for the dot radius) and the native host check.
- Installed after the game closed. The installed add-on matched the 1.0.8 release binary, which is backed up under `backup/`. Installed and candidate SHA256 match; all 979 other `bin64` files kept their path, size and modification time (`deployment-verification-1.0.9-test1.json`).
- Untested: Dotted style at a resolution below 1920x1080 in game.

## Suggested in-game check

Set the minimap style to Dotted and the route width to 1.0. Run the game below 1080p (for example 1280x720). Dots should stay visible on the minimap. No original game file, archive, executable, save, setting, Steam configuration, ReShade file or third-party mod file was changed.
