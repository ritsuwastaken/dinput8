# dinput8 for L2 · ![build status](https://github.com/ritsuwastaken/dinput8/actions/workflows/build.yml/badge.svg)
A proxy library for L2, compatible with [DxWrapper](https://github.com/elishacloud/dxwrapper) and [ReShade](https://github.com/crosire/reshade).
- Blocks API calls to [SetDeviceGammaRamp](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-setdevicegammaramp) from `d3ddrv.dll` to prevent changes to the system brightness.  
- Skips `The game may not be consistent because AGP is deactivated. Please activate AGP for consistancy.` warning.
> ❗ You won't be able to adjust the brightness via the in-game menu or `l2.ini`. Use [ReShade](https://reshade.me/) instead.
> #### ReShade settings   
> Select `Levels.fx` (installed by default), set **White point** to your preference, e.g. 140-160.

## Download
Get the latest version from [Releases](https://github.com/ritsuwastaken/dinput8/releases).

## Usage
Put `dinput8.dll` into the `system` folder of L2.  
If the file is already in use (e.g. by L2HUB), you can rename `dinput8.dll` from this project to `version.dll`.

## Build
- [cmake](https://cmake.org/download/) >= 3.11
- [VS Build Tools](https://visualstudio.microsoft.com/downloads/)

```shell
# Configure the project
cmake . -B 'build' -G 'Visual Studio 17 2022' -A Win32
# Build the library to ./build/Release/dinput8.dll
cmake --build 'build' --config Release
```
