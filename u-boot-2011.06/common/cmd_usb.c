/*
 * (C) Copyright 2001
 * Denis Peter, MPL AG Switzerland
 *
 * Most of this source has been derived from the Linux USB
 * project.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 */

#include <common.h>
#include <command.h>
#include <asm/byteorder.h>
#include <part.h>
#include <usb.h>

#if defined(CONFIG_CMD_FAT)
extern __u8 g_enable_fatbuf;   /* for fat.c using as fatbuf enable flag */
#endif

#if defined (CONFIG_USB_STORAGE)
static int usb_stor_curr_dev = -1; /* current device */
#endif
#ifdef CONFIG_USB_HOST_ETHER
static int usb_ether_curr_dev = -1; /* current ethernet device */
#endif

extern void USB_Set_WaitTime(unsigned int u32_ms);
extern unsigned int USB_Get_WaitTime(void);
extern int USB3_init(void);
extern int usb_init_exhub_port(int port, int ext_hub_port);

/* some display routines (info command) */
char *usb_get_class_desc(unsigned char dclass)
{
	switch (dclass) {
	case USB_CLASS_PER_INTERFACE:
		return "See Interface";
	case USB_CLASS_AUDIO:
		return "Audio";
	case USB_CLASS_COMM:
		return "Communication";
	case USB_CLASS_HID:
		return "Human Interface";
	case USB_CLASS_PRINTER:
		return "Printer";
	case USB_CLASS_MASS_STORAGE:
		return "Mass Storage";
	case USB_CLASS_HUB:
		return "Hub";
	case USB_CLASS_DATA:
		return "CDC Data";
	case USB_CLASS_VENDOR_SPEC:
		return "Vendor specific";
	default:
		return "";
	}
}

void usb_display_class_sub(unsigned char dclass, unsigned char subclass,
			   unsigned char proto)
{
	switch (dclass) {
	case USB_CLASS_PER_INTERFACE:
		printf("See Interface");
		break;
	case USB_CLASS_HID:
		printf("Human Interface, Subclass: ");
		switch (subclass) {
		case USB_SUB_HID_NONE:
			printf("None");
			break;
		case USB_SUB_HID_BOOT:
			printf("Boot ");
			switch (proto) {
			case USB_PROT_HID_NONE:
				printf("None");
				break;
			case USB_PROT_HID_KEYBOARD:
				printf("Keyboard");
				break;
			case USB_PROT_HID_MOUSE:
				printf("Mouse");
				break;
			default:
				printf("reserved");
				break;
			}
			break;
		default:
			printf("reserved");
			break;
		}
		break;
	case USB_CLASS_MASS_STORAGE:
		printf("Mass Storage, ");
		switch (subclass) {
		case US_SC_RBC:
			printf("RBC ");
			break;
		case US_SC_8020:
			printf("SFF-8020i (ATAPI)");
			break;
		case US_SC_QIC:
			printf("QIC-157 (Tape)");
			break;
		case US_SC_UFI:
			printf("UFI");
			break;
		case US_SC_8070:
			printf("SFF-8070");
			break;
		case US_SC_SCSI:
			printf("Transp. SCSI");
			break;
		default:
			printf("reserved");
			break;
		}
		printf(", ");
		switch (proto) {
		case US_PR_CB:
			printf("Command/Bulk");
			break;
		case US_PR_CBI:
			printf("Command/Bulk/Int");
			break;
		case US_PR_BULK:
			printf("Bulk only");
			break;
		default:
			printf("reserved");
			break;
		}
		break;
	default:
		printf("%s", usb_get_class_desc(dclass));
		break;
	}
}

void usb_display_string(struct usb_device *dev, int index)
{
	char buffer[256];
	if (index != 0) {
		if (usb_string(dev, index, &buffer[0], 256) > 0)
			printf("String: \"%s\"", buffer);
	}
}

