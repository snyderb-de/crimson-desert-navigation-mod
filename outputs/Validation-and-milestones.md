# Validation status and next milestones

## Latest live result

0.3.1 deployment completed with verified hash and all 979 other bin64 files unchanged. This supersedes earlier not-deployed status. Actual calibration disappearance on full-screen map and return in gameplay remain pending.

0.3.1 prepared, not deployed: parent-view suppression regression passes automated tests; all other current checks pass. Visual map-open/close retest remains required. Full-screen root and several named UI elements located, but pan/zoom and route projection remain unverified. Final route scope includes BOTH maps.

FAILED: full-screen map suppression in 0.3.0. User screenshot shows calibration on top of the world map. Current single-node visibility candidate is not a validated rendering gate. Controlled held-open capture requested; world projection work is paused for this bug. Evidence: local-test-0.3.0/fullscreen-map-calibration-leak.png.

Camera-only comparison completed: player position unchanged, icon/canvas matrices rotate together by about 113.59 degrees while scale and center remain stable. Live minimap rotation correlation is supported. World-axis orientation, world-unit scaling, zoom behavior, and actual route projection remain untested. No rotating route build has been deployed.

Rotation/scale baseline collected at current resolution: three candidate matrix/inverse pairs preserve minimap center with small numerical residuals. Camera-only comparison pending explicit user completion. Resolution-change testing deferred at user's request; world-coordinate projection remains unvalidated.

0.3.0 visual minimap placement passed an approximate single-resolution check: user's 1920x1080 gameplay screenshot shows the calibration inside the actual minimap, center near the player marker. Evidence: local-test-0.3.0/minimap-alignment-1920x1080.png. Resolution changes, camera/world rotation, zoom, and actual navigation remain unverified.

0.3.0 installed with the game closed; own binary hash verified and all 979 other bin64 files unchanged. This supersedes earlier not-deployed status. Next: enable calibration, hide old preview, close the ReShade menu, and inspect a full gameplay screenshot. Visual alignment and dynamic resolution behavior remain unverified in-game.

0.3.0 calibration build prepared, not deployed. Eight dynamic projection cases pass and live named-layout reading succeeds. Calibration uses runtime canvas/render dimensions; in-game alignment, resolution changes, rotation/zoom projection, and route drawing remain unverified. Next: close game for our add-on update, then full gameplay screenshot with calibration enabled.

Fresh-launch no-destination regression PASSED in the user's 0.2.1 screenshot following the requested restart: no supported collection candidate while raw UI flag remains 1 and distance is 0. This supersedes earlier pending status for that test. Evidence: local-test-0.2.1/fresh-launch-no-destination.png. Further target types, loading behavior, performance, and minimap route projection remain unverified.

0.2.1 live clear check passed through the shared native reader in PID 35784: no supported target after user-confirmed clear, even though raw UI flag remained 1. Both collections contain only retained entries. Active state was previously user-confirmed in the panel screenshot. Fresh launch with no destination before interaction and additional target types remain untested.

User confirmed the destination in the 0.2.1 screenshot was actually set. Candidate display is consistent with that active state. Requested clear-state panel check; fresh-launch without any destination remains untested.

0.2.1 panel screenshot received: matching collection candidate and XYZ display, cached distance 1059. Actual destination state is awaiting user confirmation; the no-destination regression remains unresolved. Evidence: local-test-0.2.1/first-panel-target-candidate.png.

0.2.1 is now installed with verified hash and all 979 other bin64 files unchanged. This supersedes earlier not-deployed status. Fresh-launch no-destination regression and new ReShade panel testing remain pending.

0.2.1 built, not deployed: collection-based target selection passes 12 cases and three captured-icon fixtures; live cleared-state probe finds no candidate. Removal from both collections was observed after clearing. Other reader/settings/lifecycle checks pass. Fresh-launch no-destination regression remains pending in the new panel; 0.2.0 remains installed and its raw UI classification is unreliable.

