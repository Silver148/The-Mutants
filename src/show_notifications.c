#include "show_notifications.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <windows.h>
#include <shellapi.h>

void ShowWindowsNotification(SDL_Window* window, const char* title, const char* message) {
#ifdef _WIN32
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);

    if (SDL_GetWindowWMInfo(window, &wmInfo)) {
        HWND hwnd = wmInfo.info.win.window;

        NOTIFYICONDATA nid = { sizeof(NOTIFYICONDATA) };
        nid.hWnd = hwnd;
        nid.uID = 100;
        nid.uFlags = NIF_INFO | NIF_ICON | NIF_TIP;
        nid.dwInfoFlags = NIIF_INFO;
        
        strncpy(nid.szInfoTitle, title, sizeof(nid.szInfoTitle) - 1);
        strncpy(nid.szInfo, message, sizeof(nid.szInfo) - 1);
        strncpy(nid.szTip, "The Mutant's", sizeof(nid.szTip) - 1);

        Shell_NotifyIcon(NIM_ADD, &nid);
    }
#endif
}