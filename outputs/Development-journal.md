# Development journal

## 0.3.1 deployment — September 7, 2026

Verified the game was closed and both installed/source hashes matched expectations. Replaced only our add-on with 0.3.1, SHA-256 0C56C8FC5D70BB7029E25B405E654FB17433B4E4C98FD376F19B63F3F67F7AE5. All 979 other files recursively under bin64 retained their hashes; no other files were added. Saved local-test-0.3.1/deployment-verification.json. Next is gameplay/full-map/gameplay visual verification of calibration suppression and return; no in-game fix claim yet.

## Parent visibility correction and full-screen map research — September 7, 2026

Reproduced the calibration leak in a unit test: parent MinimapView bit 0x40 cleared at +0xB0 while child layout byte still reported visible. Added parent name/link validation and repeated flag reads; all 12 map-reader cases now pass. Built 0.3.1 with all target/projection/settings/worker/host checks passing. Updated diagnostic wording to Minimap view signal rather than asserting a specific full-screen map state. Native probe reports hidden in the current state, but no corrected in-game rendering result is claimed. Prepared package remains undeployed while the game runs.

Continued independent full-screen research. A bounded single-type scan found UIGamePlayControlRootWorldMap at 0x412b390b600 in PID 40584. Its +8 UI node identifies WorldMapBody. Extended capture found WorldmapBg (+0x658), WorldmapCursorWrap (+0x668), PerspectiveWrap (+0x7A0), and WorldmapLoadingWrap (+0xBF0). Extended traversal hit its 256-block cap; arbitrary readable references are not all UI objects (one identifier was an asset filename). No map pan/zoom transform is validated yet. Evidence: world-map-roots-1788812916138134400.json, worldmap-geometry-discovery-1788812939182958400.json, worldmap-geometry-extended-discovery-1788812972771068500.json under work/reader-research.

User clarified the final route must appear on both minimap and full-screen map, each with its own projection, and asked work to continue proactively. Do not wait for a new go-ahead between independent steps. Preserve the requirement to wait for explicit completion and allow at least 60 seconds for requested game actions.

## Held-open map capture — September 7, 2026

Completed capture after explicit user confirmation that the map was open and held. The old MinimapHudBody layout flag remained visible; named MinimapView parent instead cleared bit 0x40 at +0xB0. Earlier PID 42116 captures show this bit set/clear/set over the controlled open/close cycle. Requested current-session closure to repeat the result. Capture completion was communicated before asking for closure. No add-on change yet.

## Full-screen calibration leak confirmed — September 7, 2026

User supplied a full-screen map screenshot showing the calibration circle and line still drawn at the minimap position. Saved local-test-0.3.0/fullscreen-map-calibration-leak.png and marked suppression failed in build-verification.json. Paused world-coordinate scale investigation to prioritize this bug. Subsequent live reads report map-hidden candidate 0, but the user's menu state at those later reads was not held/confirmed; do not treat them as a controlled map-open experiment. Added bounded HUD-ancestor capture (32 nodes maximum) and requested the user hold the full-screen map open until capture completion. Existing diagnostic flag is insufficient as a proven rendering gate; no fix deployed.

A development world/screen pairing script initially shadowed its process-handle variable with a rectangle height. It failed before producing evidence; corrected the local variable name. This was a research-tool error, not a game or add-on failure. Pairing analysis remains incomplete.

## Camera rotation comparison passed — September 7, 2026

User supplied the requested rotated-camera screenshot. Captured unchanged player XYZ, icon/canvas angle shift of approximately -113.586132 degrees, stable unit icon scale and 4.266666 canvas-view scale, and preserved minimap center. Saved camera-rotation-comparison.json and the screenshot in local-test-0.3.0. This establishes observed rotation correlation; world-axis orientation and world-distance scaling remain unresolved. The current static calibration shape is behaving as designed. No binary was changed.

## Rotation/scale baseline — September 7, 2026

User cannot change resolution currently; that test remains pending. Inspected named icon/background/canvas-view transform blocks in PID 40584 at the current resolution. Candidate matrices and adjacent inverses agree numerically, and all preserve the known minimap center. Icon angle is approximately -40 degrees with unit scale; canvas-view angle matches with scale 4.266667. Requested camera-only movement to determine whether these values follow camera rotation. Captures and analysis saved; no runtime change or world-projection claim.

## First actual minimap calibration screenshot — September 7, 2026

The user's full 1920x1080 gameplay screenshot shows the yellow calibration circle and bent line within the actual lower-left minimap, with the center visually near the player marker. The inset circle is consistent with the deliberately reduced 85% calibration radius. Saved local-test-0.3.0/minimap-alignment-1920x1080.png and updated build-verification.json. This is approximate visual alignment at one resolution, not measured pixel accuracy. The shape is fixed screen-space calibration, not a destination or road route. Dynamic resolution changes, world rotation, zoom-dependent projection, and gameplay frame-time impact remain unverified; the screenshot's FPS counter does not measure the mod's overhead.

## 0.3.0 deployment — September 7, 2026

After the user closed the game, independently confirmed no CrimsonDesert process. Verified installed 0.2.1 and replacement hashes and updated only our add-on. Installed SHA-256 BB84093BDDD01C763105AB9084BC83567D2DAAF3904D62E0536D326F0FCD0A33. All 979 other files under bin64 retained their hashes, with no other additions. Saved local-test-0.3.0/deployment-verification.json. Actual minimap calibration alignment and resolution changes remain pending gameplay tests.

## Dynamic minimap calibration build 0.3.0 — September 7, 2026

Captured named layout elements in PID 20544. Current MinimapHudBody reads (0,0,1920,1080), MinimapHud reads (50,845,200,200). These are observations, not constants to ship. User explicitly emphasized that resolution can change. Implemented dynamic mapping from current canvas/rectangle to current render dimensions, rejecting invalid or aspect-mismatched layouts. Unit tests initially failed positive cases, then eight projection cases passed, including uniform resolution scaling and a matching ultrawide canvas. Actual in-game resizing and visual alignment remain untested.

Added opt-in inset circle/dot/bent-line calibration to the foreground draw list, independent of the old preview checkbox. It uses route appearance settings, hides on invalid/stale state or map-hidden candidate, and defaults off each launch. This is not world projection or navigation. The shared worker samples layout with other diagnostics. Native probe validated named layout acquisition in the running game. All reader/settings/worker/host tests pass; final build has no warnings and imports only Windows bcrypt.dll and KERNEL32.dll. Prepared local-test-0.3.0 with hashes, licenses, and test instructions; installed 0.2.1 remains unchanged.

## Fresh-launch no-destination regression passed — September 7, 2026

Following the requested full restart and gameplay load without setting a destination, the user supplied a 0.2.1 screenshot showing Target: no supported candidate in collections, raw destination UI flag 1, cached distance 0, and closed/visible map candidate. Saved local-test-0.2.1/fresh-launch-no-destination.png and updated build-verification.json. This passes the reported fresh-launch false-positive regression for this user test. Restart procedure is user-provided test context, not independently observed process instrumentation. It does not establish every load, marker type, or game state. Actual minimap route drawing remains unimplemented.

## 0.2.1 live clear check — September 7, 2026

After the user confirmed clearing the destination, native discovery in PID 35784 found one validated root (0x3da788a4800). The shared collection reader returned no_candidate, zeroed target output, and cached distance 0 while the raw destination UI byte remained 1. This confirms correct negative selection for this observed clear state despite the unreliable UI signal. The map-hidden candidate was also 1; its precise cause was not independently confirmed. A bounded collection capture found only the two retained tag-21 icons in both collections. Evidence: work/reader-research/icon-collections-v021-cleared-confirmed-1788809849569623500.json. This is a native shared-reader observation, not a new screenshot of the ReShade panel. Fresh launch with no destination before any destination interaction remains a separate pending regression.

## First 0.2.1 panel screenshot — September 7, 2026

Follow-up: user explicitly confirmed an active destination was set. The displayed candidate is therefore consistent with an active-destination test, not evidence of a fresh-launch false positive. Requested clearing only the destination while preserving custom markers; wait for user confirmation and allow at least 60 seconds. The negative-state check is still pending.

User screenshot confirms the 0.2.1 panel runs and displays a matching collection candidate at (-5342.77,0,-2420.16), cached distance 1059, player (-4857.43,371.65,-3361.80), and raw UI flag 1. Saved local-test-0.2.1/first-panel-target-candidate.png. Requested confirmation of actual in-game destination state, including any restored on loading. The screenshot alone does not establish whether the fresh-launch no-destination regression passed or failed. Do not label this a successful target-validity test yet.

## 0.2.1 deployment — September 7, 2026

User closed the game; process check confirmed it. Verified installed 0.2.0 hash and built 0.2.1 hash, then replaced only our add-on. Installed SHA-256 C2FB75DC786AF418A9EB8D74DB2F524F25D68F93CF31C2BE13FB251CA58AF9A3. All 979 other files recursively under bin64 retained their hashes, with no other additions. Saved local-test-0.2.1/deployment-verification.json. Next is fresh-launch testing with no active destination. No in-game success is claimed yet.

## Collection reader and 0.2.1 — September 7, 2026

Confirmed the newly added destination icon left both collections when the user cleared it; retained entries stayed. Implemented a bounded target reader requiring agreement between both collections, expected icon type/owner, supported flags, finite XYZ, and unchanged reread evidence. Zero coordinates remain valid data; cached distance and UI flag are not selection conditions. Unknown kinds, duplicate or disagreeing entries, invalid counts, recycled objects, and ambiguity do not yield a selected target.

Test-first positive cases initially failed, then all twelve target-reader cases passed. Three fixtures preserved captured icon bytes with reconstructed collection storage and produced no-candidate/candidate/no-candidate for cleared/set/cleared. These are reconstructed fixtures, not full process-memory replays. A live native probe in PID 5736 reports no supported candidate in the cleared state. Ten map cases, five settings cases, worker checks, and host lifecycle checks also pass. Built 0.2.1 without warnings; imports remain Windows bcrypt.dll and KERNEL32.dll.

Prepared local-test-0.2.1 with instructions, licenses, and hashes. Installed 0.2.0 remains unchanged while the game runs. Fresh-launch regression and actual 0.2.1 ReShade panel behavior remain untested; do not claim the reported false positive is fully fixed yet. User requests at least 60 seconds for game actions and explicit confirmation before captures.

## Root-owned destination candidate — September 7, 2026

Traced two compact icon collections from the validated minimap root. Captured cleared baseline, then the user's destination-set state. One icon was added to both collections at (-5329,483,-2337), matching the 1133 distance through a 3D calculation (1133.579894). It shares key 0/tag 21 with a retained entry but has different candidate flags at +0xDE (3 versus 1). Requested clear-removal verification. Broad ownership traversal hit its block cap; only bounded collection captures support this narrower finding. Updated research documentation; no runtime changes.

## Fresh-session clear comparison — September 7, 2026

