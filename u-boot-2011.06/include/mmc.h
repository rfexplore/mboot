/*
 * Copyright 2008,2010 Freescale Semiconductor, Inc
 * Andy Fleming
 *
 * Based (loosely) on the Linux code
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef _MMC_H_
#define _MMC_H_

#include <linux/list.h>

//#define STATIC_RELIABLE_TEST      1
#define SD_VERSION_SD	0x20000
#define SD_VERSION_3	(SD_VERSION_SD | 0x30)
#define SD_VERSION_2	(SD_VERSION_SD | 0x20)
#define SD_VERSION_1_0	(SD_VERSION_SD | 0x10)
#define SD_VERSION_1_10	(SD_VERSION_SD | 0x1a)
#define MMC_VERSION_MMC		0x10000
#define MMC_VERSION_UNKNOWN	(MMC_VERSION_MMC)
#define MMC_VERSION_1_2		(MMC_VERSION_MMC | 0x12)
#define MMC_VERSION_1_4		(MMC_VERSION_MMC | 0x14)
#define MMC_VERSION_2_2		(MMC_VERSION_MMC | 0x22)
#define MMC_VERSION_3		(MMC_VERSION_MMC | 0x30)
#define MMC_VERSION_4		(MMC_VERSION_MMC | 0x40)

#define MMC_MODE_HS					0x001
#define SD30_MODE_SDR104			0x002
#define SD30_MODE_DDR50				0x004
#define SD30_MODE_SDR50				0x008
#define MMC_MODE_HS_52MHz			0x010
#define MMC_MODE_HS_52MHz_DDR_3V	0x010
#define MMC_MODE_4BIT				0x100
#define MMC_MODE_8BIT				0x200
#define MMC_MODE_SPI				0x400



#define SD_DATA_4BIT	0x00040000

#define IS_SD(x) (x->version & SD_VERSION_SD)

#define MMC_DATA_READ		1
#define MMC_DATA_WRITE		2

#define NO_CARD_ERR		-16 /* No SD/MMC card inserted */
#define UNUSABLE_ERR		-17 /* Unusable Card */
#define COMM_ERR		-18 /* Communications Error */
#define TIMEOUT			-19

#define MMC_CMD_GO_IDLE_STATE		0
#define MMC_CMD_SEND_OP_COND		1
#define MMC_CMD_ALL_SEND_CID		2
#define MMC_CMD_SET_RELATIVE_ADDR	3
#define MMC_CMD_SET_DSR			4
#define MMC_CMD_SWITCH			6
#define MMC_CMD_SELECT_CARD		7
#define MMC_CMD_SEND_EXT_CSD		8
#define MMC_CMD_SEND_CSD		9
#define MMC_CMD_SEND_CID		10
#define MMC_CMD_STOP_TRANSMISSION	12
#define MMC_CMD_SEND_STATUS		13
#define MMC_CMD_SET_BLOCKLEN		16
#define MMC_CMD_READ_SINGLE_BLOCK	17
#define MMC_CMD_READ_MULTIPLE_BLOCK	18
#define MMC_CMD_SET_BLOCKCOUNT		23
#define MMC_CMD_WRITE_SINGLE_BLOCK	24
#define MMC_CMD_WRITE_MULTIPLE_BLOCK	25

#define MMC_CMD_APP_CMD			55
#define MMC_CMD_SPI_READ_OCR		58
#define MMC_CMD_SPI_CRC_ON_OFF		59

#define SD_CMD_SEND_RELATIVE_ADDR	3
#define SD_CMD_SWITCH_FUNC			6
#define SD_CMD_SEND_IF_COND			8
#define SD_CMD_SEND_TUNING_PATTERN	19

#define SD_CMD_APP_SET_BUS_WIDTH	6
#define SD_CMD_APP_SEND_OP_COND		41
#define SD_CMD_APP_SEND_SCR			51

#define SD_CMD_VOLT_SWITCH			11


/* SCR definitions in different words */
#define SD_HIGHSPEED_BUSY		0x00020000
#define SD_HIGHSPEED_SUPPORTED	0x00020000
#define SD_SDR50_SUPPORTED		0x00040000
#define SD_SDR104_SUPPORTED		0x00080000
#define SD_DDR50_SUPPORTED		0x00100000

#define MMC_HS_TIMING		0x00000100
#define MMC_HS_52MHZ		0x2
#define MMC_DDR_52MHZ_18_3V             0x4
#define MMC_DDR_52MHZ_12V               0x8

#define OCR_BUSY		0x80000000
#define OCR_HCS			0x40000000
#define OCR_UHS2		0x20000000
#define OCR_S18R		0x01000000

