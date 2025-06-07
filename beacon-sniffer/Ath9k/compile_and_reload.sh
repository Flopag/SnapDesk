#!/bin/bash

if sudo make -C /lib/modules/$(uname -r)/build M=$(pwd)/ath/ath9k modules ; then
    echo "Compilation Done"
else
    exit 1
fi

sudo rm /dev/beacon-sniffer-0

sudo rmmod ./ath/ath9k/ath9k_htc.ko

if sudo insmod ./ath/ath9k/ath9k_htc.ko ; then
    echo "Driver loaded"
else
    exit 1
fi

major=$(awk "\$2==\"beacon-sniffer\" {print \$1}" /proc/devices)

sudo mknod /dev/beacon-sniffer-0 c $major 0

sudo dmesg -wH