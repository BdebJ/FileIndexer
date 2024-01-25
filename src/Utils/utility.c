#include "utility.h"

void print_error(LPTSTR function_name)
{
    LPTSTR lpMsgBuf;
    DWORD dw = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        &lpMsgBuf,
        0, NULL);

    _tprintf(_T("%s failed with error code %d. Reason: %s\n"), function_name, dw, lpMsgBuf);
}