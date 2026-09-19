# Crimson Navigation 1.0.7

Replacement for 1.0.6. Internal version: 1.0.7-test2. Regression, native host, untouched patch 2.03 archive, read-only live full-map projection, deployment, and installed both-map gameplay checks pass. The user also confirmed full-map pan/zoom and ReShade health. No upload or page edit has been performed.

Final ZIP SHA256: `E649C6B9C1614A56B8EF09160E9118E4E4103B24075B9BC66E2BADEC9FC13857` (236,930 bytes). It contains only the add-on, README, changelog, and third-party notices; its CRC, allowlist, staged-file identity, x64 exports, dependency notices, BBCode balance, and exact tested-binary checks pass.

- [Main ZIP to upload](Crimson-Navigation-1.0.7.zip)
- [Upload steps](NEXUS-UPLOAD-NOTES.md)
- [Nexus description - copy-ready BBCode](NEXUS-DESCRIPTION-BBCODE.txt)
- [Nexus description - readable preview](NEXUS-DESCRIPTION.md)
- [Changelog](NEXUS-CHANGELOG.txt)
- [Installation and operation](package/README.txt)
- [Existing gallery images and captions](screenshots/GALLERY.md)
- [GitHub README image provenance](../../docs/images/README.md)
- [Verification](package-verification.json)
- [Policy and license review](POLICY-AND-LICENSE-REVIEW.md)

This release fixes patch 2.03.00 road-archive startup compatibility and the patch's separate 16-byte full-map projection shift while preserving automatic executable-layout detection and earlier fixes. Some resolutions may still need fixes. Combat and other interactions may briefly hide the route; it should return while the active navigation destination remains set. Minimap drift improvements are ongoing. Routing to user-placed map markers, pulse animation speed, pulse blur and route glow controls are planned for future updates.
