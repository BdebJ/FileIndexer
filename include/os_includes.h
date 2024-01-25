// os_includes.h
#ifndef OS_INCLUDES_H
#define OS_INCLUDES_H

// Common includes
#include <stdio.h>

// Windows specific
#ifdef _WIN64

#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>

#endif // _WIN64

// Linux specific
#ifdef __linux__

#include <dirent.h>

#endif // __linux__

#endif // OS_INCLUDES_H