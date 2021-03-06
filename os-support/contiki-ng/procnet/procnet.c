/*
 * Copyright (c) 2020, RISE Research Institutes of Sweden AB.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/* Author: Nicolas Tsiftes <nicolas.tsiftes@ri.se> */

#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <netinet/in.h>

#include "contiki-conf.h"
#include "procnet.h"
#include "procnet.pb-c.h"
#include "dev/procnet-radio.h"
#include "net/ipv6/uip.h"
#include "sys/log.h"
#include "sys/node-id.h"

#define LOG_MODULE "ProcNet"
#define LOG_LEVEL LOG_LEVEL_INFO

static int procnet_in_fd = 0;
static int procnet_out_fd = 1;
static char *auth_token;
static enum protocol_state state = PROCNET_OFFLINE;

typedef struct procnet_message {
  uint16_t sync;
  uint16_t payload_length;
  uint8_t payload[];
} procnet_message_t;

static bool send_hello(void);

static bool
process_hello(Hello *msg)
{
  LOG_DBG("Incoming message with system type: %d\n", (int)msg->system_type);
  send_hello();
  return true;
}

static bool
process_config(Config *msg)
{
  linkaddr_t lladdr;

  LOG_DBG("Incoming config message with node id: %d\n", (int)msg->node_id);
  node_id = msg->node_id;
  memset(&lladdr, 0, sizeof(lladdr));
  lladdr.u8[sizeof(lladdr.u8) - 2] = (node_id >> 8) & 0xff;
  lladdr.u8[sizeof(lladdr.u8) - 1] = node_id & 0xff;
  linkaddr_set_node_addr(&lladdr);
  return true;
}

static bool
process_buf(Buf *msg)
{
  LOG_DBG("Incoming buf message with type: %d\n", (int)msg->type);
#if 0
  int i;
  for(i = 0; i < msg->data.len; i++) {
    printf("0x%x", msg->data.data[i]);
  }
#endif

  procnet_radio_add_packet(msg->data.data, msg->data.len);
  return true;
}

static bool
procnet_process_message(char *buf, size_t len)
{
  if(state == PROCNET_INIT) {
    Hello *msg;
    msg = hello__unpack(NULL, len, (uint8_t *)buf);
    process_hello(msg);
    hello__free_unpacked(msg, NULL);
    state = PROCNET_CONFIG;
  } else if(state == PROCNET_CONFIG) {
    Config *msg;
    msg = config__unpack(NULL, len, (uint8_t *)buf);
    process_config(msg);
    config__free_unpacked(msg, NULL);
    state = PROCNET_ACTIVE;
  } else {
    Buf *msg;
    msg = buf__unpack(NULL, len, (uint8_t *)buf);
    process_buf(msg);
    buf__free_unpacked(msg, NULL);
  }

  return true;
}

static bool
procnet_send(const void *payload, size_t payload_length)
{
  procnet_message_t msg;

  if(payload_length > UINT16_MAX) {
    return false;
  }

  msg.sync = htons(0x9e40);
  msg.payload_length = htons(payload_length);
  write(procnet_out_fd, &msg, PROCNET_HEADER_LENGTH);
  write(procnet_out_fd, payload, payload_length);

  return true;
}


bool
procnet_send_packet(void *payload, size_t payload_length)
{
  Buf msg = BUF__INIT;
  void *buf;
  unsigned len;

  msg.type = 1;

  msg.rssi = -100;
  msg.data.len = payload_length;
  msg.data.data = payload;

  len = buf__get_packed_size(&msg);

  LOG_DBG("Sending buf message of %u bytes\n", len);

  buf = malloc(len);
  buf__pack(&msg, buf);
  procnet_send(buf, len);
  free(buf);

  return true;
}

static bool
send_hello(void)
{
  Hello msg = HELLO__INIT;
  void *buf;
  unsigned len;

  msg.auth_token = auth_token;
  msg.system_type = 3;
  msg.system_name = "ProcNet Client";
  msg.system_version = CONTIKI_VERSION_STRING;

  len = hello__get_packed_size(&msg);
  LOG_DBG("Sending hello message of %u bytes\n", len);
  buf = malloc(len);
  hello__pack(&msg, buf);
  procnet_send(buf, len);
  free(buf);

  return true;
}

bool
procnet_receive_message(void)
{
  procnet_message_t *msg;
  uint32_t bytes_left;
  uint32_t offset;
  static char buf[PROCNET_MAX_MESSAGE_SIZE];
  int ret;

  offset = 0;
  bytes_left = PROCNET_HEADER_LENGTH;
  msg = NULL;

  while(bytes_left > 0) {
    ret = read(procnet_in_fd, &buf[offset], bytes_left);
    if(ret < 0) {
      LOG_ERR("read: %s\n", strerror(errno));
      return false;
    } else if(ret == 0) {
      close(procnet_in_fd);
      LOG_WARN("pipe closed!\n");
      exit(EXIT_FAILURE);
    } else if(ret > bytes_left) {
      LOG_ERR("syscall error: read more bytes than requested\n");
      exit(EXIT_FAILURE);
    } else {
      bytes_left -= ret;
      offset += ret;
      if(offset == PROCNET_HEADER_LENGTH && bytes_left == 0) {
	/* Received the header. */
	msg = (procnet_message_t *)buf;
        msg->sync = ntohs(msg->sync);
        msg->payload_length = ntohs(msg->payload_length);

	if(msg->sync != 0x9e40) {
	  LOG_WARN("invalid message\n");
          exit(EXIT_FAILURE);
	}

	if(msg->payload_length > PROCNET_MAX_PAYLOAD_LENGTH) {
	  close(procnet_in_fd);
	  LOG_ERR("invalid payload length received: %u\n",
                  (unsigned)msg->payload_length);
	  exit(EXIT_FAILURE);
	}

	if(msg->payload_length > 0) {
          LOG_DBG("Reading a payload %d bytes\n", (int)msg->payload_length);
	  bytes_left = msg->payload_length;
	}
      }
    }
  }

  return procnet_process_message(&buf[PROCNET_HEADER_LENGTH],
                                 msg->payload_length);
}

void
procnet_set_token(const char *token)
{
  auth_token = strdup(token);
}

void
procnet_set_fds(int read_fd, int write_fd)
{
  procnet_in_fd = read_fd;
  procnet_out_fd = write_fd;
  state = PROCNET_INIT;
}

enum protocol_state
procnet_get_state(void)
{
  return state;
}
