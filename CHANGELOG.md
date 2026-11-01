# Changelog

## DS Style Merkin Mix 8.0.0

This build includes the v7.4.6b fixes and follow-up improvements.

### New features since v7.4.6b

- No features this time, just fixes and optimizations.

### New fixes since v7.4.6b

- Prevented highly fragmented ROM and RTS files from overflowing the fixed FPGA FAT-table transfer buffer. Files whose fragment tables exceed the supported capacity are rejected instead of sending a corrupted table.
- This fragmented-card fix passed the host regression suite but has not been tested with a physical cartridge and highly fragmented SD card. Please report any issues.
- Updated the Help > About screen to show the DS Style Merkin Mix name and build number.

### Features added through v7.4.6b

- **v7.4.6b:** No new user-facing features; this build focused on fixes and optimizations.
- **v7.4.5:** Added an optional Wrap around setting for browser navigation.
- **v7.4.3:** Added SD-card mirrored artwork lookup with custom and built-in thumbnail fallbacks, selective transparency for marked BMP thumbnails, and refreshed console icons and startup artwork.

### Fixes through v7.4.6b

- **v7.4.6b:** Kept the kernel image within the supported 2 MiB limit, with oversized builds failing reliably instead of leaving a stale image that could appear successful. Improved save handling so failed first-time saves do not leave incomplete files and errors checking existing saves are not mistaken for missing saves.
- **v7.4.6b:** Improved cartridge-flash operation checks and feedback, refreshed the game list after successful formatting, avoided unnecessary cartridge-info writes, reduced NOR format progress redraw overhead while retaining full erase verification, and fixed format confirmation/progress redraws to clear the full menu row.
- **v7.4.6b:** Fixed cheat-file parsing so values from one entry cannot carry over into the next. This feature has not been hardware-tested; please report any issues.
- **v7.4.6b:** Restored the established settings-save polling behavior while retaining newer checks for other cartridge records, and consolidated settings readers while removing unused code. Subsequent hardware testing on v7.4.6b has not reproduced the reported settings-persistence issue.
- **v7.4.5:** Fixed game launch with Enable BIOS off that was broken with a previous fix; improved long directory-name display, selection, save paths, and artwork lookup when folder aliases differ; reduced repeat directory scans during thumbnail browsing; and updated launcher stack/heap placement for FatFs long-filename handling.
- **v7.4.3:** Hardened NOR catalog bounds, title sizes, source reads, block padding, metadata commits, interrupt restoration, and trim-probe arithmetic. Limited deletion to a verified newest title, fixed NOR progress redraws, hardened SD sector and save/RTS I/O checks, bounded paths and malformed cheat data, fixed PSRAM writes across 8 MiB boundaries and rejected odd-address writes, updated NOR guidance, and aligned installer regression expectations with the shipped packages.

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
