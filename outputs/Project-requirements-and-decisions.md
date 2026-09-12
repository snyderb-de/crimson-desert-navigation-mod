# Requirements and decisions

## Both maps and proactive work — September 7, 2026

User explicitly wants the destination route on both the minimap and the full-screen map. Minimap projection must follow player/camera/zoom; full-screen projection must follow that map's pan/zoom and clipping. The minimap calibration must not leak into an unrelated screen position when the full map opens. Full-screen route drawing remains required, not permanently suppressed.

Continue authorized independent work without waiting for the user to prompt each step. Ask only for needed in-game actions, evidence, or game closure for deployment; allow at least 60 seconds for actions and wait for explicit completion.

## Dynamic resolution requirement — September 7, 2026

User emphasized that screen resolution can change. Calibration must use the current game UI canvas, minimap rectangle, and render dimensions; observed 1920x1080 data is not a fixed runtime reference. Build 0.3.0 recalculates placement dynamically and hides unavailable, stale, or aspect-incompatible mappings. Uniform canvas-to-render scaling is an experimental mapping assumption; actual resize/ultrawide/DPI behavior still needs gameplay validation. Calibration is a screen-alignment shape, not a world-space route.

## Target selection and user test timing — September 7, 2026

Allow at least 60 seconds for requested game actions and wait for the user's explicit completion before dependent captures. Build 0.2.1 uses agreement between root-owned collections for experimental target selection. UI visibility and cached distance are diagnostics only. Unavailable or ambiguous data produces no selected target. The collection format and flags are observed behavior, not fully decoded semantics; fresh launch and other destination types still require validation.

## Diagnostic implementation decision — September 7, 2026

Build 0.2.0 adds opt-in read-only diagnostics to our ReShade add-on. An executable hash gate and bounded background discovery replace session-address assumptions. Signals remain explicitly experimental; unavailable or replaced state clears the diagnostic display. The user manually restarts discovery after loading for this test version. Windows system DLLs support memory reads and hashing; no additional mod runtime is required. Destination coordinates and route rendering are not integrated. Built package is local and not yet deployed; closing the game is required to update our loaded add-on.

Last updated: September 7, 2026. Status labels distinguish user requirements from proposed implementation choices.

## Confirmed user requirements

- Develop a new independent Crimson Desert navigation mod. Crimson Route is research only: do not reuse its files or require it.
- Support saved custom route color. Aim for reliable route display, accurate placement, good route choices, and smooth operation.
- Find practical routes between destinations and account for off-road starts and ends. Fastest travel requires a verified traversal and cost model, not merely shortest geometric distance.
- Use ReShade as the only runtime dependency for the finished mod. Local research/build tools may be used during development.
- Do not overwrite, patch, rename, or delete original game files, saves, Steam configuration, or existing third-party mod files/configuration. Uniquely named additive mod files/folders are allowed after compatibility checks and path-conflict checks.
- Keep extraction outputs, development files, and caches in this workspace during research.
- Get the mod working locally before Nexus publication. Provide mod files, placement instructions, operation instructions, and screenshots. Do not build an installer.
- Review current Nexus terms, guidelines, and third-party permissions before release. Do not publish, contact authors, or distribute game assets/other authors' content without authorization and appropriate permissions.
- Maintain documentation of the work.
- Distinguish the active navigation destination from user-placed map markers. Do not call both “waypoints” or assume that an arbitrary pin is the selected route target.

## Proposed architecture

| Decision | Status and reasoning |
|---|---|
| Own native ReShade .addon64 | Appearance-test binary built and additively deployed. Mock-host lifecycle tests pass; actual game compatibility is untested. A shader/preset alone has not been established as sufficient. |
| Read installed navigation data locally | Preferred if achievable entirely within the add-on. No separate extraction application should be required by players. |
| Separate data loading, routing, and rendering | Proposed to keep archive reads and route searches out of rendering callbacks. |
| Own configuration and optional cache | Appearance settings implemented in our own CrimsonNavigationLab/settings.txt, created on explicit Save. In-game persistence untested. Cache design remains unresolved. |
| Dijkstra reference, potential A* optimization | Offline Dijkstra exists. Production algorithm and time-cost model remain unimplemented. |
| Initial navigation display scope | Automatic movement is not part of the proposed first version. This is a scope recommendation, not a finding about technical impossibility. |

## Research choices and limitations

**Use the installed navigraph data first.** Its existence and extractability are verified. Website destination markers may later provide supplementary names/categories if reuse is permitted; they do not by themselves define road connections.

**Exact-position boundary joins are experimental.** The offline prototype merges matching three-dimensional coordinates and treats connections as bidirectional. Neither rule is yet verified against the game's traversal behavior. Do not carry these assumptions into a release silently.

**Do not promise universal fastest routes yet.** Decoded lengths support geometry checks. Movement modes, gates, elevation, off-road access, and time costs remain research work.

