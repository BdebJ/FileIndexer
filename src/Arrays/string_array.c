#include "string_array.h"

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