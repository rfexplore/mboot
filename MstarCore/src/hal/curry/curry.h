/*
 * (C) Copyright 2010
 * Texas Instruments Incorporated.
 * Steve Sakoman  <steve@sakoman.com>
 *
 * Configuration settings for the TI OMAP4 Panda board.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __CONFIG_H
#define __CONFIG_H
#include "../../../sboot/include/autoconf.h"
#include "../../../sboot/inc/curry/board/Board.h"
#include <configs/uboot_module_config.h>

/*
 * High Level Configuration Options
 */
#define CONFIG_ARMV7		1	/* This is an ARM V7 CPU core */
#define CONFIG_OMAP		1	/* in a TI OMAP core */
#define CONFIG_OMAP44XX		1	/* which is a 44XX */
#define CONFIG_OMAP4430		1	/* which is in a 4430 */
#define CONFIG_PANDA		1	/* working with Panda */
#define CONFIG_ARCH_CPU_INIT
#define CONFIG_CURRY    1
#define CONFIG_GIC_PATCH 1

/* Get CPU defs */
#include <asm/arch/cpu.h>
#define CONFIG_TV_CHIP     0
/* Display CPU and Board Info */
#define CONFIG_DISPLAY_CPUINFO		1
#define CONFIG_DISPLAY_BOARDINFO	1

/* Keep L2 Cache Disabled */
#define CONFIG_L2_OFF			1

/* Clock Defines */
#define V_OSCK			38400000	/* Clock output from T2 */
#define V_SCLK                   V_OSCK

//#undef CONFIG_USE_IRQ				/* no support for IRQs */
#define CONFIG_USE_IRQ	1			/*support for IRQs */
#define CONFIG_MISC_INIT_R

#define CONFIG_CMDLINE_TAG		1	/* enable passing of ATAGs */
#define CONFIG_SETUP_MEMORY_TAGS	1
#define CONFIG_INITRD_TAG		1
#define CONFIG_REVISION_TAG		1

//# # # # # # # # # # # MIU Setting# # # # # # # # # # # 

#define CONFIG_SYS_MIU0_BUS CONFIG_MIU0_BUSADDR
#define CONFIG_SYS_MIU1_BUS CONFIG_MIU1_BUSADDR
#define CONFIG_SYS_MIU0_CACHE 0x20000000
#define CONFIG_SYS_MIU1_CACHE 0xA0000000
#define CONFIG_SYS_MIU0_NON_CACHE 0x60000000
#define CONFIG_SYS_MIU1_NON_CACHE 0xC0000000
#define CONFIG_SYS_MIU_INTERVAL 0x80000000
#define CONFIG_SYS_MIU2_INTERVAL 0xC0000000

//# # # # # # # # # # # RIU Setting# # # # # # # # # # # 

#define CONFIG_SYS_RIU_BASE 0x1F000000
#define CONFIG_SYS_XIU 0x14000000
//#############PM###################
#define CONFIG_PM_SIZE 0x5FFF

//#############R2###################
#define CONFIG_R2_CACHE_ADDR 0x00000000

//#############Upgrade###################
#define CONFIG_UPGRADE_BUFFER_ADDR     (CONFIG_SYS_MIU0_CACHE|0x200000)//(CACHE_ADDR|0x00000000)

//#############Secure Boot###################
// For OTP Code, the RSA_Key_From_HASH0 can not be trusted.
#define CONFIG_AUTH_KEYBANK_RSA_KEY_FROM_HASH2     1

/*
 * Size of malloc() pool
 * Total Size Environment - 256k
 * Malloc - add 256k
 */

#if defined(CONFIG_HEAP_SIZE_CONFIG)
#define CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + CONFIG_UBOOT_HEAP_SIZE) // 256K + 4M
#else
#define CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + (1 << 23)) // 256K + 8M
#endif

/* Vector Base */
#define CONFIG_SYS_CA9_VECTOR_BASE	SRAM_ROM_VECT_BASE

/*
 * Hardware drivers
 */

/*
 * serial port - NS16550 compatible
 */
#define V_NS16550_CLK			48000000

#define CONFIG_SYS_NS16550
#define CONFIG_SYS_NS16550_SERIAL
#define CONFIG_SYS_NS16550_REG_SIZE	(-4)
#define CONFIG_SYS_NS16550_CLK		V_NS16550_CLK
#define CONFIG_CONS_INDEX		3
#define CONFIG_SYS_NS16550_COM3		UART3_BASE

