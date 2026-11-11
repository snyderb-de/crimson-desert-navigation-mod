# Game patch 2.04 compatibility review

Date: November 10, 2026. Game: Crimson Desert, Steam build 26480954, executable 1.0.0.3576, SHA256 `C94AD6C3EA54970A6D2DA7A2719753210A7728A91698ED3405DD9125470ED98F`. The patch also adds a DLC area (`dlc/dlc_01`).

Result: version 1.0.7 does not work on patch 2.04. Version 1.0.8-test2 restores routes on the minimap and the full-screen map. The user confirmed this in game on the current Windows 1920x1080 setup.

## Symptom

With an active navigation destination, no route appeared on either map. ReShade showed `Reader stopped after an internal error.` The add-on loaded and then stopped before map discovery.

## Verified changes in patch 2.04

Each item below was measured read-only against the installed game or the running process.

| Area | Before (2.03) | Patch 2.04 | Effect on 1.0.7 |
|---|---|---|---|
| Road archive `0008/1.paz` format version | 46 | 47 | Loader rejected the version and stopped the reader |
| Road archive size | Fixed 3,207,193-byte read | Larger file; `worldmaproad` layout unchanged | Fixed-size read was another unnecessary identity |
| Map class identities (RTTI) | 2.03 RVAs | New RVAs, found by the existing automatic detector | None. Detection still works |
| Minimap root members | Legacy offsets | Non-uniform shifts (player +0x10, HUD +0x18, terrain/destination +0x20, canvas +0x28, origin/distance +0x40) | All minimap fields read wrong data |
| Minimap HUD rectangle member | `MinimapHud` | `MinimapDisplay` (same circle rectangle) | Geometry rejected |
| Destination row tag / icon kind | 21 | 22 | Active navigation destination not found |
| Destination-category icon flags | 1 or 3 | Also exactly `0x23A` on one non-destination presentation icon | Snapshot rejected as unknown |
| Generic UI element visibility flags | +0xB0 | +0xB8 (an 8-byte member was inserted) | Both maps read as hidden |
| Full-map canvas/transform block | Root +0xA68 | Root +0xAC8 (another 0x60) | Outside the 1.0.7 search window |
| Full-map UI scale chain | Unchanged | Unchanged | None |

Road data: the base archive gained 662 nodes outside the old bounds (east near x 3,000-4,000, z -6,000 to -7,000; far south near z -15,000 to -16,000). The production loader reads 44,542 nodes and 47,248 segments. A probe route has 192 points and length 2073.76, against 189 points and 2073.71 on 2.03.

## Correction (1.0.8)

- Road loader: reads the actual file size (capped at 16 MiB) instead of a fixed byte count. It accepts format versions 46 through 255. All structural checks remain: required `worldmaproad` identity, record bounds and limits, finite coordinates, unique node identifiers, connection endpoints, and the `wagon` boundary. A version above 47 is accepted only when this structure validates. The status line then reports the road format as unverified.
- Minimap layout profiles: each profile lists the root member offsets, destination kind, HUD element name, measured presentation flag and UI flag offset. A profile is selected only when the minimap canvas class identity sits at exactly that profile's canvas member. No match or more than one match selects nothing. Every reader still checks names, links and values.
- Presentation icon: only the exact measured value `0x23A`, and only in the 2.04 profile, is skipped. Nearby values such as `0x23B`, `0x203`, `0x200` and `0x235` still reject the snapshot.
- Full-map block search: the bounded window is now root +0x900 through +0xD00. It still requires exactly one plausible canvas/transform pair, and it re-reads the block and the object identity. The window is read in one bulk read per frame, with a per-candidate fallback.
- Diagnostics: reader exceptions now show their message in the status line. The ReShade panel shows the selected minimap layout.

## Validation

Verified:

- Complete regression suite passes: 11 road archive checks, 27 minimap layout checks, 3,997 resolution checks, 15 compatibility checks, 39 automatic layout checks, and all reader, lifecycle and projection suites.
- Production loader reads the untouched patch 2.04 archive (44,542 nodes, 47,248 segments, valid probe route).
- Read-only live probe with test2 code: layout `2.04` selected, active navigation destination found, route valid, minimap visible, and 10/10 minimap display projections valid at 1920x1080.
- Full-map block at +0xAC8 is unique over root +0x100 through +0x1800. The UI scale chain is valid (scale 1.0).
- In game with 1.0.8-test2: the user confirms routes on both maps, including full-map pan and zoom. The ReShade log shows the add-on loading, one candidate for each map, route calculation, 20 full-map and 4 minimap route submissions, and no internal-error line.

Experimental or not verified:

- 1.0.8-test1 fixed the reader stop but drew no route, because the visibility flag move was not yet known. Test2 fixed that.
- Other resolutions and Linux/Proton are not tested on 2.04. The full-map UI scale chain did not move, so earlier resolution evidence is expected to carry over. That is an assumption, not a test.
- Road format versions above 47 are accepted on structure only. No such file exists yet.

## DLC area

Correction: an earlier note in this review said the DLC road data looked compressed or encrypted. That was wrong. It came from parsing the patch 2.04 index with the old layout.

Patch 2.04 index (`.pamt`) layout, verified by a parse that ends exactly at the file end and resolves every folder and file in both `0008/0.pamt` and `dlc/dlc_01/0008/0.pamt`:

- Folder and file names are a trie. Each entry is a varint back-distance to its parent entry (0 = root), a one-byte length, then the text.
- File flags: the high byte indexes a new extension table at the end of the index. The low nibble is the compression method and bits 4-7 the encryption method.
- The extension table is a 32-bit byte count followed by null-terminated extensions.
- `fielddefault.road` resolves to `0008/1.paz` offset 0, uncompressed, which matches what the add-on already reads.

DLC road content, all read-only:

- All 167 DLC road files (67 `.road`, 100 `.roadsector`) are stored uncompressed and unencrypted.
- All 67 DLC `.road` files are format version 47 with one layer, `tosidewalk`. None has a `worldmaproad` layer. Their names (`construction_*island_working`, `shipyard_*_working`, `castlegrant_*_working`, `roadlevel_dlc_01`) suggest NPC work and sidewalk paths. That reading is an inference from names, not a decoded meaning.
- Road-sector names encode 64-world-unit cells. A DLC island sector at cell x -184 holds coordinates near x -11,725, which confirms the scale.
- Base `worldmaproad` coverage per DLC sector group (`work/patch-2.04-evidence/dlc-island-coverage.txt`): the east area around x 3,700, z -5,800 has 306 nodes. Satyros, Razanui and Iliya islands have some nodes. The other 15 `construction_*island` groups have none within 500 units.

Conclusion: the world-map road graph for the new areas is in the base `fielddefault.road`, which the add-on reads. The DLC adds no world-map road layer, so reading DLC files would not add player routes. For a destination on one of the 15 islands without world-map roads, the route is expected to end at the nearest road point. That behavior is not verified in game.

## Files and evidence

- Build and test guide: [local-test-1.0.8/START-HERE.md](local-test-1.0.8/START-HERE.md)
- Deployment records: `local-test-1.0.8/deployment-verification.json` (test1), `local-test-1.0.8/deployment-verification-1.0.8-test2.json`
- Add-on log lines: `local-test-1.0.8/startup-game-log-test2.txt`
- Research probes and raw captures: `work/patch-2.04-evidence/` (workspace only), including `dlc_pamt_probe2.py`, `dlc_island_coverage.py`, `dlc-island-coverage.txt` and `dlc-road-layers.txt`
