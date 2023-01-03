protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS messages.proto)

add_library(ProtoMessages STATIC ${PROTO_SRCS} ${PROTO_HDRS})
