# Crimson Navigation 1.0.6-test1 - automatic map detection

Installed and packaged for the requested removal of the exact executable lock. [Replacement Nexus bundle](../release-1.0.6/RELEASE-INDEX.md).

The reader no longer hashes the executable or selects a version from an allowlist. It derives four map class vtable identities from the current file's MSVC x64 RTTI at startup. A compatible update can move those identities or change unrelated code without needing a new hash entry. All existing live object/name/pointer/collection/destination/projection checks remain active. Changed, missing or ambiguous structures produce an unavailable-layout status instead of falling back to old addresses.

## Verified

- Test-first reproduction: all six changed-address/unrelated-change acceptance tests failed against the original hash gate, then passed with automatic detection.
- 39 detector fixture checks pass, including malformed PE input, missing/duplicate metadata, hierarchy validation, executable method pointers and cancellation before detection.
- Read-only detection against installed executable 1.0.0.2850 returns exactly the four independently recorded RVAs: 89530200, 89514408, 89531688, 89528856. Observed detector times: 76 ms in the optimized focused test; 310 ms in the unoptimized full-build test. These are individual measurements, not startup-time guarantees.
- Full build and all regression suites pass, including 3,997 resolution checks and 15 layout compatibility checks. Native load/registration/rejection/unload and 10 reader start/stop cycles pass.
- Independent review found no blockers. A future mid-scan cancellation test was suggested as optional coverage; current checks during loading and all scans were inspected.
- Only our verified add-on was replaced while the game was closed. Backup is in backup/CrimsonNavigationLab-1.0.5-test1.addon64; all 979 other bin64 files are unchanged.

## Fresh gameplay check

The new game process (PID 22552) logs 1.0.6-test1 loading at 14:27:50, both map identities discovered at 14:29:46, full-map route submission at 14:30:15 and minimap submission at 14:30:21. [Runtime evidence](startup-game-log.txt). The user confirms "yup all good" in response to the explicit normal-startup/both-map route check. The current build therefore has a positive user visual result on both maps. The game map layout diagnostic identifies the detected layout as Auto-detected. No repeat resolution matrix is required; earlier accepted resolution evidence remains valid for the unchanged scaling implementation.

The exact built/deployed binary is SHA256 CF2508191ACD62666C6FBFD1A8407F4E486A42958E3B61E705D60AA814678FBE. Its internal label is 1.0.6-test1. Earlier visual feedback applies to 1.0.4/1.0.5, with current 1.0.6 startup/submission evidence recorded separately. Future game builds and Linux/Proton remain untested.

No changes to minimap drift, marker routing or planned appearance controls are included. ReShade remains the only runtime dependency. No original game files, saves, Steam configuration, third-party files/configuration or settings changed. No publication occurred.

[Build log](build-check.txt), [automatic detector check](automatic-detection-check.txt), [build verification](build-verification.json), [deployment verification](deployment-verification.json).

[User confirmation and scope](User-test-results.md). The prepared release is ready for the user's manual upload.