void usb_display_desc(struct usb_device *dev)
{
	if (dev->descriptor.bDescriptorType == USB_DT_DEVICE) {
		printf("%d: %s,  USB Revision %x.%x\n", dev->devnum,
		usb_get_class_desc(dev->config.if_desc[0].desc.bInterfaceClass),
				   (dev->descriptor.bcdUSB>>8) & 0xff,
				   dev->descriptor.bcdUSB & 0xff);

		if (strlen(dev->mf) || strlen(dev->prod) ||
		    strlen(dev->serial))
			printf(" - %s %s %s\n", dev->mf, dev->prod,
				dev->serial);
		if (dev->descriptor.bDeviceClass) {
			printf(" - Class: ");
			usb_display_class_sub(dev->descriptor.bDeviceClass,
					      dev->descriptor.bDeviceSubClass,
					      dev->descriptor.bDeviceProtocol);
			printf("\n");
		} else {
			printf(" - Class: (from Interface) %s\n",
			       usb_get_class_desc(
				dev->config.if_desc[0].desc.bInterfaceClass));
		}
		printf(" - PacketSize: %d  Configurations: %d\n",
			dev->descriptor.bMaxPacketSize0,
			dev->descriptor.bNumConfigurations);
		printf(" - Vendor: 0x%04x  Product 0x%04x Version %d.%d\n",
			dev->descriptor.idVendor, dev->descriptor.idProduct,
			(dev->descriptor.bcdDevice>>8) & 0xff,
			dev->descriptor.bcdDevice & 0xff);
	}

}

void usb_display_conf_desc(struct usb_configuration_descriptor *config,
			   struct usb_device *dev)
{
	printf("   Configuration: %d\n", config->bConfigurationValue);
	printf("   - Interfaces: %d %s%s%dmA\n", config->bNumInterfaces,
	       (config->bmAttributes & 0x40) ? "Self Powered " : "Bus Powered ",
	       (config->bmAttributes & 0x20) ? "Remote Wakeup " : "",
		config->bMaxPower*2);
	if (config->iConfiguration) {
		printf("   - ");
		usb_display_string(dev, config->iConfiguration);
		printf("\n");
	}
}

void usb_display_if_desc(struct usb_interface_descriptor *ifdesc,
			 struct usb_device *dev)
{
	printf("     Interface: %d\n", ifdesc->bInterfaceNumber);
	printf("     - Alternate Setting %d, Endpoints: %d\n",
		ifdesc->bAlternateSetting, ifdesc->bNumEndpoints);
	printf("     - Class ");
	usb_display_class_sub(ifdesc->bInterfaceClass,
		ifdesc->bInterfaceSubClass, ifdesc->bInterfaceProtocol);
	printf("\n");
	if (ifdesc->iInterface) {
		printf("     - ");
		usb_display_string(dev, ifdesc->iInterface);
		printf("\n");
	}
}

void usb_display_ep_desc(struct usb_endpoint_descriptor *epdesc)
{
	printf("     - Endpoint %d %s ", epdesc->bEndpointAddress & 0xf,
		(epdesc->bEndpointAddress & 0x80) ? "In" : "Out");
	switch ((epdesc->bmAttributes & 0x03)) {
	case 0:
		printf("Control");
		break;
	case 1:
		printf("Isochronous");
		break;
	case 2:
		printf("Bulk");
		break;
	case 3:
		printf("Interrupt");
		break;
	}
	printf(" MaxPacket %d", epdesc->wMaxPacketSize);
	if ((epdesc->bmAttributes & 0x03) == 0x3)
		printf(" Interval %dms", epdesc->bInterval);
	printf("\n");
}

/* main routine to diasplay the configs, interfaces and endpoints */
void usb_display_config(struct usb_device *dev)
{
	struct usb_config *config;
	struct usb_interface *ifdesc;
	struct usb_endpoint_descriptor *epdesc;
	int i, ii;

	config = &dev->config;
	usb_display_conf_desc(&config->desc, dev);
	for (i = 0; i < config->no_of_if; i++) {
		ifdesc = &config->if_desc[i];
		usb_display_if_desc(&ifdesc->desc, dev);
		for (ii = 0; ii < ifdesc->no_of_ep; ii++) {
			epdesc = &ifdesc->ep_desc[ii];
			usb_display_ep_desc(epdesc);
		}
	}
	printf("\n");
}