#if (ENABLE_MODULE_ENV_IN_NAND)
#if defined(CONFIG_INFO_IN_UBI_FILE)
#define CONFIG_ENV_IS_IN_NANDUBI
#else //defined(CONFIG_INFO_IN_NAND_ENV)
#define CONFIG_ENV_IS_IN_NAND
#endif
#elif (ENABLE_MODULE_ENV_IN_SERIAL)
#define CONFIG_ENV_IS_IN_SPI_FLASH
#elif (ENABLE_MODULE_ENV_IN_MMC)
#define CONFIG_ENV_IS_IN_MMC
#else
#define CONFIG_ENV_IS_NOWHERE
#endif

#if defined(CONFIG_ENV_IS_IN_NAND) || defined(CONFIG_ENV_IS_IN_NANDUBI)
#define CONFIG_ENV_OFFSET          cfg_env_offset
#define CONFIG_ENV_SIZE            CONFIG_ENV_VAR_SIZE  
/* the number of block allocated to environment, this should be larger than CONFIG_ENV_SIZE */
#define CONFIG_ENV_BLOCK_NUM  2
#endif //defined(CONFIG_ENV_IS_IN_NAND)
#if defined(CONFIG_ENV_IS_IN_SPI_FLASH)
#define CONFIG_ENV_IS_IN_SPIFLASH  1
#define CONFIG_ENV_SECT_SIZE    0x20000 // 128KB
#define CONFIG_ENV_SIZE		    0x10000
#define CONFIG_ENV_ADDR		    cfg_env_offset
#define CONFIG_ENV_ADDR_BAK        (CONFIG_ENV_ADDR + CONFIG_ENV_SIZE)
#if CONFIG_RESCUE_ENV
#define CONFIG_BRICK_BACKUP_WHOLE_ENV_ENABLE 1
#if CONFIG_SECURITY_BOOT
#define CONFIG_SPI_STATUS_ADDR             (CONFIG_ENV_ADDR - 5*CONFIG_ENV_SIZE)
#define CONFIG_ENV_RESCUE_ADDR             (CONFIG_ENV_ADDR - 4*CONFIG_ENV_SIZE)
#define CONFIG_ENV_RESCUE_ADDR_BAK         (CONFIG_ENV_ADDR - 3*CONFIG_ENV_SIZE)
#else
#define CONFIG_SPI_STATUS_ADDR             (CONFIG_ENV_ADDR - 3*CONFIG_ENV_SIZE)
#define CONFIG_ENV_RESCUE_ADDR             (CONFIG_ENV_ADDR - 2*CONFIG_ENV_SIZE)
#define CONFIG_ENV_RESCUE_ADDR_BAK         (CONFIG_ENV_ADDR - CONFIG_ENV_SIZE)
#endif
#endif

#endif //defined(CONFIG_ENV_IS_IN_SPI_FLASH)
#if defined(CONFIG_ENV_IS_IN_MMC)
#define CONFIG_SYS_MMC_ENV_DEV    0
#define CONFIG_ENV_SIZE		      0x10000          /* 64K bytes */
#endif //defined(CONFIG_ENV_IS_IN_MMC)

#define CONFIG_BAUDRATE			115200
#define CONFIG_SYS_BAUDRATE_TABLE	{4800, 9600, 19200, 38400, 57600,\
					115200}

/* UART */
#define CONFIG_SYS_SECOND_UART      0

/* I2C  */
//#define CONFIG_HARD_I2C			1
#define CONFIG_SYS_I2C_SPEED		100000
#define CONFIG_SYS_I2C_SLAVE		1
#define CONFIG_SYS_I2C_BUS		0
#define CONFIG_SYS_I2C_BUS_SELECT	1
#define CONFIG_DRIVER_OMAP34XX_I2C	1
#define CONFIG_I2C_MULTI_BUS		1

/* TWL6030 */
#define CONFIG_TWL6030_POWER		1

/* NET */
#if (ENABLE_MODULE_NET == 1)
#if (ENABLE_MODULE_USE_EMAC == 1)
#define CONFIG_EMAC			1
#elif (ENABLE_MODULE_USE_GMAC == 1)
#define CONFIG_GMAC			1
#else
#endif
#endif
/*----------------------------------------------------------------------- */
/*
 * NET configuration
 */
#if (ENABLE_MODULE_NET == 1)
#define     CONFIG_CMD_PING
#define     CONFIG_CMD_NET
#define     CONFIG_NET_MULTI
#define     CONFIG_CMD_DHCP
#endif

/* IR */
#if (ENABLE_MODULE_IR == 1)
#define CONFIG_IR          1
#endif

#if (ENABLE_MODULE_EEPROM == 1)
#define CONFIG_CMD_EEPROM
#endif
/* MSTAR I2C DRIVER*/
#if (ENABLE_MODULE_MSTARI2C == 1)
#define CONFIG_MSTARI2C    1
#else
#define CONFIG_MSTARI2C    0
#endif

