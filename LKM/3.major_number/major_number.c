/***************************************************************************//**
*  \file       major_number.c
*
*  \details    allocating major number module
*
*  \author     bhagwat shinde
*
* *******************************************************************************/
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<device.h>


//creating the dev with our custom major and minor number

dev_t dev = 0;
static struct class *dev_class;



/*-------------------------------------------------------------------------*/
/*
** Module init function
*/
static int __init hello_world_init(void)
{
        if(alloc_chrdev_region(&dev,0,1,"test_device")<0)
        {
            printk(KERN_INFO "Can not allocate major number\n");
        }
        printk(KERN_INFO "Major No . %d , Minor No . %d \n",MAJOR(dev),MINOR(dev));

        //creating a struct class
        if(dev_class = class_create(THIS_MODULE,"etx_driver")==NULL)
        {
                pr_err("can not create class for a device\n");
                goto r_class;
        }

        //creatin a device file
        if(device_create(dev_class,NULL,dev,NULL,"etx_device") == NULL)
        {
            printk(KERN_INFO "Can not create device \n");
            goto r_device;
        }


        printk(KERN_INFO "Kernel Module Inserted Successfully...\n");
        return 0;

r_device:
        class_destroy(dev_class);
r_class:
        unregister_chrdev_region(dev,1);
        return -1;
}
/*
** Module Exit function
*/
static void __exit hello_world_exit(void)
{
    device_destroy(dev_class,dev);
    class_destroy(dev_class);
	unregister_chrdev_region(dev,1);
    printk(KERN_INFO "Kernel Module Removed Successfully...\n");
}
 
module_init(hello_world_init);
module_exit(hello_world_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("bhagwat <shindebhagavat030@@gmail.com>");
MODULE_DESCRIPTION("kernel parameter module");
MODULE_VERSION("1.0");
