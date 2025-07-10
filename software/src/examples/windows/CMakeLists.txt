set(MODULE_NAME NcApiWindows)

add_executable(${MODULE_NAME} ../main.c WindowsUart.cpp my_ncapi_hal.cpp ) 
target_link_libraries(${MODULE_NAME} NcApi NcApiExampleCommon -static-libstdc++ -static-libgcc -pthread)