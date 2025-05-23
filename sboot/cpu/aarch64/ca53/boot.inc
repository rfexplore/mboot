################################################################################
#
# Copyright (c) 2006-2009 MStar Semiconductor, Inc.
# All rights reserved.
#
# Unless otherwise stipulated in writing, any and all information contained
# herein regardless in any format shall remain the sole proprietary of
# MStar Semiconductor Inc. and be kept in strict confidence
# ("MStar Confidential Information") by the recipient.
# Any unauthorized act including without limitation unauthorized disclosure,
# copying, use, reproduction, sale, distribution, modification, disassembling,
# reverse engineering and compiling of the contents of MStar Confidential
# Information is unlawful and strictly prohibited. MStar hereby reserves the
# rights to any and all damages, losses, costs and expenses resulting therefrom.
#
################################################################################
#include "mstar_addr.inc"

#-------------------------------------------------------------------------------------------------
# Version
#-------------------------------------------------------------------------------------------------
BOOT_VERSION                = 0x20080606
#-------------------------------------------------------------------------------------------------
# Build Options
#-------------------------------------------------------------------------------------------------
SIMULATION                  = 0

#Default ENABLE MIU1
ENABLE_MIU1                 = 1


IS_FPGA                 = 0


#ifdef CONFIG_BIST_0x0401_UNITS
BIST_LEN                    = 0x0401
#endif

#ifdef CONFIG_BIST_0x1001_UNITS
BIST_LEN                    = 0x1001
#endif

#ifdef CONFIG_BIST_0x8001_UNITS
BIST_LEN                    = 0x8001
#endif

#---------------------------------------------------------------------------------------------------------
# Start uncompressed kernel from sboot
#---------------------------------------------------------------------------------------------------------


#---------------------------------------------------------------------------------------------------------
# Start compressed kernel from sboot
#---------------------------------------------------------------------------------------------------------
SBOOT_TO_KERNEL_SELZIP      = 0
ADDR_SELZIPBIN_ROM_START    = (MS_BUS_XIU_SPI_ADDR+0x00010000)
ADDR_SELZIPBIN_RAM_START    = (MS_BUS_MIU0_0000M+0x00400000)
ADDR_SELZIPBIN_RAM_END      = (MS_BUS_MIU0_0000M+0x00900000)

#---------------------------------------------------------------------------------------------------------
# SUPPORT_UNZIP
#---------------------------------------------------------------------------------------------------------
SBOOT_TO_KERNEL_ZIP         = 0
ADDR_UNZIP_STACK_POINTER    = (MS_BUS_MIU0_0000M+0x00380000)
ADDR_ZIPBIN_ROM_START       = (MS_BUS_XIU_SPI_ADDR+0x00030000)
ADDR_ZIPBIN_RAM_START       = (MS_BUS_MIU0_0000M+0x01000000)
ADDR_ZIPBIN_RAM_END         = (MS_BUS_MIU0_0000M+0x01400000)
ADDR_ZIPBIN_RAM_START_CACHE = (ADDR_ZIPBIN_RAM_START)
ADDR_SYSTEM_RAM_START       = (MS_BUS_MIU0_0000M+0x00400000)
ADDR_UNZIP_BUFFER           = (MS_BUS_MIU0_0000M+0x00300000)
ADDR_UNZIP_ROM_START        = (MS_BUS_XIU_SPI_ADDR+0x00020000)
ADDR_UNZIP_RAM_START        = (MS_BUS_MIU0_0000M+0x00100000)
ADDR_UNZIP_RAM_END          = (MS_BUS_MIU0_0000M+0x00110000)
ADDR_UNZIP_RAM_START_CACHE  = (ADDR_UNZIP_RAM_START)

BOOT_DEBUG_VERSION          = 1                  // Set for debugging boot code

BOOT_COPY_VIA_DMA           = 0                  // Set for copying zipped binary from Flash to DRAM via DMA; otherwise via CPU

BOOT_PARALLEL_FLASH         = 0                  // Set for booting from Parallel Flash; otherwise from Serial Flash

SUPPORT_POWER_SAVING        = 1
DYNAMIC_POWER_ON_OFF        = 0

SUPPORT_UNZIP               = 0                 // Set for supporting Unzip (also need to change boot.sh)


# Set DRAM type (exclusive selection)
DRAM_TYPE_DDR16_ETRON       = 1                 // BD_MST041A / BD_MST041B
DRAM_TYPE_SDR16_ESMT        = 0                 // BD_MST041D / BD_MST041E
DRAM_TYPE_SDR16_ETRON       = 0

# Enable TLB share (SMP please do not enable this)
ENABLE_TLB_SHARE            = 0                 // IS_FPGA = 1