Root-owned collection comparison found a newly added icon in both candidate collections after setting a destination. XYZ and 3D distance agree with root distance 1133. Shared key/tag and differing flags remain experimental; removal verification pending. Initial-state validity remains unresolved; 0.2.0 is unchanged.

Fresh-session clear comparison completed: stationary player, distance 1037→0, icon byte 1→0, metadata count 1→2. Post-interaction transitions repeat, but fresh-launch no-destination classification remains failed. Add a regression requiring unknown/unverified target status until independent target data validates it; do not promote a visible UI byte into an active route target.

Destination validity FAILED in the fresh session: user confirmed no destination while 0.2.0 reported active candidate. Setting one changed distance 0→1037 but left the flag unchanged. Expanded new-session clear comparison pending. Panel operation and discovery remain verified; destination-active classification is unreliable and must not drive routing.

0.2.0 in-game panel verified by user screenshot: coordinates and experimental UI signals display, and settings report loaded. Native discovery also succeeded in the new process (PID 5736) with exactly one valid root. Destination-active candidate plus zero cached distance remains unresolved pending confirmation of actual destination state. Movement tracking in this build, target validity, map projection, and route rendering remain unverified.

Diagnostic 0.2.0 deployment completed with the game closed: own add-on hash verified and all 979 other bin64 files unchanged. This supersedes earlier not-deployed status. Next: launch gameplay, open Home > Add-ons > Crimson Navigation Lab, and start diagnostics. In-game GUI results remain pending.

Native diagnostic 0.2.0 is built, not deployed. Address-independent discovery found exactly one valid root in the running game. Ten reader cases, five settings cases, worker rejection/cancellation, and host lifecycle checks passed. Next is the ReShade diagnostic panel test after closing the game for our add-on update. Destination coordinates and minimap route drawing are not integrated. See local-test-0.2.0/START-HERE.md.

Full-screen map open/close reversal passed in the captured session: parent candidate 0→1→0, destination candidate 1 throughout, matching stationary player and distance. This is a separate visibility correlation, not a general render gate. Next: stable object acquisition and native diagnostic integration; menus/loading remain untested.

Full-screen map-open observation captured: destination candidate stays active, while parent UI state changes. Close-map reversal pending. Rendering must independently account for map/HUD visibility; no new add-on build was deployed.

Destination UI reversal passed for one stationary active/clear/reactivated cycle: linked byte 1→0→1, metadata list count 1→2→1, and identical player position. A repeated cleared snapshot stayed consistent. Experimental visibility correlation is established for this cycle; production acquisition, menu/load behavior, and live route rendering remain unverified. Evidence: work/reader-research/destination-ui-reversal-comparison.json.

Expanded named-UI clear comparison completed with stationary player. Destination elements and a linked 1-to-0 byte changed; a metadata list grew from one entry to two. These narrow the visibility investigation, but need reversal and semantic checks before production use. No new playable build was produced.

Named UI-link probe: explicit identifiers resolve the root's destination icon, wrap, and distance text links. Bounded capture completed without reaching its block cap. Expanded clear-state comparison pending; identifier resolution alone does not establish visibility or active state.

Reactivation comparison completed: identical player position, distance 383, replacement coordinate candidate with computed distance 383.939000. The coordinate correlation repeats successfully. Active-state detection remains unresolved; +0x538 is not a validated flag. The following pending references describe earlier stages, now superseded by this completed comparison. Next: inspect captured UI ownership/visibility and establish stable acquisition before integrating a native reader.

Stationary clear check completed: destination coordinates and distance remain cached after clearing. A production reader must validate active state separately; no such flag is verified yet. Reactivation comparison pending. Evidence: work/reader-research/stationary-clear-comparison.json.

Read-only destination research: a stationary destination replacement produced a new world-coordinate candidate matching the root's changed distance (7049 to 1633; computed new distance 1633.128256). This is corroborated session evidence, not a production reader. Multiple retained objects share its internal tag. Active/inactive membership, stable acquisition, projection, and in-game routing remain unverified; a stationary clear check is pending. See Live-map-reader-research.md.

