# SkillerCMP cheat engine integration review

Review date: 2026-11-03 UTC. **Recommendation: do not merge the branch or replace whole files. A staged compiler/interpreter-only port is feasible in principle, but is not yet demonstrated regression-free. Keep the legacy engine as the default until differential tests and target testing pass.** This review makes no production changes and no hardware compatibility claim.

## Provenance and scope

Remote: https://github.com/SkillerCMP/omega-de-ds-style-kernel . `git ls-remote ... HEAD` returned **e0d0d9dbd93a07b7f9f5946fc52ae40622725b27**; a separate clone in `/tmp/skiller-review` resolved to that same commit. Tip: “Add download badges to README.md”, authored 2026-09-26T01:10:42-06:00. Preceding implementation/release commit: **2ad491325bedc6fdd48eeb4192d3a1b22c0124d6**, “V7.4C”. These are observations at retrieval, not a promise that the remote remains unchanged.

Local HEAD: **bbcef88b411e42efd49b71fc906d3e1e9aedd967**. The comparison uses the **working tree**, including the coordinator's uncommitted remembered-cheat/collapse work, not merely HEAD. Existing modifications to the launcher, Makefile, translations, audit documents, tests, and other files were preserved. No checkout, reset, stash, merge, production build, or firmware installation was performed. Temporary comparison/assembly artifacts live outside the repository. Line references below describe this snapshot and can move as the coordinator finishes its candidate.

Pinned upstream source: https://github.com/SkillerCMP/omega-de-ds-style-kernel/tree/e0d0d9dbd93a07b7f9f5946fc52ae40622725b27/source . Review covered the requested patchers and UI, plus `GBApatch.h`, `reset.s`, `Ezcard_OP`, `NORflash_OP`, launcher launch/save paths, `saveMODE.h`, and the three new RTS3 includes and identity implementation. This is a static integration assessment, not exhaustive verification of every upstream change.

## File and dependency comparison

| Area | Current working tree | Skiller snapshot | Decision |
| --- | --- | --- | --- |
| `GBApatch.c` | Legacy compact address conversion; fixed trim reservations; page-split PSRAM writes; built-in compatibility patches/table | Opcode-aware emission, ROM callbacks, calculated runtime sizing, RTS3 finalization, external patch profiles | Adapt narrowly; never replace wholesale |
| `gba_rts_patch.s` | Byte-write loop; embedded 0x400-byte table; legacy RTS/menu | F1–FF interpreter; appended records; RTS3 core/menu and PC-relative macro | Extract interpreter and required helpers only |
| `gba_rts_only.s` | Legacy direct save/load runtime | RTS3 core and identity protocol | Exclude from cheat-only port |
| `showcht.c/.h` | Legacy parser plus current CHS2 remembered selections, headless load, collapse | Revision-6 compiler, parent-section model, multi-select/standalone rows, ROM groups, in-memory selection cache | Keep local persistence/UI contract; isolate compiler |
| reset/launch | EWRAM stack-aware handoff; bounded FAT/save handling; current multi-system launcher | Different stack/reset assumptions, launch/profile/cache paths and SRAM loading | Preserve local code; explicit integration hooks only |
| save metadata | Existing raw battery saves and legacy RTS; expanded save lookup table | Same broad battery-save concept, packed lookup table; incompatible RTS3 contents | No save-system migration as part of cheats |

### Compiler and interpreter contract

Upstream `showcht.h:10–78` defines revision 6 and 128 eight-byte runtime records. `showcht.c:1359–2195` parses enhanced commands and rolls back counts/work on an invalid option. Operations include byte/halfword/word writes, nested comparisons (including masked forms), ELSE/ENDIF, ADD/SUB, pointers, fill and slide. The runtime dispatcher is `gba_rts_patch.s:165–195`, with helpers through approximately line 455. This is a custom `.cht` compiler/bytecode contract; this review does not establish direct support for arbitrary encrypted Action Replay/GameShark input.

These components must travel together. Local `GBApatch.c:517` onward remaps every record to EWRAM/IWRAM and local assembly performs `strb` unconditionally. Feeding enhanced opcodes into that path would turn commands into unintended RAM byte writes. Conversely, compiler output cannot simply be copied into a differently interpreted table. Upstream `ResolveRuntimeCheatRecordAddress` (`showcht.c:990`) preserves opcode markers and full RAM addresses, bounds compact addresses, and rejects direct I/O writes. Preserve that contract and test the stricter acceptance policy against existing cheat files; legacy masking previously accepted a broader input space.

