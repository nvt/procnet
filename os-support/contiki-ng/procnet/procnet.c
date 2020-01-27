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

static int procnet_in_fd = 0;
static int procnet_out_fd = 1;

typedef struct procnet_message {
  uint16_t sync;
  uint16_t payload_length;
  uint8_t payload[];
} procnet_message_t;

static bool
procnet_process_message(char *buf, size_t len)
{
  Hello *msg;

  msg = hello__unpack(NULL, len, (uint8_t *)buf);
  printf("Incoming message with system type: %d\n", (int)msg->system_type);

  return true;
}

bool
procnet_send_message(uint8_t message_type, const void *payload, size_t payload_length)
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
    fprintf(stderr, "bytes_left=%d, offset=%d\n", bytes_left, offset);
    ret = read(procnet_in_fd, &buf[offset], bytes_left);
    fprintf(stderr, "read returned %d. max %d\n", ret, bytes_left);
    if(ret < 0) {
      if(errno == EIO) {
	perror("read EIO");
      } else
      perror("read");
      return false;
    } else if(ret == 0) {
      close(procnet_in_fd);
      fprintf(stderr, "pipe closed!\n");
      exit(EXIT_FAILURE);
    } else if(ret > bytes_left) {
      fprintf(stderr, "syscall error: read more bytes than requested\n");
      exit(EXIT_FAILURE);
    } else {
      bytes_left -= ret;
      offset += ret;
      if(offset == PROCNET_HEADER_LENGTH && bytes_left == 0) {
	/* Received the header. */
	msg = (procnet_message_t *)buf;
        msg->sync = ntohs(msg->sync);
        msg->payload_length = ntohs(msg->payload_length);

        printf("SYNC RECEIVED: 0x%x\n", msg->sync);
	if(msg->sync != 0x9e40) {
	  fprintf(stderr, "invalid message\n");
          exit(EXIT_FAILURE);
	}

	if(msg->payload_length > PROCNET_MAX_PAYLOAD_LENGTH) {
	  close(procnet_in_fd);
	  fprintf(stderr, "invalid payload length received: %u\n", (unsigned)msg->payload_length);
	  exit(EXIT_FAILURE);
	}

	if(msg->payload_length > 0) {
          printf("Getting a payload of %d bytes\n", (int)msg->payload_length);
	  bytes_left = msg->payload_length;
	}
      }
    }
  }

  return procnet_process_message(&buf[PROCNET_HEADER_LENGTH],
                                 msg->payload_length);
}

void
procnet_set_fds(int read_fd, int write_fd)
{
  procnet_in_fd = read_fd;
  procnet_out_fd = write_fd;
}