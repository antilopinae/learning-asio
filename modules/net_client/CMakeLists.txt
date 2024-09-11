add_library(net-client STATIC SimpleClient.cpp)
target_include_directories(net-client PUBLIC .)
target_link_libraries(net-client PRIVATE net-common)
