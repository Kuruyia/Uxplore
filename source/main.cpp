/*
    Uxplore
    Copyright (C) 2019-2020, Kuruyia

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <coreinit/ios.h>
#include <coreinit/thread.h>

#include <whb/log_cafe.h>
#include <whb/log_udp.h>
#include <whb/log_console.h>
#include <whb/log.h>
#include <whb/proc.h>

#include <iosuhax.h>
#include <romfs-wiiu.h>

#include "Application.h"
#include "Macros.h"

// Taken from dynamic_libs
#define BUS_SPEED                       248625000
#define SECS_TO_TICKS(sec)              (((unsigned long long)(sec)) * (BUS_SPEED/4))
#define MICROSECS_TO_TICKS(usec)        (SECS_TO_TICKS(usec) / 1000000)
#define usleep(usecs) OSSleepTicks(MICROSECS_TO_TICKS(usecs))

void someFunc(UNUSED_PARAM IOSError err, void* arg)
{
    (void) arg;
}

static int mcp_hook_fd = -1;

int MCPHookOpen()
{
    //take over mcp thread
    mcp_hook_fd = IOS_Open("/dev/mcp", (IOSOpenMode) 0);

    if (mcp_hook_fd < 0)
        return -1;

    IOS_IoctlAsync(mcp_hook_fd, 0x62, (void*) 0, 0, (void*) 0, 0, (IOSAsyncCallbackFn) &someFunc, (void*) 0);
    //let wupserver start up
    usleep(1000);

    if (IOSUHAX_Open("/dev/mcp") < 0) {
        IOS_Close(mcp_hook_fd);
        mcp_hook_fd = -1;
        return -1;
    }

    return 0;
}

void MCPHookClose()
{
    if (mcp_hook_fd < 0)
        return;

    //close down wupserver, return control to mcp
    IOSUHAX_Close();
    //wait for mcp to return
    usleep(1000);
    IOS_Close(mcp_hook_fd);
    mcp_hook_fd = -1;
}

enum AccessLevel {
    ACCESS_LEVEL_FULL,
    ACCESS_LEVEL_SD,
};
static enum AccessLevel g_accessLevel = ACCESS_LEVEL_FULL;

int main()
{
    // Init everything here
    WHBLogCafeInit();
    WHBLogUdpInit();

    romfsInit();

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO);
    TTF_Init();

    // Try to have an access to IOSUHAX
    int res = IOSUHAX_Open(nullptr);
    if (res < 0) {
        res = MCPHookOpen();
    }
    if (res < 0) {
        // Unable to get access, falling back on SD card access only
        g_accessLevel = ACCESS_LEVEL_SD;
    }

    WHBLogPrint("Uxplore started");

    auto *app = new Application();

    Uint32 lastTime, nowTime, renderTime;
    lastTime = nowTime = SDL_GetTicks();

    while (WHBProcIsRunning()) {
        app->render((nowTime - lastTime) / 1000.f);

        renderTime = SDL_GetTicks() - nowTime;
        lastTime = nowTime;

        //WHBLogPrintf("Rendering took %ims", renderTime);

        int delay = 1000 / 60 - renderTime;
        SDL_Delay(delay > 0 ? delay : 0);
        nowTime = SDL_GetTicks();
    }

    delete app;

    WHBLogPrint("Stop requested");

    // Deinit everything
    if (mcp_hook_fd >= 0)
        MCPHookClose();
    else
        IOSUHAX_Close();

    SDL_Quit();
    TTF_Quit();

    WHBLogPrint("After close");

    romfsExit();

    WHBLogCafeDeinit();
    WHBLogUdpDeinit();

    return EXIT_SUCCESS;
}
