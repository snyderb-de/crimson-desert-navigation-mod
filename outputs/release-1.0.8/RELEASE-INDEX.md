# Crimson Navigation 1.0.8

Replacement for 1.0.7, for game patch 2.04 (Steam build 26480954 / executable 1.0.0.3576). Internal version: 1.0.8-test3. No upload or page edit has been performed.

Final ZIP SHA256: `D5275E9B5BD80733B75C6A84A63E29D3199E7535D044E34293B0A55AD63F4C91` (240,274 bytes). It contains only the add-on, README, changelog and third-party notices.

Verified:

- The packaged add-on is the exact installed, in-game-tested test3 binary, SHA256 `86C35B987D251CAFC805E6A5D121E22168A26258DFE047888302CBEB51B02B5A`.
- ZIP CRC, four-file allowlist, staged-file identity, x64 PE, AddonInit/AddonUninit exports, no BCrypt import, complete ReShade/ImGui notices and BBCode balance pass. The seven earlier gallery images are unchanged.
- Full regression and native host checks pass. The production loader reads the untouched patch 2.04 road archive.
- In game: routes on both maps and full-map pan/zoom at 1920x1080 (test2). At 3840x2160, placement was correct with test2, and test3 adds width scaling. The user approved the release after the 4K test3 session. The ReShade log excerpt `local-test-1.0.8/test3-swapchain-excerpt.txt` shows test3 loading with a 3840x2160 swap chain; `startup-game-log-test3.txt` shows route submissions on both maps and no reader internal error. Test3 itself was not separately run at 1080p or re-checked for pan/zoom; its only change is width scaling, which is exactly 1.0 at 1080p.

Not verified: other resolutions and aspect ratios, Linux/Proton, and routes to destinations on DLC islands.

Known minor regression, documented in the release notes: below 1080p with the Dotted minimap style, a width under about 1.5 makes the dot radius fall below the ImGui minimum, so dots are not drawn. A minimum dot radius is planned for the next version (TODO-006).

- [Main ZIP to upload](Crimson-Navigation-1.0.8.zip)
- [Upload steps](NEXUS-UPLOAD-NOTES.md)
- [Nexus description - copy-ready BBCode](NEXUS-DESCRIPTION-BBCODE.txt)
- [Nexus description - readable preview](NEXUS-DESCRIPTION.md)
- [Changelog](NEXUS-CHANGELOG.txt)
- [Installation and operation](package/README.txt)
- [Existing gallery images and captions](screenshots/GALLERY.md)
- [Verification](package-verification.json)
- [Policy and license review](POLICY-AND-LICENSE-REVIEW.md)
- [Patch 2.04 compatibility review](../Update-2.04-compatibility-review.md)
