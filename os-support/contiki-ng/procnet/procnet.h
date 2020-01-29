#ifndef PROCNET_H
#define PROCNET_H

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

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define PROCNET_MAX_PAYLOAD_LENGTH 5000
#define PROCNET_HEADER_LENGTH 4
#define PROCNET_MAX_MESSAGE_SIZE   PROCNET_MAX_PAYLOAD_LENGTH + PROCNET_HEADER_LENGTH

#define PROCNET_MSG_VERSION 1
#define PROCNET_MSG_PING    2
#define PROCNET_MSG_PONG    3
#define PROCNET_MSG_PACKET  4
#define PROCNET_MSG_ERROR   5
#define PROCNET_MSG_QUIT    6
#define PROCNET_MSG_CONFIG  7

enum protocol_state {
  PROCNET_OFFLINE = 0,
  PROCNET_INIT    = 1,
  PROCNET_CONFIG  = 2,
  PROCNET_ACTIVE  = 3
};

bool procnet_send_packet(void *payload, size_t payload_length);
bool procnet_receive_message(void);
void procnet_set_fds(int read_fd, int write_fd);
enum protocol_state procnet_get_state(void);

#endif /* !PROCNET_H */
