<div align="center">
  <h1 style="margin-bottom:0">Resident Evil 2 Dual Shock Ver. (1998/SLUS_007.48/Leon) Disassembly/Decompilation</h1>
</div>

## About
- The entire point of this repo is to provide an educational resource that actually utilizes the real symbology that was created by the original developers. You will see romaji (for example "Kage" translates to "Shadow") and oddities such as the character Ada being internally referred to as "Aida" in code. This repo does not and will never contain any original source code, SDKs or runtime assets
- Almost all symbols were manually extracted from a [prerelease beta disc](https://tcrf.net/Proto:Resident_Evil_2_(PlayStation)/October_31,_1997_Prototype), dated October 31, 1997 (file: *MAIN.SYM*). The retail versions include some new functions and overlays, so some minor creative liberties had to be taken, but *only* for functions/structs/etc that did not have a symbol match to the beta build. During the reverse engineering process, the original developer's naming conventions were always strictly followed when forced to innovate. All symbols were manually mapped into IDA Pro around the year 2010, then eventually into Ghidra when that became available
- Every Resident Evil (aka BioHazard) game that initially launched on the Sony PlayStation (1994), excluding Gun Survivor, is an iteration of the same engine. The symbol file has been used as a base for mapping many other games over the years (RE1, 1.5, 2 and 3) across multiple platforms (PS1, GC, DC, PC, etc) and spawned many personal projects focused on RE modification. Some symbols were even retrieved from the retail DreamCast version of BioHazard 3 Last Escape, where uncompiled room scripts can be found. Everything was done with love, gradually over the course of many years when I found the free time and interest to do so. Some source files that I created for [other](https://github.com/MeganGrass/bioclone-remake) [projects](https://github.com/MeganGrass/Resident-Evil-2-Starter-Kit) and [other platforms](https://github.com/MeganGrass/RE2-Mod-SDK-Ext) are used in this repo
- I've completely lost the time required to manually export the disassembly by hand into something tangible for others. I finally used GhidraMCP with Claude (Haiku/Ultracode), asked it to export the disassembly to C, spent a few hours making sure everything was correct and matching my flawed interpretation of the Ghidra disassembly database. It's not perfect. I wasn't able to fully map only a couple structures before this, and my documentation request didn't turn out stellar at all, but the code is human-readable with all ported symbols and fairly easy to follow. With proper if/defs and some more work, the code in this repo could probably be used to conditionally compile the disc 2 executable (SLUS_007.56) and others (the beta, trial/demo editions, etc.)
- All common overlays are included, but all of the enemy overlays are missing and may be provided at a later time

## Build
- Sony PlayStation SDK (aka PSY-Q) is required
- make and linker files are provided (untested)

## Copyright Notice
Sony PlayStation is Copyright (©) [Sony Computer Entertainment Inc.](https://sonyinteractive.com/)

All rights, including the copyrights of game, scenario, music and program are reserved by [CAPCOM Co., Ltd.](https://www.capcom.com/)