The parser's per-option rollback and write-work accounting are useful separable ideas. They are not proofs that all malformed tables are safe. The interpreter trusts several compiler invariants; marker acceptance in the emitter alone does not validate a complete control-flow/data-record sequence. Reject whole invalid options before installation rather than silently dropping individual records and possibly breaking multi-record commands.

### Current remembered selections and collapse must survive

Local `showcht.c:25–218` uses full path plus cheat identity, content hashing, a checksum, bounded sorted indices, and CHS2 files in `/SYSTEM/CHEATSTATE/*.cs2`. `CheatStateLoadHeadless` is called from the current launcher before launch (`ezkernelnew.c:16944`). Upstream's cache instead uses a 128-byte filename-based identity (`showcht.c:47–135`) and has no corresponding CHS2 disk/headless implementation. A wholesale import would regress persistence and directory/long-name isolation.

The models are also semantically different: local collapse constants are expanded=0/collapsed=2; upstream uses collapsed=0/expanded=1 and adds `parent_section` to each row. Upstream supports multi-select groups and standalone rows. Local `CheatStateApply` deliberately rejects standalone or repeated selections within one section. Therefore preserving the disk struct alone does not make the new menu compatible. First retain local row semantics and compile selected values through a separate backend. Treat multi-select/standalone UI and any persistence versioning as later work. If parsing changes row indices, a matching content hash alone cannot prove old saved indices still name the same choices; retain stable legacy enumeration or add explicit schema identity/invalidation.

## Memory and placement: measured, not inferred from comments

All four assembly sources assembled successfully with installed devkitARM `arm-none-eabi-as -mcpu=arm7tdmi`, using each source directory as its include path. `arm-none-eabi-nm -n` gave:

| Symbol extent | Local | Skiller |
| --- | ---: | ---: |
| Combined runtime before `CHEAT` | 0x0D04 | 0x28F8 |
| Combined source start-to-end | 0x1104 (includes 0x400 table) | 0x28F8 (no table) |
| Combined runtime with 128 records | 0x1104 / 4,356 bytes | 0x2CF8 / 11,512 bytes |
| RTS-only start-to-end | 0x0568 / 1,384 bytes | 0x1D7C / 7,548 bytes |

Local `SetTrimSize` reserves 0x2000 with cheats, 0x1000 with RTS, or 0x300 otherwise (`GBApatch.c:310–322`). Full Skiller combined and RTS-only replacements exceed those budgets. Even local combined RTS with cheats disabled deserves an explicit actual-length assertion: the no-table body is 0xD04, which fits its 0x1000 reservation; relying on source end including an unused table would misstate that case.

Upstream calculates runtime sizes and gates them against 0x5400 staging bytes and the 32 MiB ROM end (`GBApatch.c:244–256,391–398,490–497,635–649`). Local `_UnusedVram=0x06012C00`, so 0x5400 reaches 0x06018000. The measured full upstream runtime fits staging but **not local ROM placement reservations**. An extracted interpreter has a different, presently unmeasured size. Derive every size from assembled symbols plus actual/max record count, enforce alignment and page/sector/ROM bounds, and abort launch visibly if installation fails. A silent void return is insufficient if another launch stage has already written an entry hook.

Both implementations have a 1,024-byte runtime table. Upstream additionally budgets 512 writes per pass, 16 conditional nesting levels, 32 ROM groups, 128 condition bytes and 256 patch bytes. The ROM arrays cost 2,304 bytes before counters/alignment: 384 offsets and values (1,920 bytes) plus 32 12-byte group records (384 bytes). Upstream reuses the read cache for line/value parsing instead of local two 6,000-byte buffers; that can reduce memory, but requires a shared-buffer lifetime audit before adoption. With a 0x20000 read cache and table at 0x2000, row capacity changes from floor(122880/54)=2275 to floor(122880/56)=2194.

The interpreter saves nine registers (36 bytes) and allocates a 16-byte condition stack, versus the local three-register (12-byte) save at the cheat loop, before helper pushes. Budget the full worst-case IRQ stack and cycles, not just table bytes. A 512-write compiler cap is not a measured VBlank timing guarantee, especially for pointer reads, condition/data records, ROM wait states and a game's own IRQ handler.

