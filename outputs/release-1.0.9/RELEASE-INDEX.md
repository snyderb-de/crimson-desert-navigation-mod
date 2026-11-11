# Crimson Navigation 1.0.9

Replacement for 1.0.8, for game patch 2.04 (Steam build 26480954 / executable 1.0.0.3576). Internal version: 1.0.9-test1. It fixes TODO-006: Dotted-style minimap dots no longer disappear below 1920x1080 at small route widths. No upload or page edit has been performed.

Final ZIP: see `package-verification.json` for the SHA256 and size. It contains only the add-on, README, changelog and third-party notices.

Verified:

- The packaged add-on is the exact installed 1.0.9-test1 binary, SHA256 `C86EC6A8DADAEF8E7474EEC759C33F918A81C7104C237C8EC084316D66A20B4E`. It was installed after the game closed. The 1.0.8 add-on is backed up, and all 979 other `bin64` files are unchanged (`local-test-1.0.9/deployment-verification-1.0.9-test1.json`).
- ZIP CRC, four-file allowlist, staged-file identity, x64 PE, AddonInit/AddonUninit exports, no BCrypt import, complete ReShade/ImGui notices and BBCode balance pass. The seven earlier gallery images are unchanged.
- Full regression suite, including 16 route scale checks, and the native host check pass.
- The only code change from the in-game-tested 1.0.8 release is the dot radius: half the scaled width, never under 0.5 px. Output at 1920x1080 and above is unchanged.

Not verified: 1.0.9-test1 has not been run in game. The Dotted style below 1080p, other resolutions and aspect ratios, Linux/Proton, and routes to destinations on DLC islands are unchecked.

- [Main ZIP to upload](Crimson-Navigation-1.0.9.zip)
- [Upload steps](NEXUS-UPLOAD-NOTES.md)
- [Nexus description - copy-ready BBCode](NEXUS-DESCRIPTION-BBCODE.txt)
- [Nexus description - readable preview](NEXUS-DESCRIPTION.md)
- [Changelog](NEXUS-CHANGELOG.txt)
- [Installation and operation](package/README.txt)
- [Existing gallery images and captions](screenshots/GALLERY.md)
- [Verification](package-verification.json)
- [Policy and license review](POLICY-AND-LICENSE-REVIEW.md)
- [Build notes](../local-test-1.0.9/START-HERE.md)
