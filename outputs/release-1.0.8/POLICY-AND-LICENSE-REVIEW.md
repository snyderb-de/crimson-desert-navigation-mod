# Submission and license review - November 10, 2026

The [Nexus file submission guidelines](https://help.nexusmods.com/article/28-file-submission-guidelines), last updated November 4, 2026, were re-read for this release. They now name three AI categories: AI-Generated Content (AI produced the majority of the code, UI or similar), AI Media (AI-created promotional material or descriptions) and AI Assisted (limited AI involvement with documented development history). This project's code and documentation were mostly AI-generated, so keep **AI-Generated Content** and **AI Media**. AI Assisted does not apply. The guidelines warn that incorrect tagging, including undisclosed generative AI use, may lead to moderation action.

The guidelines also require permission for reused content and accurate, informative descriptions. The ZIP contains only this independent add-on and its documentation. The description states the tested resolutions and the known DLC island limit, and marks future features as not included. No installer, game assets or other authors' mods are packaged.

The [Nexus terms of service](https://help.nexusmods.com/article/18-terms-of-service), last updated May 12, 2026, section 22, leave upload rights and game-license compliance with the uploader. This preparation does not assert Nexus approval or grant new content permissions. Existing published permission settings are preserved.

The credits now name both AI tools used: OpenAI Codex for earlier versions and Anthropic Claude Code for the 1.0.8 work.

The ReShade SDK BSD-style license and the Dear ImGui MIT license in the local build dependencies were compared with THIRD-PARTY-NOTICES.txt; see `package-verification.json`. Both complete notices are kept in the ZIP. ReShade is obtained separately; there is no new runtime dependency.

The [Pearl Abyss fan content guidelines](https://crimsondesert.pearlabyss.com/en-US/Policy?_policyNo=130), last updated March 26, 2021, ask fan content to include the statement "This is unofficial content which contains copyrighted materials and IP from Pearl Abyss, and is not official/endorsed content." That exact statement is now in the README and the Nexus description. The content stays non-commercial. The existing user-supplied gallery captures are copied unchanged to a separate optional folder, are not in the mod ZIP, and are labeled as earlier-version captures. No new publisher artwork is added.

Reading DLC road data for research was read-only and local. No extracted game data is distributed. No upload or public edit was performed.
