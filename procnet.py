#!/usr/bin/env python3

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
import sys
import yaml

# Simulator modules.
import Simulation

if __name__ == "__main__":
    sim_file = sys.argv[1]

    logging.basicConfig(level=logging.DEBUG)

    stream = open(sim_file, 'r')
    config  = yaml.load(stream, Loader=yaml.SafeLoader)
    stream.close()

    try:
        log_dir = os.path.abspath(config['log_dir'])
        logging.info("Writing node logs into {}".format(log_dir))
        os.environ['PROCNET_LOG_DIR'] = log_dir
        os.mkdir(log_dir)
    except KeyError:
        logging.warning("log_dir is not set, using /tmp")
        os.environ['PROCNET_LOG_DIR'] = "/tmp"
    except FileExistsError:
        logging.warning("Writing logs into the existing directory {}".format(
            log_dir))

    sim = Simulation.Simulation(config)
    sim.start()
    sim.run()
    sim.stop()
