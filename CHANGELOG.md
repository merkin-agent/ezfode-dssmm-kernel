# Changelog

## DS Style v7.4.6b (test build)

This test build includes the v7.4.6 fixes and follow-up improvements. It has not been tested on hardware.

### New features since v7.4.5

- No new user-facing features.

### New fixes since v7.4.5

- Kept the kernel image within the supported 2 MiB limit. Oversized builds now fail reliably without leaving a stale image that could be mistaken for a successful build.
- Improved save handling: failed first-time saves no longer leave incomplete files behind, and errors checking an existing save are not mistaken for a missing save.
- Improved cartridge-flash operation checks and feedback, and refresh the game list after a successful format.
- Avoided unnecessary cartridge-info writes when the stored data is unchanged.
- Reduced NOR format progress redraw overhead while keeping the full erase verification.
- Fixed NOR format confirmation and progress redraws so they clear the full menu row instead of overlapping the delete notice.
- Fixed cheat-file parsing so values from one entry cannot carry over into the next.
- Restored the established settings-save polling behavior while retaining the newer checks for other cartridge records. This test build has not been hardware-tested, so settings persistence still needs verification.
- Consolidated settings readers and removed unused code without intentionally changing settings-file behavior.

### Features added through v7.4.5

- **v7.4.5:** Added an optional Wrap around setting for browser navigation.
- **v7.4.3:** Added SD-card mirrored artwork lookup with custom and built-in thumbnail fallbacks, selective transparency for marked BMP thumbnails, and refreshed console icons and startup artwork.
- **v7.4:** Added a text and Markdown viewer with light/dark display options and simple formatting, plus optional wide and square folder artwork for supported systems.
- **v7.3:** Added support for full-screen themes without a separate top bar.
- **v7.1:** Added List + Art view with a floating thumbnail at the top, centre, or bottom; expanded in-kernel guidance and refined menus.
- **v7.0:** Added hidden system files, kernel style loading, 12/24-hour clocks, art borders, rounded corners, clean lists, folder-only lists, carousel side-art alignment, empty-folder/favourites messages, NOR title thumbnails, Thai language support, categorised Settings and Help pages, and built-in setting explanations; overhauled cheat selection.
- **v6.7:** Separated Start screen enablement from its Last Played/Favourites source; added direct Recently Played startup on the SD page and Select/L/R switching between recent and favourite lists.
- **v6.6:** Added custom artwork for folders and files, scraper integration, an editable CUSTOM artwork example, cached artwork names, and support for 120 x 80 title and 80 x 80 box thumbnails (up to 256 images per CUSTOM folder).
- **v6.5:** Added an Off option for the Start screen source to open the SD browser directly.
- **v6.4:** Added multi-language launcher support, Chinese language handling while retaining the DS Style font for Latin text, customisable launcher text, Start screen layout options, and thumbnail-border settings.

### Fixes through v7.4.5

- **v7.4.5:** Fixed game launch with Enable BIOS off; improved long directory-name display, selection, save paths, and artwork lookup when folder aliases differ; reduced repeat directory scans during thumbnail browsing; and updated launcher stack/heap placement for FatFs long-filename handling.
- **v7.4.3:** Hardened NOR catalog bounds, title sizes, source reads, block padding, metadata commits, interrupt restoration, and trim-probe arithmetic. Limited deletion to a verified newest title, fixed NOR progress redraws, hardened SD sector and save/RTS I/O checks, bounded paths and malformed cheat data, fixed PSRAM writes across 8 MiB boundaries and rejected odd-address writes, updated NOR guidance, and aligned installer regression expectations with the shipped packages.
- **v7.4:** Fixed empty Recently Played/Favourites crashes, restored support for 512 files per folder while reducing RAM use, fixed clean software-boot compatibility, and prevented RECENT.txt corruption at its ten-game limit.
- **v7.4.1:** Prevented failed SD reads from being used as valid save, save-state, or sector data; checked save/RTS write results and retained pending recovery after failures; bounded folder paths, cheat filenames and records, and fixed-width save/RTS names; initialized folder counts after SD recounts; fixed PSRAM patch writes across 8 MiB boundaries; and corrected installer test expectations.
- **v7.3:** Fixed translated save-type text overlap, improved List + Art redraws, refreshed Help clock/page numbers, refined cheat selection redraws, and fixed horizontal carousel border flicker.
- **v7.2:** Fixed Chinese text on the loading screen and custom backgrounds shifting or repeating in list views.
- **v7.1:** Improved list, carousel, and popup redraws; fixed translated launch-menu text; improved input handling between Settings, SD Card, and NOR Flash; and kept the launcher clock independent of the in-game RTC setting.
- **v7.0:** Fixed empty-view messages, improved menu use of memory, refined cheat selection, splash timing, and Settings tab restoration.
- **v6.9:** Improved launcher stability, bounded cheat entries to available patch space, refined list scrolling and navigation, and made menu audio playback safer.
- **v6.8:** Fixed translated settings resetting or loading incorrectly, improved accented filename support, and corrected language values written to SETTINGS.TXT.
- **v6.5:** Fixed Start-screen back navigation when the screen is disabled and improved held-button scrolling in game-launch menus.
- **v6.4:** Added safer file-copy, backup, thumbnail-read, and directory-bound handling, and kept the Omega DE and Original Omega source lines aligned where features overlap.

