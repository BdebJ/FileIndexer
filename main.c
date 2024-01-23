#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>

#define INITIAL_CAPACITY 1000

typedef struct
{
    TCHAR* data;
} String;

typedef struct
{
    String* elements;
    size_t size;
    size_t capacity;
} StringArray;

DWORD index_directory(TCHAR[], int);
void print_error(LPTSTR);

void StringArray_initialize(StringArray*);
void StringArray_resize(StringArray*);
void StringArray_add(StringArray*, const TCHAR*);
void StringArray_free(StringArray*);

void StringArray_initialize(StringArray* array)
{
    array->elements = (String*)malloc(INITIAL_CAPACITY * sizeof(String));
    array->size = 0;
    array->capacity = INITIAL_CAPACITY;
}

void StringArray_resize(StringArray* array)
{
    array->capacity *= 2;
    array->elements = (String*)realloc(array->elements, array->capacity * sizeof(String));
}

void StringArray_add(StringArray* array, const TCHAR* element)
{
    if (array->size == array->capacity)
    {
        StringArray_resize(array);
    }

    array->elements[array->size].data = _tcsdup(element);
    array->size++;
}

void StringArray_free(StringArray* array)
{
    for (size_t i = 0; i < array->size; i++)
    {
        free(array->elements[i].data);
    }

    free(array->elements);
}

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

DWORD index_directory(TCHAR directory_path[], int width_padding)
{
    WIN32_FIND_DATA ffd;
    LARGE_INTEGER filesize;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    DWORD dwError = 0;

    // Create a local copy of the directory path
    TCHAR directory_path_copy[MAX_PATH];
    if (FAILED(StringCchCopy(directory_path_copy, MAX_PATH, directory_path)))
    {
        // Handle copy failure
        _tprintf(TEXT("Copying directory_path failed\n"));
        return ERROR_INVALID_PARAMETER; // Or another appropriate error code
    }

    // Appending '\*' to the local directory name copy
    StringCchCat(directory_path_copy, MAX_PATH, TEXT("\\*"));

    hFind = FindFirstFile(directory_path_copy, &ffd);

    if (INVALID_HANDLE_VALUE == hFind)
    {
        dwError = GetLastError();
        print_error(TEXT("FindFirstFile"));
        return dwError;
    }

    do
    {
        if (_tcscmp(ffd.cFileName, TEXT(".")) == 0 || _tcscmp(ffd.cFileName, TEXT("..")) == 0)
        {
            continue;
        }
        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            _tprintf(TEXT("%*s%s   <DIR>\n"), width_padding, TEXT(""), ffd.cFileName);

            // Recurse 
            TCHAR subdirectory_path[MAX_PATH];
            StringCchCopy(subdirectory_path, MAX_PATH, directory_path);
            StringCchCat(subdirectory_path, MAX_PATH, TEXT("\\"));
            StringCchCat(subdirectory_path, MAX_PATH, ffd.cFileName);

            dwError = index_directory(subdirectory_path, width_padding + 4);
            if (dwError != ERROR_SUCCESS)
            {
                return dwError;
            }
        }
        else
        {
            filesize.LowPart = ffd.nFileSizeLow;
            filesize.HighPart = ffd.nFileSizeHigh;
            _tprintf(TEXT("%*s%s   %lld bytes\n"), width_padding, TEXT(""), ffd.cFileName, filesize.QuadPart);
        }
    } while (FindNextFile(hFind, &ffd) != 0);

    dwError = GetLastError();
    if (dwError != ERROR_NO_MORE_FILES)
    {
        print_error(TEXT("FindFirstFile"));
        return dwError;
    }

    FindClose(hFind);
    return ERROR_SUCCESS;
}



int _tmain(int argc, TCHAR* argv[])
{
    DWORD start_time = GetTickCount();
    TCHAR directory_path[MAX_PATH];
    size_t length_of_arg;
    DWORD dwError = 0;

    if (argc != 2)
    {
        _tprintf(TEXT("\nUsage: %s <directory name>\n"), argv[0]);
        return (-1);
    }

    // Check that the input path + 3 is not longer than MAX_PATH. Three characters are for the "\*" plus NULL appended below.
    StringCchLength(argv[1], MAX_PATH, &length_of_arg);
    if (length_of_arg > (MAX_PATH - 3))
    {
        _tprintf(TEXT("\nDirectory path is too long.\n"));
        return (-1);
    }

    _tprintf(TEXT("\nTarget directory is %s\n\n"), argv[1]);
    StringCchCopy(directory_path, MAX_PATH, argv[1]);


    
    dwError = index_directory(directory_path, 0);
    DWORD elapsed_time = GetTickCount() - start_time;
    _tprintf(TEXT("Time elapsed: %u milliseconds\n"), elapsed_time);
    return dwError;
}