September 7, 2026: the user confirmed the local appearance test worked in-game: preview visible, settings panel accessible, color/width changes working, Save settings operable, and preview remaining visible with the ReShade menu closed. This supersedes earlier pending status for those specific checks. Full-restart persistence, measured performance, explicit other-mod compatibility checks, and actual map navigation remain pending. Evidence is the user's report, not automated runtime instrumentation.

Last updated: September 7, 2026. Game build: 25116796; executable: 1.0.0.2760.

## Evidence-backed status

| Check | Status | Scope |
|---|---|---|
| Archive-index inventory | Passed | 2,057,944 indexed entries; override semantics not established |
| Navigraph sector framing | Passed | All 2,504 headers; contents not all decoded |
| Sample extraction | Passed | 32 saved samples with indexed-length checks and hashes |
| Regional parser | Passed | Five nearby sectors, 52 groups; exact section consumption |
| Connection references | Passed | Zero invalid node incidence references in this region |
| Polyline lengths | Passed | 8,690 edges; maximum discrepancy below 0.007803 coordinate units |
| Offline shortest path | Passed under assumptions | 39-point cross-sector path; bidirectional edges and exact-position joins |
| Movement and direction semantics | Unverified | Unknown binary fields remain |
| World-map alignment | Unverified | No calibration against in-game landmarks yet |
| ReShade-only native implementation | Appearance test passed in-game, user-reported | Static imports: KERNEL32.dll only; live navigation not implemented |
| Player reader | Live development probe supported | Exact-build checks, actor RTTI, and user-confirmed movement; not yet integrated into add-on |
| Destination/map readers | Partly investigated | Live minimap center and canvas link identified; destination world coordinates and projection remain unverified |
| Crimson Weather/RenoDX coexistence | Untested for our mod | Their existing registration is not proof of our compatibility |
| Saved route appearance settings | Controls and Save action passed, user-reported | Five parser/serialization tests; full-restart persistence remains pending |
| Off-road access and travel-time costs | Not implemented | Requires reachability and movement evidence |
| Release package and Nexus submission | Not started | Local functionality first; no installer |

A local appearance-test package now exists; it is not a Nexus release. Mock-host lifecycle checks passed. See [test instructions](local-test-0.1.0/START-HERE.md) and [build notes](Build-and-test-notes.md). Synthetic display success must not be counted as live navigation success.

Crimson Route was subsequently removed at the user's explicit request: seven files moved to a workspace backup with matching hashes, and 67 other top-level installation files verified unchanged. See [removal verification](CrimsonRoute-removal-verification.json). Future compatibility tests should use this updated installation state.

## Reproducing the offline checks

Run from the workspace root in PowerShell. These commands use the local research Python environment, not a dependency intended for players:

```powershell
.\work\.venv\Scripts\python.exe work/decode_graph_probe.py
.\work\.venv\Scripts\python.exe work/probe_region.py
.\work\.venv\Scripts\python.exe work/plot_route_probe.py
```

The first reads already extracted samples. The second reads selected installed archives and writes derived research JSON under work/datamine/region. The third reads that JSON and updates the offline route evidence and outputs/Navigation-route-probe.svg. They do not write into the game installation. Reproduction assumes the same game data and the existing candidate inventory; rebuilding the game invalidates that assumption.

The parsers are exploratory scripts with assertions, not hardened production loaders. Do not run them on arbitrary untrusted files or ship them as the finished mod. Earlier extraction scripts use exclusive file creation and can refuse to run again when their sample paths already exist; do not delete evidence merely to repeat a completed extraction.

## Next milestone: a game-validated sector crossing

1. Decode or constrain unknown connection fields affecting direction and traversal. Record supported and unsupported field combinations.
2. Confirm the meaning of shared boundary positions. Preserve separation when identical positions do not imply a traversable connection.
3. Calibrate coordinates using identifiable in-game locations and verify the candidate crossing in gameplay.

