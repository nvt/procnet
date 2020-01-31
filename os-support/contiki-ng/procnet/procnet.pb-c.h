/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: procnet.proto */

#ifndef PROTOBUF_C_procnet_2eproto__INCLUDED
#define PROTOBUF_C_procnet_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1000000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1003001 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct _Hello Hello;
typedef struct _Config Config;
typedef struct _Buf Buf;


/* --- enums --- */

typedef enum _Hello__SystemType {
  HELLO__SYSTEM_TYPE__CONTROLLER = 1,
  HELLO__SYSTEM_TYPE__FORWARDER = 2,
  HELLO__SYSTEM_TYPE__NODE = 3
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(HELLO__SYSTEM_TYPE)
} Hello__SystemType;
typedef enum _Config__Synchronization {
  CONFIG__SYNCHRONIZATION__DETERMINISTIC = 1,
  CONFIG__SYNCHRONIZATION__SEMIDETERMINISTIC = 2,
  CONFIG__SYNCHRONIZATION__NONDETERMINISTIC = 3
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(CONFIG__SYNCHRONIZATION)
} Config__Synchronization;
typedef enum _Buf__BufType {
  BUF__BUF_TYPE__BUF_PACKET = 1,
  BUF__BUF_TYPE__BUF_ACK = 2,
  BUF__BUF_TYPE__BUF_STDIN = 3,
  BUF__BUF_TYPE__BUF_STDOUT = 4,
  BUF__BUF_TYPE__BUF_STDERR = 5
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(BUF__BUF_TYPE)
} Buf__BufType;

/* --- messages --- */

struct  _Hello
{
  ProtobufCMessage base;
  Hello__SystemType system_type;
  char *auth_token;
  char *system_name;
  char *system_version;
  protobuf_c_boolean has_node_count;
  uint32_t node_count;
};
#define HELLO__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&hello__descriptor) \
    , HELLO__SYSTEM_TYPE__CONTROLLER, NULL, NULL, NULL, 0, 0 }


struct  _Config
{
  ProtobufCMessage base;
  uint32_t node_id;
  Config__Synchronization sync;
  uint32_t time_granularity;
  protobuf_c_boolean has_tick_time;
  uint32_t tick_time;
  protobuf_c_boolean has_ipv6_prefix;
  ProtobufCBinaryData ipv6_prefix;
};
#define CONFIG__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&config__descriptor) \
    , 0, CONFIG__SYNCHRONIZATION__DETERMINISTIC, 0, 0, 0, 0, {0,NULL} }


struct  _Buf
{
  ProtobufCMessage base;
  Buf__BufType type;
  ProtobufCBinaryData data;
  protobuf_c_boolean has_jiffies;
  uint64_t jiffies;
  protobuf_c_boolean has_packet_id;
  uint64_t packet_id;
  protobuf_c_boolean has_rssi;
  int32_t rssi;
  protobuf_c_boolean has_channel;
  uint32_t channel;
};
#define BUF__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&buf__descriptor) \
    , BUF__BUF_TYPE__BUF_PACKET, {0,NULL}, 0, 0, 0, 0, 0, 0, 0, 0 }


/* Hello methods */
void   hello__init
                     (Hello         *message);
size_t hello__get_packed_size
                     (const Hello   *message);
size_t hello__pack
                     (const Hello   *message,
                      uint8_t             *out);
size_t hello__pack_to_buffer
                     (const Hello   *message,
                      ProtobufCBuffer     *buffer);
Hello *
       hello__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   hello__free_unpacked
                     (Hello *message,
                      ProtobufCAllocator *allocator);
/* Config methods */
void   config__init
                     (Config         *message);
size_t config__get_packed_size
                     (const Config   *message);
size_t config__pack
                     (const Config   *message,
                      uint8_t             *out);
size_t config__pack_to_buffer
                     (const Config   *message,
                      ProtobufCBuffer     *buffer);
Config *
       config__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   config__free_unpacked
                     (Config *message,
                      ProtobufCAllocator *allocator);
/* Buf methods */
void   buf__init
                     (Buf         *message);
size_t buf__get_packed_size
                     (const Buf   *message);
size_t buf__pack
                     (const Buf   *message,
                      uint8_t             *out);
size_t buf__pack_to_buffer
                     (const Buf   *message,
                      ProtobufCBuffer     *buffer);
Buf *
       buf__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   buf__free_unpacked
                     (Buf *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*Hello_Closure)
                 (const Hello *message,
                  void *closure_data);
typedef void (*Config_Closure)
                 (const Config *message,
                  void *closure_data);
typedef void (*Buf_Closure)
                 (const Buf *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor hello__descriptor;
extern const ProtobufCEnumDescriptor    hello__system_type__descriptor;
extern const ProtobufCMessageDescriptor config__descriptor;
extern const ProtobufCEnumDescriptor    config__synchronization__descriptor;
extern const ProtobufCMessageDescriptor buf__descriptor;
extern const ProtobufCEnumDescriptor    buf__buf_type__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_procnet_2eproto__INCLUDED */
