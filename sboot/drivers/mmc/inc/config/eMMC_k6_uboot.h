//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>
#if defined(CONFIG_MSTAR_BFE_EN)|| (defined(CONFIG_MSTAR_NS_UBOOT) && defined(CONFIG_MMC))

#ifndef __eMMC_K6_UBOOT__
#define __eMMC_K6_UBOOT__

//=====================================================

//=====================================================
//[FIXME] -->
#define eMMC_CACHE_LINE                 0x40    // [FIXME]

#define eMMC_PACK0
#define eMMC_PACK1                      __attribute__((__packed__))
#define eMMC_ALIGN0
#define eMMC_ALIGN1                     __attribute__((aligned(eMMC_CACHE_LINE)))
// <-- [FIXME]

//=====================================================
// HW registers
//=====================================================
#define REG_OFFSET_SHIFT_BITS           2

#define REG_FCIE_U16(Reg_Addr)          (*(volatile U16*)(Reg_Addr))
#define GET_REG_ADDR(x, y)              ((x)+((y) << REG_OFFSET_SHIFT_BITS))

#define REG_FCIE(reg_addr)              REG_FCIE_U16(reg_addr)
#define REG_FCIE_W(reg_addr, val)       REG_FCIE(reg_addr) = (val)
#define REG_FCIE_R(reg_addr, val)       val = REG_FCIE(reg_addr)
#define REG_FCIE_SETBIT(reg_addr, val)  REG_FCIE(reg_addr) |= (val)
#define REG_FCIE_CLRBIT(reg_addr, val)  REG_FCIE(reg_addr) &= ~(val)
#define REG_FCIE_W1C(reg_addr, val)     REG_FCIE_W(reg_addr, REG_FCIE(reg_addr)&(val))

//------------------------------
#define RIU_PM_BASE                     0x1F000000
#define RIU_BASE                        0x1F200000

#define REG_BANK_FCIE0                  0x8980
#define REG_BANK_FCIE1                  0x8A00
#define REG_BANK_FCIE2                  0x8A80

#define FCIE0_BASE                      GET_REG_ADDR(RIU_BASE, REG_BANK_FCIE0)
#define FCIE1_BASE                      GET_REG_ADDR(RIU_BASE, REG_BANK_FCIE1)
#define FCIE2_BASE                      GET_REG_ADDR(RIU_BASE, REG_BANK_FCIE2)

#define FCIE_REG_BASE_ADDR              GET_REG_ADDR(FCIE0_BASE, 0x00)
#define FCIE_CMDFIFO_BASE_ADDR          GET_REG_ADDR(FCIE0_BASE, 0x20)
#define FCIE_CIFD_BASE_ADDR             GET_REG_ADDR(FCIE1_BASE, 0x00)

#define FCIE_NC_WBUF_CIFD_BASE          GET_REG_ADDR(FCIE1_BASE, 0x00)
#define FCIE_NC_RBUF_CIFD_BASE          GET_REG_ADDR(FCIE1_BASE, 0x20)

#define FCIE_LAST_CRC_BASE              GET_REG_ADDR(FCIE1_BASE, 0x40)
#define FCIE_POWEER_SAVE_MODE_BASE      GET_REG_ADDR(FCIE2_BASE, 0x00)

#include "eMMC_reg_v5.h"

//--------------------------------clock gen------------------------------------
#define REG_BANK_CLKGEN0                0x0580  // (0x100B - 0x1000) x 80h
#define CLKGEN0_BASE                    GET_REG_ADDR(RIU_BASE, REG_BANK_CLKGEN0)

#define reg_ckg_fcie                    GET_REG_ADDR(CLKGEN0_BASE, 0x64)
#define BIT_FCIE_CLK_GATING             BIT8
#define BIT_FCIE_CLK_INVERSE            BIT9
#define BIT_CLKGEN_FCIE_MASK            (BIT13|BIT12|BIT11|BIT10)
#define BIT_FCIE_CLK_SRC_SEL            BIT14
#define BIT_CKG_NFIE_DVI4_EN            BIT7