Completion requires a route supported by both decoded topology and observed game behavior. The offline probe alone does not satisfy that milestone.

## Following milestone: ReShade proof

Build our own native add-on with bounded archive reads, isolated configuration, and a clear diagnostic state. Verify player/destination/map-state acquisition and rendering before connecting production routing. A synthetic display test must be labeled as such. Use uniquely named files and check path conflicts before any additive deployment.

Record build commands, compiler/SDK versions, binary hashes, dependency imports, cleanup behavior, and actual coexistence results. Do not claim ReShade-only operation until the built binary's dependencies and runtime behavior have been checked.

## Local release validation

Once a working add-on exists, test marker changes; map pan, zoom and rotation; minimap clipping; departure from the route; loads and teleports; disconnected paths; bridges and elevation; unsupported game versions; custom-color persistence; frame-time impact; and clean removal of only our files.

Prepare manual file placement and operation instructions and actual screenshots from the tested build. Review Nexus rules and all source/dependency permissions again before proposing publication. No files are currently approved or prepared for upload.

## September 7: route development status

User-confirmed 0.3.1 minimap/full-map visibility switching. Eight offline native road routing cases passed. Preview/calibration removal compiles; not deployed. Integrated route calculation and both world projections remain outstanding. Next in-game test will wait for the road route and both map displays together.

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

User approved the four-times-wider highlight image as release header. Saved canonical header/Crimson-Navigation-header.png, verified copy hash, updated release index/provenance/Nexus publishing notes. This is the AI-edited variant, not the original-pixel-preserving image. No upload or runtime/package change.


## September 9, 2026 - Release screenshot collection

Inspected all seven user-supplied PNG captures and copied them unchanged from the Desktop into release-1.0.0/screenshots. Verified each image decodes and each destination SHA-256 matches its original. Added ordered Nexus captions, dimensions and provenance manifest. Full-map, pulse/solid minimap, off-road guide and 1.0.0 controls are visibly represented. The menu shows preserved custom RGB 255/184/46 and 2 px; do not caption these as fresh-install defaults. Automatic startup is enabled in the menu but actual startup without manual Start remains unverified. Static images do not establish motion stability. Main mod ZIP unchanged; no runtime changes or publication.


September 9, 2026: user reports Nexus publication of 1.0.0 (mod 3447). First external bug report: Linux user sees a line down the screen on the full-screen map; screenshot pending, environment unknown, not reproduced. No cause or fix established. Prepared root README, known-issues record, third-party notices and Git exclusions for a source-only GitHub repository. Local Git has no commits or remote; GitHub CLI account is snyderb-de. Awaiting public/private choice before repository creation. Runtime and release archive unchanged.


September 9: implemented 1.0.1 static dashed/dotted minimap styles alongside solid/pulse, anchored to original road distance through clipping and progress. Added minimap_style with explicit precedence over legacy minimap_pulse; all four styles persist and existing selections migrate. Full map remains solid, off-road connector unchanged. Five pattern cases, 17 settings cases and all existing build/regression/host checks passed. Game closed; deployed only own addon; 979 other bin64 files unchanged. In-game validation pending, no publication; released 1.0.0 ZIP untouched. Linux report remains unresolved.

September 9: renamed minimap menu option to Pulse (recommended), per user. Behavior and saved setting IDs unchanged. Rebuilt successfully and deployed local 1.0.1 with game closed; 979 other files unchanged. No Nexus update.


## September 9, 2026 - QoL planning milestone

Completed specification and two ordered implementation plans; reviewed coverage of all six user annotations and local source/SDK interfaces. No runtime tests or implementation in this planning turn.

Next milestones: stage one migration/reset/preset tests, safe autosave, separate map appearance and ReShade menu; stage two shortcut matching/capture/persistence; supported-host gameplay validation before packaging. Shortcut conflicts, clipping, pulse controls, save timing and keyboard context gates remain untested. Existing Linux full-map artifact is not addressed. Final approach and arrival feedback remain pinned backlog, not verified features.


