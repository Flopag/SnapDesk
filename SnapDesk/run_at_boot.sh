#!/bin/bash

# Install the service file of SnapDesk
sudo cp ./boot/SnapDesk.service /etc/systemd/system/

# Compile the code, then copy all necessary files
make

sudo mkdir /usr/local/bin/SnapDesk

sudo cp ./snapdesk /usr/local/bin/SnapDesk/

sudo cp ./snappy.txt /usr/local/bin/SnapDesk/

# Install the script that run SnapDesk
sudo cp ./boot/SnapDesk.sh /usr/local/bin/SnapDesk/

sudo chmod +x /usr/local/bin/SnapDesk/SnapDesk.sh

# Enable the service in Debian
sudo systemctl daemon-reload
sudo systemctl enable SnapDesk.service
sudo systemctl start SnapDesk.service