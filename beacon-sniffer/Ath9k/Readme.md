# Ath9k implementing beacon-sniffer

## Use

To use This modification of Ath9k, compile then reload the kernel module using the `compile_and_reload.sh` file:

```bash
./compile_and_reload.sh
```

To only compile:

```bash
sudo make -C /lib/modules/$(uname -r)/build M=$(pwd)/ath/ath9k modules
```

To only reload:

```bash
# Remove the previous character device file
sudo rm /dev/beacon-sniffer-0
# Remove the previous kernel module
sudo rmmod ./ath/ath9k/ath9k_htc.ko
# Insert the new kernel module
sudo insmod ./ath/ath9k/ath9k_htc.ko
# Get the major device number allocated
major=$(awk "\$2==\"beacon-sniffer\" {print \$1}" /proc/devices)
# Create the new character device file
sudo mknod /dev/beacon-sniffer-0 c $major 0
```

To enable beacon-sniffer to run at boot time, run the scrypt `run_at_boot.sh`

## What modification have been made?

All modification are encapsulated inside command begining with the string `Mycode`. 

- Add the file `my_module.h` that contains all character and kernel module logic.

- Inject the beacon getter code in `htc_drv_txrx.c`

- Inject the init and cleanup function of the kernel module in `hif_usb.c`