The user cleared the destination and the player stayed exactly stationary. Distance changed 1037 to 0, linked icon byte 1 to 0, and metadata list count 1 to 2. Saved expanded capture and fresh-session-clear-comparison.json. This reproduces post-interaction behavior but does not repair the confirmed initial no-destination false positive. Initial/default UI state remains a hypothesis; independent target validity is required. No new binary or deployment occurred.

## Confirmed fresh-session destination false positive — September 7, 2026

User confirmed the 0.2.0 screenshot had no destination. After setting one, native readings changed distance 0 to 1037 while the destination candidate stayed 1. This establishes that the UI byte alone is insufficient across sessions. Captured expanded active-state links in PID 5736 using automatically discovered root 0x47dc5868400; requested a stationary clear comparison. No production fix was made. Earlier destination reversals are limited session evidence, not validated active-destination detection.

## First in-game diagnostic screenshot — September 7, 2026

The user supplied a screenshot of the actual 0.2.0 ReShade panel showing successful discovery, player (-4857.43, 371.65, -3361.80), destination active candidate, full-screen map closed candidate, cached distance 0, and Saved settings loaded. Saved it as local-test-0.2.0/diagnostics-first-game-test.png. A separate native probe against new PID 5736 found one validated root without session addresses after scanning 5,446,127,616 bytes; its values matched the screenshot. This verifies panel operation and discovery after a game restart. Active candidate with distance 0 remains ambiguous; asked whether an actual navigation destination was set. Do not count destination validity or route rendering as passed.

## Diagnostic deployment completed — September 7, 2026

After the user closed the game, independently confirmed no CrimsonDesert process, verified the installed 0.1.0 and source 0.2.0 hashes, and replaced only our CrimsonNavigationLab.addon64. New SHA-256: 6C0D8AEE994289135F7770C58F3FC60DFCACD2165EF2975E2E88906A51099BD7. All 979 other files recursively under bin64 retained their hashes and no other files were added. Settings and third-party content were preserved. Saved local-test-0.2.0/deployment-verification.json. Actual in-game panel testing remains pending.

## Native diagnostic build 0.2.0 — September 7, 2026

Implemented a bounded shared reader with root/canvas vtable checks, exact UI identifiers, finite coordinates, boolean candidate checks, and identity rechecks. Positive tests failed before implementation; all ten reader cases now pass, including cached distance and mid-read replacement. Added executable SHA-256 gating and discovery without session addresses. A native read-only development host scanned 5,670,088,704 bytes, found exactly one validated root, and read the expected position, destination candidate, closed-map candidate, and distance 1451 in about 2.45 seconds overall.

Integrated opt-in ReShade diagnostics through a background worker, capped discovery at 8 GiB/90 seconds and polling at about 250 ms. Invalid state clears the display and requires manual restart after loading. No automatic reacquisition, destination-coordinate selection, or minimap routing is included. Final build had no warnings. Five settings cases, ten reader cases, unsupported-host rejection, ten worker cycles, and three registration/unload cycles passed. Imports are Windows bcrypt.dll and KERNEL32.dll.

Prepared outputs/local-test-0.2.0 with instructions, notices, and build-verification.json. Game remains running and installed 0.1.0 unchanged. In-game ReShade diagnostics, load behavior, and scan frame-time impact remain untested. Close the game before replacing only our known add-on.

## Full-screen map reversal confirmed — September 7, 2026

On the user's confirmed map closure, the parent-linked candidate returned 1 to 0, destination element +0xB5 returned 132 to 128, and the destination-linked candidate stayed 1. Position and distance 1451 remained unchanged. Saved fullscreen-map-reversal-comparison.json. The full-screen map visibility correlation is reversible independently of destination state in this session. Other menus, loads, and stable object acquisition remain unverified; no installed code changed.

## Full-screen map-open observation — September 7, 2026

Captured the user's confirmed full-screen map-open state. Player and distance 1451 were unchanged; destination candidates stayed active. A parent MinimapHudBody-linked byte changed 0 to 1, and destination UI flag bytes changed consistently. Requested map closure to check reversibility. This supports investigating destination validity and minimap visibility separately; no render gate is validated yet.

## Destination UI reversal confirmed — September 7, 2026

After explicit reactivation, the linked visibility candidate reversed 0 to 1 and the related list count 2 to 1. Icon +0xB4 bytes returned exactly to the earlier active value. Player coordinates were identical across active/clear/repeated-clear/reactivated samples; the new distance was 1451. Saved destination-ui-reversal-comparison.json. This is reproducible session correlation, not a production destination-valid flag. Menu/load/object-lifetime behavior remains untested. No game-file or installed add-on changes.

## Cleared-state stability sample — September 7, 2026

The user's readiness prompted another read-only capture. It still matches the cleared UI state: linked byte 0, candidate list count 2, unchanged player coordinates. Requested explicit destination reactivation for the reversal test. Noted that root distance is 0 in this clear cycle, unlike the earlier retained 1633; the documentation preserves this variation rather than claiming distance is always retained.

## Expanded clear comparison — September 7, 2026

The user cleared the destination and remained stationary. Captured changes in all three named destination UI elements, including a linked icon byte changing 1 to 0 and a candidate metadata list growing from one entry to two. Saved the pointed list blocks; they are structured data rather than readable class names. These are visibility candidates requiring semantic and reversal checks, not validated flags. The evidence is now more specific than the stale world-coordinate/root-distance snapshots. Updated the research report; the installed mod is unchanged.

## Named UI ownership probe — September 7, 2026

Implemented and ran a bounded read-only UI-link capture, resolving explicit string references for DestinationDistanceIcon, DestinationWrap, and DestinationDistanceText. This identifies which linked objects to compare for visibility rather than searching arbitrary adjacent strings. The probe caps traversal depth, block count, and read sizes; it remains a workspace research tool. Requested a fresh clear comparison with this expanded evidence. No active-state flag or production reader is claimed.

## Destination reactivation comparison — September 7, 2026

With the player stationary, reactivation changed the distance from cached 1633 to 383. A bounded rescan found the replacement candidate at (-4465.8193359375, 0, -3316.81689453125), computed horizontal distance 383.939000. The earlier candidate no longer validated. This repeats coordinate correlation but does not establish active-state detection; +0x538 continues to vary and is unsuitable as a simple active flag. Captured linked UI geometry for further ownership/visibility analysis. Documentation updated; no production code, deployment, or game-file changes.

## Stationary destination-clear result — September 7, 2026

The user confirmed the clear check. The player remained stationary, but the old destination icon, its node snapshot, and distance 1633 were retained. This establishes a stale-state hazard for a reader relying only on coordinates/distance. Root +0x538 changed 14 to 10, but historical active samples vary too, so no active flag is established. Saved a machine-readable comparison and requested reactivation to continue isolating the state. No game files or runtime add-on were changed.

## Stationary destination-coordinate comparison — September 7, 2026

With unchanged player coordinates, changing only the active destination replaced the previous icon candidate and changed the minimap root distance from 7049 to 1633. A further bounded read-only scan found a new candidate whose world X/Z gives distance 1633.128256. This is strong session evidence for destination coordinates. Three retained candidates share internal tag 21, so that tag is not sufficient for reliable selection. Requested a stationary destination-clear check to investigate inactive state. Updated Live-map-reader-research.md with exact values and evidence paths. The installed appearance add-on remains unchanged; live routing is not yet implemented.

## Read-only live map investigation — September 7, 2026

Verified the installed executable hash and unique player-root/static-position signatures. A read-only process reader rejected the absent player actor during startup, then passed actor-type checks in gameplay. Following the user's confirmed walk, world and physics positions changed together by approximately 44.21 coordinate units. Found live minimap/world-map UI candidates through RTTI-derived vtable scans; the minimap root contains coordinates matching the independent player reader and links directly to the icon canvas.

The user set and then changed an active destination. A linked edge-indicator candidate moved while the player remained stationary. Destination world coordinates, projection, rotation, and zoom remain unresolved. The user clarified that destinations and custom markers are separate; documentation now preserves that distinction. Extracted three minimap UI definition files locally for research, confirming separate pin/marker/destination types and dedicated destination UI elements. The working add-on was not modified during these checks. See Live-map-reader-research.md and work/reader-research.

The user then confirmed clearing the destination. Player position also changed by the next sample, preventing an isolated clear-state inference. A subsequent bounded scan found 300 minimap icon-class candidates; pooled/stale object membership and event-type mapping remain unresolved. No production reader or new add-on was deployed. Captured snapshots support continued offline investigation; no additional user action is needed for preserving this evidence.

## User-reported in-game appearance test — September 7, 2026

The user reported “all worked” for launching the game, seeing the sample preview at upper right, opening Home > Add-ons > Crimson Navigation Lab, changing route color and width, clicking Save settings, and closing the menu while the preview remained visible. This is user-observed success for the initial appearance test, not an independently captured screenshot or log verification. Persistence after a full game restart was not part of the selected steps and remains unverified. Live minimap/world-map alignment, navigation, measured performance, and explicit other-mod compatibility checks remain unverified.

Entries below summarize work completed September 7, 2026. This initial retrospective is grouped by work phase; it does not invent exact execution times.

## Installation and old-mod research

Recorded Steam build 25116796 and executable version 1.0.0.2760. Identified ReShade 6.8.0.2155, Crimson Weather 0.7.9, RenoDX, Ultimate ASI Loader, and Crimson Route. Examined existing files and logs without changing them.

Crimson Route's logs provided evidence of unavailable marker detection and empty minimap route samples. These observations do not establish every reported problem's cause or prove a measured FPS regression. Existing mod code/files are not being used as our implementation.

Evidence: [old-mod review](CrimsonRoute-review.md), [installation baseline](installation-baseline.json), [ReShade research](ReShade-navigation-research.md).

## Distribution and integration research

Investigated ReShade's native add-on APIs and existing integration patterns. Selected an independent .addon64 as the preferred architecture. Player/destination/map-state acquisition remains game-specific and unverified.

The user clarified that the finished mod must use only ReShade as its runtime dependency and ship as files with instructions/screenshots, without an installer. Local development precedes Nexus publication and a fresh policy review.

Evidence: [current decisions](Project-requirements-and-decisions.md), [Nexus research](Nexus-tooling-and-boundaries.md).

## Archive inventory and extraction

Reviewed archive-tool source, then used independent bounded workspace scripts to read installed archive indexes. Parsed 2,057,944 entries and identified 26,997 filename candidates. Found 2,504 unique navigraph sector files plus a descriptor, and 65 navigation-voxel entries.

Extracted 26 initial samples plus six targeted navigraph samples into work/datamine. All 2,504 navigraph headers passed framing and filename-coordinate checks. A post-extraction hash check matched all five baseline files: game executable, CrimsonRoute.asi, CrimsonRoute.ini, dinput8.dll, and dxgi.dll. This check covers those files, not a full-installation hash audit.

Evidence: [data-mining report](Game-navigation-data-mining.md); work/datamine/index-baseline.json, sample-manifest.json, navigraph-manifest.json, sector-validation.json.

## Geometry decoding

Decoded the smallest sector first. Initial fixed-length connection parsing failed on larger samples. Investigation identified a variable array of intermediate node IDs inside connection records. Including those points explained the difference between straight-line endpoint distance and stored path length.

