add_library(net-common STATIC NetCommon.cpp)
target_include_directories(net-common PUBLIC .)
target_link_libraries(net-common PUBLIC asio)
