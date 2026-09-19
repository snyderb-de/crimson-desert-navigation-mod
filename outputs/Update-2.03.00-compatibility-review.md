# Game patch 2.03.00 compatibility review

Reviewed September 18, 2026. The archive regression and a separate full-map projection shift are confirmed locally. Version 1.0.7-test2 is installed and passes regression, read-only live probes, and relaunched gameplay on both maps, including full-map pan/zoom and ReShade health.

## Verified

- Pearl Abyss released game patch 2.03.00 on Steam on September 18 at 01:00 UTC: <https://crimsondesert.pearlabyss.com/en-US/News/Notice/Detail?_boardNo=131>.
- A Nexus user reported that the update broke Crimson Navigation 1.0.6. The public post appears at September 18, 2:59 AM in the page view; the user relayed it as September 17, 10:59 PM in their time zone: <https://www.nexusmods.com/crimsondesert/mods/3447?tab=posts>.
- The local Steam installation is now build 25381195. `CrimsonDesert.exe` is file/product version 1.0.0.2944, 397,242,776 bytes, SHA256 `6D348BE9D52F81BD35CF7C55E73A5DBFC96CC8268438387C91F7F62C82381FA7`, with a September 17, 2026 9:42 PM local modification time.
- At initial reproduction, the installed add-on was the tested 1.0.6-test1 binary, SHA256 `CF2508191ACD62666C6FBFD1A8407F4E486A42958E3B61E705D60AA814678FBE`.
- The production RTTI detector can parse the new executable and uniquely resolves all four required map identities. New RVAs are `0x5674408`, `0x566FE78`, `0x5674F18`, and `0x56749D8`. Therefore an exact game-version lock and failure to find the four class identities are excluded as the immediate cause.
- The standard workspace build passes every regression suite: 5 route-pattern cases; route progress and pulse checks; recovery/session/routing/projection/settings/readers; 3,997 resolution checks; 15 compatibility checks; 39 layout-detector checks; and native host load/rejection/unload checks.
- Running the optional detector test against the new installed executable prints the four new RVAs but exits with one failure because that test still asserts the old 1.0.0.2850 RVAs. This is a stale diagnostic assertion, not a production version lock.
- The user launched build 25381195, set an active navigation destination, observed no route, and reported that the ReShade panel showed `Reader stopped after an internal error.` The new `ReShade.log` confirms the 1.0.6-test1 add-on loaded, entered map-layout detection, and stopped with that status before reporting map discovery.
- The production road loader independently reproduces the startup exception against the updated installation: `Unexpected road layer`.
- Patch 2.03.00 changed the top-level count at offset 20 in `0008/1.paz` from the loader's expected value 3 to 4. Format version 46 and the first section name `worldmaproad` are unchanged. The updated archive is SHA256 `D91145512890BA2F15965746FB049D08EDE8ADC70178D5C588635C34BF26C9F2`.
- A workspace-only 3,207,193-byte diagnostic copy changed only that count from 4 to 3. The unchanged production loader then read 43,655 nodes and 46,338 road segments and calculated a valid 189-point route. This isolates the rejection to the top-level count check and shows that the required first road section remains parseable by the existing reader.
- Version 1.0.7-test1, containing the bounded archive correction, was installed after the game closed. Its SHA256 is `84497AF09507B23DC2D6E66BF1D9451ECC088C54F6D77E27F16BA9E7FC1F7CBD`; all other 979 files under `bin64` retained their path, size, and modification time during replacement.
- In the following gameplay session, ReShade remained responsive with no reader internal error. Automatic discovery found one minimap and one full-map object, the active navigation destination was acquired, a road route was calculated, and the user confirmed that the minimap route worked. The user confirmed that the full-screen map still showed no route.
- The live full-map object retained the detected class identity, visible flag, 1920x1080 physical body, and native UI scale. Patch 2.03 moved the paired reference canvas and cursor/world transform by 16 bytes: the old `+0xA58/+0xA78` fields now contain other data, while the valid pair is at `+0xA68/+0xA88`.
- A test-first patch 2.03 fixture failed against the fixed-offset projection reader. Version 1.0.7-test2 now requires exactly one structurally plausible paired block in the bounded `+0xA40..+0xAC0` canvas search window, then re-reads the block and object identity before accepting it.
- The complete test2 build passes, including 3,997 resolution checks, 15 build compatibility checks, 39 automatic layout checks, all reader/session/routing suites, and native host load/rejection/unload checks. Against the still-running patch 2.03 game, 20/20 diagnostic samples validate the raw and display full-map projection; a production ReaderCore probe reports 10/10 valid samples, destinations, raw full-map projections, minimap projections, and 1920x1080 display projections.

## Remaining limits

- The successful test covers the current Windows setup at 1920x1080. It does not establish Linux/Proton or every display layout; earlier verified resolution results remain separate evidence.
- The added fourth top-level archive section has not been decoded because the mod does not consume it. The fix must continue to validate format version 46, the `worldmaproad` first-section identity, all bounds/counts, and the `wagon` boundary rather than broadly accepting unknown formats.
- The generic catch in `ReaderService` discards the useful `Unexpected road layer` exception text. Better diagnostics are desirable, but that logging limitation is separate from the compatibility correction.

## Current conclusion

Game patch 2.03.00 breaks Crimson Navigation 1.0.6 in two independent ways. The strict `top-level count == 3` archive check stops the reader before discovery, and the full-map projection block moved by 16 bytes. Test2 removes both unnecessary exact-build assumptions while retaining bounded structural validation. It passes current live values and the user-visible minimap/full-screen route, full-map pan/zoom, and ReShade-health checks.

## Release status

The release archive is prepared from the exact tested test2 binary. Test2 is installed with SHA256 `BC5FB52B86966358C950E7BB8DF2B8DF017E7FF80C2189EADFB7D228C3AE35D8`; the verified test1 backup is retained and all 979 other `bin64` files are unchanged. The fresh policy/license review and complete package integrity check pass. Publication remains a separate user-authorized action. [Candidate record](local-test-1.0.7/START-HERE.md), [release record](release-1.0.7/RELEASE-INDEX.md).

No game files, saves, Steam files, third-party files, or installed mod files were changed during this review. No Nexus post or publication was made.