// 1X clocks
#define BIT_FCIE_CLK_1X_XTAL            0x0
#define BIT_FCIE_CLK_1X_12M             0x1
#define BIT_FCIE_CLK_1X_15_5M           0x2
#define BIT_FCIE_CLK_1X_18M             0x3
#define BIT_FCIE_CLK_1X_21_5M           0x4
#define BIT_FCIE_CLK_1X_27M             0x5
#define BIT_FCIE_CLK_1X_36M             0x6
#define BIT_FCIE_CLK_1X_40M             0x7
#define BIT_FCIE_CLK_1X_48M             0x8
#define BIT_FCIE_CLK_1X_54M             0x9
#define BIT_FCIE_CLK_1X_60M             0xA
#define BIT_FCIE_CLK_1X_88_5M           0xB
#define BIT_FCIE_CLK_1X_75K             0xC
#define BIT_FCIE_CLK_1X_80M             0xD

// 4X clocks
#define eMMC_4X_FLAG                    0x80

#define BIT_FCIE_CLK_4X_XTAL            (0x0|eMMC_4X_FLAG)
#define BIT_FCIE_CLK_4X_48M             (0x1|eMMC_4X_FLAG)
#define BIT_FCIE_CLK_4X_62M             (0x2|eMMC_4X_FLAG)
#define BIT_FCIE_CLK_4X_72M             (0x3|eMMC_4X_FLAG)
#define BIT_FCIE_CLK_4X_86M             (0x4|eMMC_4X_FLAG)
#define BIT_FCIE_CLK_4X_108M            (0x5|eMMC_4X_FLAG)
#define BIT_FCIE_CLK_4X_144M            (0x6|eMMC_4X_FLAG)
#define BIT_FCIE_CLK_4X_160M            (0x7|eMMC_4X_FLAG)
#define BIT_FCIE_CLK_4X_192M            (0x8|eMMC_4X_FLAG)
#define BIT_FCIE_CLK_4X_216M            (0x9|eMMC_4X_FLAG)
#define BIT_FCIE_CLK_4X_240M            (0xA|eMMC_4X_FLAG)
#define BIT_FCIE_CLK_4X_354M            (0xB|eMMC_4X_FLAG)
#define BIT_FCIE_CLK_4X_300K            (0xC|eMMC_4X_FLAG)
#define BIT_FCIE_CLK_4X_320M            (0xD|eMMC_4X_FLAG)

#define BIT_CLK_XTAL_12M                BIT_FCIE_CLK_4X_XTAL
#define BIT_FCIE_CLK_48M                BIT_FCIE_CLK_4X_192M // 192/4 = 48
#define BIT_FCIE_CLK_300K               BIT_FCIE_CLK_4X_300K

#define eMMC_FCIE_VALID_CLK_CNT         3   // FIXME

#define PLL_SKEW4_CNT                   9
#define MIN_OK_SKEW_CNT                 7

extern  U8 gau8_FCIEClkSel[];
extern  U8 gau8_eMMCPLLSel_52[];
extern  U8 gau8_eMMCPLLSel_200[];           // for DDR52 or HS200

typedef eMMC_PACK0 struct _eMMC_FCIE_ATOP_SET {
    U32 u32_ScanResult;
    U8  u8_Clk;
    U8  u8_Reg2Ch, u8_Skew4;
    U8  u8_Cell;
    U8  u8_Skew2, u8_CellCnt;
} eMMC_PACK1 eMMC_FCIE_ATOP_SET_t;

typedef eMMC_PACK0 struct _eMMC_FCIE_ATOP_SET_EXT {
    U32 au32_RXDLLResult[5];
    U8 u8_Skew4Idx;
    U8 au8_Reg2Ch[5], au8_Skew4[5];
    U8 au8_Cell[5], au8_CellCnt[5];
} eMMC_PACK1 eMMC_FCIE_ATOP_SET_EXT_t;

//--------------------------------chiptop--------------------------------------
#define REG_BANK_CHIPTOP                0x0F00  // (0x101E - 0x1000) x 80h
#define PAD_CHIPTOP_BASE                GET_REG_ADDR(RIU_BASE, REG_BANK_CHIPTOP)

#define reg_chiptop_0x08                GET_REG_ADDR(PAD_CHIPTOP_BASE, 0x08)
#define BIT_SD_CONFIG                   BIT8
#define BIT_NAND_MODE                   BIT3
#define BIT_EMMC_CONFIG                 BIT0
#define BIT_EMMC_CONFIG_EMMC_MODE_1     BIT_EMMC_CONFIG

