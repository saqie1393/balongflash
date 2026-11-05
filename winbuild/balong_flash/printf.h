#ifndef WIN_PRINTF_H
#define WIN_PRINTF_H

#include <stdio.h>
#include <stdarg.h>
#include <windows.h>

static int win_printf(const char* format, ...)
{
    static char ostr[2048];
    static wchar_t wstr[2048];
    va_list args;

    va_start(args, format);
    vsprintf(ostr, format, args);
    va_end(args);
    MultiByteToWideChar(CP_UTF8, 0, ostr, -1, wstr, 2048);
    WideCharToMultiByte(CP_OEMCP, 0, wstr, -1, ostr, 2048, NULL, NULL);
    return printf_s("%s", ostr);
}

#define printf win_printf

#endif
