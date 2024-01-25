cmake_minimum_required (VERSION 3.27)

# Project Name
project ("FileIndexer")

# Source files from each module
file(GLOB_RECURSE UTIL_SOURCES "src/Utils/*.c")
file(GLOB_RECURSE ARRAYS_SOURCES "src/Arrays/*.c")
file(GLOB_RECURSE INDEXER_SOURCES "src/Indexer/*.c")

set(SOURCE_FILES
    ${UTIL_SOURCES}
    ${ARRAYS_SOURCES}
    ${INDEXER_SOURCES}
    "src/main.c"
)

# Header files to include
file(GLOB_RECURSE HEADER_FILES "include/*.h")

# Make header files available globally
include_directories("include/")

# Add sources to project's executable.
add_executable (FileIndexer ${SOURCE_FILES} ${HEADER_FILES})

# Set Project C standard to C11
set_property(TARGET FileIndexer PROPERTY C_STANDARD 11)
