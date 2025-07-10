set(MODULE_NAME NcApiExampleCommon)

add_library(${MODULE_NAME} 
    NcApiExample.c
    NcApiSupportExample.c
)
add_library(NcApi::${MODULE_NAME} ALIAS ${MODULE_NAME})

target_include_directories(${MODULE_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${MODULE_NAME} NcApi)