#-------------------------------------------------------------------------------------------------
# Addressed in ROM / RAM
#-------------------------------------------------------------------------------------------------
# Start address after boot-up
ADDR_BOOT_ROM_START         = MS_BUS_BOOT_ADDR


# Reserve Bytes for boot code (to avoid recursive defines SIZE_BOOT in boot1.inc)
.if(EMMC_BOOT == 1)
SIZE_BOOT_RESERVED          = 1024
SIZE_BOOTROM_RESERVED       = 10240
SIZE_BOOTRAM_RESERVED       = 1024
.else
SIZE_BOOT_RESERVED          = 3072
SIZE_BOOTROM_RESERVED       = 8192
SIZE_BOOTRAM_RESERVED       = 2048
.endif
SIZE_BOOTNAND_RESERVED      = 53248     // 52 KB

SIZE_CHUNK_HEADER_RESERVED  = 128       // 16 chunks.


#### Copy UBoot binary from SPI Flash to RAM ###
#SIZE_UBOOT                 in boot1.c
#ADDR_UBOOT_ROM_START        = (ADDR_BOOT_ROM_START + SIZE_BOOT_RESERVED + SIZE_BOOTROM_RESERVED + SIZE_BOOTRAM_RESERVED + SIZE_BOOTNAND_RESERVED + SIZE_CHUNK_HEADER_RESERVED)
#ADDR_UBOOT_ROM_END          = (ADDR_UBOOT_ROM_START + SIZE_UBOOT) #actually end+1
#ADDR_UBOOT_RAM_START        = (UBOOT_ENTRY_ADDR | 0x20000000)
#ADDR_UBOOT_RAM_START_CACHE  = (ADDR_UBOOT_RAM_START & ~0x20000000 )
#ADDR_UBOOT_RAM_END          = (ADDR_UBOOT_RAM_START + SIZE_UBOOT) #actually end+1
#ADDR_UBOOT_RAM_ENTRY_CACHE  = UBOOT_ENTRY_ADDR    #ToDo: copy general exception handler at OS init instead of linking it to code


### Unzip binary ###
#Initialize system stack pointer for c subroutine
ADDR_STACK_POINTER    = (MS_BUS_MIU0_0000M+0x00A80000)    // Initial stack pointer (downwards)


#-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
REG_GIC_BASE_ADDRESS                    = 0x16000000
REG_GIC_CPU_BASE_ADDRESS                = (REG_GIC_BASE_ADDRESS + 0x2000)
REG_GIC_DIST_BASE_ADDRESS               = (REG_GIC_BASE_ADDRESS + 0x1000)

# FCIE registers
#-------------------------------------------------------------------------------------------------
FCIE0_BASE                   =   0x1F222600
REG_MMA_PRI                  =   (FCIE0_BASE + 0x02*4)
REG_MIU_ADDR_H               =   (FCIE0_BASE + 0x03*4)
REG_MIU_ADDR_L               =   (FCIE0_BASE + 0x04*4)
REG_MMA_PATH_CTL             =   (FCIE0_BASE + 0x0A*4)
REG_JOB_BL_CNT               =   (FCIE0_BASE + 0x0B*4)
REG_SD_MODE                  =   (FCIE0_BASE + 0x10*4)
REG_SD_CTRL                  =   (FCIE0_BASE + 0x11*4)
REG_BOOT_CONFIG              =   (FCIE0_BASE + 0x2F*4)

#-------------------------------------------------------------------------------------------------
# Other registers
#-------------------------------------------------------------------------------------------------
REG_TOP_POWER_STATUS        = (MS_BUS_RIU_ADDR+0x00203cdc)

#-------------------------------------------------------------------------------------------------
# registers
#-------------------------------------------------------------------------------------------------
PM_BASE                     = (MS_BUS_RIU_ADDR+0x00001C00)
MAU_BASE                    = (MS_BUS_RIU_ADDR+0x00001D00)

ENABLE_RIU32_BITMASK = 1

#-------------------------------------------------------------------------------------------------
# G3D setup
#-------------------------------------------------------------------------------------------------
ENABLE_G3D_PLL          = 0

//-------------------------------------------------------------------------------------------------
// secondary core entry
//-------------------------------------------------------------------------------------------------
BOOT_INFO_REGISTER_CORE1_START_ADDR_LOW  = (MS_BUS_RIU_ADDR + (0x100510 << 1))
BOOT_INFO_REGISTER_CORE1_START_ADDR_HIGH = (MS_BUS_RIU_ADDR + (0x100512 << 1))
BOOT_INFO_REGISTER_MAGIC                 = (MS_BUS_RIU_ADDR + (0x100514 << 1))
BOOT_INFO_REGISTER_CORE_WAIT             = (MS_BUS_RIU_ADDR + (0x100500 << 1))

