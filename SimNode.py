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
import secrets
import signal
import subprocess
import sys

import Protocol

class SimNodeType:
    def __init__(self, name, path, args):
        self.name = name
        self.path = path
        self.args = args

    def __repr__(self):
        return "NodeType " + self.name

class SimNode:
    def __init__(self, simulation, node_type, node_id):
        self.simulation = simulation
        self.node_type = node_type
        self.node_id = node_id
        self.status = 0
        self.reachable_nodes = list()
        self.token = secrets.token_hex(32)
        logging.info("Node {} initialized with token {}".format(
            self.node_id, self.token))

    def __repr__(self):
        return "Node " + str(self.node_id)

    def start(self):
        logging.info("Starting node {}".format(self.node_id))
        child_read_fd, self.write_fd = os.pipe()
        self.read_fd, child_write_fd = os.pipe()
        self.pid = os.fork()
        sys.stderr.flush()
        sys.stdout.flush()
        if self.pid != 0:
            # In the parent process.
            os.close(child_read_fd)
            os.close(child_write_fd)
            self.proto = Protocol.Protocol(self)
        else:
            # In the child process.
            os.dup2(child_read_fd, 0)
            os.dup2(child_write_fd, 1)
            os.dup2(child_write_fd, 2)
            argv = self.node_type.args.split( )
            argv.insert(0, self.node_type.path)
            env = os.environ
            env['PROCNET_URI'] = "tcp://localhost:14395"
            try:
                os.execvpe(self.node_type.path, argv, env)
            except FileNotFoundError:
                os._exit(1)

    def stop(self):
        try:
            os.kill(self.pid, signal.SIGQUIT)
        except ProcessLookupError:
            logging.info("Process with PID {} does not exist".format(self.pid))

        try:
            pid, status = os.waitpid(self.pid, 0)
            kill_signal = (status >> 8) & 0xff
            exit_status = status & 0xff
            logging.info("Exited with code {} or signal {}".format(
                exit_status,kill_signal))
        except ChildProcessError:
            logging.debug("Process with PID {} has already exited".format(
                self.pid))

    def make_reachable(self, node):
        self.reachable_nodes.append(node)
