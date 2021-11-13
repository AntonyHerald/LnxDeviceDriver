# LnxDeviceDriver
Linux Device Driver using C programming language.
### Important - Write-up on Kernel Driver usage, updates into kernel when loaded

After creating the character device, you wouldnt be able to access it from the user space. 
To do this, you need to add a device node under /dev. You can do this in two ways.

Use mknod to manually add a device node (old)

 "_mknod /dev/<name> c major minor_"

 (OR)

Use udev   //
   //As the successor of devfsd and hotplug, udev primarily manages device nodes in the /dev directory. 
     At the same time, udev also handles all user space events raised when hardware devices are added 
     into the system or removed from it, including firmware loading as required by certain devices.

This is where the class_create and device_create or class_device_create (old) come in.

To notify udev from your kernel module, you first create a virtual device class using
struct class * class_create(owner, name)

Now, the name will appear in /sys/class/<name>. Then, create a device and register it with sysfs.

struct device *device_create(struct class *class, struct device *parent,
                 dev_t devt, void *drvdata, const char *fmt, ...)

Now, device name will appear in /sys/devices/virtual/class_name/device_name and /dev/device_name

### Update in /proc/modules
 Note: After your module is loaded, it will appear in /proc/modules (do a cat /proc/modules to see it). 
      And, after you allocate the device numbers, say with 
  int register_chrdev_region(dev_t first, unsigned int count, char *name)

### Update in Proc filesystem:
  The name will appear in /proc/devices (do a cat /proc/devices to see it).
 
### Refer Kernel source:
  Please check the kernel sources for these functions as well, as they provide a good description of what they do in their comments.

  The good old LDD3 does not provide these mechanisms, but it's a very good source
