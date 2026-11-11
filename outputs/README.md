# Crimson Desert navigation mod — project documentation

Last updated: November 10, 2026.

Source-control scope: core project records, current release text and 1.0.6 user-validation notes are versioned. Generated packages, binaries, screenshots, research/dependency caches and raw evidence artifacts remain local; links to those artifacts require the development workspace.

Current release: [1.0.9](release-1.0.9/RELEASE-INDEX.md) (GitHub tag `v1.0.9`), build [1.0.9-test1 minimum dotted radius](local-test-1.0.9/START-HERE.md). It fixes TODO-006 in code. Installed; not yet run in game. Previous release: [1.0.8](release-1.0.8/RELEASE-INDEX.md) (tag `v1.0.8`).

Current compatibility result: [game patch 2.04 review](Update-2.04-compatibility-review.md). Version 1.0.7 stops with a reader internal error on Steam build 26480954 / executable 1.0.0.3576. The road format version, minimap members, destination kind, UI visibility flag offset and full-map block position changed. [Version 1.0.8-test2](local-test-1.0.8/START-HERE.md) adds forward road-version acceptance, minimap layout profiles selected by class identity, and a wider bounded full-map search. The full regression suite, untouched archive probe, read-only live probe and deployment checks pass. The user confirms routes on both maps, including full-map pan/zoom. Other resolutions and DLC-only areas are not verified on 2.04. Test3 adds resolution-scaled route width and was approved by the user at 3840x2160. [Release 1.0.8](release-1.0.8/RELEASE-INDEX.md) is prepared for manual upload; ZIP SHA256 `D5275E9B5BD80733B75C6A84A63E29D3199E7535D044E34293B0A55AD63F4C91`. No Nexus upload or page edit was performed.

Previous compatibility result: [game patch 2.03.00 review](Update-2.03.00-compatibility-review.md). The 1.0.6 archive failure and the patch's separate 16-byte full-map projection shift are confirmed locally on Steam build 25381195 / executable 1.0.0.2944. [Version 1.0.7-test2](local-test-1.0.7/START-HERE.md) adds bounded structural discovery for that moved projection block. Its complete regression build, untouched archive probe, 10/10 read-only live full-map/display projection samples, deployment checks, and installed gameplay validation pass. The user confirmed the minimap, full-screen map, full-map pan/zoom, and ReShade health.

[Release 1.0.7](release-1.0.7/RELEASE-INDEX.md) is prepared for the user's manual upload. The final four-file ZIP is SHA256 `E649C6B9C1614A56B8EF09160E9118E4E4103B24075B9BC66E2BADEC9FC13857`; no Nexus upload or page edit was performed.

Latest bundle: [1.0.6 release bundle](release-1.0.6/RELEASE-INDEX.md) removes the exact executable version/hash lock and supersedes the 1.0.5 preparation. The updated description lists tested game versions as history. Full regression/native host checks and automatic detection against the installed game executable pass. The user confirms normal startup and visible routes on both maps with 1.0.6-test1; the game log also confirms startup, automatic discovery and route submissions. The bundle is ready for manual upload. The user will upload it; no public page change or upload has been performed.

Installed: [1.0.6-test1 automatic map detection](local-test-1.0.6/START-HERE.md). Only the verified previous own add-on changed, with a workspace backup; all 979 other bin64 files are hash-identical. [Deployment record](local-test-1.0.6/deployment-verification.json).

Prior gameplay validation: [1.0.5-test1 native full-map UI scaling](local-test-1.0.5/START-HERE.md). Corrected reader checks passed at 4096x2160, 2560x1440, 3840x2160 and 1920x1080 in the same game session. Only our verified add-on changed; all 979 other files are hash-identical. [Deployment record](local-test-1.0.5/deployment-verification.json). The user now reports the 4096x2160 full-map test looks good after relaunch. [Visual test result](local-test-1.0.5/User-test-results.md). [Scaling evidence](local-test-1.0.5/Resolution-evidence.md).

TODO-002 is resolved locally with the user-accepted validation scope. Further display configurations will be investigated through bug reports; no additional resolution testing is required for this task. [Tracker](../docs/known-issues.md).

Previous user validation on 1.0.4: both maps worked at 2560x1440 and the route stayed active through vendor, enemy and NPC interactions. Minimap turn lag remains TODO-003; remaining destination lifecycle checks are pending. [User results](local-test-1.0.4/User-test-results.md). The 2760 and 2850 recorded addresses are retained as regression fixtures, not a runtime allowlist. The UI-group fields are independently verified on 2850 only; future layout changes still need investigation.

## Start here

- [Requirements and decisions](Project-requirements-and-decisions.md): current user constraints, intended architecture, and unresolved choices.
- [Development journal](Development-journal.md): completed work, evidence, and discoveries in sequence.
- [Prepared Nexus description for 1.0.6](release-1.0.6/NEXUS-DESCRIPTION.md), with [copy-ready BBCode](release-1.0.6/NEXUS-DESCRIPTION-BBCODE.txt). The [previous published description](release-1.0.0/NEXUS-DESCRIPTION.md) is retained as history.
- [Validation and next milestones](Validation-and-milestones.md): what has passed, what remains unverified, and how to continue.

