#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#include <sys/utsname.h>
#endif
#include <stdlib.h>

int scalanative_platform_is_linux() {
#ifdef __linux__
    return 1;
#else
    return 0;
#endif
}

int scalanative_platform_is_mac() {
#ifdef __APPLE__
    return 1;
#else
    return 0;
#endif
}

int scalanative_platform_is_windows() {
#ifdef _WIN32
    return 1;
#else
    return 0;
#endif
}

char *scalanative_windows_get_user_lang() {
#ifdef _WIN32
    char *dest = malloc(9);
    GetLocaleInfoA(LOCALE_USER_DEFAULT, LOCALE_SISO639LANGNAME, dest, 9);
    return dest;
#endif
    return "";
}

char *scalanative_windows_get_user_country() {
#ifdef _WIN32
    char *dest = malloc(9);
    GetLocaleInfoA(LOCALE_USER_DEFAULT, LOCALE_SISO3166CTRYNAME, dest, 9);
    return dest;
#endif
    return "";
}

// See http://stackoverflow.com/a/4181991
int scalanative_little_endian() {
    int n = 1;
    return (*(char *)&n);
}

void scalanative_set_os_props(void (*add_prop)(const char *, const char *)) {
#ifdef _WIN32
    add_prop("os.name", "Windows (Unknown version)");
    wchar_t wcharPath[MAX_PATH];
    size_t size = sizeof(wchar_t) * wcslen(wcharPath);
    char path[MAX_PATH];
    if (GetTempPathW(MAX_PATH, wcharPath) &&
        wcstombs_s(&size, path, MAX_PATH, wcharPath, MAX_PATH - 1) == 0 &&
        size > 0) {
        add_prop("java.io.tmpdir", (const char *)path);
    }
#else
#ifdef __APPLE__
    add_prop("os.name", "Mac OS X");
#else
    struct utsname name;
    if (uname(&name) == 0) {
        add_prop("os.name", name.sysname);
        add_prop("os.version", name.release);
    }
#endif
    add_prop("java.io.tmpdir", "/tmp");
#endif
}