#define OCR_VOLTAGE_MASK	0x007FFF80
#define OCR_ACCESS_MODE		0x60000000

#define MMC_STATUS_MASK		(~0x0206BF7F)
#define MMC_STATUS_RDY_FOR_DATA (1 << 8)
#define MMC_STATUS_CURR_STATE	(0xf << 9)
#define MMC_STATUS_ERROR	(1 << 19)

#define MMC_VDD_165_195		0x00000080	/* VDD voltage 1.65 - 1.95 */
#define MMC_VDD_20_21		0x00000100	/* VDD voltage 2.0 ~ 2.1 */
#define MMC_VDD_21_22		0x00000200	/* VDD voltage 2.1 ~ 2.2 */
#define MMC_VDD_22_23		0x00000400	/* VDD voltage 2.2 ~ 2.3 */
#define MMC_VDD_23_24		0x00000800	/* VDD voltage 2.3 ~ 2.4 */
#define MMC_VDD_24_25		0x00001000	/* VDD voltage 2.4 ~ 2.5 */
#define MMC_VDD_25_26		0x00002000	/* VDD voltage 2.5 ~ 2.6 */
#define MMC_VDD_26_27		0x00004000	/* VDD voltage 2.6 ~ 2.7 */
#define MMC_VDD_27_28		0x00008000	/* VDD voltage 2.7 ~ 2.8 */
#define MMC_VDD_28_29		0x00010000	/* VDD voltage 2.8 ~ 2.9 */
#define MMC_VDD_29_30		0x00020000	/* VDD voltage 2.9 ~ 3.0 */
#define MMC_VDD_30_31		0x00040000	/* VDD voltage 3.0 ~ 3.1 */
#define MMC_VDD_31_32		0x00080000	/* VDD voltage 3.1 ~ 3.2 */
#define MMC_VDD_32_33		0x00100000	/* VDD voltage 3.2 ~ 3.3 */
#define MMC_VDD_33_34		0x00200000	/* VDD voltage 3.3 ~ 3.4 */
#define MMC_VDD_34_35		0x00400000	/* VDD voltage 3.4 ~ 3.5 */
#define MMC_VDD_35_36		0x00800000	/* VDD voltage 3.5 ~ 3.6 */

#define MMC_SWITCH_MODE_CMD_SET		0x00 /* Change the command set */
#define MMC_SWITCH_MODE_SET_BITS	0x01 /* Set bits in EXT_CSD byte
						addressed by index which are
						1 in value field */
#define MMC_SWITCH_MODE_CLEAR_BITS	0x02 /* Clear bits in EXT_CSD byte
						addressed by index, which are
						1 in value field */
#define MMC_SWITCH_MODE_WRITE_BYTE	0x03 /* Set target byte to value */

#define SD_SWITCH_CHECK		0
#define SD_SWITCH_SWITCH	1

#define SD_SW_GROUP_ACS_MODE		1
#define SD_SW_GROUP_CMD_SYSTEM		2
#define SD_SW_GROUP_DRV_STRENGTH	3
#define SD_SW_GROUP_CURRENT_LIMIT	4

/*
 * EXT_CSD fields
 */
#define EXT_CSD_ENH_START_ADDR_0     136    /* R/W */
#define EXT_CSD_ENH_START_ADDR_1     137    /* R/W */
#define EXT_CSD_ENH_START_ADDR_2     138    /* R/W */
#define EXT_CSD_ENH_START_ADDR_3     139    /* R/W */
#define EXT_CSD_ENH_SIZE_MULT_0      140    /* R/W */
#define EXT_CSD_ENH_SIZE_MULT_1      141    /* R/W */
#define EXT_CSD_ENH_SIZE_MULT_2      142    /* R/W */
#define EXT_CSD_GP_SIZE_MULT_1_0	 143    /* R/W */
#define EXT_CSD_GP_SIZE_MULT_1_1	 144    /* R/W */
#define EXT_CSD_GP_SIZE_MULT_1_2	 145    /* R/W */
#define EXT_CSD_GP_SIZE_MULT_2_0	 146    /* R/W */
#define EXT_CSD_GP_SIZE_MULT_2_1	 147    /* R/W */
#define EXT_CSD_GP_SIZE_MULT_2_2	 148    /* R/W */
#define EXT_CSD_GP_SIZE_MULT_3_0	 149    /* R/W */
#define EXT_CSD_GP_SIZE_MULT_3_1	 150    /* R/W */
#define EXT_CSD_GP_SIZE_MULT_3_2	 151    /* R/W */
#define EXT_CSD_GP_SIZE_MULT_4_0	 152    /* R/W */
#define EXT_CSD_GP_SIZE_MULT_4_1	 153    /* R/W */
#define EXT_CSD_GP_SIZE_MULT_4_2	 154    /* R/W */
#define EXT_CSD_PARTITION_SETTING_COMPLETED 155	/* R/W */
#define EXT_CSD_PARTITIONS_ATTRIBUTE 156    /* R/W */
#define EXT_CSD_WR_REL_SET           167    /*R, W or not depend on 166*/
#define EXT_CSD_WR_REL_PARAM         166    /*R*/