## Current result

The installed archives contain 2,504 navigraph sectors. Their headers passed structural checks. An independent research parser decoded five nearby sectors with 6,180 node records and 8,690 connection records. An offline Dijkstra experiment produced a 39-point route across two sectors using provisional bidirectional connections and exact-position joins.

This result verifies an offline geometric path under those assumptions. It does not yet verify player traversability, fastest travel time, alignment with the game map, or live ReShade operation.

- [Route plot](Navigation-route-probe.svg)
- [Detailed decoding results and provisional binary format](Navigation-decoding-progress.md)
- [Archive inventory and extraction findings](Game-navigation-data-mining.md)

## Research library

| Document | Purpose |
|---|---|
| [Initial investigation](Navigation-investigation.md) | Earlier research context |
| [Crimson Route review](CrimsonRoute-review.md) | Research-only examination of the old mod and its logs |
| [ReShade integration research](ReShade-navigation-research.md) | Host architecture, installed versions, and integration gaps |
| [Map-data construction research](Map-data-construction-path.md) | Website markers and alternative data sources |
| [Nexus tooling and boundaries](Nexus-tooling-and-boundaries.md) | Earlier distribution/tooling research |
| [Installation baseline](installation-baseline.json) | Recorded versions and selected file hashes |

Earlier reports reflect knowledge at the time they were written. The requirements document records later user clarifications: ReShade is the sole intended runtime dependency, distribution will use files and instructions without an installer, and uniquely named additions are allowed while existing game/mod files remain protected. Earlier planning language about an installer or undecoded data is historical.

## Keeping these records current

After each meaningful development step, update the journal and validation status. Record the exact game build and relevant dependency versions, commands or evidence files, test outcomes, limitations, and changes in assumptions. Keep observations separate from hypotheses. Do not label an offline result as an in-game test.

Store user-facing documentation in outputs. Keep scratch scripts, extracted game data, and experimental artifacts in work. Local research material is not a Nexus release package.


## Historical progress entries

Latest local test: [0.4.2 recovery and render timing](local-test-0.4.2/START-HERE.md). Installed; offline checks passed; live verification pending.

Work in progress: 0.4.3 automatic off-course rerouting built and offline-tested, not installed. Minimap locking awaits a coordinated motion capture.

0.4.3 now also includes the terrain projection correction from the successful motion capture. Built; awaiting game closure for installation and visual validation.

Latest installed build: 0.4.3. Deployment verified; awaiting visual terrain-alignment and off-course rerouting tests.

Latest installed build: [0.4.4 minimap pulse](local-test-0.4.4/START-HERE.md). Build/deployment verified; pulse appearance pending user feedback.

Latest installed: [0.4.5 moving route start](local-test-0.4.5/START-HERE.md). Offline and deployment checks passed; in-game test pending.

Latest: [1.0.0 release package and documents](release-1.0.0/RELEASE-INDEX.md). Installed locally; automatic startup verification remains before public upload.

Header artwork and source notes: release-1.0.0/header. Original official screenshot plus localized gold route overlay; separate from gameplay screenshots.

Approved 1.0 header: [wide gold route artwork](release-1.0.0/header/Crimson-Navigation-header.png).

Gameplay media: [seven verified screenshots and Nexus captions](release-1.0.0/screenshots/GALLERY.md). Supplied September 9; settings screenshot shows preserved custom appearance.

Nexus 1.0.0 published per user; first Linux full-map artifact report pending screenshot. GitHub source repository preparation is underway.

Latest local test: [1.0.1 dashed and dotted minimap styles](local-test-1.0.1/START-HERE.md). Installed; visual validation pending.


## September 9, 2026 - QoL planning

- [QoL specification](QoL-spec.md): selected scope, proposed ReShade menu, migration, acceptance criteria and pinned backlog.
- [Plan 1: appearance and settings](QoL-plan-1-appearance-and-settings.md): separate map color/width, accessibility controls, automatic saving and appearance-only reset.
- [Plan 2: route shortcut](QoL-plan-2-route-shortcut.md): configurable Ctrl+Shift+R proposal, capture, persistence and validation.

Planning only; no runtime changes, build, deployment or publication. Final approach and gentle arrival feedback remain pinned backlog items.

Current bug tracker: [three open TODOs](../docs/known-issues.md): TODO-001 remaining lifecycle checks, TODO-003 minimap turn lag, and TODO-004 patch 2.03.00 startup compatibility. TODO-002 resolution scaling is resolved for its accepted validation scope. The 1440p session and vendor/enemy/NPC persistence have positive user validation.

Latest installed local candidate: [1.0.4-test1](local-test-1.0.4/START-HERE.md). [User test results](local-test-1.0.4/User-test-results.md) confirm 1440p operation and persistence through vendor, enemy and NPC interactions, with minimap turn lag remaining.

Current TODO-002 evidence: [resolution math and validation limits](local-test-1.0.2/Resolution-math-and-evidence.md); [deployment record](local-test-1.0.2/deployment-verification.json).
