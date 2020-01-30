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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "contiki.h"
#include "procnet.h"

void
platform_init_stage_one(void)
{
  const char *socket_path = getenv("PROCNET_URI");
  const char *log_dir = getenv("PROCNET_LOG_DIR");

  if(socket_path == NULL) {
    fprintf(stderr, "PROCNET_URI is not set\n");
  } else {
    fprintf(stderr, "PROCNET_URI = %s\n", socket_path);
  }

  if(log_dir == NULL) {
    fprintf(stderr, "PROCNET_LOG_DIR is not set, using current dir\n");
    log_dir = "";
  } else {
    fprintf(stderr, "PROCNET_LOG_DIR = %s\n", log_dir);
  }

  if(procnet_stdio_init(log_dir) == 0) {
    exit(EXIT_FAILURE);
  }

#if 0
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);
#endif
  procnet_set_fds(0, 1);

  do {
    procnet_receive_message();
  } while(procnet_get_state() != PROCNET_ACTIVE);
}

void
platform_init_stage_two(void)
{
}

void
platform_init_stage_three(void)
{
}

void
platform_idle(void)
{
  fd_set read_fds;
  struct timeval timeout;
  int ret;

  printf("Idle at %ld. next expiration time %ld\n", (unsigned long)clock_time(),
         (long)etimer_next_expiration_time());

  timeout.tv_sec = 0;
  timeout.tv_usec = (etimer_next_expiration_time() - clock_time()) * 1000;

  FD_ZERO(&read_fds);
  FD_SET(0, &read_fds);

  ret = select(1, &read_fds, NULL, NULL, &timeout);
  if(ret < 0) {
    perror("select");
    return;
  }

  if(FD_ISSET(0, &read_fds)) {
    procnet_receive_message();
  }
  etimer_request_poll();
}

void
platform_process_args(int argc, char *argv[])
{

}
