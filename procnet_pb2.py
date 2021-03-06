# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: procnet.proto

import sys
_b=sys.version_info[0]<3 and (lambda x:x) or (lambda x:x.encode('latin1'))
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor.FileDescriptor(
  name='procnet.proto',
  package='',
  syntax='proto2',
  serialized_options=None,
  serialized_pb=_b('\n\rprocnet.proto\"\xbb\x01\n\x05Hello\x12&\n\x0bsystem_type\x18\x01 \x02(\x0e\x32\x11.Hello.SystemType\x12\x12\n\nauth_token\x18\x02 \x01(\t\x12\x13\n\x0bsystem_name\x18\x03 \x02(\t\x12\x16\n\x0esystem_version\x18\x04 \x02(\t\x12\x12\n\nnode_count\x18\x05 \x01(\r\"5\n\nSystemType\x12\x0e\n\nCONTROLLER\x10\x01\x12\r\n\tFORWARDER\x10\x02\x12\x08\n\x04NODE\x10\x03\"\xd5\x01\n\x06\x43onfig\x12\x0f\n\x07node_id\x18\x01 \x02(\r\x12%\n\x04sync\x18\x02 \x02(\x0e\x32\x17.Config.Synchronization\x12\x18\n\x10time_granularity\x18\x03 \x02(\r\x12\x11\n\ttick_time\x18\x04 \x01(\r\x12\x13\n\x0bipv6_prefix\x18\x05 \x01(\x0c\"Q\n\x0fSynchronization\x12\x11\n\rDETERMINISTIC\x10\x01\x12\x15\n\x11SEMIDETERMINISTIC\x10\x02\x12\x14\n\x10NONDETERMINISTIC\x10\x03\"\xc9\x01\n\x03\x42uf\x12\x1a\n\x04type\x18\x01 \x02(\x0e\x32\x0c.Buf.BufType\x12\x0c\n\x04\x64\x61ta\x18\x02 \x02(\x0c\x12\x0f\n\x07jiffies\x18\x03 \x01(\x04\x12\x11\n\tpacket_id\x18\x04 \x01(\x04\x12\x0c\n\x04rssi\x18\x05 \x01(\x05\x12\x0f\n\x07\x63hannel\x18\x06 \x01(\r\"U\n\x07\x42ufType\x12\x0e\n\nBUF_PACKET\x10\x01\x12\x0b\n\x07\x42UF_ACK\x10\x02\x12\r\n\tBUF_STDIN\x10\x03\x12\x0e\n\nBUF_STDOUT\x10\x04\x12\x0e\n\nBUF_STDERR\x10\x05')
)



_HELLO_SYSTEMTYPE = _descriptor.EnumDescriptor(
  name='SystemType',
  full_name='Hello.SystemType',
  filename=None,
  file=DESCRIPTOR,
  values=[
    _descriptor.EnumValueDescriptor(
      name='CONTROLLER', index=0, number=1,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='FORWARDER', index=1, number=2,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='NODE', index=2, number=3,
      serialized_options=None,
      type=None),
  ],
  containing_type=None,
  serialized_options=None,
  serialized_start=152,
  serialized_end=205,
)
_sym_db.RegisterEnumDescriptor(_HELLO_SYSTEMTYPE)

_CONFIG_SYNCHRONIZATION = _descriptor.EnumDescriptor(
  name='Synchronization',
  full_name='Config.Synchronization',
  filename=None,
  file=DESCRIPTOR,
  values=[
    _descriptor.EnumValueDescriptor(
      name='DETERMINISTIC', index=0, number=1,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='SEMIDETERMINISTIC', index=1, number=2,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='NONDETERMINISTIC', index=2, number=3,
      serialized_options=None,
      type=None),
  ],
  containing_type=None,
  serialized_options=None,
  serialized_start=340,
  serialized_end=421,
)
_sym_db.RegisterEnumDescriptor(_CONFIG_SYNCHRONIZATION)

_BUF_BUFTYPE = _descriptor.EnumDescriptor(
  name='BufType',
  full_name='Buf.BufType',
  filename=None,
  file=DESCRIPTOR,
  values=[
    _descriptor.EnumValueDescriptor(
      name='BUF_PACKET', index=0, number=1,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='BUF_ACK', index=1, number=2,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='BUF_STDIN', index=2, number=3,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='BUF_STDOUT', index=3, number=4,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='BUF_STDERR', index=4, number=5,
      serialized_options=None,
      type=None),
  ],
  containing_type=None,
  serialized_options=None,
  serialized_start=540,
  serialized_end=625,
)
_sym_db.RegisterEnumDescriptor(_BUF_BUFTYPE)


