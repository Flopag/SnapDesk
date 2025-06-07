#!/bin/bash

# Compile the Kernel Module

if sudo make -C /lib/modules/$(uname -r)/build M=$(pwd)/ath/ath9k modules ; then
    echo "Compilation Done"
else
    exit 1
fi

# Install the kernel module

sudo cp ./ath/ath9k/ath9k_htc.ko /lib/modules/$(uname -r)/kernel/drivers/net/wireless/

sudo depmod

if ! grep -Fxq "ath9k_htc" /etc/modules; then
    echo "ath9k_htc" | sudo tee -a /etc/modules > /dev/null
fi

# Install the sniffer-beacon service

cd ..

sudo cp ./boot/beacon-sniffer.service /etc/systemd/system/

sudo cp ./boot/beacon-sniffer.sh /usr/local/bin/

sudo chmod +x /usr/local/bin/beacon-sniffer.sh

sudo systemctl daemon-reload
sudo systemctl enable beacon-sniffer.service
sudo systemctl start beacon-sniffer.service