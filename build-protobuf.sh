#!/bin/sh

SRC_DIR=.
DST_DIR=.
protoc -I=$SRC_DIR --python_out=$DST_DIR $SRC_DIR/procnet.proto
protoc --c_out=$DST_DIR/os-support/contiki-ng/procnet $SRC_DIR/procnet.proto