**Do not package extracted game data by default.** The proposed release contains our own implementation and appropriately licensed dependencies. Whether and how local derived caches can be used needs technical and applicable-terms review.

## Superseded ideas

- Installer-based distribution: replaced by files and manual placement/operation instructions at the user's request.
- Blanket prohibition on all additions to the installation: clarified to allow additive deployment while protecting existing files.
- Website-only map reconstruction: demoted to a supplementary/fallback approach after installed navigation data was found.

Changes to these requirements should be recorded here and in the development journal. User instructions take precedence over earlier reports.

## Specific removal authorization — September 7, 2026

The user explicitly requested removal of the old Crimson Route plugin files. Its ASI, configuration, and five identified logs were moved out of the installation into a local research backup. This completed, scoped exception does not authorize removing or replacing other mods or shared loaders. See CrimsonRoute-removal-verification.json and the development journal.

## September 7: road endpoints and combined test

Route follows roads and ends at the closest point along a road segment to an off-road active destination. Remove the synthetic preview window and yellow calibration circle; preserve appearance settings. The user wants the next in-game test only when the road route and both minimap and full-screen displays are ready together. Current geometric start snapping does not establish a traversable off-road approach.

Destination route lifecycle: calculate automatically when the active destination is set or changed. Do not require a recalculation button. Retain the route during ordinary player movement.


September 7: temporary read failures must recover automatically; absence of a line must expose a diagnostic reason. Destination-driven route caching remains in place. Off-course automatic rerouting is not yet implemented. A route outside the minimap must not be described as a crash without evidence.


## Minimap lock investigation and off-course recovery (in progress)
User makes solid/pulse options conditional on verified minimap locking. Do not add either as a substitute for alignment. PID 32220 read-only discovery found mini root 0x4c51613de00 and world root 0x4c515a89a00. Current ReShade log shows valid route outside minimap and one transient read failure recovering within a second; observed disappearance is consistent with route leaving the viewport, not established crash. Saved baseline in work/reader-research/minimap-lock-baseline.json. Icon and NoSticky matrices agree at rest; MinimapScaleLayer identity at rest; CanvasView scale is 4.266667. No dynamic equivalence established. Prepared bounded read-only work/capture_minimap_motion.py; waiting for explicit user readiness for camera rotation.

Added automatic deviation rerouting to RouteSession: more than 15 horizontal world units from saved path, at least 10 units moved since solve, sustained two seconds, minimum five seconds between solves. Destination changes still solve immediately. Local regression covers stable destination paths, brief deviation, sustained deviation, and following replanned path. Thresholds are experimental, not in-game tuned. Source version 0.4.3 is not deployed; installed version remains 0.4.2. Minimap locking and pulse options remain incomplete.

User accepted 0.4.3 behavior and authorized pulse option. 0.4.4 offers saved solid/pulse minimap styles sharing terrain alignment, color and width; full map remains solid.

User prioritizes route start tracking current player position on both maps. 0.4.5 trims traveled road prefix and adds dashed player-to-road direction connector. This updates the earlier no-off-road-connector behavior for the starting leg only; destination continues to terminate on the road. Connector is not an obstacle-validated path.

1.0 requested: auto-start on, #B17C01 RGB 177/124/1, 6 px, pulse defaults. Existing settings preserved on upgrade. Prepare files and Nexus documents locally; no publishing authorized. Current screenshots and new startup verification pending.


September 9: user supplied seven gameplay/settings screenshots for Nexus materials. Preserve these captures unchanged and keep gallery media outside the mod archive. Label saved custom settings accurately; retain the approved AI-edited header as promotional artwork, distinct from actual gameplay captures.


September 9, 2026: user reports Nexus publication of 1.0.0 (mod 3447). First external bug report: Linux user sees a line down the screen on the full-screen map; screenshot pending, environment unknown, not reproduced. No cause or fix established. Prepared root README, known-issues record, third-party notices and Git exclusions for a source-only GitHub repository. Local Git has no commits or remote; GitHub CLI account is snyderb-de. Awaiting public/private choice before repository creation. Runtime and release archive unchanged.


September 9: implemented 1.0.1 static dashed/dotted minimap styles alongside solid/pulse, anchored to original road distance through clipping and progress. Added minimap_style with explicit precedence over legacy minimap_pulse; all four styles persist and existing selections migrate. Full map remains solid, off-road connector unchanged. Five pattern cases, 17 settings cases and all existing build/regression/host checks passed. Game closed; deployed only own addon; 979 other bin64 files unchanged. In-game validation pending, no publication; released 1.0.0 ZIP untouched. Linux report remains unresolved.

September 9: renamed minimap menu option to Pulse (recommended), per user. Behavior and saved setting IDs unchanged. Rebuilt successfully and deployed local 1.0.1 with game closed; 979 other files unchanged. No Nexus update.


## September 9, 2026 - Selected QoL scope