#define reg_nand_pad_driving            GET_REG_ADDR(PAD_CHIPTOP_BASE, 0x30)

#define reg_chiptop_0x36                GET_REG_ADDR(PAD_CHIPTOP_BASE, 0x36)
#define BIT_reg_emmc_rstz_sw            BIT5
#define BIT_reg_emmc_rstz_en            BIT4

#define reg_chiptop_0x50                GET_REG_ADDR(PAD_CHIPTOP_BASE, 0x50)
#define BIT_ALL_PAD_IN                  BIT15

//--------------------------------emmc pll--------------------------------------
#define REG_BANK_EMMC_PLL               0x11F80 // (0x123F - 0x1000) x 80h
#define EMMC_PLL_BASE                   GET_REG_ADDR(RIU_BASE, REG_BANK_EMMC_PLL)

#define reg_emmcpll_0x1a                GET_REG_ADDR(EMMC_PLL_BASE, 0x1A)
#define reg_emmc_test                   reg_emmcpll_0x1a

#define reg_emmcpll_0x1c                GET_REG_ADDR(EMMC_PLL_BASE, 0x1c)

#define reg_emmcpll_0x68                GET_REG_ADDR(EMMC_PLL_BASE, 0x68)

#define reg_emmcpll_0x6c                GET_REG_ADDR(EMMC_PLL_BASE, 0x6c)
#define BIT_DQS_DELAY_CELL_MASK         (BIT4|BIT5|BIT6|BIT7)
#define BIT_DQS_DELAY_CELL_SHIFT        4
#define BIT_DQS_MODE_MASK               (BIT0|BIT1|BIT2)
#define BIT_DQS_MDOE_SHIFT              0
#define BIT_DQS_MODE_2T                 (0 << BIT_DQS_MDOE_SHIFT)
#define BIT_DQS_MODE_1_5T               (1 << BIT_DQS_MDOE_SHIFT)
#define BIT_DQS_MODE_2_5T               (2 << BIT_DQS_MDOE_SHIFT)
#define BIT_DQS_MODE_1T                 (3 << BIT_DQS_MDOE_SHIFT)

#define reg_emmcpll_0x6d                GET_REG_ADDR(EMMC_PLL_BASE, 0x6d)

#define reg_emmcpll_0x6f                GET_REG_ADDR(EMMC_PLL_BASE, 0x6f)


//---------------------------------pad top-------------------------------------
#define REG_BANK_PADTOP                 0x1280  // (0x1025 - 0x1000) x 80h
#define PADTOP_BASE                     GET_REG_ADDR(RIU_BASE, REG_BANK_PADTOP)

#define reg_nand_gpio                   GET_REG_ADDR(PADTOP_BASE, 0x44)
#define BIT_NAND_GPIO_OEN_12            BIT5
#define BIT_NAND_GPIO_OUT_12            BIT4

#define eMMC_RST_L()                    {REG_FCIE_SETBIT(reg_chiptop_0x36, BIT_reg_emmc_rstz_en);\
                                         REG_FCIE_CLRBIT(reg_chiptop_0x36, BIT_reg_emmc_rstz_sw);\
                                         REG_FCIE_SETBIT(FCIE_BOOT_CONFIG, BIT_EMMC_RSTZ_EN);\
                                         REG_FCIE_CLRBIT(FCIE_BOOT_CONFIG, BIT_EMMC_RSTZ);\
                                         REG_FCIE_CLRBIT(reg_nand_gpio, BIT_NAND_GPIO_OEN_12);\
                                         REG_FCIE_CLRBIT(reg_nand_gpio, BIT_NAND_GPIO_OUT_12);}

#define eMMC_RST_H()                    {REG_FCIE_SETBIT(reg_chiptop_0x36, BIT_reg_emmc_rstz_en);\
                                         REG_FCIE_SETBIT(reg_chiptop_0x36, BIT_reg_emmc_rstz_sw);\
                                         REG_FCIE_SETBIT(FCIE_BOOT_CONFIG, BIT_EMMC_RSTZ_EN);\
                                         REG_FCIE_SETBIT(FCIE_BOOT_CONFIG, BIT_EMMC_RSTZ);\
                                         REG_FCIE_CLRBIT(reg_nand_gpio, BIT_NAND_GPIO_OEN_12);\
                                         REG_FCIE_SETBIT(reg_nand_gpio, BIT_NAND_GPIO_OUT_12);}

