# Uxplore
A file explorer for the Wii U, it lets you browse all the files availables on your Wii U, as well as on supported external drives.

## Usage instructions
_TODO: Write this_

## Build instructions
At this time, only Linux and macOS are supported. To build in Windows, you must use WSL.

This project depends on the [wut](https://github.com/devkitPro/wut) toolkit, which must be installed before being able to build.  
You might want to [read this](https://github.com/yawut/ProgrammingOnTheU/blob/master/tutorial/Chapter%201.md#setting-up-the-build-environment) for a clear explanation on how to do so.

#### First time
Once ``(dkp-)pacman`` has been installed on your system, you must install a few libraries to be able to build Uxplore (Note that you will need to have configured the [fling](https://gitlab.com/QuarkTheAwesome/wiiu-fling/tree/master#installing) repository):  
```shell
(dkp-)pacman -Sy wiiu-libromfs wiiu-sdl2 wiiu-sdl2_gfx wiiu-sdl2_image wiiu-sdl2_mixer wiiu-sdl2_ttf
```
You are now ready clone this repository :
```
git clone --recursive https://github.com/Arc13/Uxplore.git
```

#### Building
```shell
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

## Acknowlegments
- The [wut](https://github.com/devkitPro/wut) development team for providing the toolkit
- [SDL2](https://github.com/yawut/SDL) maintainers, and the Wii U version maintainers
- UI icons by [Icons8](https://icons8.com/)
- [libiosuhax](https://github.com/Arc13/libiosuhax)
- [libfat](https://github.com/Arc13/libfat)
- [universal-tween-engine](https://github.com/Arc13/universal-tween-engine-cpp)
- [libromfs](https://github.com/yawut/libromfs-wiiu)
- libpng
- mpg123
- libvorbisidec
- libjpeg-turbo
- zlib
- bzip2
