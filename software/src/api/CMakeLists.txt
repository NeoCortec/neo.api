set(MODULE_NAME NcApi)

add_library(${MODULE_NAME} 
    NcApi.c
    NeoParser.c
)
add_library(NcApi::${MODULE_NAME} ALIAS ${MODULE_NAME})

target_include_directories(${MODULE_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})