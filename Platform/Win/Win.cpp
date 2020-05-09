#include "Platform/Win/Win.h"

#include <cstdio>
#include <string>
#include <vector>

#include "src/Application/Game.h"

#include "Platform/Api.h"

#pragma comment(lib, "winmm.lib")

void OS_MsgBox(const char *msg, const char *title) {
    MessageBoxA(nullptr, msg, title, 0);
}

unsigned int OS_GetTime() { return GetTickCount(); }

unsigned __int64 OS_GetPrecisionTime() { return timeGetTime(); }

void OS_ShowCursor(bool show) { ShowCursor(show ? 1 : 0); }

void OS_Sleep(int ms) { Sleep(ms); }

bool OS_OpenConsole() {
    if (AllocConsole()) {
        freopen("conin$", "r", stdin);
        freopen("conout$", "w", stdout);
        freopen("conout$", "w", stderr);

        return true;
    }

    return false;
}

std::vector<std::string> OS_FindFiles(const std::string &folder, const std::string &mask) {
    std::vector<std::string> result;

    std::string path = folder + "\\" + mask;

    WIN32_FIND_DATAA ffd = { 0 };
    HANDLE hFind = FindFirstFileA(path.c_str(), &ffd);
    if (INVALID_HANDLE_VALUE == hFind) {
        return result;
    }

    do {
        if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
            result.push_back(ffd.cFileName);
        }
    } while (FindNextFileA(hFind, &ffd) != 0);

    FindClose(hFind);

    return result;
}


FILE *fcaseopen(char const *path, char const *mode) {
    return fopen(path, mode);
}

std::string OS_GetDirSeparator() {
    return "/";
}

std::string OS_FindMm7Directory() {
    // env variable override to a custom folder
    if (const char *path = std::getenv("WOMM_PATH_OVERRIDE")) {
        return path;
    }

    // standard 1.0 installation
    char path[2048];
    if (OS_GetAppString("HKEY_LOCAL_MACHINE/SOFTWARE/New World Computing/Might and Magic VII/1.0/AppPath",
                        path, sizeof(path))) {
        return path;
    }

    // GoG old version
    if (OS_GetAppString("HKEY_LOCAL_MACHINE/SOFTWARE/GOG.com/GOGMM7/PATH", path, sizeof(path))) {
        return path;
    }

    // GoG new version ( 2018 builds )
    if (OS_GetAppString("HKEY_LOCAL_MACHINE/SOFTWARE/WOW6432Node/GOG.com/Games/1207658916/Path",
                        path, sizeof(path))) {
        return path;
    }

    // Hack path fix - if everything else fails, set your path here.
    return "E:\\Programs\\GOG Galaxy\\Games\\Might and Magic 7";
}