#define REG_BANK_TIMER1                 0x1800
#define TIMER1_BASE                     GET_REG_ADDR(RIU_PM_BASE, REG_BANK_TIMER1)

#define TIMER1_ENABLE                   GET_REG_ADDR(TIMER1_BASE, 0x20)
#define TIMER1_HIT                      GET_REG_ADDR(TIMER1_BASE, 0x21)
#define TIMER1_MAX_LOW                  GET_REG_ADDR(TIMER1_BASE, 0x22)
#define TIMER1_MAX_HIGH                 GET_REG_ADDR(TIMER1_BASE, 0x23)
#define TIMER1_CAP_LOW                  GET_REG_ADDR(TIMER1_BASE, 0x24)
#define TIMER1_CAP_HIGH                 GET_REG_ADDR(TIMER1_BASE, 0x25)

//=====================================================
// API declarations
//=====================================================
extern  U32 eMMC_hw_timer_delay(U32 u32us);
extern  U32 eMMC_hw_timer_sleep(U32 u32ms);
#define eMMC_HW_TIMER_HZ                12000000    //12MHz  [FIXME]

extern void *eMMC_memset (void *str, int c, unsigned int len);
extern void *eMMC_memcpy (void *destaddr, void const *srcaddr, unsigned int len);

#define memset(str, c, len) eMMC_memset(str, c, len)
#define memcpy(destaddr, srcaddr, len) eMMC_memcpy(destaddr, srcaddr, len)


////////////////////////////////////////////////////////////////////////////////////////////////////

// define what latch method (mode) fcie has
// implement switch pad function with below cases

#define FCIE_eMMC_BYPASS                0
#define FCIE_MODE_GPIO_PAD_DEFO_SPEED   FCIE_eMMC_BYPASS
#define EMMC_DEFO_SPEED_MODE            FCIE_MODE_GPIO_PAD_DEFO_SPEED
#define FCIE_eMMC_SDR                   1
#define FCIE_eMMC_DDR                   2
#define FCIE_eMMC_DDR_8BIT_MACRO        FCIE_eMMC_DDR
#define FCIE_eMMC_HS200                 3
#define FCIE_eMMC_HS400                 4
#define FCIE_eMMC_HS400_5_1             5

// define what speed we want this chip/project run
//------------------------------
// DDR48, DDR52, HS200, HS400
#define ENABLE_eMMC_ATOP                0

#define eMMC_IF_TUNING_TTABLE()         (g_eMMCDrv.u32_DrvFlag&DRV_FLAG_TUNING_TTABLE)

////////////////////////////////////////////////////////////////////////////////////////////////////
extern  U32 eMMC_pads_switch(U32 u32_FCIE_IF_Type);
extern  U32 eMMC_clock_setting(U16 u16_ClkParam);
extern void eMMC_set_WatchDog(U8 u8_IfEnable);
extern void eMMC_reset_WatchDog(void);
extern  U32 eMMC_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt);
extern void eMMC_flush_data_cache_buffer(U32 u32_DMAAddr, U32 u32_ByteCnt);
extern void eMMC_Invalidate_data_cache_buffer(U32 u32_DMAAddr, U32 u32_ByteCnt);
extern void eMMC_flush_miu_pipe(void);
extern  U32 eMMC_PlatformResetPre(void);
extern  U32 eMMC_PlatformResetPost(void);
extern  U32 eMMC_PlatformInit(void);
extern  U32 eMMC_CheckIfMemCorrupt(void);
extern void eMMC_DumpPadClk(void);

#define eMMC_BOOT_PART_W                BIT0
#define eMMC_BOOT_PART_R                BIT1