static inline char *portspeed(int speed)
{
	if (speed == USB_SPEED_HIGH)
		return "480 Mb/s";
	else if (speed == USB_SPEED_SUPER)
		return "5 Gb/s";
	else if (speed == USB_SPEED_LOW)
		return "1.5 Mb/s";
	else
		return "12 Mb/s";
}

/* shows the device tree recursively */
void usb_show_tree_graph(struct usb_device *dev, char *pre)
{
	int i, index;
	int has_child,last_child/*,port*/;

	index = strlen(pre);
	printf(" %s", pre);
	/* check if the device has connected children */
	has_child = 0;
	for (i = 0; i < dev->maxchild; i++) {
		if (dev->children[i] != NULL)
			has_child = 1;
	}
	/* check if we are the last one */
	last_child = 1;
	if (dev->parent != NULL) {
		for (i = 0; i < dev->parent->maxchild; i++) {
			/* search for children */
			if (dev->parent->children[i] == dev) {
				/* found our pointer, see if we have a
				 * little sister
				 */
				//port = i;
				while (i++ < dev->parent->maxchild) {
					if (dev->parent->children[i] != NULL) {
						/* found a sister */
						last_child = 0;
						break;
					} /* if */
				} /* while */
			} /* device found */
		} /* for all children of the parent */
		printf("\b+-");
		/* correct last child */
		if (last_child)
			pre[index-1] = ' ';
	} /* if not root hub */
	else
		printf(" ");
	printf("%d ", dev->devnum);
	pre[index++] = ' ';
	pre[index++] = has_child ? '|' : ' ';
	pre[index] = 0;
	printf(" %s (%s, %dmA)\n", usb_get_class_desc(
					dev->config.if_desc[0].desc.bInterfaceClass),
					portspeed(dev->speed),
					dev->config.desc.bMaxPower * 2);
	if (strlen(dev->mf) || strlen(dev->prod) || strlen(dev->serial))
		printf(" %s  %s %s %s\n", pre, dev->mf, dev->prod, dev->serial);
	printf(" %s\n", pre);
	if (dev->maxchild > 0) {
		for (i = 0; i < dev->maxchild; i++) {
			if (dev->children[i] != NULL) {
				usb_show_tree_graph(dev->children[i], pre);
				pre[index] = 0;
			}
		}
	}
}

/* main routine for the tree command */
void usb_show_tree(struct usb_device *dev)
{
	char preamble[32];

	memset(preamble, 0, 32);
	usb_show_tree_graph(dev, &preamble[0]);
}


/******************************************************************************
 * usb boot command intepreter. Derived from diskboot
 */