The existing ELF is dated earlier than this review and is **not evidence of the current candidate's final headroom**. Its `.sbss` is 230,352 bytes; IWRAM `.iwram + .bss + .data + init/fini arrays` totals 31,488 bytes. Local Makefile sets user stack top 0x0203F000, heap end 0x0203C000 and a 2 MiB kernel limit. These make fresh final link-map and binary-size checks mandatory after any port. Do not equate aggregate ELF debug size with firmware size.

## RTS, save formats, and loader exclusions

**RTS is incompatible despite identical file length.** Both loaders accept 0x70000 bytes (458,752), but local assembly uses `EZ-OmegaRTCFILE.` (`gba_rts_patch.s:1134`, `gba_rts_only.s:536`). Upstream uses the `EZRTS3K0` header, version 3, ABI 0x138A0001, 18 region descriptors/checksums, ROM game code/size/header CRC, runtime identity and a footer. See `gba_rts3_defs.inc`, `gba_rts3_core.inc:1069–1097`, and `rts3_identity.c`. Runtime installation patches this metadata, so changes in runtime bytes/address can affect state acceptance.

Upstream `LoadRTSfile` (`ezkernelnew.c:5004`) loads fourteen 32 KiB windows with page step 0x08, invalidates the footer on page 0xA8, and publishes its first byte only after successful final close. Local loads seven 64 KiB chunks with page step 0x10 (`ezkernelnew.c:5095`). RTS3 uses live SRAM page 0x30, metadata page 0xA8, and a backed-up VRAM work/stack area 0x06017C00–0x06018000. These are a coordinated layout/protocol change, not optional cheat helpers. Do not mix either loader with the other's state engine or overwrite existing `.rts` files as an implicit migration. Any future RTS3 effort needs separate naming/version detection, old-state preservation and explicit rejection/migration policy.

RTS3 itself records untracked hardware classes (DMA state, timer reload, LCD write-only state, sound phase); its source comments describe IRQ-boundary checkpoints, not complete hardware replay. CRC success cannot prove gameplay correctness.

Battery `.sav` data is separate from RTS state. No new encrypted/container battery-save format was identified in the inspected paths; that does not make all save code interchangeable. Upstream replaces `saveMODE.h`'s struct table with a packed 2,768-entry lookup and changes its consumers. That is an independent compatibility-table project. Local `SavefileWrite` checks write status, count, close and failure cleanup; upstream's corresponding path differs. Keep local save-error handling, FAT bounds, NOR catalog transaction/status policy, and save-info persistence. No byte-for-byte equivalence claim is made for the packed game database.

Local `ResetWithEwramClear` moves to IWRAM before clearing an EWRAM user stack; local `Ezcard_OP.c:358–369` calls it for the relevant boot mode. Upstream removes that helper and hardcodes IWRAM stack addresses in `reset.s`, whereas local uses `__sp_usr` elsewhere. Importing these changes without the complete memory model risks clearing a live stack or overlapping kernel IWRAM data. Keep local reset/launch behavior.

## Concrete regression risks outside the interpreter

1. **PSRAM boundary corruption:** local `Write` splits halfword writes across 8 MiB pages and rejects odd addresses (`GBApatch.c:57–89`); upstream chooses one page and copies the entire request. Preserve local implementation. Test 8/16/24 MiB crossings and 32 MiB end placement.
2. **Trim arithmetic regressions:** upstream `SetTrimSize` restores `buffer[(romsize-1)%iSize-1]` and unsigned `bottom-16`; local has null/zero guards and safer decrement/subtraction. Port calculated sizes without reintroducing those underflow cases.
3. **Game compatibility loss:** upstream replaces built-in reset/trim/fixed patches with external profiles (`AutoPatch_*`), removes local `reset_table.h` use and special game functions, and changes `.pat` cache policy. A missing external file must not silently remove current game fixes. None of this is required for a RAM cheat interpreter.
4. **ROM cheats are a separate integration layer:** upstream evaluates ROM conditions before kernel changes and applies final ROM bytes in PSRAM (`GBApatch.c:597–619`). NOR has separate original-ROM read and sector-buffer write callbacks and application points (`NORflash_OP.c:282,376,398`). Copying only the PSRAM path would make NOR behavior inconsistent. Its PSRAM writer uses volatile byte stores: bus/write-width behavior requires validation; prefer a reviewed halfword read/modify/write adapter consistent with local patch writes. Test conflicting patches, offsets, odd bytes and page boundaries.
5. **Control flow/table corruption:** per-record filtering can separate opcode/data records; count limits alone do not preserve IF/PTR/SLIDE semantics. Test compilation and final emission together.
6. **Behavioral policy changes:** stricter RAM ranges, alignment, one-vs-multi selection, invalid-option rollback and workload caps can disable previously accepted cheats. Surface failures and preserve legacy behavior by default rather than claiming universal compatibility.
7. **Stale compiled state/cache:** new launches must reset all compiled counts/work/ROM groups, preserve headless remembered selections, and avoid reusing stale trim/patch assumptions from `.pat` or already-installed NOR games. Existing NOR images do not automatically acquire a new runtime.

