
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

import os
import logging
import SimNode
import selectors

def read_data(file, mask):
    buf = os.read(file, 5000)
    node = Simulation.fd_to_node[file]
    logging.debug("Read {} bytes from node {}".format(
        len(buf), node.node_id))
    if len(buf) == 0:
        node.stop()
        Simulation.sel.unregister(node.read_fd)
        Simulation.fd_to_node.pop(node.read_fd)
    else:
        node.proto.process_message(buf)

class Simulation:
    fd_to_node = dict()
    sel = selectors.DefaultSelector()
    
    def __init__(self, config):
        self.config = config
        self.node_types = []
        self.nodes = set()
        self.in_range = dict()
        self.event_time = 0
        self.state = 0
        next_node_id = 1

        try:
            self.log_dir = config['log_dir']
            logging.info("Writing the I/O log to the directory {}".format(
                self.log_dir))
        except KeyError:
            logging.info("No log dir specified")
            self.log_dir = None
        
        for node_type in config['node_types']:
            d = config['node_types'][node_type]
            node_count = d['nodes']
            path = d['path']
            args = d['args']
            
            sim_node_type = SimNode.SimNodeType(node_type, path, args)

            logging.info("Nodes: {}".format(d['nodes']))
            for i in range(0, node_count):
                node = SimNode.SimNode(self, sim_node_type, next_node_id)
                self.add_node(node)
                next_node_id = next_node_id + 1
            
        for in_range in config['in_range']:
            logging.debug("In range: {}".format(in_range))
            for node_id in in_range:
                node = self.get_node(node_id)
                if node is None:
                    logging.warn("Invalid node ID in range specification: {}".format(node_id))
                    continue
                other_nodes = list(filter(lambda node_id: node_id != node.node_id, in_range))
                for node_id in other_nodes:
                    dst_node = self.get_node(node_id)
                    if dst_node is None:
                        logging.warn("Invalid node ID in range specification: {}".format(node_id))
                        continue
                    node.make_reachable(dst_node)
            
    def add_node(self, node):
        self.nodes.add(node)

    def get_node(self, node_id):
        return next((node for node in self.nodes if node.node_id == node_id),
                    None)
        
    def forward_packet(self, from_node, payload):
        logging.debug("Forwardarding packet from {} to {}".format(
            from_node, from_node.reachable_nodes))
        for node in from_node.reachable_nodes:
            if node!= from_node:
                node.proto.send_packet(payload)

    def start(self):
        logging.info("Start the simulation with {} nodes".format(
            len(self.nodes)))
        for node in self.nodes:
            logging.info("Start {} of type {}".format(node,type(node)))
            node.start()
            self.sel.register(node.read_fd, selectors.EVENT_READ, read_data)
            self.fd_to_node[node.read_fd] = node
        self.state = 1

    def stop(self):
        logging.info("Stopping the simulation")
        for node in self.nodes:
            self.sel.unregister(node.read_fd)
            self.fd_to_node.pop(node.read_fd)
            node.stop()
        self.state = 0

    def stop_node(self, node):
        self.sel.unregister(node.read_fd)
        self.fd_to_node.pop(node.read_fd)
        node.stop()

    def run(self):
        while True:
            self.iterate()

    def iterate(self):
        logging.debug("Starting select...")
        events = self.sel.select()
        for key, mask in events:
            callback = key.data
            callback(key.fileobj, mask)
