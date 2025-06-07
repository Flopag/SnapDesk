# RTL8188 implementing beacon-sniffer

## Use

To use This modification of Rtl8188, compile then reload the kernel module using the `compile_and_reload.sh` file:

```bash
./compile_and_reload.sh
```

To only compile:

```bash
cd rtl8188eus-5.3.9
make
```

To only reload:

```bash
# Remove the previous character device file
sudo rm /dev/beacon-sniffer-0
# Remove the previous kernel module
sudo rmmod ./rtl8188eus-5.3.9/8188eu.ko
# Insert the new kernel module
sudo insmod ./rtl8188eus-5.3.9/8188eu.ko
# Get the major device number allocated
major=$(awk "\$2==\"beacon-sniffer\" {print \$1}" /proc/devices)
# Create the new character device file
sudo mknod /dev/beacon-sniffer-0 c $major 0
```

To enable beacon-sniffer to run at boot time, run the scrypt `run_at_boot.sh`

## What modification have been made?

All modification are encapsulated inside command begining with the string `Mycode`. 

- Add the file `my_module.h` that contains all character and kernel module logic.

- Inject the beacon getter code in `usb_ops_linux.c`

- Inject the init and cleanup function of the kernel module in `usb_intf.c`
