# Copyright (c) 2020, RISE Research Institutes of Sweden AB.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. Neither the name of the Institute nor the names of its contributors
#    may be used to endorse or promote products derived from this software
#    without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.

# Author: Nicolas Tsiftes <nicolas.tsiftes@ri.se>

import logging
import os
import socket
import sys

import procnet_pb2

class Protocol:
  UNINITIALIZED  = 1
  HELLO_SENT     = 2
  ACTIVE         = 3

  packet_id = 1

  def __init__(self, node):
    self.node = node
    self.state = Protocol.UNINITIALIZED
    Protocol.send_hello(self)

  def process_message(self, buf):
    msg_sync = int.from_bytes(buf[0:2], byteorder='big')
    if msg_sync != 0x9e40:
      logging.debug("Invalid sync, discarding the message")
      return
    msg_len = int.from_bytes(buf[2:4], byteorder='big')
    logging.debug("Sync {} Len {}".format(msg_sync, msg_len))
    protobuf_payload = buf[4:]
    protobuf_payload_len = msg_len
    if self.state == Protocol.HELLO_SENT:
      hello = procnet_pb2.Hello()
      hello.ParseFromString(protobuf_payload)
      logging.debug("Received a hello message with name {}".format(hello.system_name))
      self.send_config()
    elif self.state == Protocol.ACTIVE:
      buf = procnet_pb2.Buf()
      buf.ParseFromString(protobuf_payload)
      logging.debug("Received a buf message with a {}-byte payload".format(len(buf.data)))
      self.node.simulation.forward_packet(self, buf.data)

  def send(self, payload):
    sync = (0x9e40).to_bytes(2, byteorder='big')
    logging.debug("sending a payload of {} bytes".format(
      len(payload)))
    length = len(payload).to_bytes(2, byteorder='big')
    os.write(self.node.write_fd, sync)
    os.write(self.node.write_fd, length)
    os.write(self.node.write_fd, payload)

  def send_hello(self):
    hello = procnet_pb2.Hello()
    hello.system_type = procnet_pb2.Hello.CONTROLLER
    hello.system_name = "ProcNet"
    hello.system_version = "1.0"
    payload = hello.SerializeToString()
    self.send(payload)
    self.state = Protocol.HELLO_SENT

  def send_config(self):
    config = procnet_pb2.Config()
    config.sync = procnet_pb2.Config.NONDETERMINISTIC
    config.node_id = self.node.node_id
    config.time_granularity = 1000
    payload = config.SerializeToString()
    logging.debug("Sending config of {} bytes".format(len(payload)))
    self.send(payload)
    self.state = Protocol.ACTIVE

  def send_packet(self, data):
    packet = procnet_pb2.Buf()
    packet.type = procnet_pb2.Buf.BUF_PACKET
    packet.data = data
    packet.packet_id = Protocol.packet_id
    Protocol.packet_id = Protocol.packet_id + 1
    payload = packet.SerializeToString()
    self.send(payload)

  def send_timesync(self, jiffies, run_until):
    timesync = procnet_pb2.TimeSync()
    timesync.jiffies = jiffies
    timesync.run_until = run_until