September 10, 2026: assigned TODO-001 to route disappearance after the archery activity and TODO-002 to full-screen route misalignment at reported 1440p/4K resolutions. Per user clarification, the earlier Linux artifact is consolidated into TODO-002; no separate Linux issue remains. Both are open reports, not locally reproduced or fixed. Canonical tracker: docs/known-issues.md.


## September 10, 2026 - TODO-002 offline correction ready for TV test

Verified: existing renderer used minimap dimensions to scale the full-map transform. Synthetic characterization: 5 checks, 4 expected failures before correction. Corrected production boundary: 3,970 resolution checks passed, including every integer height 480..4320, different logical canvases, 4:3/16:10/ultrawide fixtures, repeated display changes, pan/zoom/cursor movement, minimap rotation, invalid inputs and changing full-map reads. Final build and all existing regression/native host checks passed; log: work/build/resolution-build.log. Native host checks do not invoke the GUI or prove in-game compatibility.

Deployment verified: 1.0.2-test1 SHA256 E367E580AB9CFDCCC69000C77566FD05C5BF80D3D8C158B1285AF4870994F66D; known own 1.0.1 backed up to local-test-1.0.2/backup. Game closed at replacement; 979 other bin64 files hash-identical; no added or removed files. Public release untouched.

Unverified: actual 1440p/4K game field behavior, live route alignment at all resolutions, UI scaling, DLAA/upscaling combinations, letterbox offsets and Linux/Proton. Historical saved canvases only show 1920x1080. Keep TODO-002 open. Next milestone is the same-destination TV test at 1920x1080 -> 2560x1440 -> 3840x2160 -> 1920x1080, including pan/zoom and minimap camera rotation. Menu screenshots show overlay size, both canvas sizes and full-map scale. Details: local-test-1.0.2/START-HERE.md and Resolution-math-and-evidence.md.

## September 10, 2026 - TODO-001 expanded reproduction coverage

Reported, unverified locally: route disappearance after archery, during nearby NPC speech, and when enemies become aggroed on the player. The latter two are downloader reports relayed by the user; affected maps, reporting environments, duration, and recovery are unknown. Source inspection identifies visibility-based renderer selection and destination-status suppression as investigation leads, not a confirmed cause.

Pending validation: reproduce each trigger separately with an active navigation destination; compare minimap and full-screen map visibility, destination status, reader status, and Display status before/during/after. Record whether the game retains the destination and whether the route resumes automatically. Preserve diagnostics before testing map reopening and setting a new active destination as separate recovery checks. These checks have not been run. TODO-001 remains open; documentation changes do not verify a fix. No runtime build or tests were run for this report update.

## September 10, 2026 - TODO-001 offline characterization

Final cmd /c work\test-todo001.cmd completed with no warnings: 23 characterization checks, 12 existing target-reader cases, and three historical set/clear capture replays passed. Evidence: work/build/todo001-characterization.log. The probe exercises production readers, RouteSession and ReadRecovery; failed discovery is a synthetic outcome, not a running-worker or game reproduction. It verifies the 30-second scan-backoff mechanism and shows that losing the previous address can delay recovery by 29 seconds after that address becomes readable again.

Existing local log intervals are 31.071/31.139 seconds during acquisition before the first usable map sample, with no incident annotations. Filtered log/provenance: work/reader-research/todo001. User's visible-minimap and approximate 20-30-second return report strengthens this lead without confirming it. Game was closed; no production edits/deployment/full add-on build. Installed hash still matches 1.0.2-test1. Next milestone: status/timing evidence during a real speech/combat/activity trigger to identify initial map-read rejection versus destination or projection failure. TODO-001 remains open; no verified in-game fix. Details: TODO-001-investigation.md.

September 10, 2026 - TODO-001 additional acceptance checks from user: preserve route through each interruption; remove active destination and verify immediate termination/no resurrection; reach the actual destination and verify completion; select another destination and verify replacement. Include an off-road destination to ensure reaching the nearest-road endpoint does not complete the route, and a fresh session with no destination to prevent false cached routing. Reliable removal/arrival inputs and arrival tolerance remain unverified. These acceptance checks are planned, not run; existing 23 characterization checks do not validate the new lifecycle requirement.