These changes and fixes were completed with the assistance of AI.

## DS Style v7.4.1

- Prevented failed SD reads from loading stale data into saves, save states, or sectors.
- Checked save and RTS writes before reporting success, and kept pending save recovery available after a failed write.
- Bounded malformed cheat data and filename handling before parsing or extension replacement.
- Fixed ROM patch writes that cross an 8 MiB PSRAM page boundary.
- Corrected bundled installer test expectations to match the delivered v7.4 style packages.

## DS Style v7.4

- Added a new DS Style text and Markdown viewer with a clean English interface, light and dark modes, faster navigation, and simple formatting for headings, bold text and dividers.
- Added optional wide and square console folder artwork for every emulated system supported by DS Style.
- Fixed a crash when pressing A in an empty Recently Played or Favourites view.
- Restored support for up to 512 files in a folder while reducing launcher RAM usage.
- Fixed compatibility with games that require a clean software-boot memory state on the Omega Definitive Edition.
- Fixed `RECENT.txt` corruption when Recently Played reached its ten-game limit.

## DS Style v7.3

- Fixed translated save-type labels overlapping the detected save type in the game-launch menu, and limited save-type changes to the affected value.
- Refined List + Art redraw handling when moving between entries with and without artwork or switching from List view.
- Added source support for full-screen themes without a separate top bar.
- Fixed the clock and page-number area not refreshing correctly on Help screens.
- Refined cheat-screen redraw behaviour, including smoother list movement, clean counter changes and immediate updates to mutually exclusive selections.
- Fixed artwork border flicker in the horizontal carousel.

## DS Style v7.2

- Fixed Chinese text rendering on the game loading screen.
- Fixed custom backgrounds being shifted or repeated while scrolling in List and List + Art views.

## DS Style v7.1

- Added List + Art view, combining list navigation with a floating thumbnail at the top, centre or bottom of the screen.
- Refined list, carousel and pop-up redraw behaviour for smoother menu navigation and less flicker.
- Fixed translated text handling in game-launch pop-up menus, including Chinese text.
- Improved input handling while artwork loads and when moving between Settings, SD Card and NOR Flash.
- Kept the launcher clock independent from the In-game RTC option, matching the official kernel behaviour.
- Added general menu refinements and updated the built-in guidance.

## DS Style v7.0

- Added a Hide system files setting for keeping kernel, system, and metadata files out of the root file browser.
- Added Settings > Load style for installing `.bin` kernels from `SYSTEM/KERNELS`.
- Added 12-hour and 24-hour clock format options.
- Added an Art border option for drawing a one-pixel outline around the selected carousel artwork.
- Added a Rounded corners option for carousel artwork and Start screen artwork.
- Added Clean list and List folders options for simpler file browsing.
- Added vertical and horizontal carousel side-art alignment options.
- Added empty folder and empty favourites messages.
- Added thumbnail artwork support for games written to NOR.
- Added Thai language support.
- Reworked the Settings and Help pages into clearer categories and in-kernel guidance screens.
- Added built-in Using DS Style help pages with clear setting explanations.
- Overhauled the cheat screen and cheat selection behaviour.
- Added general user-interface refinements across menus, pop-ups and carousel views.
- Refined splash-screen timing so boot waits for the startup sound without lingering longer than needed.
- Refined settings tabbing so nested Settings pages are remembered when moving to SD Card or NOR Flash and back.

## DS Style v6.9

- Improved launcher stability by reducing RAM pressure in the menu.
- Added a safety limit for cheat entries to match the space available in the in-game patch.
- Refined list-view scrolling, redraw behaviour, and navigation consistency across launcher menus.
- Refined the sound system for safer, more reliable audio playback during menu navigation.

## DS Style v6.8

- Fixed translated settings values resetting or loading incorrectly after restarting.
- Improved accented character support for SD card filenames.
- Corrected the language options written to SETTINGS.TXT.

## DS Style v6.7

- Split Start screen On/Off from the Last played/Favourites source setting.
- Select on the Start screen now only switches between Last played and Favourites.
- Start on the SD page now opens Recently Played, then cycles between Recently Played and Favourites.
- L and R also switch between Recently Played and Favourites while inside those lists.

## DS Style v6.6

- Added custom thumbnail support for folders and all files. Improved thumbnail scraper integration.
- Each CUSTOM folder supports up to 256 images due to the GBA's RAM limitations.
- Replaced the hardcoded GBA-folder preview with an editable CUSTOM artwork example.
- Cached the available custom artwork names so normal file browsing remains responsive.
- Added support for both 120 x 80 title thumbnails and 80 x 80 box thumbnails in the CUSTOM folders.

## DS Style v6.5

- Added an Off option for the Start screen source, allowing startup to go straight to the SD browser.
- Prevented B from returning to the Start screen when the Start screen is disabled.
- Updated game launch menus to use held-button scrolling without wrapping at the top or bottom.
- Continued parity fixes shared with the original OMEGA source where applicable.

## DS Style v6.4

- Added multi-language launcher support.
- Added Chinese language handling while keeping Latin text on the DS Style font.
- Added customisable launcher text through DS Style Customiser.
- Added Start screen layout customisation support.
- Added thumbnail border option support.
- Added safer file copy, backup, thumbnail-read, and directory bounds handling.
- Updated both Omega DE and Original Omega source lines to stay aligned where features overlap.
