#!/bin/bash

major=$(awk "\$2==\"beacon-sniffer\" {print \$1}" /proc/devices)

sudo mknod /dev/beacon-sniffer-0 c $major 0