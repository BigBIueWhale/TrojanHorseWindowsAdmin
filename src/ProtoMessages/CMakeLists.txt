add_library(ProtoMessages STATIC messages.pb.cc messages.pb.h CommandHash.cpp CommandHash.hpp)
target_link_libraries(ProtoMessages PRIVATE ${Protobuf_LIBRARY} cryptb)