extern  U32 eMMC_BootPartitionHandler_WR(U8 *pDataBuf, U16 u16_PartType, U32 u32_StartSector, U32 u32_SectorCnt, U8 u8_OP);
extern  U32 eMMC_BootPartitionHandler_E(U16 u16_PartType);
extern  U32 eMMC_hw_timer_start(void);
extern  U32 eMMC_hw_timer_tick(void);
extern  U32 eMMC_FCIE_ResetToHS400(U8 u8_ClkParam);


//=====================================================
// partitions config
//=====================================================
// every blk is 512 bytes (reserve 2MB-64KB for internal use)
#define eMMC_DRV_RESERVED_BLK_CNT       ((0x200000-0x10000)/0x200)

#define eMMC_CIS_NNI_BLK_CNT            2
#define eMMC_CIS_PNI_BLK_CNT            2
#define eMMC_TEST_BLK_CNT               (0x100000/0x200)    // 1MB

#define eMMC_CIS_BLK_0                  (64*1024/512)       // from 64KB
#define eMMC_NNI_BLK_0                  (eMMC_CIS_BLK_0+0)
#define eMMC_NNI_BLK_1                  (eMMC_CIS_BLK_0+1)
#define eMMC_PNI_BLK_0                  (eMMC_CIS_BLK_0+2)
#define eMMC_PNI_BLK_1                  (eMMC_CIS_BLK_0+3)
#define eMMC_DDRTABLE_BLK_0             (eMMC_CIS_BLK_0+4)
#define eMMC_DDRTABLE_BLK_1             (eMMC_CIS_BLK_0+5)
#define eMMC_ALLRSP_BLK_0               (eMMC_CIS_BLK_0+12)
#define eMMC_ALLRSP_BLK_1               (eMMC_CIS_BLK_0+13)
#define eMMC_BURST_LEN_BLK_0            (eMMC_CIS_BLK_0+14)
#define eMMC_LIFE_TEST_BYTE_CNT_BLK     (eMMC_CIS_BLK_0+15)

#define eMMC_CIS_BLK_END                eMMC_LIFE_TEST_BYTE_CNT_BLK
// last 1MB in reserved area, use for eMMC test
#define eMMC_TEST_BLK_0                 (eMMC_CIS_BLK_END+1)
#define eMMC_TOTAL_RESERVED_BLK_CNT     (eMMC_DRV_RESERVED_BLK_CNT+eMMC_CIS_BLK_0)


//=====================================================
// Driver configs
//=====================================================
#define eMMC_UPDATE_FIRMWARE            0

#define eMMC_ST_PLAT                    0x80000000
// [CAUTION]: to verify IP and HAL code, defaut 0
#define IF_IP_VERIFY                    0   // [FIXME] -->

// need to eMMC_pads_switch
// need to eMMC_clock_setting
#define IF_FCIE_SHARE_IP                0

//------------------------------
#define FICE_BYTE_MODE_ENABLE           1   // always 1
#define ENABLE_eMMC_INTERRUPT_MODE      0
#define ENABLE_eMMC_RIU_MODE            0   // for debug cache issue

//------------------------------
#define eMMC_FEATURE_RELIABLE_WRITE     0
#if eMMC_UPDATE_FIRMWARE
#undef  eMMC_FEATURE_RELIABLE_WRITE
#define eMMC_FEATURE_RELIABLE_WRITE     0
#endif

//------------------------------
#define eMMC_RSP_FROM_RAM               0
#define eMMC_BURST_LEN_AUTOCFG          0
//------------------------------
#define eMMC_SECTOR_BUF_BYTECTN         eMMC_SECTOR_BUF_16KB
extern U8 gau8_eMMC_SectorBuf[];
extern U8 gau8_eMMC_PartInfoBuf[];


//=====================================================
// debug option
//=====================================================
#define eMMC_TEST_IN_DESIGN             0   // [FIXME]: set 1 to verify HW timer

#ifndef eMMC_DEBUG_MSG
#define eMMC_DEBUG_MSG                  0
#endif

/* Define trace levels. */
#define eMMC_DEBUG_LEVEL_ERROR          (1) /* Error condition debug messages. */
#define eMMC_DEBUG_LEVEL_WARNING        (2) /* Warning condition debug messages. */
#define eMMC_DEBUG_LEVEL_HIGH           (3) /* Debug messages (high debugging). */
#define eMMC_DEBUG_LEVEL_MEDIUM         (4) /* Debug messages. */
#define eMMC_DEBUG_LEVEL_LOW            (5) /* Debug messages (low debugging). */

