# dinput8 for L2 · ![build status](https://github.com/ritsuwastaken/dinput8/actions/workflows/build.yml/badge.svg)
This is a proxy library that fixes a few issues in older L2 clients
- Blocks API calls to [SetDeviceGammaRamp](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-setdevicegammaramp) from `d3ddrv.dll` to prevent changes to the system brightness.  
- Skips `The game may not be consistent because AGP is deactivated. Please activate AGP for consistancy.` warning.
>❗ You won't be able to set the brightness with the in-game menu nor via `l2.ini` editing. Use [ReShade](https://reshade.me/) instead.

## Download
Get the latest version from [Releases](https://github.com/ritsuwastaken/dinput8/releases).

## Usage
Put into `system` folder of L2.

## Build
- [cmake](https://cmake.org/download/) >= 3.11
- [VS Build Tools](https://visualstudio.microsoft.com/downloads/)

```shell
# Configure the project
cmake . -B 'build' -G 'Visual Studio 17 2022' -A Win32
# Build the library to ./build/Release/dinput8.dll
cmake --build 'build' --config Release
```