#define EXT_CSD_ERASE_GROUP_DEF	 175    /* R/W */

#define EXT_CSD_BOOT_BUS_WIDTH	 177	/* R/W */
#define EXT_CSD_PART_CONF   179 /* R/W */
#define EXT_CSD_BUS_WIDTH	183	/* R/W */
#define EXT_CSD_HS_TIMING	185	/* R/W */
#define EXT_CSD_CARD_TYPE	196	/* RO */
#define EXT_CSD_REV		192	/* RO */
#define EXT_CSD_SEC_CNT		212	/* RO, 4 bytes */
#define EXT_CSD_ERASE_TIMEOUT_MULT 223 /*R*/
#define EXT_CSD_HC_ERASE_GRP_SIZE 224 /*R*/

#define BOOT_CONFIG		179 /*RW 1byte */
#define BOOT_BUS_WIDTH		177 /*RW 1byte */

/*
 * EXT_CSD field definitions
 */

#define EXT_CSD_CMD_SET_NORMAL		(1 << 0)
#define EXT_CSD_CMD_SET_SECURE		(1 << 1)
#define EXT_CSD_CMD_SET_CPSECURE	(1 << 2)

#define EXT_CSD_CARD_TYPE_26	(1 << 0)	/* Card can run at 26MHz */
#define EXT_CSD_CARD_TYPE_52	(1 << 1)	/* Card can run at 52MHz */

#define EXT_CSD_BUS_WIDTH_1	0	/* Card is in 1 bit mode */
#define EXT_CSD_BUS_WIDTH_4	1	/* Card is in 4 bit mode */
#define EXT_CSD_BUS_WIDTH_8	2	/* Card is in 8 bit mode */
#define EXT_CSD_BUS_WIDTH_DDR_4         5
#define EXT_CSD_BUS_WIDTH_DDR_8         6

#define BOOT_CONFIG_ENABLE 0x49 /*BOOT_ACK=YES, nBOOT_PARTITION_ENABLE=Partition1, PARTITION_ACCESS=Partition1*/
#define BOOT_CONFIG_DISABLE 0x48 /*BOOT_ACK=YES, nBOOT_PARTITION_ENABLE=Partition1, PARTITION_ACCESS=Partition1*/
#define BOOT_BUS_WIDTH_8 0x2 /*BOOT_MODE=default, RESET_BOOT_BUS_WIDTH=default, BOOT_BUS_WIDTH=8-bit*/

#define R1_ILLEGAL_COMMAND		(1 << 22)
#define R1_APP_CMD			(1 << 5)

#define MMC_RSP_PRESENT (1 << 0)
#define MMC_RSP_136		(1 << 1)		/* 136 bit response */
#define MMC_RSP_CRC		(1 << 2)		/* expect valid crc */
#define MMC_RSP_BUSY	(1 << 3)		/* card may send busy */
#define MMC_RSP_OPCODE	(1 << 4)		/* response contains opcode */

#define MMC_RSP_NONE	(0)
#define MMC_RSP_R1	(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R1b	(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE| \
			MMC_RSP_BUSY)
#define MMC_RSP_R2	(MMC_RSP_PRESENT|MMC_RSP_136|MMC_RSP_CRC)
#define MMC_RSP_R3	(MMC_RSP_PRESENT)
#define MMC_RSP_R4	(MMC_RSP_PRESENT)
#define MMC_RSP_R5	(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R6	(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R7	(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)

#define MMCPART_NOAVAILABLE	(0xff)
#define PART_ACCESS_MASK	(0x7)
#define PART_SUPPORT		(0x1)

struct mmc_cid {
	unsigned long psn;
	unsigned short oid;
	unsigned char mid;
	unsigned char prv;
	unsigned char mdt;
	char pnm[7];
};