## Recommended implementation boundary and order

1. Finish and record the coordinator's current candidate. Keep its production source and current legacy tests as the baseline.
2. In a later isolated change, extract the enhanced compiler into a module with bounded input/output and explicit error results. Keep the local UI, full-path identity, CHS2 handling, legacy enumeration and headless launch. Initially disable ROM commands and UI schema extensions explicitly.
3. Extract only the F1–FF interpreter, its position-independent address helper, and necessary constants into the legacy combined assembly. Preserve legacy RTS save/load/menu/reset/sleep code and `gba_rts_only.s`. Update record emission and calculated placement as one change. Do not import RTS3 includes/identity/finalization.
4. Retain a legacy default/fallback and expose enhanced execution only after passing differential tests. Rejection must occur before any hook mutation; never run malformed enhanced records through the old byte-write loop.
5. Add ROM/ROMIF later only if both PSRAM and NOR paths can be supported and tested with local transactional/error protections. Keep external compatibility profiles, packed save tables and RTS3 out of that change.

This is a feasible separation plan, not a ready cherry-pick. The current upstream commit bundles changes whose correctness depends on one another; an extracted hybrid needs its own validation.

## Executed checks and remaining gates

Executed without production edits:

- Remote exact-commit retrieval, source diff/trace, and standalone assembly of both RTS variants from both trees: passed.
- `bash scripts/test-cheat-state-host.sh`: passed under its address/undefined sanitizers; covers persistence, failures, identity, bounded selections and collapsed viewport.
- `bash scripts/test-cheat-row-host.sh`: passed; existing signedness/unused-parameter warnings were emitted.
- `bash scripts/test-savefile-host.sh`: passed. Its overwrite-failure fixture explicitly confirms the previous save becomes unavailable; this is not a claim of atomic overwrite preservation.

These tests cover the current local implementation, **not enhanced execution**. No production firmware was rebuilt, and no emulator or hardware gameplay tests were run. Upstream's `tools/inspect_rts3.py` is a state inspection utility, not proof of engine correctness.

Feasible host/assembler gates for a future port:

- Golden/differential legacy `.cht` corpus through parsing, selection, address expansion and emitted bytes; valid and malformed enhanced syntax; 0/1/128/129 records, 16/17 levels, 512/513 writes, overflow/alignment/region edges, unmatched conditions, orphan/truncated data, per-option rollback and repeated launches.
- CHS2 persistence/headless/collapse suite retained, plus any changed row enumeration/schema and same-name games in different directories; failure injection and long paths.
- Reference bytecode executor vs assembled ARM execution in an instruction emulator, covering register/SP restoration, nested disabled branches, pointers and compact operations. A host reference alone cannot verify assembly.
- Symbol-derived layout assertions, ROM tail/page/sector boundary fixtures, full-link RAM/heap/stack bounds, kernel binary cap and patch-cache invalidation tests. Run existing PSRAM/NOR/FAT/reset/save audit suites where applicable.
- Target matrix: cheats off/on, RTS off/on, sleep/reset combinations, PSRAM/NOR, all launch modes and representative RAM/IRQ-heavy games; legacy RTS load/save compatibility, maximum work and long sessions. Emulator coverage helps isolate logic, but flashcart mapping, timing, persistence across power cycles and hardware resume behavior remain physical-device gates.

**Release decision:** no-go for wholesale integration or “without regressions” claims now; conditional go for a separately scoped, measured compiler/interpreter-only prototype after the current candidate is finalized. Leave production unchanged at this review stage.