#if defined (CONFIG_USB_STORAGE)
int do_usbboot(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	char *boot_device = NULL;
	char *ep;
	int dev, part = 1, rcode;
	ulong addr, cnt;
	disk_partition_t info;
	image_header_t *hdr;
	block_dev_desc_t *stor_dev;
#if defined(CONFIG_FIT)
	const void *fit_hdr = NULL;
#endif

	switch (argc) {
	case 1:
		addr = CONFIG_SYS_LOAD_ADDR;
		boot_device = getenv("bootdevice");
		break;
	case 2:
		addr = simple_strtoul(argv[1], NULL, 16);
		boot_device = getenv("bootdevice");
		break;
	case 3:
		addr = simple_strtoul(argv[1], NULL, 16);
		boot_device = argv[2];
		break;
	default:
		return cmd_usage(cmdtp);
	}

	if (!boot_device) {
		puts("\n** No boot device **\n");
		return 1;
	}

	dev = simple_strtoul(boot_device, &ep, 16);
	stor_dev = usb_stor_get_dev(dev);
	if (stor_dev == NULL || stor_dev->type == DEV_TYPE_UNKNOWN) {
		printf("\n** Device %d not available\n", dev);
		return 1;
	}
	if (stor_dev->block_read == NULL) {
		printf("storage device not initialized. Use usb scan\n");
		return 1;
	}
	if (*ep) {
		if (*ep != ':') {
			puts("\n** Invalid boot device, use `dev[:part]' **\n");
			return 1;
		}
		part = simple_strtoul(++ep, NULL, 16);
	}

	if (get_partition_info(stor_dev, part, &info)) {
		/* try to boot raw .... */
		strncpy((char *)&info.type[0], BOOT_PART_TYPE,
			sizeof(BOOT_PART_TYPE));
		strncpy((char *)&info.name[0], "Raw", 4);
		info.start = 0;
		info.blksz = 0x200;
		info.size = 2880;
		printf("error reading partinfo...try to boot raw\n");
	}
	if ((strncmp((char *)info.type, BOOT_PART_TYPE,
	    sizeof(info.type)) != 0) &&
	    (strncmp((char *)info.type, BOOT_PART_COMP,
	    sizeof(info.type)) != 0)) {
		printf("\n** Invalid partition type \"%.32s\""
			" (expect \"" BOOT_PART_TYPE "\")\n",
			info.type);
		return 1;
	}
	printf("\nLoading from USB device %d, partition %d: "
		"Name: %.32s  Type: %.32s\n",
		dev, part, info.name, info.type);

	debug("First Block: %ld,  # of blocks: %ld, Block Size: %ld\n",
		info.start, info.size, info.blksz);

	if (stor_dev->block_read(dev, info.start, 1, (ulong *)addr) != 1) {
		printf("** Read error on %d:%d\n", dev, part);
		return 1;
	}

	switch (genimg_get_format((void *)addr)) {
	case IMAGE_FORMAT_LEGACY:
		hdr = (image_header_t *)addr;

		if (!image_check_hcrc(hdr)) {
			puts("\n** Bad Header Checksum **\n");
			return 1;
		}

		image_print_contents(hdr);

		cnt = image_get_image_size(hdr);
		break;
#if defined(CONFIG_FIT)
	case IMAGE_FORMAT_FIT:
		fit_hdr = (const void *)addr;
		puts("Fit image detected...\n");

		cnt = fit_get_size(fit_hdr);
		break;
#endif
	default:
		puts("** Unknown image type\n");
		return 1;
	}

	cnt += info.blksz - 1;
	cnt /= info.blksz;
	cnt -= 1;

	if (stor_dev->block_read(dev, info.start+1, cnt,
		      (ulong *)(addr+info.blksz)) != cnt) {
		printf("\n** Read error on %d:%d\n", dev, part);
		return 1;
	}

#if defined(CONFIG_FIT)
	/* This cannot be done earlier, we need complete FIT image in RAM
	 * first
	 */
	if (genimg_get_format((void *)addr) == IMAGE_FORMAT_FIT) {
		if (!fit_check_format(fit_hdr)) {
			puts("** Bad FIT image format\n");
			return 1;
		}
		fit_print_contents(fit_hdr);
	}
#endif

	/* Loading ok, update default load address */
	load_addr = addr;

	flush_cache(addr, (cnt+1)*info.blksz);

	/* Check if we should attempt an auto-start */
	if (((ep = getenv("autostart")) != NULL) && (strcmp(ep, "yes") == 0)) {
		char *local_args[2];
		local_args[0] = argv[0];
		local_args[1] = NULL;
		printf("Automatic boot of image at addr 0x%08lX ...\n", addr);
		rcode = do_bootm(cmdtp, 0, 1, local_args);
		return rcode;
	}
	return 0;
}
#endif /* CONFIG_USB_STORAGE */

/*********************************************************************************
 * usb command intepreter
 */
