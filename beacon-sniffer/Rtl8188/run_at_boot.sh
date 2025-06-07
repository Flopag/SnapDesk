#!/bin/bash

# Compile the Kernel Module

cd rtl8188eus-5.3.9

if make ; then
    echo "Compilation Done"
else
    exit 1
fi

# Install the kernel module

sudo cp ./8188eu.ko /lib/modules/$(uname -r)/kernel/drivers/net/wireless/

sudo depmod

if ! grep -Fxq "8188eu" /etc/modules; then
    echo "8188eu" | sudo tee -a /etc/modules > /dev/null
fi

# Install the sniffer-beacon service

cd ..

sudo cp ./boot/beacon-sniffer.service /etc/systemd/system/

sudo cp ./boot/beacon-sniffer.sh /usr/local/bin/

sudo chmod +x /usr/local/bin/beacon-sniffer.sh

sudo systemctl daemon-reload
sudo systemctl enable beacon-sniffer.service
sudo systemctl start beacon-sniffer.service