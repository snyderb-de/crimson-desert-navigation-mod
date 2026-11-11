# Crimson Navigation 1.0.8-test3 - patch 2.04 compatibility

Built and installed November 10, 2026 for Crimson Desert patch 2.04, Steam build 26480954 / executable 1.0.0.3576.

## Root cause and correction

Patch 2.04 changed several structures at once. The road archive format version went from 46 to 47. The minimap object members moved by different amounts. The destination icon kind changed from 21 to 22. Every UI element gained an 8-byte member before its visibility flags. The full-map canvas/transform block moved outside the 1.0.7 search window. Version 1.0.7 stopped with `Reader stopped after an internal error.` Details: [patch 2.04 review](../Update-2.04-compatibility-review.md).

Version 1.0.8 reads the road file by its actual size and accepts newer format versions when the structure validates. It selects a minimap layout profile by class identity at exactly one profile's canvas member, uses a wider bounded full-map search, and shows reader exception messages in the status line.

## Builds

| Build | SHA256 | Result |
|---|---|---|
| 1.0.8-test1 | `E8390D22ECFE15CDC6775C6C41F45FF0DEA378D3D09A4130FB7E63B4C3EB6071` | Reader ran and found the destination; no route on either map (visibility flag still read at +0xB0) |
| 1.0.8-test2 | `CA735B8DF98F7FBCAB79919F86AAA4339D2647F5A6C3F1168474A27EE13D7741` | Routes on both maps and full-map pan/zoom at 1920x1080; correct placement at 3840x2160 but routes too thin |
| 1.0.8-test3 | `86C35B987D251CAFC805E6A5D121E22168A26258DFE047888302CBEB51B02B5A` | Route width scales with display resolution; approved by the user at 3840x2160; release binary |

## Verification

- Complete regression suite passes, including 11 road archive checks, 27 minimap layout checks, 3,997 resolution checks, 15 compatibility checks and 39 automatic layout checks.
- Production loader reads the untouched 2.04 archive: 44,542 nodes, 47,248 segments, valid probe route.
- Read-only live probe with test2 code: layout `2.04`, active navigation destination found, route valid, 10/10 minimap display projections at 1920x1080.
- Each install happened only after the game closed and the installed add-on matched the expected prior hash. Prior files are backed up in `backup/`. All 979 other `bin64` files kept their path, size and modification time. Records: `deployment-verification.json` (test1), `deployment-verification-1.0.8-test2.json`.

## Gameplay result

The user launched test2, set an active navigation destination, and confirmed routes on the minimap and the full-screen map, including full-map pan and zoom. The saved log (`startup-game-log-test2.txt`) shows test2 loading, one candidate for each map, route calculation, 20 full-map and 4 minimap route submissions, and no internal-error line.

Test3 (`deployment-verification-1.0.8-test3.json`, `startup-game-log-test3.txt`) scales route width by the uniform display fit: 1.0 at 1920x1080, 2.0 at 3840x2160. The user approved it after the 4K session. Other resolutions and Linux/Proton are not tested on 2.04.

No original game file, archive, executable, save, setting, Steam configuration, ReShade file or third-party mod file was changed. The release bundle is in [release-1.0.8](../release-1.0.8/RELEASE-INDEX.md) (ZIP SHA256 `D5275E9B5BD80733B75C6A84A63E29D3199E7535D044E34293B0A55AD63F4C91`). No upload or page edit was performed.