int do_usb(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{

	int i;
	int ext_hub_port = -1;
	extern char usb_started;
#if defined (CONFIG_USB_STORAGE)
#if CONFIG_MINIUBOOT
#else
	struct usb_device *dev = NULL;
	block_dev_desc_t *stor_dev;
#endif
#endif

	if (argc < 2)
		return cmd_usage(cmdtp);

	if (argc > 3 )
	{
		ext_hub_port = simple_strtol(argv[3], NULL, 10);
		printf("hello: target_ext_hub_port %d\n", ext_hub_port);
	}

	if ((strncmp(argv[1], "reset", 5) == 0) ||
		 (strncmp(argv[1], "start", 5) == 0)) {

#if defined(CONFIG_CMD_FAT)
        g_enable_fatbuf = 0;   /* disable the buffer fat, open it in fat open */
#endif
        
        if(strncmp(argv[2],"0",1)==0)
        {
            usb_stop(USB_PORT0);
            printf("(Re)start USB 0...\n");
            if (ext_hub_port == -1)
                i = usb_init(USB_PORT0);
            else
                i = usb_init_exhub_port(USB_PORT0, ext_hub_port);
            //return 0;
        }
#if defined(ENABLE_SECOND_EHC)
        else if(strncmp(argv[2],"1",1)==0)
        {
            usb_stop(USB_PORT1);
            printf("(Re)start USB 1...\n");
            if (ext_hub_port == -1)
                i = usb_init(USB_PORT1);
            else
                i = usb_init_exhub_port(USB_PORT1, ext_hub_port);
            //return 0;
        }
#endif
#if defined(ENABLE_THIRD_EHC)
        else if(strncmp(argv[2],"2",1)==0)
        {
            usb_stop(USB_PORT2);
            printf("(Re)start USB 2...\n");
            if (ext_hub_port == -1)				
                i = usb_init(USB_PORT2);				
            else
                i = usb_init_exhub_port(USB_PORT2, ext_hub_port);
            //return 0;
        }
#endif
#if defined(ENABLE_FOURTH_EHC)
        else if(strncmp(argv[2],"3",1)==0)
        {
            usb_stop(USB_PORT3);
            printf("(Re)start USB 3...\n");
            if (ext_hub_port == -1)
                i = usb_init(USB_PORT3);
            else
                i = usb_init_exhub_port(USB_PORT3, ext_hub_port);
            //return 0;
        }
#endif
#if defined(ENABLE_FIFTH_EHC)
        else if(strncmp(argv[2],"4",1)==0)
        {
            usb_stop(USB_PORT4);
            printf("(Re)start USB 4...\n");
            if (ext_hub_port == -1)
                i = usb_init(USB_PORT4);
            else
                i = usb_init_exhub_port(USB_PORT4, ext_hub_port);
            //return 0;
        }
#endif
        else
        {
            printf("Usb start command error..\n");
            return cmd_usage(cmdtp);
        }

		if (i >= 0) {
#if defined (CONFIG_USB_STORAGE)
			/* try to recognize storage devices immediately */
			usb_stor_curr_dev = usb_stor_scan(1);
			if (usb_stor_curr_dev < 0)
			{
				return 1;
			}
#endif
#ifdef CONFIG_USB_HOST_ETHER
			/* try to recognize ethernet devices immediately */
			usb_ether_curr_dev = usb_host_eth_scan(1);
#endif
		}
		return i;
	}
	if (strncmp(argv[1], "stop", 4) == 0) {
#ifdef CONFIG_USB_KEYBOARD
		if (argc == 2) {
			if (usb_kbd_deregister() != 0) {
				printf("USB not stopped: usbkbd still"
					" using USB\n");
				return 1;
			}
		} else {
			/* forced stop, switch console in to serial */
			console_assign(stdin, "serial");
			usb_kbd_deregister();
		}
#endif
		printf("stopping USB..\n");
		usb_stop(0xff);
		return 0;
	}
    if (strncmp(argv[1], "check", 5) == 0) {		
        if(argv[2]==NULL)
        {
            usb_stop(USB_PORT0); 
            printf("(Re)start USB...\n");
            i = usb_post_init(USB_PORT0);
        }
        else if(strncmp(argv[2],"0",1)==0)
        {
            usb_stop(USB_PORT0); 
            printf("(Re)start USB 0...\n");
            i = usb_post_init(USB_PORT0);
        }
#if defined(ENABLE_SECOND_EHC)
        else if(strncmp(argv[2],"1",1)==0)
        {
            usb_stop(USB_PORT1);
            printf("(Re)start USB 1...\n");
            i = usb_post_init(USB_PORT1);
        }
#endif
#if defined(ENABLE_THIRD_EHC)
        else if(strncmp(argv[2],"2",1)==0)
        {
            usb_stop(USB_PORT2);
            printf("(Re)start USB 2...\n");
            i = usb_post_init(USB_PORT2);
        }
#endif
#if defined(ENABLE_FOURTH_EHC)
        else if(strncmp(argv[2],"3",1)==0)
        {
            usb_stop(USB_PORT3);
            printf("(Re)start USB 3...\n");
            i = usb_post_init(USB_PORT3);
        }
#endif
#if defined(ENABLE_FIFTH_EHC)
        else if(strncmp(argv[2],"4",1)==0)
        {
            usb_stop(USB_PORT4);
            printf("(Re)start USB 4...\n");
            i = usb_post_init(USB_PORT4);
        }
#endif
        else
        {
            printf("Usb start command error..\n");
            return 1;
        }

#if defined (CONFIG_USB_STORAGE)
        /* try to recognize storage devices immediately */
        if (i >= 0)
            usb_stor_curr_dev = usb_stor_scan(1);
        if (usb_stor_curr_dev < 0)
        {
            return 1;
        }
#endif
        return 0;
    }

	if (strcmp(argv[1], "wait") == 0) {
		if (argc == 3) {
			int waittime = (int)simple_strtoul(argv[2], NULL, 10);
			printf("\nUSB connection waiting time : %d\n", waittime);
			USB_Set_WaitTime(waittime);
			return 0;
		} else {
			printf("\nUSB connection waiting time : %d\n", USB_Get_WaitTime());
			return 0;
		}
		return 0;
	}

	if (!usb_started) {
		printf("USB is stopped. Please issue 'usb start' first.\n");
		return 1;
	}
#if CONFIG_MINIUBOOT
#else
	if (strncmp(argv[1], "tree", 4) == 0) {
		struct usb_device *dev=NULL;
		printf("\nDevice Tree:\n");
		dev = usb_get_dev_index(0);
		if(dev != NULL)
		{
			usb_show_tree(dev);
		}
		return 0;
	}
	if (strncmp(argv[1], "inf", 3) == 0) {
		int d;
		if (argc == 2) {
			for (d = 0; d < USB_MAX_DEVICE; d++) {
				dev = usb_get_dev_index(d);
				if (dev == NULL)
					break;
				usb_display_desc(dev);
				usb_display_config(dev);
			}
			return 0;
		} else {
			int d;

			i = simple_strtoul(argv[2], NULL, 16);
			printf("config for device %d\n", i);
			for (d = 0; d < USB_MAX_DEVICE; d++) {
				dev = usb_get_dev_index(d);
				if (dev == NULL)
					break;
				if (dev->devnum == i)
					break;
			}
			if (dev == NULL) {
				printf("*** No device available ***\n");
				return 0;
			} else {
				usb_display_desc(dev);
				usb_display_config(dev);
			}
		}
		return 0;
	}
#endif
#if defined (CONFIG_USB_STORAGE)
#if CONFIG_MINIUBOOT
#else
	if (strncmp(argv[1], "stor", 4) == 0)
		return usb_stor_info();

	if (strncmp(argv[1], "part", 4) == 0) {
		int devno, ok = 0;
		if (argc == 2) {
			for (devno = 0; ; ++devno) {
				stor_dev = usb_stor_get_dev(devno);
				if (stor_dev == NULL)
					break;
				if (stor_dev->type != DEV_TYPE_UNKNOWN) {
					ok++;
					if (devno)
						printf("\n");
					debug("print_part of %x\n", devno);
					print_part(stor_dev);
				}
			}
		} else {
			devno = simple_strtoul(argv[2], NULL, 16);
			stor_dev = usb_stor_get_dev(devno);
			if (stor_dev != NULL &&
			    stor_dev->type != DEV_TYPE_UNKNOWN) {
				ok++;
				debug("print_part of %x\n", devno);
				print_part(stor_dev);
			}
		}
		if (!ok) {
			printf("\nno USB devices available\n");
			return 1;
		}
		return 0;
	}
	if (strcmp(argv[1], "read") == 0) {
		if (usb_stor_curr_dev < 0) {
			printf("no current device selected\n");
			return 1;
		}
		if (argc == 5) {
			unsigned long addr = simple_strtoul(argv[2], NULL, 16);
			unsigned long blk  = simple_strtoul(argv[3], NULL, 16);
			unsigned long cnt  = simple_strtoul(argv[4], NULL, 16);
			unsigned long n;
			printf("\nUSB read: device %d block # %ld, count %ld"
				" ... ", usb_stor_curr_dev, blk, cnt);
			stor_dev = usb_stor_get_dev(usb_stor_curr_dev);
			n = stor_dev->block_read(usb_stor_curr_dev, blk, cnt,
						 (ulong *)addr);
			printf("%ld blocks read: %s\n", n,
				(n == cnt) ? "OK" : "ERROR");
			if (n == cnt)
				return 0;
			return 1;
		}
	}
	if (strcmp(argv[1], "write") == 0) {
		if (usb_stor_curr_dev < 0) {
			printf("no current device selected\n");
			return 1;
		}
		if (argc == 5) {
			unsigned long addr = simple_strtoul(argv[2], NULL, 16);
			unsigned long blk  = simple_strtoul(argv[3], NULL, 16);
			unsigned long cnt  = simple_strtoul(argv[4], NULL, 16);
			unsigned long n;
			printf("\nUSB write: device %d block # %ld, count %ld"
				" ... ", usb_stor_curr_dev, blk, cnt);
			stor_dev = usb_stor_get_dev(usb_stor_curr_dev);
			n = stor_dev->block_write(usb_stor_curr_dev, blk, cnt,
						(ulong *)addr);
			printf("%ld blocks write: %s\n", n,
				(n == cnt) ? "OK" : "ERROR");
			if (n == cnt)
				return 0;
			return 1;
		}
	}
	if (strncmp(argv[1], "dev", 3) == 0) {
		if (argc == 3) {
			int dev = (int)simple_strtoul(argv[2], NULL, 10);
			printf("\nUSB device %d: ", dev);
			stor_dev = usb_stor_get_dev(dev);
			if (stor_dev == NULL) {
				printf("unknown device\n");
				return 1;
			}
			printf("\n    Device %d: ", dev);
			dev_print(stor_dev);
			if (stor_dev->type == DEV_TYPE_UNKNOWN)
				return 1;
			usb_stor_curr_dev = dev;
			printf("... is now current device\n");
			return 0;
		} else {
			printf("\nUSB device %d: ", usb_stor_curr_dev);
			stor_dev = usb_stor_get_dev(usb_stor_curr_dev);
			dev_print(stor_dev);
			if (stor_dev->type == DEV_TYPE_UNKNOWN)
				return 1;
			return 0;
		}
		return 0;
	}
#endif
#endif /* CONFIG_USB_STORAGE */
	return cmd_usage(cmdtp);
}

#if defined (CONFIG_USB_STORAGE)
U_BOOT_CMD(
	usb,	5,	1,	do_usb,
	"USB sub-system",
	"reset [dev] - reset (rescan) USB controller\n"
	"usb start [dev] - start (scan) USB controller\n"
	"usb stop [f]  - stop USB [f]=force stop\n"
	"usb tree  - show USB device tree\n"
	"usb info [dev] - show available USB devices\n"
	"usb storage  - show details of USB storage devices\n"
	"usb dev [dev] - show or set current USB storage device\n"
	"usb part [dev] - print partition table of one or all USB storage"
	" devices\n"
	"usb read addr blk# cnt - read `cnt' blocks starting at block `blk#'\n"
	"    to memory address `addr'\n"
	"usb write addr blk# cnt - write `cnt' blocks starting at block `blk#'\n"
	"    from memory address `addr'\n"
	"usb wait [time] - set usb connection detection waiting time (ms)\n"
);

#if CONFIG_MINIUBOOT
#else
U_BOOT_CMD(
	usbboot,	3,	1,	do_usbboot,
	"boot from USB device",
	"loadAddr dev:part"
);
#endif

#else
U_BOOT_CMD(
	usb,	5,	1,	do_usb,
	"USB sub-system",
	"reset - reset (rescan) USB controller\n"
	"usb  tree  - show USB device tree\n"
	"usb  info [dev] - show available USB devices"
);
#endif
