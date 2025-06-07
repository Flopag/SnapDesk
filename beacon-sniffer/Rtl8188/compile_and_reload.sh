#!/bin/bash

cd rtl8188eus-5.3.9

if make ; then
    echo "Compilation Done"
else
    exit 1
fi

sudo rm /dev/beacon-sniffer-0

sudo rmmod ./8188eu.ko

if sudo insmod ./8188eu.ko ; then
    echo "Driver loaded"
else
    exit 1
fi

major=$(awk "\$2==\"beacon-sniffer\" {print \$1}" /proc/devices)

sudo mknod /dev/beacon-sniffer-0 c $major 0

sudo dmesg -wH