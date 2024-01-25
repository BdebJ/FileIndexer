// string_array.h
#ifndef STRING_ARRAY_H
#define STRING_ARRAY_H

#include "os_includes.h"

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

void StringArray_initialize(StringArray*);
void StringArray_resize(StringArray*);
void StringArray_add(StringArray*, const TCHAR*);
void StringArray_free(StringArray*);

#endif // STRING_ARRAY_H