## September 10, 2026 - 1.0.3-test1 offline and deployment validation

Final build completed without warnings/errors. Passed 7 map-acquisition cases, 29 lifecycle cases, 18 ReaderCore integration cases, 9 BackgroundDiscovery cases, 16 target-reader cases and 14 map-reader cases; all existing pattern/progress/pulse/recovery/routing/projection/geometry/settings/host suites including 3970 resolution checks. Independent review findings fixed with regressions: unreadable world parent, same-root body/parent replacement, long sampling gaps, quick-close hidden identity replacement. Background tests verify actual ReaderCore sampling while a scan is blocked, one pending scan/result, exception handling and cancellation/join. Three historical set/clear target fixtures replayed successfully. Logs: work/build/todo001-full-build.log, todo001-fix-components.log, todo001-capture-replay.log. Pre-fix characterization evidence remains archived separately.

Deployment verified with game closed: installed SHA256 CDDD381A0EAC9985EA5E8F92D07CDEA7C47364DF1EB5E2EE395698D98DF7E9C0. Previous own 1.0.2-test1 addon backed up. All 979 other bin64 files unchanged; zero added/removed files. Manifests and deployment-verification.json under outputs/local-test-1.0.3. No publication, installer, original-file changes, or new extraction.

Still pending: in-game persistence during NPC speech, aggro and archery; explicit removal with prompt map closure; selecting same/new destination; game removal on actual arrival, including off-road destinations; fresh launch with only custom markers. UI clear detection and corroboration tolerances may require adjustment. No proximity-only completion occurs. The reported 20-30-second symptom is not reproduced locally and no in-game fix is claimed. TODO-001 and TODO-002 remain open pending their respective live tests.


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


## September 18, 2026 - patch 2.03.00 triage, runtime validation pending

Confirmed the official Steam release, the public Nexus report, and the local executable update to Steam build 25381195 / 1.0.0.2944. Read-only detection finds all four required map RTTI identities at new RVAs. A fresh full build passes all offline regressions and native host checks. The optional installed-executable detector assertion still names and expects build 2850, so its one failure against 2944 is a test-fixture mismatch after successful detection.

No post-update game session is available: the latest ReShade log predates the executable replacement. Consequently, add-on startup, unique live map discovery, active-navigation-destination reading, projection validity, route submission, and visible routes on 2.03.00 remain unverified. Milestone status is investigation open; no fix candidate, deployment, release package, or public response exists. [Detailed review](Update-2.03.00-compatibility-review.md).


### Patch 2.03.00 failure and root cause confirmed

Live reproduction on Steam build 25381195 confirms no route with an active navigation destination and the ReShade status `Reader stopped after an internal error.` The log proves the add-on loads and stops before map discovery. The standalone loader returns `Unexpected road layer` because the updated archive's top-level count is 4 and version 1.0.6 requires exactly 3.

A workspace-only copy with that single header value restored to 3 passes the unchanged loader: 43,655 nodes, 46,338 segments, and a valid 189-point route. Root-cause milestone reached. A corrected binary, installation, and post-fix minimap/full-map validation remain pending; current 1.0.6 is confirmed incompatible with patch 2.03.00.


## September 18, 2026 - 1.0.7-test1 built and deployed, gameplay pending

TDD evidence: observed count 4 failed before the fix and passed after; additive count 5 then failed under exact handling and passed after bounded 3..64 metadata handling. Legacy count 3 passes; counts 2 and 65 and a wrong first-section identity are rejected. Full regression/native host build passes, and the freshly compiled production loader parses the untouched patch 2.03 archive and calculates a valid probe route.

