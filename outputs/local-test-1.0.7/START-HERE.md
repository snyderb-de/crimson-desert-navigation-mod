# Crimson Navigation 1.0.7-test2 - patch 2.03 compatibility

Built and installed September 18, 2026 for Crimson Desert patch 2.03.00, Steam build 25381195 / executable 1.0.0.2944.

## Root cause and correction

Game patch 2.03.00 increased the top-level section count in `0008/1.paz` from 3 to 4 while retaining road format version 46 and the required first `worldmaproad` section. Version 1.0.6 required the total count to equal exactly 3, threw `Unexpected road layer`, and stopped before map discovery.

Version 1.0.7 accepts a bounded top-level count from 3 through 64, then retains every existing check for format version, required section identity, field bounds, node and edge limits, finite coordinates, unique node identifiers, connection endpoints, condition encoding, and the following `wagon` boundary. This prevents harmless added sections from blocking the road reader without accepting changed or malformed road data.

Installed test1 proved that correction: ReShade stayed healthy, automatic map discovery completed, the active navigation destination was acquired, a road route was calculated, and the user saw the minimap route. The full-screen map remained blank. A read-only live diagnostic then established that patch 2.03 moved the full-map reference-canvas and cursor/world transform pair from `+0xA58/+0xA78` to `+0xA68/+0xA88`; the object identity, visibility, physical body extent, and UI scale remained valid.

Test2 searches a narrow bounded part of the already validated full-map object for exactly one plausible canvas/transform pair with their known 32-byte relationship. It re-reads the selected data and object identity before accepting the projection. Missing, ambiguous, unstable, non-finite, or out-of-range data remains rejected. This removes the unnecessary exact member-offset dependency while preserving the safety checks that prevent drawing from unrelated memory.

## Verification

- Test-first reproduction: the new four-section fixture failed against 1.0.6 while legacy and rejection cases behaved correctly.
- The exact four-section case passed after the minimal correction.
- A second test-first resilience case proved that an added fifth section no longer blocks the named road layer; counts below 3 or above 64 and a wrong first-section name remain rejected.
- All 6 archive-header tests and the complete regression build pass, including 3,997 resolution checks, 15 build compatibility checks, 39 automatic layout checks, reader lifecycle suites, and native host load/rejection/unload checks.
- The production loader reads the untouched patch 2.03 archive: 43,655 nodes, 46,338 segments, and a valid 189-point probe route.
- The new patch 2.03 shifted-block fixture failed against the fixed-offset reader before the projection correction and passes afterward.
- Against the running updated game, 20/20 diagnostic samples validate the raw and display full-map projection; the production ReaderCore probe reports 10/10 valid map samples, destinations, raw full-map projections, minimap projections, and 1920x1080 display projections.
- Final staged test2 candidate SHA256: `BC5FB52B86966358C950E7BB8DF2B8DF017E7FF80C2189EADFB7D228C3AE35D8`.
- Test2 was installed only after the game process closed and the installed file matched verified test1 SHA256 `84497AF09507B23DC2D6E66BF1D9451ECC088C54F6D77E27F16BA9E7FC1F7CBD`. That file is backed up under this local-test folder. The installed test2 hash matches the candidate, and all 979 other `bin64` files retained their path, size, and modification time. See `deployment-verification-test2.json`.

## Gameplay result

After installing test2, the user launched normally, set an active navigation destination, and confirmed the minimap route, full-screen map route, full-map pan/zoom, and ReShade health. The saved log confirms test2 loaded, automatic discovery found one candidate for each map, the active route was calculated, and the full-map route was submitted repeatedly.

This passes the requested patch 2.03 acceptance check on the current Windows 1920x1080 setup. Other resolutions retain their earlier evidence and known limitations; Linux/Proton remains unverified. The exact tested binary is the staged release binary.

The finalized four-file archive is `release-1.0.7/Crimson-Navigation-1.0.7.zip`, SHA256 `E649C6B9C1614A56B8EF09160E9118E4E4103B24075B9BC66E2BADEC9FC13857`, 236,930 bytes. Package integrity, x64 exports, dependency notices, exact binary identity, BBCode balance, and runtime evidence checks pass. No Nexus upload or page edit was performed.

No original game file, archive, executable, save, setting, Steam configuration, ReShade file, or third-party mod file was changed. Only the positively identified previous Crimson Navigation add-on was replaced. No public upload or page edit was performed.
