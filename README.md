# ASSIGNMENT LINUX ON RASPBERRY PI 3B+

## Starting condition

The starting condition is to have the Linux machine/virtual machine ready to build a kernel module with the command bitbake. 

## Cloning the repository

To repository must be cloned in the `/home/yoctotrainee/poky` folder. To move into that folder we can use the terminal command `cd /home/yoctotrainee/poky`. After this we can clone the repository using:

```bash
git clone https://github.com/giop98/Assignment-Linux-Sensor
```

## Setup & configuration

First we need to setup our build environment. We have to move in the poky folder `cd /home/yoctotrainee/poky` and then we have to execute the following command:

```bash 
source oe-init-build-env build_rpi3
```

In addition to that, we have to go in the following path `/home/yoctotrainee/poky/build_rpi3/conf` and open the file **bblayers.conf**. 

The content of the file must be modified with the following:

```bash
# POKY_BBLAYERS_CONF_VERSION is increased each time build/conf/bblayers.conf 
# changes incompatibly
POKY_BBLAYERS_CONF_VERSION = "2"

BBPATH = "${TOPDIR}" 
BBFILES ?= ""

BBLAYERS ?= " \
	/home/yoctotrainee/poky/meta \
	/home/yoctotrainee/poky/meta-poky \ 
	/home/yoctotrainee/poky/meta-yocto-bsp \ 
	/home/yoctotrainee/poky/meta-openembedded/meta-oe \ 
	/home/yoctotrainee/poky/meta-openembedded/meta-multimedia \ 
	/home/yoctotrainee/poky/meta-openembedded/meta-networking \ 
	/home/yoctotrainee/poky/meta-openembedded/meta-python \ 
	/home/yoctotrainee/poky/meta-raspberrypi \
	/home/yoctotrainee/poky/meta-ppg-sensor \
	"
```

In the same folder, namely `/home/yoctotrainee/poky/build_rpi3/conf `, we need to add to the file **layer.conf** the following lines:

```bash
MACHINE ?= "raspberrypi3" 
ENABLE_UART = "1"
EXTRA_IMAGE_FEATURES += "debug-tweaks tools-debug eclipse-debug ssh-server-openssh" 
IMAGE_INSTALL_append = " linux-firmware-rpidistro-bcm43455"
IMAGE_INSTALL_append = " connman connman-client "
IMAGE_INSTALL_append = " ppg-app"
IMAGE_INSTALL_append = " mymod"
KERNEL_MODULE_AUTOLOAD += "mymod"
IMAGE_FSTYPES = "tar.xz ext3 rpi-sdimg"
```
These are used to specify the machine where we want to deploy our application as well as the application and the kernel module.

After these steps, we can simply launch the following bitbake command in order to build the module:

```bash
bitbake core-image-full-cmdline
```

## Deploy on Raspberry Pi & Timing analysis

Once the build process is completed, we can copy the obtained Linux image to a Micro SD, boot on the Raspberry Pi and then connect via ssh to test the results.

The timing obtained is pretty good. In fact we expect a time of 40,96s for a signle execution of the application. I obtained, on the Raspberry Pi 3B+, 41.20s. The error is 0.58% and it is an accettable value. 