The corrected research parser decoded five nearby sectors: 52 groups, 6,180 node records and 8,690 connections, with no section-consumption failures. All node-to-connection references resolved. Maximum polyline-length discrepancy was 0.007802586481336249 coordinate units.

Evidence: [provisional format and findings](Navigation-decoding-progress.md); work/decode_graph_probe.py, work/probe_region.py, work/datamine/region/validation.json.

## Offline cross-sector route

Found 75 exact positions shared across sectors. Under experimental exact-position joins and bidirectional edges, the sample graph contains 5,470 unique connected positions and 53 connected components.

The first attempt using the largest component did not cross a sector boundary. The probe was changed to select a component that actually spans sectors. Dijkstra then produced a 39-point route between sectors (-136, -140) and (-136, -136), with length 273.7301404890961 coordinate units. Validated route adjacency, summed distance, and shortest-distance inequalities.

This is an offline geometric result. It does not validate one-way restrictions, player access, travel time, coordinate calibration against the game map, or a live overlay.

Evidence: [route plot](Navigation-route-probe.svg); work/plot_route_probe.py, work/datamine/region/route-validation.json.

## Build environment and documentation

Located Visual Studio Build Tools 2026 and x64 MSVC 14.51.36231 through vswhere; the compiler was not on the normal shell PATH. No add-on was compiled or deployed in this phase. The game was not running at the process check.

Created the documentation index, requirements/decisions record, this journal, and a validation/milestone checklist. Existing detailed reports remain available as supporting evidence. Future entries should record what changed, how it was verified, and what remains uncertain.

## Workspace relocation and first native build

The active workspace moved to C:\Users\bag\code\crimson-desert-navigation-mod. The destination contained all 1,427 original files with matching aggregate size. Cleanup of remaining old-location duplicates was blocked; the old location is not the active workspace.

Built Crimson Navigation Lab 0.1.0 as an independent x64 ReShade add-on using the pinned v6.8.0 SDK and its ImGui headers. It provides a clearly labeled synthetic route preview and saved color, opacity, width, and visibility. It does not read game state or implement navigation yet.

Observed two expected failures with the initial settings parser stub, then implemented parsing and passed the final five settings cases. Native mock-host verification passed rejected registration and three init/uninit cycles, followed by unloading. Binary dependency inspection found only KERNEL32.dll. This does not establish actual in-game rendering or coexistence.

Prepared manual local-test instructions and third-party license notices. Checked that the game was closed, ReShade was 6.8.0.2155, and the unique add-on/settings paths were unused before preparing deployment. Refer to the deployment verification record for whether placement succeeded. Live testing and screenshots remain pending.

Additive deployment succeeded: placed only CrimsonNavigationLab.addon64 in the game's bin64 folder. Its SHA-256 matched the built artifact, and all 73 pre-existing top-level files retained their hashes. No settings folder was created during deployment. Evidence: outputs/local-test-0.1.0/deployment-verification.json. The game was not launched; the next step is the user's live appearance/compatibility test.

## Explicitly authorized removal of Crimson Route

Follow-up: the user requested permanent deletion of the retained backup because the mod can be downloaded again. Automatic approval review rejected both the bounded recursive folder deletion and the narrower explicit eight-file deletion with “blocked by policy.” Neither deletion executed. The backup therefore remains pending manual deletion; do not describe it as deleted or retain it as a user preference.

At the user's subsequent request, removed CrimsonRoute.asi, CrimsonRoute.ini, and its five identified logs from bin64 by moving them to work/backups/CrimsonRoute-removed-20260907. This was a specific user-authorized exception to the general protection of existing third-party mod files. The game was closed. All seven backup hashes matched, and all 67 other top-level files retained their hashes, including shared loaders, other mods, and our test add-on. Verification: outputs/CrimsonRoute-removal-verification.json. A restore-manifest.json in the backup records original paths and hashes. The old plugin will no longer be loaded from bin64 on the next game launch.

## September 7: road core and preview removal

User confirmed installed 0.3.1 visibility switching works. Removed synthetic preview and calibration drawing from development source, retaining settings. Workspace-only add-on compilation passed /W4 without warnings. No deployment. Eight native geometric routing cases passed, including segment-interior snapping and disconnected-road rejection. The routing header is not yet integrated with live rendering. Whole-network extraction is read-only with explicit workspace output; stored edge lengths differ from geometry in expanded sectors and are not verified travel costs. Both live world projections and native archive loading remain outstanding. The game is currently closed; no further calibration test requested.

Whole-network extraction completed: 2504 sectors, 4868885 unique XYZ nodes, 9993726 segments, 0 decoding failures. A native offline probe between previously recorded player/destination X/Z produced 195 points and approximately 1253 geometric units. This is NOT verified road-following: the navigraph may include general walkable terrain. The very dense network and a snapped start only about two units from the prior off-road player position require comparison with actual road assets/map roads before using it for the requested road-only route. There are also 1701 .road candidate assets to investigate. Stored-length mismatch count: 2970991; maximum error 2.122593733035747.

## Combined route test 0.4.0 installed — September 7 evening

Decoded the separate worldmaproad layer of fielddefault.road: 43,653 nodes and 19,367 connection records. Its record format includes optional 33-byte condition data, absent from most sampled navigraph records. Excluded 124 conditional connections; wagon and train layers are not used. The dense navigraph is not used for the road test. Independent native loading reads the uncompressed archive directly; no extracted data/Python dependency is required at runtime.

Integrated geometric Dijkstra, nearest segment endpoints, a background route worker, minimap live scale/rotation, and full-map icon-based affine projection. Full-map projection requires at least eight agreeing icons and 80% consensus because one observed icon differs from the common transform. Added five passing synthetic transform cases, including outlier handling and hidden-map rejection. Color/width settings retained; old preview visibility does not disable the new route. Synthetic preview and calibration circle removed. A small edge dot indicates an off-screen road start.

0.4.0 built and deployed after user confirmed readiness and process absence was verified. SHA256 CF74DF5A1B9A4C9221E88FA13C6722DB23B3C94D3A732CBD7461F0F75C6EB86F. All 979 other bin64 files unchanged. Full in-game visual verification remains pending. Road direction/traversal semantics and fastest-time optimality remain unverified; this is an experimental combined test, not a finished Nexus release.

Windows computer-use control was found and launched the game earlier in this session. Captures and menu clicking worked; injected keyboard map/overlay actions were not reliably observed, and user input caused guarded pauses. Do not claim reliable keyboard control yet.

## 0.4.0 feedback and 0.4.1 correction

User reports a minimap route that realigns/snaps near turns and no full-screen route. Source confirmed replanning every three metres (with a 500 ms minimum interval), which can replace the entry point and path during travel. New RouteSession retains the full plan for a destination; only setting/changing/clearing the destination changes it. No recalculation button, per user clarification. Five session cases verify movement stability and destination changes. This does not alone establish that every visual alignment issue is fixed.

Read-only capture while the user held the full Map tab open: WorldMapView flags D0, valid root and 86 map icons, but many icons retain positions from a prior location/zoom. Old icon consensus therefore failed. A newer current cluster placed the player at 960,540; most stale icons projected the player far off screen. Discovered contiguous root fields A78/A7C cursor UI, A80/A84 cursor world X/Z, A88 world units per UI pixel, and A58/A5C canvas dimensions. Direct anchored projection agrees with the current cluster and places the player at 960,540. Replaced icon fitting with bounded, finite-checked, reread-stable anchor data and retained the visibility gate. Eight synthetic projection cases passed.

0.4.1 built with no warnings; all existing checks and new cases passed. SHA256 598D9618B5E121E3F96C8EC8B4F948160FC55725423BB7208A356DEDF42DF5D2. Installation and in-game route rendering retest pending.

0.4.1 deployment completed September 7 evening (2026-09-08 02:10 UTC) after user confirmed game closure and process absence was checked. Installed SHA256 598D9618B5E121E3F96C8EC8B4F948160FC55725423BB7208A356DEDF42DF5D2. Replaced only our 0.4.0 add-on; all 979 other bin64 files verified unchanged, no extra files added. Destination-driven route stability and full-map rendering await the user retest.


## 0.4.1 user feedback and 0.4.2 recovery test (September 7 evening)

User confirms full map alignment looks good, but pan/zoom shimmers; minimap still realigns near turns. Navigation disappeared after veering off course and could not be recovered with restart/destination changes. User clarified ReShade remained responsive and no errors were shown. A native add-on crash is not established.

Source findings: one rejected read_map observation permanently returned from the worker; UI transforms were consumed from a roughly 30-ms worker sample; NaN minimap scales passed the comparison guard. Added automatic read retries and bounded rediscovery after two seconds of sustained failure, with 30-second scan backoff. Route solving remains on the worker and destination-driven. Drawing now reads small UI state/projection at presentation time. Added explicit destination-unavailable and render visibility reasons plus rate-limited ReShade state logging. NaN guard regression failed before the fix and passed afterward. Recovery state/backoff checks and all existing build checks passed; host unload passed.

UI research: minimap terrain lives under MinimapScaleLayer with CSS zoom animations (scale 0.8/1.25), while IconContainer is outside that layer. This is a hypothesis for road misalignment, not yet a verified projection correction. No guessed scale multiplier was added. No automatic off-course replanning is implemented; the saved full route can be outside the minimap, now explicitly reported.

0.4.2 deployed with game absent; SHA256 4F91A264EFDFE4F8A1E1E9DDE6AC5A8B11C7C9505B1ADAB7CF2738F12B16C10B. All 979 other bin64 files hash-verified unchanged. Live recovery, pan/zoom smoothness, and minimap alignment remain unverified. See local-test-0.4.2 build/deployment records and START-HERE.md.


## Minimap lock investigation and off-course recovery (in progress)
User makes solid/pulse options conditional on verified minimap locking. Do not add either as a substitute for alignment. PID 32220 read-only discovery found mini root 0x4c51613de00 and world root 0x4c515a89a00. Current ReShade log shows valid route outside minimap and one transient read failure recovering within a second; observed disappearance is consistent with route leaving the viewport, not established crash. Saved baseline in work/reader-research/minimap-lock-baseline.json. Icon and NoSticky matrices agree at rest; MinimapScaleLayer identity at rest; CanvasView scale is 4.266667. No dynamic equivalence established. Prepared bounded read-only work/capture_minimap_motion.py; waiting for explicit user readiness for camera rotation.

Added automatic deviation rerouting to RouteSession: more than 15 horizontal world units from saved path, at least 10 units moved since solve, sustained two seconds, minimum five seconds between solves. Destination changes still solve immediately. Local regression covers stable destination paths, brief deviation, sustained deviation, and following replanned path. Thresholds are experimental, not in-game tuned. Source version 0.4.3 is not deployed; installed version remains 0.4.2. Minimap locking and pulse options remain incomplete.


