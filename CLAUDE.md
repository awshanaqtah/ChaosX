# ChaosX Engine

A Windows C++20 game engine. It builds as a DLL (`ChaosX`) consumed by a console app
(`Sandbox`). Engine symbols cross the DLL boundary via the `ChaosX_API` macro, and the
project uses the `CS_` / `ChaosX` naming convention throughout.

## Layout

```
premake5.lua              Build script — SOURCE OF TRUTH for all project config
GeneratePro.bat           Runs Vendor\premake5.exe vs2022 to regenerate VS projects
Vendor/premake5.exe       Vendored build tool (do not edit)
ChaosX/                   Engine DLL
  Src/cspch.h / .cpp      Precompiled header (STL + Windows.h)
  Src/ChaosX.h            Umbrella header clients include
  Src/ChaosX/             Core, Application, Log, Entry Point, Event/
  Lib/spdlog/             Vendored logging submodule (do not edit)
Sandbox/Src/Sandbox.cpp   Client app: subclasses Application, defines CreateApplication()
bin/ , bin-int/           Build output (gitignored)
```

## Build system — premake is the source of truth

- **`premake5.lua` at the repo root is authoritative.** Change build settings there.
- **Generated `.sln` / `.vcxproj` / `.vcxproj.filters` are gitignored — NEVER hand-edit
  them.** Manual edits in the VS property pages get silently wiped on the next regen.
  (A real `/utf-8` setting was lost exactly this way.) Regenerate instead.
- Regenerate projects with `GeneratePro.bat` (or `Vendor\premake5.exe vs2022`). The build
  script's paths are root-relative, so premake must run from the repo root.

### Toolchain specifics (this machine)
- Visual Studio 2026 (v18) → MSVC 14.50 → PlatformToolset **`v145`**.
- premake 5.0-beta7's newest action is `vs2022`, which defaults to **`v143`** — so
  `premake5.lua` pins `toolset "msc-v145"` to match the installed toolset.
- spdlog's bundled fmt static-asserts on `/utf-8`, so `premake5.lua` sets
  `buildoptions { "/utf-8" }` on both projects.
- The post-build DLL copy uses a relative `../bin/...` path: `%{wks.location}` expands to
  an absolute path containing a space ("ChaosX Engine") that the unquoted `{MKDIR}`/`{COPY}`
  tokens break on.

## Building

`Sandbox` links `ChaosX`, so building Sandbox (or the solution) builds the DLL first
automatically — no manual ordering needed.

msbuild is not on PATH; it needs the VS developer environment:

```
cmd /c '"C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" && msbuild "ChaosX.sln" -t:Rebuild -p:Configuration=Debug -p:Platform=x64'
```

A `vswhere.exe ... not recognized` line printed when invoking cmd from PowerShell is
harmless noise; vcvars still initializes.

## Running

Run **`Sandbox.exe`** (not the DLL):
`bin\Debug-windows-x86_64\Sandbox\Sandbox.exe` — `ChaosX.dll` is copied next to it by the
post-build step.

⚠️ `Application::Run()` currently ends in `while (true);` (placeholder for the future
window/event loop). The app prints its startup log lines, then hangs intentionally — it is
not crashing. To verify output programmatically: start it, capture stdout for ~2s, then
kill the process.

## Conventions

- Everything lives in `namespace ChaosX`.
- **Every public/exported class is tagged `ChaosX_API`** (defined in `Core.h` as
  dllexport when `CS_BUILD_DLL` is set, else dllimport). A new public class without it
  won't cross the DLL boundary. Watch for typos (`ChoasX` ≠ `ChaosX`).
- Prefer **exporting functions, not data**: `Log` exports its accessor functions, not the
  class, keeping `std::shared_ptr` members internal (avoids C4251).
- Query methods are `const` (const-correct interfaces).
- Logging via macros: `CS_CORE_*` (engine logger "ChaosX") and `CS_*` (client logger
  "App"). Events implement `ToString()` and the `EVENT_CLASS_TYPE/CATEGORY` macros.
- `cspch.h` is the precompiled header; engine `.cpp` files include it first.

## Do not touch

- `ChaosX/Lib/spdlog/**` — vendored submodule.
- `Vendor/**` — vendored premake tool.
- Generated `*.sln` / `*.vcxproj*` — regenerate via premake instead.