/* Higer debug level means more verbose */
#ifndef eMMC_DEBUG_LEVEL
#define eMMC_DEBUG_LEVEL                eMMC_DEBUG_LEVEL_WARNING
#endif

#if defined(eMMC_DEBUG_MSG) && eMMC_DEBUG_MSG
#define eMMC_printf                     printf  // <-- [FIXME]
#define eMMC_debug(dbg_lv, tag, str, ...)\
    do {\
        if (dbg_lv > eMMC_DEBUG_LEVEL)\
            break;\
        else if(eMMC_IF_DISABLE_LOG())\
            break;\
        else {\
            if (tag)\
                eMMC_printf("[ %s() ] ", __func__);\
            \
            eMMC_printf(str, ##__VA_ARGS__);\
        }\
    } while(0)
#else   /* eMMC_DEBUG_MSG */
#define eMMC_printf(...)
#define eMMC_debug(enable, tag, str, ...)   do{}while(0)
#endif  /* eMMC_DEBUG_MSG */

#define eMMC_die(msg) do {						\
		while(1);						\
	} while(0)

#define eMMC_pause()\
    do {\
        printf("delay ");\
        eMMC_hw_timer_delay(HW_TIMER_DELAY_1s);\
        printf("1 ");\
        eMMC_hw_timer_delay(HW_TIMER_DELAY_1s);\
        printf("2 ");\
        eMMC_hw_timer_delay(HW_TIMER_DELAY_1s);\
        printf("3 ");\
        eMMC_hw_timer_delay(HW_TIMER_DELAY_1s);\
        printf("4 ");\
        eMMC_hw_timer_delay(HW_TIMER_DELAY_1s);\
        printf("5 ");\
        eMMC_hw_timer_delay(HW_TIMER_DELAY_1s);\
        printf("6\n");\
        eMMC_hw_timer_delay(HW_TIMER_DELAY_1s);\
    } while(0)

#define eMMC_stop() \
    while(1) eMMC_reset_WatchDog();

//=====================================================
// unit for HW Timer delay (unit of us)
//=====================================================
#define HW_TIMER_DELAY_1us              1
#define HW_TIMER_DELAY_5us              5
#define HW_TIMER_DELAY_10us             10
#define HW_TIMER_DELAY_100us            100
#define HW_TIMER_DELAY_500us            500
#define HW_TIMER_DELAY_1ms              (1000 * HW_TIMER_DELAY_1us)
#define HW_TIMER_DELAY_5ms              (5    * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_10ms             (10   * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_100ms            (100  * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_500ms            (500  * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_1s               (1000 * HW_TIMER_DELAY_1ms)

//=====================================================
// set FCIE clock
//=====================================================
#define FCIE_SLOWEST_CLK                BIT_FCIE_CLK_4X_300K
#define FCIE_SLOW_CLK                   BIT_FCIE_CLK_4X_XTAL
#define FCIE_DEFAULT_CLK                BIT_FCIE_CLK_4X_192M

#define eMMC_PLL_CLK_SLOW               BIT_FCIE_CLK_4X_XTAL
#define eMMC_PLL_CLK_FAST               BIT_FCIE_CLK_4X_192M

//=====================================================
// transfer DMA Address
//=====================================================
#define MIU_BUS_WIDTH_BITS              4 // Need to confirm

/*
 * Important:
 * The following buffers should be large enough for a whole eMMC block
 */
// FIXME, this is only for verifing IP
#define DMA_W_ADDR                      0x20400000
#define DMA_R_ADDR                      0x22500000
#define DMA_W_DDR_ADDR                  0x20600000
#define DMA_R_DDR_ADDR                  0x20700000
#define DMA_MBOOT_ADDR                  0x23000000

#define eMMC_FCIE_CLK_DIS()             {REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);\
                                         REG_FCIE_CLRBIT(FCIE_DDR_MODE, BIT_8BIT_MACRO_EN);}

//=====================================================
// misc
//=====================================================
//#define BIG_ENDIAN
#define LITTLE_ENDIAN

#endif /* __eMMC_K6_UBOOT__ */

#else

#endif