See [QoL specification](QoL-spec.md). User requests specification and implementation plans, not execution this turn. Selected configurable R-based shortcut, separate minimap/full-map width and color, automatic saving/cleaner settings and fixing Reset appearance. Proposed Ctrl+Shift+R and expandable native ImGui accessibility controls with contrast/reduced-motion presets and outline/pulse sliders. Proposed defaults require visual and input validation.

Final approach to the actual active navigation destination and gentle arrival feedback are pinned backlog. No additional HUD, arrival threshold, per-map visibility controls, solver or projection change is included. Preserve legacy appearance on migration and preserve behavior during appearance reset. Existing ReShade-only and protected-file constraints remain in force.


September 10, 2026: assigned TODO-001 to route disappearance after the archery activity and TODO-002 to full-screen route misalignment at reported 1440p/4K resolutions. Per user clarification, the earlier Linux artifact is consolidated into TODO-002; no separate Linux issue remains. Both are open reports, not locally reproduced or fixed. Canonical tracker: docs/known-issues.md.


## September 10, 2026 - Automatic resolution conversion

User authorizes implementing the mathematical correction for TODO-002 before testing on a 4K TV, and expects generic handling from low resolutions upward. Keep independent minimap terrain rotation/zoom/origin and full-map pan/zoom/anchor projections. Shared world road geometry does not imply a shared UI canvas. Convert each projection using its own live canvas and the actual ReShade overlay dimensions each frame, without resolution presets or graphics-configuration extraction. Reading DLAA/upscaling settings is not needed for this chosen correction; actual configuration combinations remain part of visual validation.

1.0.2-test1 is installed locally; ReShade remains the only runtime dependency, existing settings are preserved, no installer or publication. The cross-map scaling defect and synthetic correction are verified. Whether this explains the reported 1440p/4K artifact is unverified until the TV test. Do not claim arbitrary aspect/letterbox compatibility: the current conversion requires compatible live canvas/overlay aspects. Test same active navigation destination at 1080p, 1440p, 4K and back, then optional low-resolution/UI-scale/upscaling variants. User-placed map markers remain distinct.

## September 10, 2026 - TODO-001 report scope

Track nearby NPC speech and enemies becoming aggroed on the player under TODO-001 alongside the original post-archery disappearance, based on Nexus downloader feedback relayed by the user. This groups a reported symptom without asserting a common root cause. Do not equate nearby NPC speech with interactive dialogue, assume both maps are affected, or assume the active navigation destination was cleared. User-placed map markers remain distinct. Runtime changes require further investigation; no fix or in-game recovery is established by this documentation update. Canonical details and next checks: docs/known-issues.md.

## September 10, 2026 - TODO-001 diagnostic priority

User reports the minimap remains visible during disappearance, and the route can return after an approximate 20-30 seconds for one person who waited. Prioritize map acquisition/backoff over simple HUD hiding. The failed-scan/discarded-root/30-second delay mechanism is verified in source and offline production-component probes; the triggering game-state transition remains unknown. Whole-icon target validation is a separate lead and does not itself invoke recovery scans. Do not generalize the approximate wait or assume automatic recovery after archery. Proposed correction would retain an address only for fully validated retry reads and preserve conservative destination identity checks; it is not implemented. See TODO-001-investigation.md.

September 10, 2026 - Updated TODO-001 acceptance rule: automatic route termination is limited to confirmed removal of the active navigation destination or reaching that actual destination. Speech, combat, activities, reader failure, and transient missing UI icons must preserve navigation intent. A replacement destination replaces the route. Existing explicit mod controls remain available. User-placed markers and the road-snapped endpoint do not establish cancellation/arrival. Basic arrival completion is now in TODO-001 scope; optional arrival notifications and fades remain QoL backlog. This supersedes earlier exclusion of all basic arrival work from the active bug's requirements, without executing the separate QoL plans. No arrival radius or unverified cancellation heuristic is adopted.

## September 10, 2026 - Implemented destination lifecycle candidate

User says the game removes the active destination on arrival. Follow corroborated game-clear observations; do not add a distance-only arrival trigger or stop at the road-snapped endpoint. 1.0.3-test1 retains a destination indefinitely across unknown reads and allows deliberate new destinations to replace it. Clear confirmation uses two consistent UI signals plus an observed same-view full-map clear or zero game distance near the actual destination. Five world units and 200 ms confirmation / 250 ms maximum sample gap are experimental validation parameters, not proven game semantics. UI replacement or lost identity invalidates clear evidence. No game state is written to complete/remove a destination. Optional arrival messages/fades remain backlog.

Known acquisition flaw corrected with retained validated-address candidates and cancellable asynchronous scans. Original/third-party files remain protected; only verified own addon replaced with workspace backup. Installed 1.0.3-test1 includes TODO-002 resolution correction and preserves existing appearance settings. Local candidate only; no publication and no claim that reported in-game interruptions are fixed until tested. Full behavior and limitations: local-test-1.0.3/START-HERE.md.


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