/*
 * WARNING!
 *
 * This structure is used by atmel_mci.c only.
 * It works for the AVR32 architecture but NOT
 * for ARM/AT91 architectures.
 * Its use is highly depreciated.
 * After the atmel_mci.c driver for AVR32 has
 * been replaced this structure will be removed.
 */
struct mmc_csd
{
	u8	csd_structure:2,
		spec_vers:4,
		rsvd1:2;
	u8	taac;
	u8	nsac;
	u8	tran_speed;
	u16	ccc:12,
		read_bl_len:4;
	u64	read_bl_partial:1,
		write_blk_misalign:1,
		read_blk_misalign:1,
		dsr_imp:1,
		rsvd2:2,
		c_size:12,
		vdd_r_curr_min:3,
		vdd_r_curr_max:3,
		vdd_w_curr_min:3,
		vdd_w_curr_max:3,
		c_size_mult:3,
		sector_size:5,
		erase_grp_size:5,
		wp_grp_size:5,
		wp_grp_enable:1,
		default_ecc:2,
		r2w_factor:3,
		write_bl_len:4,
		write_bl_partial:1,
		rsvd3:5;
	u8	file_format_grp:1,
		copy:1,
		perm_write_protect:1,
		tmp_write_protect:1,
		file_format:2,
		ecc:2;
	u8	crc:7;
	u8	one:1;
};

struct mmc_cmd {
	ushort cmdidx;
	uint resp_type;
	uint cmdarg;
	uint response[4];
	uint flags;
};

struct mmc_data {
	union {
		char *dest;
		const char *src; /* src buffers don't get written to */
	};
	uint flags;
	uint blocks;
	uint blocksize;
};

struct mmc {
	struct list_head link;
	char name[32];
	void *priv;
	uint voltages;
	uint version;
	uint has_init;
	uint f_min;
	uint f_max;
	int high_capacity;
	uint bus_width;
	uint clock;
	uint card_caps;
	uint host_caps;
	uint ocr;
	uint scr[2];
	uint csd[4];
	uint cid[4];
	ushort rca;
	char part_config;
	char part_num;
	uint tran_speed;
	uint read_bl_len;
	uint write_bl_len;
	u64 capacity;

	// eMMC specific
    char ext_csd[512];
	int reliable_write; // 0:unsupported 1:supported but unconfigured 2:supported and configured
	u64 slc_size;
	u64 max_slc_size;

	// SD specific
	unsigned char volt_sw_18v;
	unsigned int op_mode;

	block_dev_desc_t block_dev;
	int (*send_cmd)(struct mmc *mmc,
			struct mmc_cmd *cmd, struct mmc_data *data);
	void (*set_ios)(struct mmc *mmc);
	int (*init)(struct mmc *mmc);
	uint b_max;
};

int mmc_register(struct mmc *mmc);
int mmc_initialize(bd_t *bis);
int mmc_slc_mode(struct mmc *mmc, u64 size, int reliable_write);
int mmc_ecsd_read(struct mmc *mmc);
int mmc_ecsd_write(struct mmc *mmc, u8 num, u8 mask, u8 value);
int mmc_init(struct mmc *mmc);
int mmc_slc_check(struct mmc *mmc);
int mmc_relwr_check(struct mmc *mmc);
int mmc_slc_relwr_check(struct mmc *mmc);
void mmc_set_clock(struct mmc *mmc, uint clock);
struct mmc *find_mmc_device(int dev_num);
int mmc_set_dev(int dev_num);
int print_mmc_devices(char separator);
int get_mmc_num(void);
int board_mmc_getcd(u8 *cd, struct mmc *mmc);
int mmc_switch_part(int dev_num, unsigned int part_num);
int mmc_get_alignsize(struct mmc *mmc);

#define mmc_host_is_spi(mmc)	((mmc)->host_caps & MMC_MODE_SPI)

#ifdef CONFIG_GENERIC_MMC
int atmel_mci_init(void *regs);
struct mmc *mmc_spi_init(uint bus, uint cs, uint speed, uint mode);
#else
int mmc_legacy_init(int verbose);
#endif

#if defined(CONFIG_SD)
	int sd_register(struct mmc *mmc);
	int sd_initialize(bd_t *bis);
	int get_sd_num(void);
	struct mmc *find_sd_device(int dev_num);
	int sd_init(struct mmc *mmc);
	void print_sd_devices(char separator);
	int sd_switch_part(int dev_num, unsigned int part_num);
	int sd_set_bus_width(struct mmc * mmc, uint width);
	void mmc_read_write_test(int dev_num, unsigned int u32Loop);
	void mmc_read_write_test_each_op_mode(int dev_num);
#endif

#endif /* _MMC_H_ */