Installed candidate hash `84497AF09507B23DC2D6E66BF1D9451ECC088C54F6D77E27F16BA9E7FC1F7CBD` after confirming the game was closed and the previous file was verified 1.0.6-test1. Workspace backup retained; all 979 other `bin64` files unchanged during deployment. Required remaining milestone: normal startup without reader error and visible route on the minimap and full-screen map with an active navigation destination. No Nexus upload or page edit.


## September 18, 2026 - test1 partial gameplay pass; test2 live projection pass

Installed 1.0.7-test1: normal startup without a reader internal error passes; automatic discovery and active-navigation-destination acquisition pass; route calculation passes; the user confirms the minimap route works. The user confirms the full-screen map route fails.

Root cause: patch 2.03 shifted the full-map reference-canvas/cursor-world pair by 16 bytes while retaining valid object identity, visibility and layout. The new shifted-block regression fails before the correction and passes after bounded structural discovery is added. Full test2 regression/native host build passes. Untouched archive parsing remains 43,655 nodes and 46,338 segments.

Current-game read-only evidence: 20/20 raw/display projection diagnostics pass, and the production ReaderCore probe reports 10/10 valid map, destination, full projection, minimap projection and 1920x1080 display projection samples. Test2 candidate SHA256 F619FBF800EA479CDFAF40825B731EE2386C8AD8F81FBEAED19294198ED2EEAC. Remaining milestone: close the game, replace only the verified test1 add-on, relaunch, and obtain a user-visible minimap/full-screen route result including full-map pan and zoom. No release or publication claim yet.


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


## November 10, 2026 - patch 2.04: 1.0.8-test2 passes in game

Failure confirmed on Steam build 26480954 / executable 1.0.0.3576: 1.0.7 stops with `Reader stopped after an internal error.` RTTI detection passes; the road version, minimap members, destination kind, UI visibility flag offset and full-map block position changed.

Passed:

- Full regression suite with 1.0.8 code: 11 road archive, 27 minimap layout, 3,997 resolution, 15 compatibility, 39 automatic layout checks and all lifecycle, reader and projection suites.
- Untouched 2.04 archive: 44,542 nodes, 47,248 segments, valid 192-point probe route.
- Read-only live probe: layout 2.04 selected, active navigation destination found, route valid, 10/10 minimap display projections at 1920x1080. Full-map block unique over root +0x100..+0x1800; UI scale chain valid.
- Deployment: test2 SHA256 `CA735B8DF98F7FBCAB79919F86AAA4339D2647F5A6C3F1168474A27EE13D7741` installed after verifying test1; backup kept; 979 other `bin64` files unchanged.
- In game: the user confirms routes on the minimap and the full-screen map, including full-map pan and zoom. The log shows one candidate for each map, 20 full-map and 4 minimap submissions, and no internal error.

Failed then fixed: 1.0.8-test1 drew no route because UI visibility flags moved to +0xB8.

Not verified: other resolutions and Linux/Proton on 2.04, routing inside DLC-only areas, and road format versions above 47.

Next milestone: a 1.0.8 release bundle only if the user asks. No Nexus upload or page edit.

DLC data check (read-only): patch 2.04 index layout decoded and verified against both `0008` indexes. All DLC road files are plain and carry only `tosidewalk` layers. 15 DLC island groups have no world-map road nodes. Route behavior for destinations on those islands is not verified in game.

Native 4K (3840x2160) with test2, user-reported: route position correct on both maps; route too thin. Test3 scales route width with display size (1.0 at 1080p, 2.0 at 4K). Built and unit-tested; installation and the 4K visual check are pending.

### 1.0.8 release bundle

Test3 at 3840x2160: the user approved the result; the log corroborates test3 at 3840 width with route submissions on both maps and no reader internal error. Release bundle verification passes (exact tested binary, CRC, allowlist, staging identity, x64 exports, no BCrypt import, notices, BBCode, unchanged gallery). ZIP SHA256 `D5275E9B5BD80733B75C6A84A63E29D3199E7535D044E34293B0A55AD63F4C91`. Milestone: ready for the user's manual upload. Not verified: other resolutions and aspect ratios, Linux/Proton, DLC island destinations.
