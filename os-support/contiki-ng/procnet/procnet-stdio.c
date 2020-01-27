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

#define _POSIX_C_SOURCE 1
#include <limits.h>
#include <stdarg.h>
#include <stdint.h>
#include "procnet-stdio.h"

/* Temporary includes. */
#include <sys/types.h>
#include <unistd.h>

#define IO_BUF_SIZE 1024

static char out_buf[IO_BUF_SIZE];
static uint16_t out_offset;
static char err_buf[IO_BUF_SIZE];
static uint16_t err_offset;

static FILE *fp;

static void
flush_io(char *buf, uint16_t *offset)
{
  if(fp != NULL) {
    fprintf(fp, "%s: %s", buf == out_buf ? "STDOUT" : "STDERR", buf);
    fflush(fp);
  }
  *offset = 0;
  buf[0] = '\0';
}

int
procnet_stdio_init(const char *log_dir)
{
  char filename[PATH_MAX];

  if(fp != NULL) {
    return 0;
  }

  snprintf(filename, sizeof(filename), "%s/procnet.log.pid.%d",
           (log_dir == NULL || *log_dir == '\0') ? "." : log_dir, getpid());

  fp = fopen(filename, "w");
  if(fp == NULL) {
    fprintf(stderr, "failed to open %s\n", filename);
    return 0;
  }

  return 1;
}

int
procnet_fprintf(FILE *stream, const char *fmt, ...)
{
  va_list ap;
  int r;
  int unhandled_stream;
  char *buf;
  uint16_t *offset;

  /*
   * We intercept standard output and standard error, and pass it on to
   * the simulator for logging. Other streams, such as files, are written
   * to directly.
   */
  unhandled_stream = stream != stdout && stream != stderr;
  if(stream == stdout) {
    buf = out_buf;
    offset = &out_offset;
  } else {
    buf = err_buf;
    offset = &err_offset;
  }

  va_start(ap, fmt);
  if(unhandled_stream) {
    r = vfprintf(stream, fmt, ap);
  } else {
    r = vsnprintf(&buf[*offset], IO_BUF_SIZE - *offset, fmt, ap);
  }
  va_end(ap);

  if(r > 0) {
    if(buf[*offset + r - 1] == '\n') {
      flush_io(buf, offset);
    } else {
      *offset += r;
    }
  }

  return r;
}

int
procnet_fputc(int ch, FILE *stream)
{
  int unhandled_stream;
  char *buf;
  uint16_t *offset;

  /*
   * We intercept standard output and standard error, and pass it on to
   * the simulator for logging. Other streams, such as files, are written
   * to directly.
   */
  unhandled_stream = stream != stdout && stream != stderr;

  if(unhandled_stream) {
    return fputc(ch, fp);
  }

  if(stream == stdout) {
    buf = out_buf;
    offset = &out_offset;
  } else {
    buf = err_buf;
    offset = &err_offset;
  }

  if(*offset == IO_BUF_SIZE) {
    flush_io(buf, offset);
  }

  buf[*offset++] = ch;

  return 1;
}

int
procnet_fputs(const char *s, FILE *stream)
{
  return procnet_fprintf(stream, "%s", s);
}