/* MMC */
#if (ENABLE_MODULE_MMC == 1)
#define CONFIG_MMC          1
#define CONFIG_GENERIC_MMC  1
#define CONFIG_CMD_MMC      1
#endif

/* SD CARD */
#if (ENABLE_MODULE_SD == 1)
#define CONFIG_SD                   1
#define CONFIG_GENERIC_SD          1
#define CONFIG_CMD_SD               1
#endif
//#define CONFIG_OMAP_HSMMC		1
//#define CONFIG_SYS_MMC_SET_DEV		1
#define CONFIG_DOS_PARTITION		1
#define CONFIG_EMMC_PARTITION           1

/*NAND */
#if (ENABLE_MODULE_NAND_FLASH == 1)
#define CONFIG_NAND			1
#define CONFIG_CMD_NAND		1
#endif
#define CONFIG_STB_BFN 1
#define CONFIG_STB_BFN_VERSION 5

#define CONFIG_SYS_NAND_BASE       0x00000000
#define CONFIG_SYS_MAX_NAND_DEVICE 1

/*
 * Board NAND Info.
 */

#ifdef CONFIG_CMD_NAND
#define CONFIG_CMD_MTDPARTS
#define CONFIG_MTD_PARTITIONS
#define CONFIG_MTD_DEVICE
#if (ENABLE_MODULE_UBI == 1)
#define CONFIG_CMD_UBI
#define CONFIG_RBTREE
//add by mstar

#if (ENABLE_MODULE_UBI_ENHANCE_INIT == 1)
#define 	CONFIG_MTD_UBI_ENHANCE_INIT
#define 	CONFIG_MTD_UBI_ENHANCE_INIT_PART	"UBI"
#endif
#endif

#if (ENABLE_MODULE_UBI_MLC_BACKUP_LSB == 1)
#define     CONFIG_MTD_UBI_BACKUP_LSB
#endif
#if (ENABLE_MODULE_UBI_FS == 1)
#define CONFIG_CMD_UBIFS
//#define CONFIG_LZO
#endif

#define MTDIDS_DEFAULT      {"nand0=edb64M-nand"}
#define MTDPARTS_DEFAULT    {"mtdparts=nand:0x40000(NPT),0x40000(KL_BP),0x500000(KL),0x7500000(UBI)"}

#else
#define MTDPARTS_DEFAULT
#endif

/* USB */
//#define CONFIG_MUSB_UDC			1
//#define CONFIG_USB_OMAP3		1

/* USB device configuration */
//#define CONFIG_USB_DEVICE		1
//#define CONFIG_USB_TTY			1
//#define CONFIG_SYS_CONSOLE_IS_IN_ENV	1
/*----------------------------------------------------------------------- */
/*
 * USB configuration
 */
#if (ENABLE_MODULE_USB == 1)
#define     CONFIG_USB		1
#define     CONFIG_CMD_USB
#define     CONFIG_USB_EHCI
#define     CONFIG_USB_HOST0
#define     CONFIG_USB_STORAGE
#else
#undef      CONFIG_CMD_USB
#undef      CONFIG_USB_EHCI
#undef      CONFIG_USB_HOST0
#undef      CONFIG_USB_STORAGE
#endif

/* Flash */
#define CONFIG_SYS_NO_FLASH	1
#if (ENABLE_MODULE_SPI == 1)
//#define	CONFIG_CMD_FLASH
#define	CONFIG_CMD_SPI
#define	PHYS_FLASH_1			0x00000000	/* Flash Bank #1 */
#define	CONFIG_SYS_FLASH_BASE		PHYS_FLASH_1
#define	CONFIG_SYS_MAX_FLASH_SECT	(4 + 255)
#define	CONFIG_SYS_MAX_FLASH_BANKS	1
#define	CONFIG_SYS_FLASH_ERASE_TOUT	(25*CONFIG_SYS_HZ)
#define	CONFIG_SYS_FLASH_WRITE_TOUT	(25*CONFIG_SYS_HZ)
#define	CONFIG_SYS_FLASH_USE_BUFFER_WRITE	1
#else
#endif

/* commands to include */
#include <config_cmd_default.h>

/* Enabled commands */
//#define CONFIG_CMD_EXT2		/* EXT2 Support                 */
#if (ENABLE_MODULE_FAT == 1)
#define CONFIG_CMD_FAT		/* FAT support                  */
#define CONFIG_FAT	1	/* FAT support                  */
#endif
//#define CONFIG_CMD_I2C		/* I2C serial bus support	*/

/* Disabled commands */
#undef CONFIG_CMD_NFS
#undef CONFIG_CMD_FPGA		/* FPGA configuration Support   */
#undef CONFIG_CMD_IMLS		/* List all found images        */
#if (ENABLE_MODULE_NET != 1)
#undef CONFIG_CMD_NET
#endif