### Motion capture attempt, September 8
User replied ready. Ran read-only capture against PID 32220/root 0x4c51613de00 for 20 seconds: work/reader-research/minimap-motion-1788841244042055700.json, 1906 samples. Player position and all four layer matrices remained constant. IconContainer and CanvasView normalized rotation agree within 0.00000008 degrees in these stationary samples; CanvasView scale 4.266666, icon scale 1, animated parent scale 1. This does not test dynamic lag or establish locking. Asked whether rotation occurred during capture or a menu was open; awaiting clarification before interpreting as a reader failure. No runtime changes or deployment this attempt.


### Successful motion capture and terrain projection candidate
Recorded 2851 read-only samples during user movement/rotation in work/reader-research/minimap-motion-1788841646215147000.json. Icon rotation and CanvasView rotation agree within 0.0000034 degrees. Icon scale remains 1; ScaleLayer animates to 0.8; CanvasView final scale changes from 4.2666667 to 2.7306667 (two compounded zoom stages). Root +0x53c remains 3.333333, so prior icon projection omits terrain zoom. Cached canvas world origin is root +0x4bc (XYZ). Using CanvasView final matrix, cached origin and a 256-world-unit tile interpretation places the current player within 0.791 UI pixels of the minimap center in all 2851 samples; median residual 0.000122 pixels. Tile interpretation is inferred from numerical agreement, not independent format documentation. Rotation-only timing is not proven to explain every visible lag.

Replaced minimap projection with that canvas-based affine mapping. Added finite/bounds checks and stable rereads of origin/matrix/links; temporary torn samples hide the route for that frame. Synthetic terrain-zoom regression failed on the icon projection and passed after correction. Full build and host checks pass. 0.4.3 artifact updated, not deployed; awaiting game closure. No pulse options yet: user conditions them on verified in-game locking. Full-map projection unchanged. Automatic off-course rerouting included from prior work. Visual road alignment and resolution/UI-scale behavior still need verification.


0.4.3 installed on 2026-09-08 at 04:37 UTC after user confirmed closure and process absence was checked. SHA256 EAFFC6ADA03AC2D0EFCBCF81D14C5851CD6975607814BC136DA18E54ADE312C0. Only our add-on replaced; all 979 other bin64 files verified unchanged, no extra files added. Terrain projection locking and automatic off-course rerouting await in-game retest.


## 0.4.4 � optional minimap pulse
User reports 0.4.3 is "pretty great" and requests the pulse option. This is positive in-game feedback authorizing the previously conditional presentation option; it does not establish exhaustive resolution or traversal validation.

Added Minimap route style: Solid line / Traveling pulse, persisted as minimap_pulse. Existing configs default to solid. Pulse uses existing route color/width and the same terrain transform and circle clipping. A faint baseline plus four brightness bands/glow move forward along cumulative road length (18-world-unit spacing, 6-unit pulse, 10 units/second). Arc distance is accumulated before clipping, so animation does not restart at bends or viewport edges. Full map remains solid. Routing/projection unchanged.

Build cmd /c build.cmd passed pulse, settings and existing checks; host unload passed. Game was already closed when checked. Deployed only own add-on; SHA256 2AA821611C7799A77F23F0929E3EEA324369834BFD131809B982F59F03FBAEF7. All 979 other bin64 files unchanged, no additional files. Exact timestamp in local-test-0.4.4/deployment-verification.json. Pulse appearance awaits user in-game test.


## Residual minimap drift reported after 0.4.4
User requests tighter minimap alignment. Reviewed current terrain projection and presentation callback: route uses the canvas final transform and cached origin, with stable rereads; no smoothing filter is applied. Game is not running at investigation time. Earlier player-centering replay does not establish synchronization with the actually displayed terrain or road geometry. Asked whether drift occurs only in motion or persists at rest, to separate temporal and geometric causes. No speculative offset, smoothing or prediction change made. 0.4.4 remains installed; pulse preserved. Live residual-drift observation is the next required evidence.


## 0.4.5 � player-following route start
User confirms pulse and road appearance are good and prioritizes keeping the route start attached to the player as they move. Implemented per-frame display progress along the saved road path: nearest horizontal segment point, traveled prefix removed, remaining original path retained. Backtracking is supported by re-evaluating nearest position. Added dashed player-to-road bearing on both maps; explicitly a direction guide, not obstacle-aware off-road routing. Destination endpoint remains on road. Removed the old fixed-start edge dot. Pulse phase uses original cumulative arc length, so trimming does not restart animation. Solver and terrain projection unchanged.

Six synthetic progress cases and all previous build/host checks pass. Game was closed; replaced only our addon, all 979 other bin64 files unchanged. Installed SHA256 1C6248F8A73A4D4632D95E037B7931DB17ED14472D020F578827B81139C5E4B3. Visual test pending. Nearest-path choice at overlapping roads remains ambiguous; connector can cross terrain obstacles. Residual drift investigation remains outstanding, deprioritized by current user request.


## 1.0.0 release preparation
User accepts 0.4.5 and requests 1.0, automatic startup, default #B17C01 / 6 px / pulse, and Nexus documentation. Implemented auto_start default true and saved checkbox; start occurs once in first presentation callback, not during loader initialization. Existing appearance settings preserved; Reset appearance adopts all defaults. New color exact RGB 177/124/1. All settings (10 cases) and previous tests pass; host registration/unload checks pass. Initial build path separator issue corrected and full build rerun successfully.

Prepared outputs/release-1.0.0: ZIP with only add-on/readmes/changelog/notices; Nexus description, policy/publishing notes and screenshot checklist. Archive entries and hashes verified. ReShade BSD and Dear ImGui MIT notices copied from local SDK licenses. No game assets or old mod content packaged. Current Nexus rules reviewed: AI-Generated Content for code, AI Media for generated description. No upload performed, publisher permission/Nexus approval not established.

Game closed; installed 1.0.0 SHA256 B558538C8828BA785ECA96951BF5E05D7BCB3AA0F66822B745F0698F5BE126AF. All 979 other bin64 files unchanged, including settings. Automatic-start in-game smoke test and current screenshots remain pending before public upload. Residual drift not claimed fixed. Long documentation shell write was rejected by automatic review; structured workspace apply_patch succeeded with the same authorized documentation scope.

