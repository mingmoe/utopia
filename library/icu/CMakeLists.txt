
project(ICU LANGUAGES CXX C)

file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/icu/icu4c/source/common/sources.txt" COMMON_RAW_SOURCES 
ENCODING "UTF-8")
file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/icu/icu4c/source/i18n/sources.txt" I18N_RAW_SOURCES
ENCODING "UTF-8")
file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/icu/icu4c/source/stubdata/sources.txt" STUBDATA_RAW_SOURCES
ENCODING "UTF-8")

foreach(S IN LISTS COMMON_RAW_SOURCES)
    list(APPEND COMMON_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/icu/icu4c/source/common/${S}")
endforeach()
foreach(S IN LISTS I18N_RAW_SOURCES)
    list(APPEND I18N_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/icu/icu4c/source/i18n/${S}")
endforeach()
foreach(S IN LISTS STUBDATA_RAW_SOURCES)
    list(APPEND STUBDATA_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/icu/icu4c/source/stubdata/${S}")
endforeach()

add_library(icu SHARED ${COMMON_SOURCES} ${I18N_SOURCES} ${STUBDATA_SOURCES})

# compiler options depends on root
# we only set those we need
target_compile_definitions("icu" PUBLIC "U_COMBINED_IMPLEMENTATION=1")
target_compile_definitions("icu" PUBLIC "U_COMMON_IMPLEMENTATION=1")
target_compile_definitions("icu" PUBLIC "U_I18N_IMPLEMENTATION=1")
target_compile_definitions("icu" INTERFACE "U_HIDE_INTERNAL_API=1")
target_compile_definitions("icu" INTERFACE "U_HIDE_DEPRECATED_API=1")
target_include_directories("icu" SYSTEM PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/icu/icu4c/source/common/")
target_include_directories("icu" SYSTEM PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/icu/icu4c/source/i18n/")
target_include_directories("icu" SYSTEM PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/icu/icu4c/source/stubdata/")

target_compile_options(icu PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W1>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-W>
)
