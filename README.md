# Name Overflow Fix (Kyubu port)

> This is a **port to Kyubu** of the original mod [`coremaze/Cube-World-Name-Overflow-Fix`](https://github.com/coremaze/Cube-World-Name-Overflow-Fix).
> Original author: **coremaze**. This port is not an official release by the author.

**What it does:** Fixes a buffer overflow in the `/name` command with names that are too long.

**Original:** <https://github.com/coremaze/Cube-World-Name-Overflow-Fix>

**Release of the original this was ported from:** <https://github.com/coremaze/Cube-World-Name-Overflow-Fix/releases/tag/v1.0>

**Side:** client (`sides` in `mod.toml`).

## Installation

Requirements: **Cube World Alpha** (the 2013-07-20 `Cube.exe`, SHA-256 `84a7a132...30bf`, unpatched) and **Kyubu** (the mod loader, <https://github.com/kyubumods/kyubu>; this mod needs `kyubu = "^1.1"`, see `mod.toml`).

1. Install Kyubu with its own installer (`install\install.cmd`). The loader is not part of this mod.
2. Put **`name_overflow_fix.zip`** (do not unpack it) into the **`Mods\`** folder next to `Cube.exe`. If `name_overflow_fix.zip` is already there, replace it.
3. Start the game. `kyubu\logs\latest.log` should have a line saying `name_overflow_fix` loaded.
4. To turn the mod off or remove it, take `name_overflow_fix.zip` out of `Mods\` (or switch it off in the mod manager: **Mods** on the title screen).

The old Cube World Mod Launcher is **not needed** for this port and is not compatible with it: do not put the original mod's DLL next to it.


Mod id: `name_overflow_fix`. Source: `Mods\name_overflow_fix\src\main.cpp`.
