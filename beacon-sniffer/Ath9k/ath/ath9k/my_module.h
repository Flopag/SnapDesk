/* Mycode */
#ifndef MY_MODULE_H
#define MY_MODULE_H

// kernel module import
#include <linux/init.h>
#include <linux/module.h>
#include <linux/stat.h>

// character device imports
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/kernel.h>

// compile parameters
#define COUNT 1
#define BEACON_MAX_SIZE 2346 // max size of management frames
#define MODULE_NAME "beacon-sniffer"

// file operations declarations
static int my_open(struct inode *inode, struct file *file);
static int my_release(struct inode *inode, struct file *file);
static ssize_t my_read(struct file *file, char *user_buffer, size_t buffer_size, loff_t *offset);

static struct file_operations my_fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_release,
    .read = my_read
};

// cdev container definition
struct my_cdev_container {
    struct cdev cdev;
    // Data
    u8 loaded_frame[BEACON_MAX_SIZE];
    size_t frame_size;
    size_t open_count;
};

// Array of cdev container, one per registred device
static struct my_cdev_container *my_cdev_containers;

// First allocated device number
static dev_t dev;

// Main Frame buffer
static u8 *buffer;
static size_t frame_size;

/* -------- Utils --------*/

void inline fill_buffer(u8 *to, size_t to_size, u8 *from, size_t from_size){
    size_t min_size = min(to_size, from_size);

    for(size_t i = 0; i < min_size; ++i)
        to[i] = from[i];
}

/* -------- Kernel module functions -------- */

static int __init my_init(void){
    printk(KERN_INFO "%s: initialisation\n", MODULE_NAME);

    if(alloc_chrdev_region(&dev, 0, COUNT, MODULE_NAME)){
        printk(KERN_ERR "%s: failed to register device\n", MODULE_NAME);
        return -1;
    }

    // Allocation of module data
    buffer = (u8 *) kmalloc(sizeof(u8) * BEACON_MAX_SIZE, GFP_KERNEL);

    // Creation of cdev structure, one per registred device
    my_cdev_containers = kmalloc(sizeof(struct my_cdev_container) * COUNT, GFP_KERNEL);
    for(size_t i=0; i < COUNT; ++i){
        // init the cdev then add it
        cdev_init(&my_cdev_containers[i].cdev, &my_fops);
        my_cdev_containers[i].cdev.owner = THIS_MODULE;
        my_cdev_containers[i].open_count = 0;
        cdev_add(&my_cdev_containers[i].cdev, MKDEV(MAJOR(dev), MINOR(dev)+i), 1);
    }

    printk(KERN_INFO "%s: %d character devices has been registred with major %d\n", MODULE_NAME, COUNT, MAJOR(dev));

    return 0;
}

static void __exit my_cleanup(void){
    printk(KERN_INFO "%s: clean up\n", MODULE_NAME);

    // removing all cdev structure
    for(size_t i=0; i < COUNT; ++i)
        cdev_del(&my_cdev_containers[i].cdev);
    
    kfree(my_cdev_containers);

    kfree(buffer);

    unregister_chrdev_region(dev, COUNT);
}

/* -------- Character device functions -------- */

int my_open(struct inode *inode, struct file *file){
    // Put the character device data into the file structure
    // Then, we can use it in other operations
    struct my_cdev_container *my_cdev_container =  container_of(inode->i_cdev, struct my_cdev_container, cdev);
    file->private_data = my_cdev_container;

    // Load the buffer into the container buffer
    if(my_cdev_container->open_count < 1){
        fill_buffer(my_cdev_container->loaded_frame, BEACON_MAX_SIZE, buffer, frame_size);
        my_cdev_container->frame_size = frame_size;
    }

    my_cdev_container->open_count++;

    return 0;
}

int my_release(struct inode *inode, struct file *file){
    struct my_cdev_container *my_cdev_container = file->private_data;
    my_cdev_container->open_count--;
    
    return 0;
}

ssize_t my_read(struct file *file, char *user_buffer, size_t buffer_size, loff_t *offset){
    // Get the character device data
    struct my_cdev_container *my_cdev_container = file->private_data;

    // Get the size of the smaller buffer
    // To avoid overflow
    ssize_t len = min(my_cdev_container->frame_size - *offset, buffer_size);

    if(len < 0)
        return -1;

    // Copy the frame to the user buffer
    if(copy_to_user(user_buffer, my_cdev_container->loaded_frame, len))
        return -1;
    
    // Update the offset to go after the last red byte
    *offset += len;

    return len;
}

/* -------- Ath9k specifics -------- */

void inline update_beacon(u8 *beacon_body, size_t remaining_len){
    u8 *cursor = beacon_body;
    frame_size = min(remaining_len, (size_t) BEACON_MAX_SIZE);

    for(size_t i = 0; i < remaining_len; ++i){
        buffer[i] = *((u8 *)(cursor));
        cursor += 1;
    }
}

/* --------------------*/

#endif

/* ------- */