_HELLO = _descriptor.Descriptor(
  name='Hello',
  full_name='Hello',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='system_type', full_name='Hello.system_type', index=0,
      number=1, type=14, cpp_type=8, label=2,
      has_default_value=False, default_value=1,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='auth_token', full_name='Hello.auth_token', index=1,
      number=2, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=_b("").decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='system_name', full_name='Hello.system_name', index=2,
      number=3, type=9, cpp_type=9, label=2,
      has_default_value=False, default_value=_b("").decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='system_version', full_name='Hello.system_version', index=3,
      number=4, type=9, cpp_type=9, label=2,
      has_default_value=False, default_value=_b("").decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='node_count', full_name='Hello.node_count', index=4,
      number=5, type=13, cpp_type=3, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
    _HELLO_SYSTEMTYPE,
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=18,
  serialized_end=205,
)


_CONFIG = _descriptor.Descriptor(
  name='Config',
  full_name='Config',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='node_id', full_name='Config.node_id', index=0,
      number=1, type=13, cpp_type=3, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='sync', full_name='Config.sync', index=1,
      number=2, type=14, cpp_type=8, label=2,
      has_default_value=False, default_value=1,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='time_granularity', full_name='Config.time_granularity', index=2,
      number=3, type=13, cpp_type=3, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='tick_time', full_name='Config.tick_time', index=3,
      number=4, type=13, cpp_type=3, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='ipv6_prefix', full_name='Config.ipv6_prefix', index=4,
      number=5, type=12, cpp_type=9, label=1,
      has_default_value=False, default_value=_b(""),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
    _CONFIG_SYNCHRONIZATION,
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=208,
  serialized_end=421,
)


_BUF = _descriptor.Descriptor(
  name='Buf',
  full_name='Buf',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='type', full_name='Buf.type', index=0,
      number=1, type=14, cpp_type=8, label=2,
      has_default_value=False, default_value=1,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='data', full_name='Buf.data', index=1,
      number=2, type=12, cpp_type=9, label=2,
      has_default_value=False, default_value=_b(""),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='jiffies', full_name='Buf.jiffies', index=2,
      number=3, type=4, cpp_type=4, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='packet_id', full_name='Buf.packet_id', index=3,
      number=4, type=4, cpp_type=4, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='rssi', full_name='Buf.rssi', index=4,
      number=5, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='channel', full_name='Buf.channel', index=5,
      number=6, type=13, cpp_type=3, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
    _BUF_BUFTYPE,
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=424,
  serialized_end=625,
)

_HELLO.fields_by_name['system_type'].enum_type = _HELLO_SYSTEMTYPE
_HELLO_SYSTEMTYPE.containing_type = _HELLO
_CONFIG.fields_by_name['sync'].enum_type = _CONFIG_SYNCHRONIZATION
_CONFIG_SYNCHRONIZATION.containing_type = _CONFIG
_BUF.fields_by_name['type'].enum_type = _BUF_BUFTYPE
_BUF_BUFTYPE.containing_type = _BUF
DESCRIPTOR.message_types_by_name['Hello'] = _HELLO
DESCRIPTOR.message_types_by_name['Config'] = _CONFIG
DESCRIPTOR.message_types_by_name['Buf'] = _BUF
_sym_db.RegisterFileDescriptor(DESCRIPTOR)

Hello = _reflection.GeneratedProtocolMessageType('Hello', (_message.Message,), dict(
  DESCRIPTOR = _HELLO,
  __module__ = 'procnet_pb2'
  # @@protoc_insertion_point(class_scope:Hello)
  ))
_sym_db.RegisterMessage(Hello)

Config = _reflection.GeneratedProtocolMessageType('Config', (_message.Message,), dict(
  DESCRIPTOR = _CONFIG,
  __module__ = 'procnet_pb2'
  # @@protoc_insertion_point(class_scope:Config)
  ))
_sym_db.RegisterMessage(Config)

Buf = _reflection.GeneratedProtocolMessageType('Buf', (_message.Message,), dict(
  DESCRIPTOR = _BUF,
  __module__ = 'procnet_pb2'
  # @@protoc_insertion_point(class_scope:Buf)
  ))
_sym_db.RegisterMessage(Buf)


# @@protoc_insertion_point(module_scope)