/*
 * Environment setup
 */
#if CONFIG_RESCUE_ENV
#define CONFIG_EXTRA_RESCUE_ENV_SETTINGS \
		"DEFAULT_ENV=1\0" \

#endif
#define CONFIG_BOOTDELAY	0
#define CONFIG_ZERO_BOOTDELAY_CHECK

#define CONFIG_ENV_OVERWRITE

#define CONFIG_EXTRA_ENV_SETTINGS \
	"loadaddr=0x82000000\0" \
	"console=ttyS2,115200n8\0" \
	"usbtty=cdc_acm\0" \
	"vram=16M\0" \
	"mmcdev=0\0" \
	"mmcroot=/dev/mmcblk0p2 rw\0" \
	"mmcrootfstype=ext3 rootwait\0" \
	"mmcargs=setenv bootargs console=${console} " \
		"vram=${vram} " \
		"root=${mmcroot} " \
		"rootfstype=${mmcrootfstype}\0" \
	"loadbootscript=fatload mmc ${mmcdev} ${loadaddr} boot.scr\0" \
	"bootscript=echo Running bootscript from mmc${mmcdev} ...; " \
		"source ${loadaddr}\0" \
	"loaduimage=fatload mmc ${mmcdev} ${loadaddr} uImage\0" \
	"mmcboot=echo Booting from mmc${mmcdev} ...; " \
		"run mmcargs; " \
		"bootm ${loadaddr}\0" \
		"ubispeedup=UBI\0" \

#define CONFIG_BOOTCOMMAND \
	"if mmc rescan ${mmcdev}; then " \
		"if run loadbootscript; then " \
			"run bootscript; " \
		"else " \
			"if run loaduimage; then " \
				"run mmcboot; " \
			"fi; " \
		"fi; " \
	"fi"

#define CONFIG_AUTO_COMPLETE		1
#define CONFIG_CMDLINE_EDITING      1

/*
 * Miscellaneous configurable options
 */

#define CONFIG_SYS_LONGHELP	/* undef to save memory */
#define CONFIG_SYS_HUSH_PARSER	/* use "hush" command parser */
#define CONFIG_SYS_PROMPT_HUSH_PS2	"> "
#define CONFIG_SYS_PROMPT		"Curry# "
#define CONFIG_SYS_CBSIZE		1024
#define CONFIG_SYS_DCACHE
/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + \
					sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_MAXARGS		32
/* Boot Argument Buffer Size */
#define CONFIG_SYS_BARGSIZE		(CONFIG_SYS_CBSIZE)

/*
 * memtest setup
 */
#define CONFIG_SYS_MEMTEST_START	0x80000000
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_MEMTEST_START + (32 << 20))

/* Default load address */
#define CONFIG_SYS_LOAD_ADDR		0x80000000

/* Use General purpose timer 1 */
//#define CONFIG_SYS_TIMERBASE		GPT2_BASE
#define CONFIG_SYS_TIMERBASE		0x16000600
#define CONFIG_SYS_PTV			2	/* Divisor: 2^(PTV+1) => 8 */
#define CONFIG_SYS_HZ			1000

/*
 * Stack sizes
 *
 * The stack sizes are set up in start.S using the settings below
 */
#define CONFIG_STACKSIZE	(128 << 10)	/* Regular stack */
#ifdef CONFIG_USE_IRQ
#define CONFIG_STACKSIZE_IRQ	(4 << 10)	/* IRQ stack */
#define CONFIG_STACKSIZE_FIQ	(4 << 10)	/* FIQ stack */
#endif

/*
 * SDRAM Memory Map
 * Even though we use two CS all the memory
 * is mapped to one contiguous block
 */
#define CONFIG_NR_DRAM_BANKS	1

//#define CONFIG_SYS_SDRAM_BASE		0x40000000
//#define CONFIG_SYS_INIT_RAM_ADDR	0x4030D800
#define CONFIG_SYS_SDRAM_BASE		0x20000000
#define CONFIG_SYS_INIT_RAM_ADDR	(CONFIG_DRAM_SIZE_MB_RELOCATE - 1) * 0x100000 + CONFIG_SYS_SDRAM_BASE
#define CONFIG_SYS_INIT_RAM_SIZE	0x800
#define CONFIG_SYS_INIT_SP_ADDR		(CONFIG_SYS_INIT_RAM_ADDR + \
					 CONFIG_SYS_INIT_RAM_SIZE - \
					 GENERATED_GBL_DATA_SIZE)

#undef CONFIG_GZIP
#define CONFIG_HDMITX_SSC 1
#endif /* __CONFIG_H */
