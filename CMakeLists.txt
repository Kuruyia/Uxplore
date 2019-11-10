cmake_minimum_required(VERSION 3.12)
add_definitions(-D__wiiu__)
set(CMAKE_TOOLCHAIN_FILE $ENV{WUT_ROOT}/share/wut.toolchain.cmake)

project(uxplore_cpp)

# Include stuff from the devkitPro folder
include("$ENV{WUT_ROOT}/share/wut.cmake" REQUIRED)
include("${DEVKITPRO}/portlibs/wiiu/share/romfs-wiiu.cmake" REQUIRED)

include_directories($ENV{DEVKITPRO}/portlibs/ppc/include
        $ENV{DEVKITPRO}/portlibs/wiiu/include)
link_directories($ENV{DEVKITPRO}/portlibs/ppc/lib
        $ENV{DEVKITPRO}/portlibs/wiiu/lib)

# Add libraries from the external/ folder
add_subdirectory(external/universal-tween-engine)
include_directories(external/universal-tween-engine/include)

add_subdirectory(external/libiosuhax)
include_directories(external/libiosuhax/source)

add_subdirectory(external/libfat)
include_directories(external/libfat/include)

# Add files from this project
include_directories(source)
add_executable(uxplore_cpp
        source/main.cpp
        source/Application.cpp
        source/BrowserList.cpp
        source/BrowseSession.cpp
        source/File.cpp
        source/ImageCache.cpp
        source/LocalFile.cpp
        source/NavigationHistory.cpp
        source/RenderKit.cpp
        source/TweenObjects.hpp
        source/Device/PhysicalDevice.cpp
        source/Device/PhysicalDevice/PhyDeviceManager.cpp
        source/Device/PhysicalDevice/PhyDeviceUtils.cpp
        source/DiscInterface/DiscInterface.cpp
        source/DiscInterface/DiscInterfaceWrapper.cpp
        source/Entry/EntryPhyDevice.cpp
        source/Entry/EntryFile.cpp
        source/GUI/BrowserItem.cpp
        source/GUI/Drawable.cpp
        source/GUI/Text.cpp
        source/Overlay/Browser.cpp
        source/Overlay/Dialog.cpp
        source/Device/PhysicalDevice/PartitionTableReader.cpp
        )

romfs_add(uxplore_cpp "romfs")

target_link_libraries(uxplore_cpp
        coreinit
        vpad
        sndcore2
        nlibcurl
        whb
        gfd
        gx2
        proc_ui
        nsysnet
        sysapp
        SDL2
        SDL2_gfx
        SDL2_image
        SDL2_mixer
        SDL2_ttf
        freetype
        png
        mpg123
        vorbisidec
        jpeg
        z
        bz2
        iosuhax
        fat
        tween)

# Get our .rpx
wut_create_rpx(uxplore.rpx uxplore_cpp)