Header artwork: sourced official Pearl Abyss horse/path screenshot. Rejected generative edit because it changed scenery detail. Final original-image-plus-line PNG saved under release-1.0.0/header with provenance and pixel comparison. Promotional world-space line is illustrative, not an actual runtime feature. No publication or ZIP changes.
`nUser selected the earlier AI-edited image by its explicit image path and requested a four-times-wider line. Saved that edited variation as header/Crimson-Navigation-highlight-wide-draft.png. It is an AI-edited draft, separate from the original-pixel-preserving promotional image; not added to the mod ZIP.

User approved the four-times-wider highlight image as release header. Saved canonical header/Crimson-Navigation-header.png, verified copy hash, updated release index/provenance/Nexus publishing notes. This is the AI-edited variant, not the original-pixel-preserving image. No upload or runtime/package change.


## September 9, 2026 - Release screenshot collection

Inspected all seven user-supplied PNG captures and copied them unchanged from the Desktop into release-1.0.0/screenshots. Verified each image decodes and each destination SHA-256 matches its original. Added ordered Nexus captions, dimensions and provenance manifest. Full-map, pulse/solid minimap, off-road guide and 1.0.0 controls are visibly represented. The menu shows preserved custom RGB 255/184/46 and 2 px; do not caption these as fresh-install defaults. Automatic startup is enabled in the menu but actual startup without manual Start remains unverified. Static images do not establish motion stability. Main mod ZIP unchanged; no runtime changes or publication.

September 9 Nexus draft: created unpublished mod 3447 under wilsonistrey, User Interface category. Saved 1.0.0 description, installation, limitations, AI-Generated Content and AI Media tags, plus external ReShade requirement. Header upload blocked by Chrome extension file-URL access permission. No files or gallery images uploaded; permissions/credits completion and final publication remain pending. Draft: https://www.nexusmods.com/games/crimsondesert/mods/3447/edit/general


September 9, 2026: user reports Nexus publication of 1.0.0 (mod 3447). First external bug report: Linux user sees a line down the screen on the full-screen map; screenshot pending, environment unknown, not reproduced. No cause or fix established. Prepared root README, known-issues record, third-party notices and Git exclusions for a source-only GitHub repository. Local Git has no commits or remote; GitHub CLI account is snyderb-de. Awaiting public/private choice before repository creation. Runtime and release archive unchanged.


September 9: implemented 1.0.1 static dashed/dotted minimap styles alongside solid/pulse, anchored to original road distance through clipping and progress. Added minimap_style with explicit precedence over legacy minimap_pulse; all four styles persist and existing selections migrate. Full map remains solid, off-road connector unchanged. Five pattern cases, 17 settings cases and all existing build/regression/host checks passed. Game closed; deployed only own addon; 979 other bin64 files unchanged. In-game validation pending, no publication; released 1.0.0 ZIP untouched. Linux report remains unresolved.

September 9: renamed minimap menu option to Pulse (recommended), per user. Behavior and saved setting IDs unchanged. Rebuilt successfully and deployed local 1.0.1 with game closed; 979 other files unchanged. No Nexus update.


## September 9, 2026 - QoL specification and plans

Created QoL-spec.md and two ordered implementation plans following the user's six annotations. Selected configurable R-based visibility shortcut, independent map color/width, automatic saving/cleaner menu and appearance-only reset. Proposed native ImGui accessibility controls and Ctrl+Shift+R default; conflict-free gameplay is not verified. Recorded final approach and arrival feedback as pinned backlog.

Inspected current 1.0.1 source, tests, build and local SDK. Reviewed document coverage, interfaces and placeholders. No runtime edits, tests, installation writes, commits or publication performed. outputs remains Git-ignored. Existing journal encoding prevents structured patching; appended this entry without rewriting prior bytes.


September 10, 2026: assigned TODO-001 to route disappearance after the archery activity and TODO-002 to full-screen route misalignment at reported 1440p/4K resolutions. Per user clarification, the earlier Linux artifact is consolidated into TODO-002; no separate Linux issue remains. Both are open reports, not locally reproduced or fixed. Canonical tracker: docs/known-issues.md.


## September 10, 2026 - TODO-002 resolution candidate, 1.0.2-test1

User requested automatic mathematical resolution handling before a 1080p/1440p/4K TV test, extended conceptually to 480p and other sizes. Confirmed that the two maps need different projections while sharing road geometry. No DLAA/upscaling configuration extraction is needed for the selected correction: live map canvas dimensions and ReShade overlay dimensions determine the conversion.

Verified source defect: the full-map reader discarded its canvas dimensions and draw_route scaled it with the minimap canvas; it also required a valid minimap circle while the full map was open. Historical captures all use matching 1920x1080 canvases, so higher-resolution behavior is not yet observed. A characterization of the existing consumer math reproduced four failures out of five checks. Replaced it with a tested production presentation boundary using each map's own dimensions, once per frame; full-map canvas, anchor, visibility and identity re-reads reject changing snapshots. Added read-only canvas/overlay/scale menu diagnostics. Road solving, appearance persistence and route styles retain their existing behavior.

Final full build passed, including 3,970 synthetic resolution checks, existing regression suites and native host load/registration/unload checks. The resolution sweep covers all integer heights 480..4320 at rounded 16:9 widths; native alternate-aspect fixtures include 640x480, 16:10 and ultrawide. This validates coordinate math, not in-game road alignment or universal display compatibility. Unknown canvas/display aspect mismatch is suppressed; letterbox offsets and same-aspect stale game values remain unverified. TODO-002 stays open.

Installed 1.0.2-test1 with game closed after verifying the installed 1.0.1 hash and taking a workspace backup. Only our own addon was replaced; all 979 other bin64 files retained hashes, with no added or removed files. Installed SHA256: E367E580AB9CFDCCC69000C77566FD05C5BF80D3D8C158B1285AF4870994F66D. No Nexus publication. See local-test-1.0.2/START-HERE.md, Resolution-math-and-evidence.md and deployment-verification.json.

## September 10, 2026 - TODO-001 additional downloader reports

User relayed Nexus downloader feedback that the route disappears when nearby NPCs talk and when enemies become aggroed on the player. Added both triggers to the existing archery/activity report in docs/known-issues.md and updated the outputs index. No direct Nexus lookup or contact was needed for this relayed feedback.

Read-only source triage found that map_hidden_candidate selects the map rendering path and that unavailable/ambiguous destination observations suppress route output; no_candidate clears the route session. These are verified code paths, not verified causes of the reports. The new reports do not identify the affected map, versions, whether interactive dialogue occurs, or whether normal gameplay restores the route. Recorded separate before/during/after reproduction and recovery checks. Documentation only; no runtime edits, build, deployment, or publication. TODO-001 remains open and has not been reproduced locally.

## September 10, 2026 - TODO-001 recovery-delay investigation

User clarifies that the minimap stays visible while the route disappears and can return later; roughly 20-30 seconds was relayed for one person who waited, not measured for all reports. Source inspection found that a failed rediscovery discards the previous root and enforces a 30-second delay after scan completion. Production-reader/timer characterization demonstrates a recovered visible map still being unreadable via the discarded root zero with 29 seconds until the next scan. Exact speech/combat/activity trigger is not established.

Also characterized whole-icon/unrelated-row target snapshot rejection, unsupported icon flags, visibility recovery, raw UI byte rejection, full-map-only root recovery gap, and same-destination replanning after session clear. Final warning-free probe run: 23 characterization checks, 12 existing target cases, three historical capture replays passed. No runtime fix implemented. Existing September 9 ReShade log shows 31.071/31.139-second scan intervals before first usable map data; not an incident capture. Game closed; installed addon still matches recorded 1.0.2-test1 hash. Source runtime, installation and public package unchanged. Added tests/todo001_characterization_probe.cpp, workspace runner/log/evidence, and outputs/TODO-001-investigation.md; updated tracker/index. No publication or extraction.

September 10, 2026 - TODO-001 lifecycle clarification: user says only removing the marker or touching it at route completion should stop the route. Interpret marker as active navigation destination. Recorded acceptance behavior for preserving navigation through NPC speech, combat, activities and temporary UI/read failures; changing destination replaces the route. Current code has no independent explicit-removal event or verified arrival detector, and no_candidate from UI membership cannot by itself distinguish cancellation from temporary UI absence. Arrival must use the actual destination, not the nearest-road endpoint. Source inspection and existing captured evidence do not yet supply those signals. Added this requirement and implementation prerequisites to TODO-001-investigation.md, tracker and index; no runtime implementation or deployment.

## September 10, 2026 - TODO-001 implemented, 1.0.3-test1 installed

User authorized implementation and confirmed the game removes the active destination on arrival. Added DestinationSession and ReaderCore to preserve navigation intent across unavailable, ambiguous and transient empty observations. No retention timeout or proximity-only completion. Corroborated game clear uses empty supported collections, valid inactive label, stable full-map removal evidence or game distance zero near the actual destination. The five-world-unit proximity/elevation filter corroborates a game clear only; its tolerance and UI semantics are experimental. Missed/false clear risk and in-game checks are documented in local-test-1.0.3/START-HERE.md.

Failed/incomplete/ambiguous scans retain previous addresses for full revalidation. BackgroundDiscovery permits live sampling while scanning and cancels/joins on shutdown. Full-map identity recovery is independent and demand-driven. Optional destination label/distance failures no longer invalidate core map reads. Target snapshots validate semantic membership/identity/owner/flags/coordinates without rejecting irrelevant animation bytes. Diagnostics include retained state, target status and acquisition results. Kept TODO-002 resolution behavior and tests.

Used test-first changes and independent code review. Addressed unreadable world-parent recovery, same-root body/view reconstruction, stale clear evidence across gaps, and quick-close confirmation with hidden identity validation. New tests directly exercise reader output during a blocked background scan, scan exceptions and cancellation. Final full build warning-free: 7 acquisition, 29 lifecycle, 18 reader integration, 9 background, 16 target, 14 map-reader cases; previous suites including 3970 resolution checks and native host checks passed. Three historical target capture replays passed. Logs under work/build/todo001-*. Archived pre-fix characterization source under work/reader-research/todo001; replacement regression tests are current.

Game closed; verified installed own 1.0.2-test1 hash, backed it up under local-test-1.0.3/backup, and replaced only CrimsonNavigationLab.addon64. Installed SHA256 CDDD381A0EAC9985EA5E8F92D07CDEA7C47364DF1EB5E2EE395698D98DF7E9C0. All 979 other bin64 files hash-identical; no added/removed files. No original game files, third-party mods, saves, Steam configuration or settings changed. No publication or installer. In-game speech/combat/archery/removal/arrival tests remain pending; TODO-001 stays open.

## September 10, 2026 - Public description cleanup

User requested removal of confusing references to the research-only mod from the description. Removed both references from the published Nexus mod 3447 description using the authenticated author editor, saved, and verified the resulting public page. Synchronized outputs/release-1.0.0/NEXUS-DESCRIPTION.md with the current public wording and removed the corresponding root README reference. Updated the outputs index. Runtime files, release downloads, compatibility statements and required credits/disclosures were unchanged. Historical research records remain intact.

September 10, 2026 - ReShade format emphasis: per user request, replaced the plain format sentence in the public Nexus Requirements section with a bold statement, "Mod format: ReShade add-on (.addon64), not an ASI mod." Added "Requires ReShade with full add-on support. No ASI loader is needed." Saved and verified the published text, and synchronized the local Nexus description. No runtime or download changes.


## September 12, 2026 - Game build compatibility blocks testing

User reports an unsupported executable warning while preparing resolution tests. Read-only inspection confirms installed CrimsonDesert.exe is now 1.0.0.2850 / Steam build 25246367, SHA256 BCBF623AD5690147DC462AEAED5B4F97BD73296BA0D6AB54663586E7088B1C0E. The current supported_executable check accepts only the earlier 1.0.0.2760 SHA256 4D99C15C58BD20A94D354D10AE395D1FAC777D59EF52CBA8080DC3FC8DC6F454. ReShade logs show 1.0.3-test1 loaded, followed by repeated build-check rejection. This occurs before map discovery or resolution projection, so retrying or changing resolution cannot resolve this hash mismatch.

Compatibility with the new executable is unverified. Validate the build-specific reader identities/layouts before adding support; accepting the new hash alone would not establish compatibility. TODO-001 and TODO-002 gameplay validation remain pending behind this blocker. No source runtime, installation, saves, settings or Steam configuration changed. Evidence: Build-compatibility-2026-09-12.json.


## September 12, 2026 - 1.0.4-test1 new-build reader validated, awaiting game closure

User authorized compatibility work for executable 1.0.0.2850. Added exact-SHA256-selected GameBuild profiles for 2760 and 2850, with separate minimap/full-map/icon-canvas/icon vtable RVAs. The selected profile is carried through initial and background discovery, core identity/recovery/read paths, snapshots and per-frame rendering. Unknown hashes remain rejected. Preserved 1.0.3 destination lifecycle logic and 1.0.2 resolution math. New menu identifies the selected game reader profile.

Independent read-only PE/RTTI inventory establishes the four new identities. Five inspected 1800-byte map-method regions match saved old disassembly after relocation normalization; the sixth is a jump stub, so no whole-program equivalence is claimed. Current road archive parses as 43655 nodes / 46341 segments, and the existing offline route probe succeeds. Research/evidence stays in work/reader-research/build-2850; original installation files were read only.

Live PID 40236: unique validated discovery of both map roots. Ten gameplay samples validate minimap position/rotation/zoom at 2560x1440 and correctly have no active destination. User then confirmed destination set and full map open on the Hisense TV at 2560x1440. Ten more production ReaderCore samples all validate map, destination, full projection and calculated road route. Minimap canvas 2560x1440 versus full-map canvas 1920x1080 directly confirms TODO-002's cross-map scaling defect for this session: the full map requires 4/3 conversion. Actual add-on visual alignment, resolution switching and lifecycle behavior still await testing.

Final full build passes, including 19 new compatibility checks and all 3970 resolution checks plus previous regressions/native host checks. Independent reviewer found no critical/important defects. Addressed its only coverage suggestion by exercising real production discovery against a relocated fixture in the test process, then reading those roots. Build SHA256 E8B512A397D4D1D20A9258FCE22C54D628A0D65BEEFF3812F31C65A300D44865. Verified/backed up own installed 1.0.3-test1; no deployment yet because the game is running. User has been asked to exit before replacement. See local-test-1.0.4/START-HERE.md, Compatibility-evidence.md and live-validation-summary.json.


September 12, 2026 - 1.0.4-test1 deployed after the game exited. Rechecked the known own 1.0.3-test1 artifact and backup, recorded a fresh recursive bin64 hash manifest, and replaced only CrimsonNavigationLab.addon64. Installed SHA256 E8B512A397D4D1D20A9258FCE22C54D628A0D65BEEFF3812F31C65A300D44865. All 979 other files are hash-identical, with no added/removed files. Installation record: local-test-1.0.4/deployment-verification.json. New-build reader and route calculation were validated live before deployment; the installed add-on's post-relaunch display and TODO-001/TODO-002 visual behavior remain pending. No public release or installer.


## September 12, 2026 - User confirms 1440p operation and interaction persistence

Following the 1.0.4-test1 relaunch/2560x1440 test instructions, the user reports that both full map and minimap work and that the route stayed active through vendor, enemy and NPC interactions. This is positive user validation of 1440p map operation and persistence through those tested interactions on the Hisense TV / game 1.0.0.2850. No separate pan/zoom result, precise combat/speech sequence, duration measurement or 4K result was supplied.

The user also confirms residual minimap path lag when turning, followed by catch-up. Added TODO-003 to track this independently of resolution conversion and route disappearance; cause and delay remain unmeasured, and a new regression is not established. TODO-001 remains partially validated pending archery and destination removal/replacement/arrival. TODO-002 remains partially validated pending 3840x2160 and return-to-1080p checks. Next resolution test is 4K with the same active destination.

Recorded source/scope in local-test-1.0.4/User-test-results.md and updated the tracker/index/test guide. No code, runtime, installation, settings or public release changes; documentation-only follow-up to the user test.


## September 12, 2026 - TODO-002 wider 4K full-map diagnosis and candidate

User reports full-map route absent at full 4K while minimap route remains visible. Read-only capture of the user-ready Hisense session establishes actual output 4096x2160, a centered 1920x1080 full-map reference, native uniform UI scale 2 and horizontal offset128. The previous reader rejects the legitimate negative reference cursor x=-27.2 and, independently, rejects the reference/display aspect difference. The destination and calculated road route remain valid. This capture is not a verified 3840x2160 test.

1.0.5-test1 reads native full-map UI-group scale and physical body dimensions, with repeated metadata/identity checks, then applies centered conversion each frame. Native code and the independently measured game cursor agree to less than 0.001 pixel. No resolution presets or DLAA/DLSS configuration parsing. Full-map and minimap retain separate mappings; TODO-003 minimap turn lag is unchanged. New UI-group fields are independently verified on build2850; applicability to retained older build2760 remains an assumption.

Six new regressions reproduce the old defect; 3,997 resolution checks pass after correction. Full regression build, 19 compatibility and native host checks pass; independent code review found no actionable findings. Read-only production probe against the same open session now yields 10/10 valid full-map display projections at4096x2160, versus0/10 raw full projections before. In-game add-on rendering/alignment remains pending deployment/relaunch; do not treat reader validity or synthetic geometry as visual validation.

Candidate and precise evidence: local-test-1.0.5/START-HERE.md and Resolution-evidence.md. Awaiting game closure for replacement of only the verified own add-on with a workspace backup. Original/third-party files, settings, saves and Steam configuration remain protected. No installer or public release. Visual test order:4096x2160,3840x2160 if available,2560x1440,1920x1080, including pan/zoom and minimap checks. TODO-002 stays open.


Additional live resolution switch before deployment: while the same PID31716 remained running, the full-map body changed from4096x2160 to2560x1440. The new reader produced10/10 valid full-map display projections at2560x1440. Native UI scale changed automatically to1.333333373, with reference1920x1080. Predicted cursor(-0.000038,-0.000021) matches native cursor(-0.000081,0) to under0.001pixel. Evidence: work/reader-research/fullmap-4k/fixed-live-display-1440p.json and native-1440p-comparison.json. This is a read-only same-session resolution-change check; installed binary remains1.0.4-test1 and candidate visual validation remains pending.


## September 12, 2026 - same-session 3840x2160 comparison

The user confirmed the next resolution was ready. Read-only inspection establishes a visible full-map body of3840x2160 in the same PID31716, following4096x2160 and2560x1440. The logical reference remains1920x1080; native UI scale is2 with zero centering offset. The predicted cursor(78,2089.5) matches the game cursor(78.0001220703,2089.5), an error under0.001pixel.

The production candidate probe passes10/10 full-map display projections, valid active destinations and calculated routes at3840x2160. Both raw map projections remain valid. Evidence: work/reader-research/fullmap-4k/native-3840x2160-comparison-1789232470066373300.json and fixed-live-display-3840x2160.json. The installed add-on remains1.0.4-test1; the user has not separately reported visible route appearance at this resolution. Candidate1.0.5 visual alignment remains unverified until deployment/relaunch. Next live comparison:1920x1080. No source/runtime or installation writes in this check.


## September 12, 2026 - 1080p comparison completes the four-mode live data checks

The same running game (PID 31716) now exposes a visible 1920x1080 full-map body. The reference canvas remains 1920x1080 and native UI scale is 1. The predicted cursor (145.5,712.5) exactly matches the game's cursor position. All ten production-probe samples have valid full-map display projections, an active destination and a calculated road route.

Evidence: work/reader-research/fullmap-4k/fixed-live-display-1920x1080.json and native-1920x1080-comparison-1789232806615414300.json. The candidate reader has now passed ten samples each at 4096x2160, 2560x1440, 3840x2160 and 1920x1080 in this same session, with native cursor agreement within 0.001 pixel at every size. No graphics configuration or per-resolution preset was required.

These are live data/conversion checks with the candidate reader; the installed add-on is still 1.0.4-test1. The visible candidate route and road alignment remain pending installation of 1.0.5-test1 and relaunch. No source or installation changes were made in this check. The four-mode pre-deployment comparison is complete; the next step requires closing the game to replace only our verified add-on.


## September 12, 2026 - 1.0.5-test1 installed after the game exited

Verified the game had exited, rechecked the exact candidate/previous-own-add-on/backup hashes, and recorded fresh recursive bin64 manifests immediately before and after replacement. Replaced only CrimsonNavigationLab.addon64 with 1.0.5-test1. Installed SHA256: E0C9CCEEE0910B101981E693B3A18409F1FFD77DF79BA4011B876D437A3AC120. All 979 other files are hash-identical; no files were added or removed. The verified 1.0.4-test1 backup remains in outputs/local-test-1.0.5/backup/.

Deployment record: outputs/local-test-1.0.5/deployment-verification.json. All four pre-deployment live data comparisons passed, as did the full regression build and independent review. Visible route alignment with the installed candidate remains pending relaunch. First visual test is the originally failing 4096x2160 mode, with an active navigation destination and full map open, including pan/zoom; then verify minimap operation. No original game files, third-party configuration, settings, saves or Steam configuration changed. No installer or publication.


## September 12, 2026 - positive 1.0.5 full-map visual test

The user reports "looks good" in direct response to the post-deployment 4096x2160 full-map appearance/alignment test. The new process (PID 29540) logs 1.0.5-test1 loading, a 4096x2160 swapchain resize followed by runtime recreation, and subsequent full-map and minimap route submissions. This supports a positive user visual result for the reported 4096x2160 failure, distinct from the earlier four-mode read-only math checks.

Recorded source and scope in local-test-1.0.5/User-test-results.md, with log excerpts under work/reader-research/fullmap-4k/post-deployment-1.0.5-user-pass-log.txt. No separate per-step pan/zoom, duration, or lower-resolution post-deployment visual result is asserted. TODO-002 has local visual confirmation for this reported failure; broader visual coverage and the earlier Linux report remain unverified. TODO-003 minimap turn lag and TODO-001's remaining lifecycle checks remain separate. Documentation only; no source, installation, settings, original/third-party files or publication changes.


## September 12, 2026 - accepted resolution scope; TODO-002 resolved locally

After confirming the general scaling calculation covers resolutions between 1080p and 4K, with unusual layouts still untested, the user says: "those people can report their bugs". Treat this as acceptance of the completed validation scope. Mark TODO-002 resolved locally in installed 1.0.5-test1 for the validated setup; additional display-configuration testing is not a prerequisite to finishing this task. New reproducible display reports can reopen it or become follow-up issues.

Verified evidence remains unchanged: four same-session live data comparisons (1920x1080, 2560x1440, 3840x2160 and 4096x2160), 3,997 synthetic resolution checks, full regression/native host checks, independent review, earlier 1440p user validation and positive 1.0.5 visual feedback for the 4096x2160 failure. This scope decision does not assert universal display compatibility or verified Linux/older-build behavior.

Updated the tracker, index and local test records to distinguish completed work from possible future bug reports. TODO-001's remaining lifecycle checks and TODO-003 minimap turn lag remain open. Documentation only; no further runtime, installation or publication changes.


## September 12, 2026 - 1.0.5 Nexus bundle prepared for manual upload

The user asks to close out this update, revise the public description and prepare the upload bundle; the user will upload it. Accepted public wording: reported vendor/NPC interaction bugs are fixed in the tested setup, some resolutions may still have issues and users can report them, and combat/other interactions may temporarily hide the route but it should return while the active navigation destination remains set. This describes expected recovery rather than a guarantee for every interaction. User-placed map markers remain distinct from the active destination.

Minimap drift tightening remains ongoing. Pulse animation speed, pulse blur and route glow controls are announced as planned, explicitly not included in this release. No implementation of those controls is part of this closeout.

Prepared outputs/release-1.0.5/ with Crimson-Navigation-1.0.5.zip, copy-ready Nexus BBCode and readable Markdown descriptions, short summary/file description, changelog, manual install/update/use/remove instructions, current policy/license review, upload notes and seven unchanged prior gallery captures labeled as version 1.0.0 references. Existing permissions and author settings are to be retained on the current Nexus page. No online page edits or upload were performed.

The ZIP contains exactly four files: CrimsonNavigationLab.addon64, README.txt, CHANGELOG.md and THIRD-PARTY-NOTICES.txt. It ships the exact locally installed and user-tested binary, SHA256 E0C9CCEEE0910B101981E693B3A18409F1FFD77DF79BA4011B876D437A3AC120. Release/file version is 1.0.5; the retained in-addon label 1.0.5-test1 is explained in the upload notes and user instructions. No runtime rebuild or game installation write was performed.

Fresh checks passed: ZIP CRC and allowlist, archived/staged/tested binary identity, x64 PE/exports, complete ReShade and Dear ImGui license notices, no personal settings/game assets in the mod ZIP, balanced BBCode tags, seven screenshot hashes and native host registration/rejection/unload checks. The pre-existing full regression and user visual evidence are linked rather than claimed rerun. ZIP SHA256: 15EBADE6A40EFD6A0A397CFB54003533E6D8C14CB704FC61B5E57CFF7CF96C31. Package verification and manifest are in the release folder.

Fresh primary-source review covered Nexus submission rules, terms, author guidance and Pearl Abyss fan-content guidance. The description retains AI disclosure and upload notes identify the relevant tags. The main ZIP contains no screenshots, game archives, extracted map data, other mods, ReShade installation or installer. The optional unchanged gallery is separate. Ready for the user's manual upload; publication status remains pending.


## September 12, 2026 - marker routing added to upcoming features

The user requests paths to user-placed map markers soon. Added this to the public Coming next list and internal roadmap, alongside pulse speed, pulse blur and route glow controls. Marker routing is explicitly planned, not implemented or included in 1.0.5; the current runtime still routes to the active navigation destination selected with Set Destination.

Updated the Nexus Markdown/BBCode/file description, changelog, packaged README, release index and packaging source. Refreshed Crimson-Navigation-1.0.5.zip and its verification/manifest. CRC, exact file allowlist, staging equality, unchanged tested binary and BBCode checks pass. New ZIP SHA256: A9EF2A06CE2EC6982962FB0D13632E8EC56E086AF2E07A5A21EC93785358CAFC. No runtime rebuild, installation write or Nexus edit/upload. This is the replacement bundle for the user's pending manual upload.


## September 12, 2026 - remove exact executable lock; 1.0.6-test1

The user explicitly requests removal of the exact executable/build lock so a minor game update does not require a new version allowlist entry. This supersedes the historical exact-hash gate requirement. Tested executable versions remain useful evidence, not installation requirements. It does not authorize using stale addresses blindly or changing original game files.

Replaced whole-executable BCrypt SHA256 selection with bounded read-only PE32+ x64 RTTI discovery of the four required map classes. Type descriptors, primary complete-object locators, hierarchy relationships, executable vtable methods and uniqueness are checked. File/section/image bounds, a 128 MiB initialized-data budget and cancellation checks bound detection. Required live object, destination, collection and projection checks remain unchanged. No fixed version/hash allowlist or unknown-build fallback remains in production; known 2760/2850 identities are historical regression fixtures. The detected layout is copied by value into the reader, snapshots and background reacquisition.

Verification: six new acceptance tests first failed on the old hash gate. All 39 new fixture checks now pass, including changed addresses, an unrelated code edit and malformed/missing/duplicate metadata. Detection against the current 2850 executable matches the independently recorded four addresses exactly. The complete build passes all suites, including 3,997 resolution and 15 compatibility checks, 10 reader lifecycle cycles and native host checks. Independent code review found no blockers; a mid-scan cancellation test was an optional coverage suggestion, not an observed defect.

Installed 1.0.6-test1 after verifying the game was closed and the installed file was the prior own add-on. Workspace backup retained. Candidate/deployed SHA256 CF2508191ACD62666C6FBFD1A8407F4E486A42958E3B61E705D60AA814678FBE. All 979 other bin64 files remain hash-identical. No original game files, settings, third-party files, saves or Steam configuration changed. Fresh gameplay confirmation for this new binary is pending; earlier 1.0.4/1.0.5 user results remain separately identified.

Prepared outputs/release-1.0.6/ as the replacement for the pending 1.0.5 upload, with the exact new binary, revised version-independent requirements, tested versions explicitly labeled as history, and retained requested known-issue/planned-feature wording. ZIP contains only the add-on, README, changelog and complete dependency notices. CRC, file allowlist, staged/archive identity, x64 exports, BBCode balance and seven unchanged older gallery hashes pass. Archive SHA256 32EBD0A239FCD731EA373AAC29CAF6A59ABA3D9CFCAE40F732C3F374DB82D5BC. User handles upload; no Nexus page edit or publication occurred. Evidence: local-test-1.0.6/START-HERE.md and release-1.0.6/package-verification.json.


### 1.0.6 same-session startup follow-up

The game has relaunched as PID 22552. Its ReShade log confirms 1.0.6-test1 loaded at 14:27:50, dynamic discovery found one candidate for each map at 14:29:46, and route submissions occurred on the full map at 14:30:15 and minimap at 14:30:21. This is current-binary runtime startup/discovery/render-submission evidence; it is not a user visual alignment report. User visual confirmation remains pending. Saved own-addon log excerpts to local-test-1.0.6/startup-game-log.txt. Updated current records and prepared release text to distinguish this result from the pending visual check. The unchanged binary remains CF2508191ACD62666C6FBFD1A8407F4E486A42958E3B61E705D60AA814678FBE. ZIP refreshed for documentation only, with CRC/allowlist/staging/binary/BBCode checks passing; latest archive SHA256 D23AC37A0C2378FDC2314525F66EE532D5D463C63A80DFF2CAFCAAC412A6ACAB. No further installation writes or publication.


### 1.0.6 user startup/visual pass and final bundle

The user replies "yup all good" to the explicit 1.0.6-test1 normal-startup/active-destination/both-map route check. Record a positive current-binary startup and both-map visual result, corroborated by the saved runtime log. This does not imply a repeated resolution matrix, long interaction session or future-build validation. Evidence: local-test-1.0.6/User-test-results.md.

Removed pending-check wording from the replacement description, README, changelog, upload notes and current indexes. The exact tested binary is unchanged. Final refreshed ZIP SHA256 659DCC04593C30A275D3FDFCA819251026371B67EDB9533D655CE8D470656D46; size 236056 bytes. ZIP CRC, four-file allowlist, staged/archive identity, unchanged binary and balanced BBCode checks pass. outputs/release-1.0.6/ is ready for the user's manual upload and supersedes 1.0.5. The tested executable is history, never a fixed requirement. No further installation write or Nexus publication/page edit.


## September 12, 2026 - initial local Git commit

The user authorizes committing all project changes, initializing a repository if needed. Inspection found an existing repository on main with no commits. The initial commit includes the independent runtime source, regression tests and probes, build command, dependency notices, known-issue tracker, core project records and current 1.0.6 release/user-validation documentation. Refined the broad outputs ignore rule to keep those maintained documents in version control. Generated binaries/ZIPs, screenshots, extracted game data, caches, dependency checkouts and raw research/build outputs remain local and ignored. Referenced local evidence artifacts are not part of a source checkout.

No runtime implementation changed for this repository operation. The previously completed full regression/native host checks and current 1.0.6 user startup/both-map pass remain the applicable validation. This is a local commit only; no remote is configured, and no push or publication is requested.


## September 18, 2026 - game patch 2.03.00 compatibility triage

Verified the public Nexus report and the official 2.03.00 Steam rollout. The local installation has moved from Steam build 25246367 / executable 1.0.0.2850 to build 25381195 / executable 1.0.0.2944, SHA256 `6D348BE9D52F81BD35CF7C55E73A5DBFC96CC8268438387C91F7F62C82381FA7`. The installed 1.0.6-test1 add-on hash is unchanged.

The production RTTI scan successfully resolves all four required map identities in the new executable at `0x5674408`, `0x566FE78`, `0x5674F18`, and `0x56749D8`. The focused executable check reports one failure only because its optional installed-build assertion still expects the historical 2850 addresses. The ordinary no-argument detector suite passes all 39 cases. The full build passes all regression and native host checks, including 3,997 resolution and 15 compatibility checks.

The latest ReShade log predates the executable replacement, so no post-update local startup, discovery, active-navigation-destination, or route-rendering result exists. Code review confirms that automatic class identity discovery is followed by many fixed, live-validated object offsets. A layout change can therefore break runtime behavior without breaking RTTI detection. Record TODO-004 as a credible report requiring reproduction; no root cause or fix is claimed. No installation or publication changes were made. [Review](Update-2.03.00-compatibility-review.md).


### Patch 2.03.00 live reproduction and root cause

The user launched Steam build 25381195, set an active navigation destination, observed no route, and reported that ReShade showed `Reader stopped after an internal error.` The current log confirms 1.0.6-test1 loaded at 15:09:38, began layout detection at 15:10:41, and stopped with the internal-error status at 15:10:42 before any map discovery status. Preserved the three add-on log lines under `work/patch-2.03-evidence/post-update-reader-failure.log`.

The standalone production road loader reproduces `Unexpected road layer` against the updated `0008/1.paz`. Its format version remains 46 and its first section remains `worldmaproad`, but the top-level count at byte offset 20 changed from 3 to 4. The loader rejects any count other than 3 and the reader service hides the exception text.

Created an explicit workspace-only 3,207,193-byte diagnostic copy under `work/patch-2.03-road-header-probe` and changed only offset 20 from 4 to 3. The unchanged production loader then read 43,655 nodes and 46,338 segments and calculated a valid 189-point route of length 2073.71. This confirms the strict header-count gate as the startup root cause. The installed archive, add-on, game files, saves, settings, and third-party files were not changed. No fix has been implemented or deployed.


## September 18, 2026 - 1.0.7-test1 resilient archive fix installed

The user authorizes implementation and update, then asks why the patch broke the mod and how to reduce recurring patch breaks. Added a real-loader regression fixture before production changes. The observed four-section case failed against 1.0.6 while legacy count 3 and malformed cases behaved correctly. The minimal exact-count-4 correction passed. A second red-green cycle established the broader requirement: an additive count 5 failed under exact 3/4 handling, then passed after treating the total as bounded metadata from 3 through 64. Counts 2 and 65 and a wrong first-section name remain rejected.

All downstream validation remains unchanged: format version 46, `worldmaproad` identity, record bounds and limits, finite coordinates, unique identifiers, connection endpoints, condition encoding, and the following `wagon` boundary. The full build passes 6 archive-header checks and all prior suites, including 3,997 resolution, 15 compatibility, 39 layout detection, reader lifecycle, and native host checks. A freshly rebuilt production probe reads the untouched patch 2.03 archive with 43,655 nodes and 46,338 segments and produces a valid 189-point route.

Built 1.0.7-test1, SHA256 `84497AF09507B23DC2D6E66BF1D9451ECC088C54F6D77E27F16BA9E7FC1F7CBD`. After verifying the game was closed and the installed add-on exactly matched 1.0.6-test1, backed it up in the workspace and replaced only `CrimsonNavigationLab.addon64`. All 979 other `bin64` files retained their path, size, and modification time. Post-fix normal startup and routes on both maps remain pending; no release package or publication claim yet.


## September 18, 2026 - 1.0.7-test1 live result and test2 full-map fix

The installed test1 session passes the archive correction in game. ReShade remains responsive without a reader internal error; automatic discovery finds one minimap and one full-map object; the active navigation destination is acquired; a route is calculated; and the user confirms the minimap route works. The user confirms the full-screen map still has no route.

A read-only staged diagnostic isolates the remaining rejection. The full-map class identity, visible flag, 1920x1080 body, and native UI group scale all validate. Patch 2.03 moved the reference-canvas and cursor/world transform pair from root offsets +0xA58/+0xA78 to +0xA68/+0xA88. The old canvas offset now holds player world coordinates, so the existing plausibility guard correctly rejects it rather than drawing from unrelated data.

Added a patch 2.03 shifted-block fixture before changing production; it failed against the fixed-offset reader. Production now searches only root +0xA40 through +0xAC0 for exactly one plausible canvas/transform pair with the established 32-byte relationship, then re-reads that block, the object identity, parent, and visibility. Missing, ambiguous, unstable, non-finite, and out-of-range data still fail closed. The complete build passes all suites, including 3,997 resolution, 15 compatibility, 39 automatic layout, reader lifecycle, and native host checks.

Against the running updated game, 20/20 diagnostic samples validate raw and display full-map projection. A freshly compiled production ReaderCore probe reports 10/10 valid map samples, active destinations, raw full-map projections, minimap projections, and 1920x1080 display projections. Built 1.0.7-test2, SHA256 F619FBF800EA479CDFAF40825B731EE2386C8AD8F81FBEAED19294198ED2EEAC. The game remains running with test1 loaded, so test2 is staged but not installed. Visible test2 routes on both maps remain pending. No publication.


Final test2 rebuild note: after adding explicit bounded-shift and ambiguous-candidate regression cases, the complete build passed again. The final staged binary SHA256 is BC5FB52B86966358C950E7BB8DF2B8DF017E7FF80C2189EADFB7D228C3AE35D8 and supersedes the intermediate F619FBF800EA479CDFAF40825B731EE2386C8AD8F81FBEAED19294198ED2EEAC candidate. The running game still has test1 loaded; no installation change occurred.


### 1.0.7-test2 deployment

After the user confirmed Crimson Desert was closed, verified that no game process remained and that the installed add-on exactly matched test1 SHA256 84497AF09507B23DC2D6E66BF1D9451ECC088C54F6D77E27F16BA9E7FC1F7CBD. Backed that file up in the workspace, then replaced only CrimsonNavigationLab.addon64 with test2. Installed and candidate SHA256 match BC5FB52B86966358C950E7BB8DF2B8DF017E7FF80C2189EADFB7D228C3AE35D8. A normalized before/after inventory confirms all 979 other bin64 files retain the same path, size, and modification time. Deployment verification is recorded in local-test-1.0.7/deployment-verification-test2.json. Relaunched both-map gameplay remains pending; no publication.


### 1.0.7-test2 gameplay acceptance

The user reports the requested four-part result: minimap ok, full-screen map good, full-map pan/zoom good, and ReShade status good. The saved test2 log corroborates add-on startup, unique minimap/full-map discovery, active route calculation, and repeated full-map route submissions with no reader internal-error line. This passes the patch 2.03 acceptance gate on the current 1920x1080 Windows setup. It does not add Linux/Proton or universal-display claims.

The exact installed, staged, and package binary remains SHA256 BC5FB52B86966358C950E7BB8DF2B8DF017E7FF80C2189EADFB7D228C3AE35D8. Release documentation is updated to the final observed behavior. Prepare the four-file 1.0.7 archive and verification records; do not upload or edit Nexus without user authorization.


### 1.0.7 release bundle finalized

Prepared Crimson-Navigation-1.0.7.zip from the exact in-game-tested test2 binary. The archive contains only CrimsonNavigationLab.addon64, README.txt, CHANGELOG.md, and THIRD-PARTY-NOTICES.txt. Final ZIP SHA256 E649C6B9C1614A56B8EF09160E9118E4E4103B24075B9BC66E2BADEC9FC13857; size 236930 bytes. CRC, four-file allowlist, staging equality, x64 PE, AddonInit/AddonUninit exports, no BCrypt import, complete ReShade/ImGui notices, no settings or game assets, balanced BBCode, seven unchanged historical gallery images, exact tested binary, host lifecycle, and runtime-log checks pass. Fresh policy and license review is recorded. No Nexus upload or page edit.


### GitHub README and image preparation

At the user's request, prepared the repository for a visual GitHub presentation before committing. Added tracked stable copies of the approved 1920x1080 promotional header and representative full-map, minimap pulse, and ReShade-menu captures under docs/images. Rewrote the main README around version 1.0.7 with the header, gameplay gallery, current compatibility explanation, installation, development, limitations, and licensing. The header is explicitly labeled as AI-edited promotional artwork based on an official Pearl Abyss screenshot; gameplay images are explicitly labeled as unchanged user-supplied 1.0.0 captures, not patch 2.03 validation images. Source URLs and exact hashes are recorded in docs/images/README.md.


## November 10, 2026 - game patch 2.04 compatibility (1.0.8)

The user reports that routes no longer appear after game patch 2.04 (Steam build 26480954, executable 1.0.0.3576, SHA256 `C94AD6C3EA54970A6D2DA7A2719753210A7728A91698ED3405DD9125470ED98F`). Installed 1.0.7 logged `Reader stopped after an internal error.` The automatic RTTI detector still resolves all four map classes at new RVAs, so the executable identity is not the cause.

Read-only investigation found five separate changes. The road archive format version is 47 instead of 46, with the consumed `worldmaproad` layout unchanged. Minimap root members moved by different amounts (+0x10 to +0x40). The HUD rectangle is now carried by `MinimapDisplay`. The destination row tag and icon kind changed from 21 to 22, and one non-destination icon in that category carries flag value `0x23A`. The full-map canvas/transform block moved from root +0xA68 to +0xAC8, outside the 1.0.7 window.

Implemented 1.0.8: a size-based road read with forward version acceptance on validated structure; minimap layout profiles selected by canvas class identity at exactly one profile offset; per-profile destination kind, HUD name and the exact measured presentation flag; a wider bounded full-map search (+0x900 to +0xD00) read in one bulk read; and exception text in the reader status. A review pass narrowed an over-broad flag mask to the single measured value, replaced per-candidate reads with one bulk read, and added an "unverified road format" status note for versions above 47.

1.0.8-test1 (SHA256 `E8390D22ECFE15CDC6775C6C41F45FF0DEA378D3D09A4130FB7E63B4C3EB6071`) ran without the reader error and found the active navigation destination, but drew no route on either map. The user's screenshot showed `Minimap view signal: hidden candidate`. Live inspection found a sixth change: every generic UI element gained an 8-byte member, moving the visibility flags from +0xB0 to +0xB8 (0xD0 visible, 0x90 hidden). The profile now carries the UI flag offset, and the minimap, full-map and world-identity readers use it.

1.0.8-test2 (SHA256 `CA735B8DF98F7FBCAB79919F86AAA4339D2647F5A6C3F1168474A27EE13D7741`) passes the full regression suite, the untouched archive probe (44,542 nodes, 47,248 segments) and a read-only live probe (layout 2.04, 10/10 minimap display projections). It was installed after the game closed, with the test1 file backed up; all 979 other `bin64` files are unchanged. The user confirms routes on both maps. The log shows 20 full-map and 4 minimap route submissions and no internal error. Also fixed a buffer overflow in the truncated road-archive test fixture. No release package or publication. [Review](Update-2.04-compatibility-review.md).

DLC: the base road archive gained 662 nodes in new areas. `dlc/dlc_01` has its own road index, but its data is unreadable with current research tools and the mod does not read it. No user action is needed.

The user then confirms full-map pan and zoom also work with test2.

### DLC road data decoded

The user chose to try reading the DLC road files. The patch 2.04 index layout is now decoded: trie names with varint back-distances, and file flags whose high byte indexes a new trailing extension table. The parse ends exactly at the file end and resolves every entry in both the base and DLC `0008` indexes. The earlier note that DLC road data looked compressed or encrypted was wrong; it came from the old index layout. All 167 DLC road files are plain. All 67 DLC `.road` files carry a single `tosidewalk` layer and no `worldmaproad` layer. Base `worldmaproad` nodes cover the new east area and three islands; 15 DLC island groups have no world-map road nodes in any file. Reading DLC files would not add player routes, so no runtime change is made. All reads were read-only; outputs are under `work/patch-2.04-evidence/`.

### Native 4K check and 1.0.8-test3 route width scaling

The user tested native 3840x2160 with test2. Routes are positioned correctly on both maps, but the route is very thin on both, because widths were fixed physical pixels. The game UI doubles at 4K (native full-map UI scale 2.0), so the route looked half as thick relative to the maps.

Test3 treats the width setting and on-screen spacing (outline, pulse glow, connector dashes, minimap edge inset) as 1080p pixels. It scales them by `min(width/1920, height/1080)`, clamped to 0.5..4. At 1920x1080 the result is identical to test2. The saved width value is unchanged; the panel labels it "px at 1080p" and shows the current scale. New `route_scale_tests` (10 checks) and the full build pass. Test3 SHA256 `86C35B987D251CAFC805E6A5D121E22168A26258DFE047888302CBEB51B02B5A`. Not installed yet: the game is running.

Test3 installed after the user closed the game: the installed file matched test2, which is backed up under `local-test-1.0.8/backup/`. Installed and candidate SHA256 match; all 979 other `bin64` files are unchanged (`deployment-verification-1.0.8-test3.json`). 4K visual check pending.

### 1.0.8 release bundle prepared

The user relaunched test3 at 3840x2160 (the ReShade log shows a 3840x2160 swap chain and test3 loading; the saved add-on log shows route submissions on both maps and no reader internal error) and then said the build is good to go. Prepared `release-1.0.8` from the exact installed test3 binary. `Crimson-Navigation-1.0.8.zip` contains only the add-on, README, changelog and third-party notices; SHA256 `D5275E9B5BD80733B75C6A84A63E29D3199E7535D044E34293B0A55AD63F4C91`, 240,274 bytes. Entry timestamps are fixed so the archive hash is reproducible. `work/release-1.0.8/build_release.py` assembles and verifies the bundle: exact tested binary, CRC, allowlist, staging identity, x64 PE, required exports, no BCrypt import, complete ReShade/ImGui notices, BBCode balance and seven unchanged gallery images.

A fresh policy review found the Nexus file submission guidelines updated on November 4, 2026 with a new "AI Assisted" category; AI-Generated Content and AI Media remain the correct tags. The credits now name Anthropic Claude Code alongside OpenAI Codex. The README and description now use the exact attribution statement from the Pearl Abyss fan content guidelines. The GitHub README describes 1.0.8 and the patch 2.04 changes. No upload or Nexus page edit.

### Release text audit

An independent audit with adversarial verification (19 agents) confirmed seven issues, fixed before final packaging. The public text credited test2's 1080p and pan/zoom results to the test3 release binary; it now separates evidence by build. The DLC island route behavior is now marked as expected, not checked in game. The width note now says routes draw thinner below 1080p as well as thicker above it. Reset appearance is documented as restoring all settings, including automatic startup. The citation for the 3840x2160 session now points to a saved ReShade swap-chain excerpt. Minor wording: the AI disclosure says "most of the code", and the gallery is identified as 1.0.0 captures.

One code regression was confirmed: below 1080p, the Dotted style with a width under about 1.5 produces a dot radius under ImGui's 0.5 minimum, so dots are not drawn. The tested binary is kept unchanged; the issue is documented in the release notes and tracked as TODO-006. Final ZIP SHA256 `D5275E9B5BD80733B75C6A84A63E29D3199E7535D044E34293B0A55AD63F4C91`, 240,274 bytes.

## November 11, 2026 - 1.0.9-test1 minimum dotted radius (TODO-006)

After the 1.0.8 GitHub release (tag `v1.0.8`), added `route_dot_radius`: half the resolution-scaled route width, never under 0.5 px. ImGui's `AddCircleFilled` returns without drawing below a 0.5 px radius (checked in the bundled ImGui source), so Dotted-style dots previously vanished when width x scale was under 1. The minimap edge test now uses the same radius as the drawn dot. Dots are unchanged whenever the old radius was 0.5 px or more, which includes every width at 1920x1080 and above. Route scale tests grew from 10 to 16 checks; the full suite and native host check pass.

The build output moved from `local-test-1.0.8/build` to `local-test-1.0.9/build`. The first 1.0.9 build briefly overwrote the 1.0.8 build-folder copy; it was restored byte-identical from the release ZIP (SHA256 `86C35B98...2B5A`). Candidate SHA256 `C86EC6A8DADAEF8E7474EEC759C33F918A81C7104C237C8EC084316D66A20B4E`. Not installed and not tested in game.

### 1.0.9 installed and released

The user asked to install, package, commit, push and release 1.0.9 without an in-game run first. Installed 1.0.9-test1 after confirming the game was closed: the installed add-on matched the 1.0.8 release binary, which is backed up under `local-test-1.0.9/backup/`. All 979 other `bin64` files are unchanged (`deployment-verification-1.0.9-test1.json`).

Prepared `release-1.0.9` with a builder derived from the 1.0.8 one. It packages the exact installed binary, keeps the 1.0.8 notices and gallery unchanged, and repeats the ZIP, PE, export, import and notice checks. ZIP SHA256 `CFC47E6903326FDF43D747499FB3654418B70E086E9C15F58AC6A591B5F65492`. The Nexus guidelines (November 4, 2026) and terms (May 12, 2026) were re-checked and are unchanged since the 1.0.8 review. The public text says the 1.0.9 change is not yet checked in game. Nothing was uploaded to Nexus.
