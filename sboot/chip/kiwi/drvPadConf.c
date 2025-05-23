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
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (��MStar Confidential Information��) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
///////////////////////////////////////////////////////////////////////////////

//#include <stdio.h>

#include "datatype.h"
#include "drvRIU.h"
#include "Board.h"
//#include "c_riubase.h"
#include "hwreg_KIWI.h"
#include "chip/bond.h"

#define GPIO_NONE               0       // Not GPIO pin (default)
#define GPIO_IN                 1       // GPI
#define GPIO_OUT_LOW            2       // GPO output low
#define GPIO_OUT_HIGH           3       // GPO output high

#if defined (MIPS_CHAKRA) || defined(MSOS_TYPE_LINUX)
#define _MapBase_nonPM_KIWI (RIU_MAP + 0x200000)
#define _MapBase_PM_KIWI    RIU_MAP
#else
#define _MapBase_nonPM_KIWI 0xA0200000
#define _MapBase_PM_KIWI   0xA0000000
#endif

#define _MEMMAP_nonPM_  _RVM1(0x0000, 0x10, 0xFF)
#define _MEMMAP_PM_     _RVM1(0x0000, 0x00, 0xFF)

#define _END_OF_TBL2_       0xFF, 0xFF

const U8 padInitTbl[] =
{
    0x39, 0xB6, 0x5B, 0x53,     // magic code for ISP_Tool

    // programable device number
    // spi flash count
    1 + (PIN_SPI_CZ1 != 0) + (PIN_SPI_CZ2 != 0) + (PIN_SPI_CZ3 != 0),
    0x00,                       // nor
    0x00,                       // nand
    0x00,                       // reserved
    0x00,                       // reserved
    0x00,                       // reserved

#if (ENABLE_MSTAR_KIWI_FPGA==1)
    _END_OF_TBL2_
#else
//---------------------------------------------------------------------
// GPIO Configuartion
//---------------------------------------------------------------------    
    _MEMMAP_nonPM_,

    #if(PAD_SPDIF_OUT_IS_GPIO != GPIO_NONE)
    #define PAD_SPDIF_OUT_OEN (PAD_SPDIF_OUT_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_SPDIF_OUT_OUT (PAD_SPDIF_OUT_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x262c, PAD_SPDIF_OUT_OUT, BIT4),
    _RVM1(0x262d, PAD_SPDIF_OUT_OEN, BIT0),
    //reg_allpad_in 
    _RVM1(0x1ea0, 0, BIT15 ),   //reg[101ea0]#15 = 0b
    //reg_spdif_out_mode 
    _RVM1(0x1e0a, 0, BIT6|BIT5|BIT4 ),   //reg[101e0a]#6~#4 = 000b
    #endif

    #if(PAD_PWR_CTRL_IS_GPIO != GPIO_NONE)
    #define PAD_PWR_CTRL_OEN (PAD_PWR_CTRL_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_PWR_CTRL_OUT (PAD_PWR_CTRL_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25ae, PAD_PWR_CTRL_OUT, BIT4),
    _RVM1(0x25ae, PAD_PWR_CTRL_OEN, BIT5),
    //reg_allpad_in 
    _RVM1(0x1ea0, 0, BIT15),   //reg[101ea0]#15 = 0b
    //reg_test_in_mode 
    _RVM1(0x1e24, 0, BIT2|BIT1|BIT0 ),   //reg[101e24]#2~#0 = 000b
    //reg_test_out_mode 
    _RVM1(0x1e24, 0, BIT6|BIT5|BIT4 ),   //reg[101e24]#6~#4 = 000b
    #endif

    #if(PAD_TM_GPIO0_IS_GPIO != GPIO_NONE)
    #define PAD_TM_GPIO0_OEN (PAD_TM_GPIO0_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TM_GPIO0_OUT (PAD_TM_GPIO0_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25e6, PAD_TM_GPIO0_OUT, BIT4),
    _RVM1(0x25e6, PAD_TM_GPIO0_OEN, BIT5),
    //reg_SecondUARTMode
    _RVM1(0x1e06, 0, BIT2|BIT1|BIT0 ),   //reg[101e06]#2~#0 = 000b
    //reg_ThirdUARTMode 
    _RVM1(0x1e06, 0, BIT6|BIT5|BIT4 ),   //reg[101e06]#6~#4 = 000b
    //reg_ForthUARTMode 
    _RVM1(0x1e06, 0, BIT10|BIT9|BIT8 ),   //reg[101e06]#10~#8 = 000b
    //reg_FastUartMode 
    _RVM1(0x1e06, 0, BIT13|BIT12|BIT11 ),   //reg[101e06]#13~#11 = 000b
    #endif

    #if(PAD_TM_GPIO1_IS_GPIO != GPIO_NONE)
    #define PAD_TM_GPIO1_OEN (PAD_TM_GPIO1_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TM_GPIO1_OUT (PAD_TM_GPIO1_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25e8, PAD_TM_GPIO1_OUT, BIT4),
    _RVM1(0x25e8, PAD_TM_GPIO1_OEN, BIT5),
    //reg_SecondUARTMode
    _RVM1(0x1e06, 0, BIT2|BIT1|BIT0 ),   //reg[101e06]#2~#0 = 000b
    //reg_ThirdUARTMode 
    _RVM1(0x1e06, 0, BIT6|BIT5|BIT4 ),   //reg[101e06]#6~#4 = 000b
    //reg_ForthUARTMode 
    _RVM1(0x1e06, 0, BIT10|BIT9|BIT8 ),   //reg[101e06]#10~#8 = 000b
    //reg_FastUartMode 
    _RVM1(0x1e06, 0, BIT13|BIT12|BIT11 ),   //reg[101e06]#13~#11 = 000b
    #endif

    #if(PAD_SM0_SWITCH_IS_GPIO != GPIO_NONE)
    #define PAD_SM0_SWITCH_OEN (PAD_SM0_SWITCH_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SM0_SWITCH_OUT (PAD_SM0_SWITCH_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2500, PAD_SM0_SWITCH_OUT, BIT4),
    _RVM1(0x2500, PAD_SM0_SWITCH_OEN, BIT5),
    //reg_allpad_in 
    _RVM1(0x1ea0, 0, BIT15 ),   //reg[101ea0]#15 = 0b
    //reg_test_in_mode 
    _RVM1(0x1e24, 0, BIT2|BIT1|BIT0 ),   //reg[101e24]#2~#0 = 000b
    //reg_test_out_mode 
    _RVM1(0x1e24, 0, BIT6|BIT5|BIT4 ),   //reg[101e24]#6~#4 = 000b
    //reg_i2s_in_mode 
    _RVM1(0x1e0a, 0, BIT9|BIT8 ),   //reg[101e0a]#9~#8 = 00b
    #endif

    #if(PAD_SM0_IO_IS_GPIO != GPIO_NONE)
    #define PAD_SM0_IO_OEN (PAD_SM0_IO_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SM0_IO_OUT (PAD_SM0_IO_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2502, PAD_SM0_IO_OUT, BIT4),
    _RVM1(0x2502, PAD_SM0_IO_OEN, BIT5),
    //chip_config != 1
    //chip_config != 2
    //chip_config != 3
    //reg_allpad_in 
    _RVM1(0x1ea0, 0, BIT15 ),   //reg[101ea0]#15 = 0b
    //reg_test_in_mode 
    _RVM1(0x1e24, 0, BIT2|BIT1|BIT0 ),   //reg[101e24]#2~#0 = 000b
    //reg_test_out_mode 
    _RVM1(0x1e24, 0, BIT6|BIT5|BIT4 ),   //reg[101e24]#6~#4 = 000b
    //reg_sm0_en 
    _RVM1(0x1e19, 0, BIT0 ),   //reg[101e19]#0 = 0b
    //reg_i2cm0_mode
    _RVM1(0x1e12, 0, BIT1|BIT0 ),   //reg[101e12]#1~#0 = 00b
    //reg_i2cm1_mode
    _RVM1(0x1e12, 0, BIT6|BIT5|BIT4 ),   //reg[101e12]#6~#4 = 000b
    //reg_i2s_in_mode 
    _RVM1(0x1e0a, 0, BIT9|BIT8 ),   //reg[101e0a]#9~#8 = 00b
    //reg_SecondUARTMode
    _RVM1(0x1e06, 0, BIT2|BIT1|BIT0 ),   //reg[101e06]#2~#0 = 000b
    //reg_ThirdUARTMode 
    _RVM1(0x1e06, 0, BIT6|BIT5|BIT4 ),   //reg[101e06]#6~#4 = 000b
    //reg_ForthUARTMode 
    _RVM1(0x1e06, 0, BIT10|BIT9|BIT8 ),   //reg[101e06]#10~#8 = 000b
    //reg_FastUartMode 
    _RVM1(0x1e06, 0, BIT13|BIT12|BIT11 ),   //reg[101e06]#13~#11 = 000b
    #endif

    #if(PAD_SM0_CLK_IS_GPIO != GPIO_NONE)
    #define PAD_SM0_CLK_OEN (PAD_SM0_CLK_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SM0_CLK_OUT (PAD_SM0_CLK_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2504, PAD_SM0_CLK_OUT, BIT4),
    _RVM1(0x2504, PAD_SM0_CLK_OEN, BIT5),
    //chip_config != 1
    //chip_config != 2
    //chip_config != 3
    //reg_allpad_in 
    _RVM1(0x1ea0, 0, BIT15 ),   //reg[101ea0]#15 = 0b
    //reg_test_in_mode 
    _RVM1(0x1e24, 0, BIT2|BIT1|BIT0 ),   //reg[101e24]#2~#0 = 000b
    //reg_test_out_mode 
    _RVM1(0x1e24, 0, BIT6|BIT5|BIT4 ),   //reg[101e24]#6~#4 = 000b
    //reg_sm0_open
    _RVM1(0x1e18, 0, BIT0 ),   //reg[101e18]#0 = 0b
    //reg_sm0_en 
    _RVM1(0x1e19, 0, BIT0 ),   //reg[101e19]#0 = 0b
    //reg_i2cm0_mode
    _RVM1(0x1e12, 0, BIT1|BIT0 ),   //reg[101e12]#1~#0 = 00b
    //reg_i2cm1_mode
    _RVM1(0x1e12, 0, BIT6|BIT5|BIT4 ),   //reg[101e12]#6~#4 = 000b
    //reg_i2s_in_mode 
    _RVM1(0x1e0a, 0, BIT9|BIT8 ),   //reg[101e0a]#9~#8 = 00b
    //reg_SecondUARTMode
    _RVM1(0x1e06, 0, BIT2|BIT1|BIT0 ),   //reg[101e06]#2~#0 = 000b
    //reg_ThirdUARTMode 
    _RVM1(0x1e06, 0, BIT6|BIT5|BIT4 ),   //reg[101e06]#6~#4 = 000b
    //reg_ForthUARTMode 
    _RVM1(0x1e06, 0, BIT10|BIT9|BIT8 ),   //reg[101e06]#10~#8 = 000b
    #endif

    #if(PAD_SM0_CD_IS_GPIO != GPIO_NONE)
    #define PAD_SM0_CD_OEN (PAD_SM0_CD_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SM0_CD_OUT (PAD_SM0_CD_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2506, PAD_SM0_CD_OUT, BIT4),
    _RVM1(0x2506, PAD_SM0_CD_OEN, BIT5),
    //chip_config != 1
    //chip_config != 2
    //chip_config != 3
    //reg_allpad_in 
    _RVM1(0x1ea0, 0, BIT15 ),   //reg[101ea0]#15 = 0b
    //reg_test_in_mode 
    _RVM1(0x1e24, 0, BIT2|BIT1|BIT0 ),   //reg[101e24]#2~#0 = 000b
    //reg_test_out_mode 
    _RVM1(0x1e24, 0, BIT6|BIT5|BIT4 ),   //reg[101e24]#6~#4 = 000b
    //reg_sm0_en 
    _RVM1(0x1e19, 0, BIT0 ),   //reg[101e19]#0 = 0b
    #endif

    #if(PAD_SM0_RST_IS_GPIO != GPIO_NONE)
    #define PAD_SM0_RST_OEN (PAD_SM0_RST_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SM0_RST_OUT (PAD_SM0_RST_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2508, PAD_SM0_RST_OUT, BIT4),
    _RVM1(0x2508, PAD_SM0_RST_OEN, BIT5),
    //chip_config != 1
    //chip_config != 2
    //chip_config != 3
    //reg_allpad_in 
    _RVM1(0x1ea0, 0, BIT15 ),   //reg[101ea0]#15 = 0b
    //reg_test_in_mode 
    _RVM1(0x1e24, 0, BIT2|BIT1|BIT0 ),   //reg[101e24]#2~#0 = 000b
    //reg_test_out_mode 
    _RVM1(0x1e24, 0, BIT6|BIT5|BIT4 ),   //reg[101e24]#6~#4 = 000b
    //reg_sm0_open
    _RVM1(0x1e18, 0, BIT0 ),   //reg[101e18]#0 = 0b
    //reg_sm0_en 
    _RVM1(0x1e19, 0, BIT0 ),   //reg[101e19]#0 = 0b
    #endif

    #if(PAD_SM0_VCC_IS_GPIO != GPIO_NONE)
    #define PAD_SM0_VCC_OEN (PAD_SM0_VCC_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SM0_VCC_OUT (PAD_SM0_VCC_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x250a, PAD_SM0_VCC_OUT, BIT4),
    _RVM1(0x250a, PAD_SM0_VCC_OEN, BIT5),
    //chip_config != 1
    //chip_config != 2
    //chip_config != 3
    //reg_allpad_in 
    _RVM1(0x1ea0, 0, BIT15 ),   //reg[101ea0]#15 = 0b
    //reg_test_in_mode 
    _RVM1(0x1e24, 0, BIT2|BIT1|BIT0 ),   //reg[101e24]#2~#0 = 000b
    //reg_test_out_mode 
    _RVM1(0x1e24, 0, BIT6|BIT5|BIT4 ),   //reg[101e24]#6~#4 = 000b
    //reg_ocp0_en
    _RVM1(0x1e1c, 0, BIT0 ),   //reg[101e1c]#0 = 0b
    //reg_sm0_en 
    _RVM1(0x1e19, 0, BIT0 ),   //reg[101e19]#0 = 0b
    #endif

    #if(PAD_S_GPIO0_IS_GPIO != GPIO_NONE)
    #define PAD_S_GPIO0_OEN (PAD_S_GPIO0_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_S_GPIO0_OUT (PAD_S_GPIO0_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x259c, PAD_S_GPIO0_OUT, BIT4),
    _RVM1(0x259c, PAD_S_GPIO0_OEN, BIT5),
    //chip_config != 3
    //chip_config != 7
    //reg_allpad_in 
    _RVM1(0x1ea0, 0, BIT15 ),   //reg[101ea0]#15 = 0b
    //reg_test_in_mode 
    _RVM1(0x1e24, 0, BIT2|BIT1|BIT0 ),   //reg[101e24]#2~#0 = 000b
    //reg_test_out_mode 
    _RVM1(0x1e24, 0, BIT6|BIT5|BIT4 ),   //reg[101e24]#6~#4 = 000b
    #endif

    #if(PAD_S_GPIO1_IS_GPIO != GPIO_NONE)
    #define PAD_S_GPIO1_OEN (PAD_S_GPIO1_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_S_GPIO1_OUT (PAD_S_GPIO1_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x259e, PAD_S_GPIO1_OUT, BIT4),
    _RVM1(0x259e, PAD_S_GPIO1_OEN, BIT5),
    //chip_config != 3
    //chip_config != 7
    //reg_allpad_in 
    _RVM1(0x1ea0, 0, BIT15 ),   //reg[101ea0]#15 = 0b
    //reg_test_in_mode 
    _RVM1(0x1e24, 0, BIT2|BIT1|BIT0 ),   //reg[101e24]#2~#0 = 000b
    //reg_test_out_mode 
    _RVM1(0x1e24, 0, BIT6|BIT5|BIT4 ),   //reg[101e24]#6~#4 = 000b
    //reg_i2s_out_mode 
    _RVM1(0x1e0b, 0, BIT5|BIT4 ),   //reg[101e0b]#5~#4 = 00b
    #endif

    #if(PAD_S_GPIO2_IS_GPIO != GPIO_NONE)
    #define PAD_S_GPIO2_OEN (PAD_S_GPIO2_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_S_GPIO2_OUT (PAD_S_GPIO2_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25a0, PAD_S_GPIO2_OUT, BIT4),
    _RVM1(0x25a0, PAD_S_GPIO2_OEN, BIT5),
    //chip_config != 3
    //chip_config != 7
    //reg_allpad_in 
    _RVM1(0x1ea0, 0, BIT15 ),   //reg[101ea0]#15 = 0b
    //reg_test_in_mode 
    _RVM1(0x1e24, 0, BIT2|BIT1|BIT0 ),   //reg[101e24]#2~#0 = 000b
    //reg_test_out_mode 
    _RVM1(0x1e24, 0, BIT6|BIT5|BIT4 ),   //reg[101e24]#6~#4 = 000b
    //reg_i2s_out_mode 
    _RVM1(0x1e0b, 0, BIT5|BIT4 ),   //reg[101e0b]#5~#4 = 00b
    //PE
    _RVM1(0x1e4a, BIT7 , BIT7 ),   //reg[101e4a]#7 = 1b
    #endif

    #if(PAD_S_GPIO3_IS_GPIO != GPIO_NONE)
    #define PAD_S_GPIO3_OEN (PAD_S_GPIO3_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_S_GPIO3_OUT (PAD_S_GPIO3_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25a2, PAD_S_GPIO3_OUT, BIT4),
    _RVM1(0x25a2, PAD_S_GPIO3_OEN, BIT5),
    //chip_config != 3
    //chip_config != 7
    //reg_allpad_in 
    _RVM1(0x1ea0, 0, BIT15 ),   //reg[101ea0]#15 = 0b
    //reg_test_in_mode 
    _RVM1(0x1e24, 0, BIT2|BIT1|BIT0 ),   //reg[101e24]#2~#0 = 000b
    //reg_test_out_mode 
    _RVM1(0x1e24, 0, BIT6|BIT5|BIT4 ),   //reg[101e24]#6~#4 = 000b
    //reg_i2s_out_mode 
    _RVM1(0x1e0b, 0, BIT5|BIT4 ),   //reg[101e0b]#5~#4 = 00b
    //PE
    _RVM1(0x1e4a, BIT12 , BIT12 ),   //reg[101e4a]#12 = 1b
    #endif

    #if(PAD_S_GPIO4_IS_GPIO != GPIO_NONE)
    #define PAD_S_GPIO4_OEN (PAD_S_GPIO4_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_S_GPIO4_OUT (PAD_S_GPIO4_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25a4, PAD_S_GPIO4_OUT, BIT4),
    _RVM1(0x25a4, PAD_S_GPIO4_OEN, BIT5),
    //chip_config != 3
    //chip_config != 7
    //reg_allpad_in 
    _RVM1(0x1ea0, 0, BIT15 ),   //reg[101ea0]#15 = 0b
    //reg_test_in_mode 
    _RVM1(0x1e24, 0, BIT2|BIT1|BIT0 ),   //reg[101e24]#2~#0 = 000b
    //reg_test_out_mode 
    _RVM1(0x1e24, 0, BIT6|BIT5|BIT4 ),   //reg[101e24]#6~#4 = 000b
    //reg_i2cm1_mode
    _RVM1(0x1e12, 0, BIT6|BIT5|BIT4 ),   //reg[101e12]#6~#4 = 000b
    //reg_i2cm2_mode = reg_pwm_mode
    _RVM1(0x1e13, 0, BIT2|BIT1|BIT0 ),   //reg[101e13]#2~#0 = 000b
    //reg_i2s_out_mode 
    _RVM1(0x1e0b, 0, BIT5|BIT4 ),   //reg[101e0b]#5~#4 = 00b
    //reg_SecondUARTMode
    _RVM1(0x1e06, 0, BIT2|BIT1|BIT0 ),   //reg[101e06]#2~#0 = 000b
    //reg_ThirdUARTMode 
    _RVM1(0x1e06, 0, BIT6|BIT5|BIT4 ),   //reg[101e06]#6~#4 = 000b
    //reg_ForthUARTMode 
    _RVM1(0x1e06, 0, BIT10|BIT9|BIT8 ),   //reg[101e06]#10~#8 = 000b
    //reg_FastUartMode 
    _RVM1(0x1e06, 0, BIT13|BIT12|BIT11 ),   //reg[101e06]#13~#11 = 000b
    #endif

    #if(PAD_S_GPIO5_IS_GPIO != GPIO_NONE)
    #define PAD_S_GPIO5_OEN (PAD_S_GPIO5_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_S_GPIO5_OUT (PAD_S_GPIO5_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25a6, PAD_S_GPIO5_OUT, BIT4),
    _RVM1(0x25a6, PAD_S_GPIO5_OEN, BIT5),
    //chip_config != 3
    //chip_config != 7
    //reg_allpad_in 
    _RVM1(0x1ea0, 0, BIT15 ),   //reg[101ea0]#15 = 0b
    //reg_test_in_mode 
    _RVM1(0x1e24, 0, BIT2|BIT1|BIT0 ),   //reg[101e24]#2~#0 = 000b
    //reg_test_out_mode 
    _RVM1(0x1e24, 0, BIT6|BIT5|BIT4 ),   //reg[101e24]#6~#4 = 000b
    //reg_i2cm1_mode
    _RVM1(0x1e12, 0, BIT6|BIT5|BIT4 ),   //reg[101e12]#6~#4 = 000b
    //reg_i2cm2_mode = reg_pwm_mode
    _RVM1(0x1e13, 0, BIT2|BIT1|BIT0 ),   //reg[101e13]#2~#0 = 000b
    //reg_i2s_out_mute_mode
    _RVM1(0x1e11, 0, BIT4 ),   //reg[101e11]#4 = 0b
    //reg_SecondUARTMode
    _RVM1(0x1e06, 0, BIT2|BIT1|BIT0 ),   //reg[101e06]#2~#0 = 000b
    //reg_ThirdUARTMode 
    _RVM1(0x1e06, 0, BIT6|BIT5|BIT4 ),   //reg[101e06]#6~#4 = 000b
    //reg_ForthUARTMode 
    _RVM1(0x1e06, 0, BIT10|BIT9|BIT8 ),   //reg[101e06]#10~#8 = 000b
    //reg_FastUartMode 
    _RVM1(0x1e06, 0, BIT13|BIT12|BIT11 ),   //reg[101e06]#13~#11 = 000b
    #endif

    #if(PAD_HSYNC_OUT_IS_GPIO != GPIO_NONE)
    #define PAD_HSYNC_OUT_OEN (PAD_HSYNC_OUT_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_HSYNC_OUT_OUT (PAD_HSYNC_OUT_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x263c, PAD_HSYNC_OUT_OUT, BIT4),
    _RVM1(0x263c, PAD_HSYNC_OUT_OEN, BIT5),
    //chip_config != 3
    //chip_config != 7
    //reg_allpad_in 
    _RVM1(0x1ea0, 0, BIT15 ),   //reg[101ea0]#15 = 0b
    //reg_test_in_mode 
    _RVM1(0x1e24, 0, BIT2|BIT1|BIT0 ),   //reg[101e24]#2~#0 = 000b
    //reg_test_out_mode 
    _RVM1(0x1e24, 0, BIT6|BIT5|BIT4 ),   //reg[101e24]#6~#4 = 000b
    //reg_i2cm1_mode
    _RVM1(0x1e12, 0, BIT6|BIT5|BIT4 ),   //reg[101e12]#6~#4 = 000b
    //reg_i2cm2_mode = reg_pwm_mode
    _RVM1(0x1e13, 0, BIT2|BIT1|BIT0 ),   //reg[101e13]#2~#0 = 000b
    //reg_spdif_in_mode 
    _RVM1(0x1e0a, 0, BIT2|BIT1|BIT0 ),   //reg[101e0a]#2~#0 = 000b
    //reg_hsync_en
    _RVM1(0x1e16, 0, BIT0 ),   //reg[101e16]#0 = 0b
    #endif

    #if(PAD_VSYNC_OUT_IS_GPIO != GPIO_NONE)
    #define PAD_VSYNC_OUT_OEN (PAD_VSYNC_OUT_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_VSYNC_OUT_OUT (PAD_VSYNC_OUT_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x263e, PAD_VSYNC_OUT_OUT, BIT4),
    _RVM1(0x263e, PAD_VSYNC_OUT_OEN, BIT5),
    //chip_config != 3
    //chip_config != 7
    //reg_allpad_in 
    _RVM1(0x1ea0, 0, BIT15 ),   //reg[101ea0]#15 = 0b
    //reg_test_in_mode 
    _RVM1(0x1e24, 0, BIT2|BIT1|BIT0 ),   //reg[101e24]#2~#0 = 000b
    //reg_test_out_mode 
    _RVM1(0x1e24, 0, BIT6|BIT5|BIT4 ),   //reg[101e24]#6~#4 = 000b
    //reg_i2cm1_mode
    _RVM1(0x1e12, 0, BIT6|BIT5|BIT4 ),   //reg[101e12]#6~#4 = 000b
    //reg_i2cm2_mode = reg_pwm_mode
    _RVM1(0x1e13, 0, BIT2|BIT1|BIT0 ),   //reg[101e13]#2~#0 = 000b
    //reg_spdif_out_mode 
    _RVM1(0x1e0a, 0, BIT6|BIT5|BIT4 ),   //reg[101e0a]#6~#4 = 000b
    //reg_vsync_en
    _RVM1(0x1e16, 0, BIT4 ),   //reg[101e16]#4 = 0b
    #endif

    #if(PAD_TS0_SYNC_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_SYNC_OEN (PAD_TS0_SYNC_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_SYNC_OUT (PAD_TS0_SYNC_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2524, PAD_TS0_SYNC_OUT, BIT4),
    _RVM1(0x2524, PAD_TS0_SYNC_OEN, BIT5),
    //reg_allpad_in 
    _RVM1(0x1ea0, 0, BIT15 ),   //reg[101ea0]#15 = 0b
    //reg_test_in_mode 
    _RVM1(0x1e24, 0, BIT2|BIT1|BIT0 ),   //reg[101e24]#2~#0 = 000b
    //reg_test_out_mode 
    _RVM1(0x1e24, 0, BIT6|BIT5|BIT4 ),   //reg[101e24]#6~#4 = 000b
    //reg_i2cm1_mode
    _RVM1(0x1e12, 0, BIT6|BIT5|BIT4 ),   //reg[101e12]#6~#4 = 000b
    //reg_i2cm2_mode = reg_pwm_mode
    _RVM1(0x1e13, 0, BIT2|BIT1|BIT0 ),   //reg[101e13]#2~#0 = 000b
    //reg_ts0_mode
    _RVM1(0x1e04, 0, BIT3|BIT2|BIT1|BIT0 ),   //reg[101e04]#3~#0 = 0000b
    #endif

    #if(PAD_TS0_VLD_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_VLD_OEN (PAD_TS0_VLD_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_VLD_OUT (PAD_TS0_VLD_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2526, PAD_TS0_VLD_OUT, BIT4),
    _RVM1(0x2526, PAD_TS0_VLD_OEN, BIT5),
    //reg_allpad_in 
    _RVM1(0x1ea0, 0, BIT15 ),   //reg[101ea0]#15 = 0b
    //reg_test_in_mode 
    _RVM1(0x1e24, 0, BIT2|BIT1|BIT0 ),   //reg[101e24]#2~#0 = 000b
    //reg_test_out_mode 
    _RVM1(0x1e24, 0, BIT6|BIT5|BIT4 ),   //reg[101e24]#6~#4 = 000b
    //reg_i2cm1_mode
    _RVM1(0x1e12, 0, BIT6|BIT5|BIT4 ),   //reg[101e12]#6~#4 = 000b
    //reg_i2cm2_mode = reg_pwm_mode
    _RVM1(0x1e13, 0, BIT2|BIT1|BIT0 ),   //reg[101e13]#2~#0 = 000b
    //reg_ts0_mode
    _RVM1(0x1e04, 0, BIT3|BIT2|BIT1|BIT0 ),   //reg[101e04]#3~#0 = 0000b
    //reg_SecondUARTMode
    _RVM1(0x1e06, 0, BIT2|BIT1|BIT0 ),   //reg[101e06]#2~#0 = 000b
    //reg_ThirdUARTMode 
    _RVM1(0x1e06, 0, BIT6|BIT5|BIT4 ),   //reg[101e06]#6~#4 = 000b
    //reg_ForthUARTMode 
    _RVM1(0x1e06, 0, BIT10|BIT9|BIT8 ),   //reg[101e06]#10~#8 = 000b
    //reg_FastUartMode 
    _RVM1(0x1e06, 0, BIT13|BIT12|BIT11 ),   //reg[101e06]#13~#11 = 000b
    #endif

    #if(PAD_TS0_D0_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D0_OEN (PAD_TS0_D0_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_D0_OUT (PAD_TS0_D0_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2528, PAD_TS0_D0_OUT, BIT4),
    _RVM1(0x2528, PAD_TS0_D0_OEN, BIT5),
    //reg_allpad_in 
    _RVM1(0x1ea0, 0, BIT15 ),   //reg[101ea0]#15 = 0b
    //reg_test_in_mode 
    _RVM1(0x1e24, 0, BIT2|BIT1|BIT0 ),   //reg[101e24]#2~#0 = 000b
    //reg_test_out_mode 
    _RVM1(0x1e24, 0, BIT6|BIT5|BIT4 ),   //reg[101e24]#6~#4 = 000b
    //reg_ts0_mode
    _RVM1(0x1e04, 0, BIT3|BIT2|BIT1|BIT0 ),   //reg[101e04]#3~#0 = 0000b
    //reg_SecondUARTMode
    _RVM1(0x1e06, 0, BIT2|BIT1|BIT0 ),   //reg[101e06]#2~#0 = 000b
    //reg_ThirdUARTMode 
    _RVM1(0x1e06, 0, BIT6|BIT5|BIT4 ),   //reg[101e06]#6~#4 = 000b
    //reg_ForthUARTMode 
    _RVM1(0x1e06, 0, BIT10|BIT9|BIT8 ),   //reg[101e06]#10~#8 = 000b
    //reg_FastUartMode 
    _RVM1(0x1e06, 0, BIT13|BIT12|BIT11 ),   //reg[101e06]#13~#11 = 000b
    #endif

    #if(PAD_TS0_D1_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D1_OEN (PAD_TS0_D1_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_D1_OUT (PAD_TS0_D1_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x252a, PAD_TS0_D1_OUT, BIT4),
    _RVM1(0x252a, PAD_TS0_D1_OEN, BIT5),
    //reg_allpad_in 
    _RVM1(0x1ea0, 0, BIT15 ),   //reg[101ea0]#15 = 0b
    //reg_test_in_mode 
    _RVM1(0x1e24, 0, BIT2|BIT1|BIT0 ),   //reg[101e24]#2~#0 = 000b
    //reg_test_out_mode 
    _RVM1(0x1e24, 0, BIT6|BIT5|BIT4 ),   //reg[101e24]#6~#4 = 000b
    //reg_ts0_mode
    _RVM1(0x1e04, 0, BIT3|BIT2|BIT1|BIT0 ),   //reg[101e04]#3~#0 = 0000b
    //reg_ts1_mode
    _RVM1(0x1e04, 0, BIT6|BIT5|BIT4 ),   //reg[101e04]#6~#4 = 0000b
    #endif

    #if(PAD_TS0_D2_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D2_OEN (PAD_TS0_D2_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_D2_OUT (PAD_TS0_D2_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x252c, PAD_TS0_D2_OUT, BIT4),
    _RVM1(0x252c, PAD_TS0_D2_OEN, BIT5),
    //reg_allpad_in 
    _RVM1(0x1ea0, 0, BIT15 ),   //reg[101ea0]#15 = 0b
    //reg_test_in_mode 
    _RVM1(0x1e24, 0, BIT2|BIT1|BIT0 ),   //reg[101e24]#2~#0 = 000b
    //reg_test_out_mode 
    _RVM1(0x1e24, 0, BIT6|BIT5|BIT4 ),   //reg[101e24]#6~#4 = 000b
    //reg_ts0_mode
    _RVM1(0x1e04, 0, BIT3|BIT2|BIT1|BIT0 ),   //reg[101e04]#3~#0 = 0000b
    //reg_ts1_mode
    _RVM1(0x1e04, 0, BIT6|BIT5|BIT4 ),   //reg[101e04]#6~#4 = 0000b
    #endif

    #if(PAD_TS0_D3_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D3_OEN (PAD_TS0_D3_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_D3_OUT (PAD_TS0_D3_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x252e, PAD_TS0_D3_OUT, BIT4),
    _RVM1(0x252e, PAD_TS0_D3_OEN, BIT5),
    //reg_allpad_in 
    _RVM1(0x1ea0, 0, BIT15 ),   //reg[101ea0]#15 = 0b
    //reg_test_in_mode 
    _RVM1(0x1e24, 0, BIT2|BIT1|BIT0 ),   //reg[101e24]#2~#0 = 000b
    //reg_test_out_mode 
    _RVM1(0x1e24, 0, BIT6|BIT5|BIT4 ),   //reg[101e24]#6~#4 = 000b
    //reg_ts0_mode
    _RVM1(0x1e04, 0, BIT3|BIT2|BIT1|BIT0 ),   //reg[101e04]#3~#0 = 0000b
    //reg_ts1_mode
    _RVM1(0x1e04, 0, BIT6|BIT5|BIT4 ),   //reg[101e04]#6~#4 = 0000b
    #endif

    #if(PAD_TS0_D4_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D4_OEN (PAD_TS0_D4_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_D4_OUT (PAD_TS0_D4_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2530, PAD_TS0_D4_OUT, BIT4),
    _RVM1(0x2530, PAD_TS0_D4_OEN, BIT5),
    //reg_allpad_in 
    _RVM1(0x1ea0, 0, BIT15 ),   //reg[101ea0]#15 = 0b
    //reg_test_in_mode 
    _RVM1(0x1e24, 0, BIT2|BIT1|BIT0 ),   //reg[101e24]#2~#0 = 000b
    //reg_test_out_mode 
    _RVM1(0x1e24, 0, BIT6|BIT5|BIT4 ),   //reg[101e24]#6~#4 = 000b
    //reg_ts0_mode
    _RVM1(0x1e04, 0, BIT3|BIT2|BIT1|BIT0 ),   //reg[101e04]#3~#0 = 0000b
    //reg_ts1_mode
    _RVM1(0x1e04, 0, BIT6|BIT5|BIT4 ),   //reg[101e04]#6~#4 = 0000b
    #endif

    #if(PAD_TS0_D5_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D5_OEN (PAD_TS0_D5_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_D5_OUT (PAD_TS0_D5_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2532, PAD_TS0_D5_OUT, BIT4),
    _RVM1(0x2532, PAD_TS0_D5_OEN, BIT5),
    //reg_allpad_in 
    _RVM1(0x1ea0, 0, BIT15 ),   //reg[101ea0]#15 = 0b
    //reg_test_in_mode 
    _RVM1(0x1e24, 0, BIT2|BIT1|BIT0 ),   //reg[101e24]#2~#0 = 000b
    //reg_test_out_mode 
    _RVM1(0x1e24, 0, BIT6|BIT5|BIT4 ),   //reg[101e24]#6~#4 = 000b
    //reg_ts0_mode
    _RVM1(0x1e04, 0, BIT3|BIT2|BIT1|BIT0 ),   //reg[101e04]#3~#0 = 0000b
    #endif

    #if(PAD_TS0_D6_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D6_OEN (PAD_TS0_D6_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_D6_OUT (PAD_TS0_D6_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2534, PAD_TS0_D6_OUT, BIT4),
    _RVM1(0x2534, PAD_TS0_D6_OEN, BIT5),
    //chip_config != 4
    //reg_allpad_in 
    _RVM1(0x1ea0, 0, BIT15 ),   //reg[101ea0]#15 = 0b
    //reg_test_in_mode 
    _RVM1(0x1e24, 0, BIT2|BIT1|BIT0 ),   //reg[101e24]#2~#0 = 000b
    //reg_test_out_mode 
    _RVM1(0x1e24, 0, BIT6|BIT5|BIT4 ),   //reg[101e24]#6~#4 = 000b
    //reg_ts0_mode
    _RVM1(0x1e04, 0, BIT3|BIT2|BIT1|BIT0 ),   //reg[101e04]#3~#0 = 0000b
    #endif

    #if(PAD_TS0_D7_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D7_OEN (PAD_TS0_D7_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_D7_OUT (PAD_TS0_D7_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2536, PAD_TS0_D7_OUT, BIT4),
    _RVM1(0x2536, PAD_TS0_D7_OEN, BIT5),
    //chip_config != 4
    //reg_allpad_in 
    _RVM1(0x1ea0, 0, BIT15 ),   //reg[101ea0]#15 = 0b
    //reg_test_in_mode 
    _RVM1(0x1e24, 0, BIT2|BIT1|BIT0 ),   //reg[101e24]#2~#0 = 000b
    //reg_test_out_mode 
    _RVM1(0x1e24, 0, BIT6|BIT5|BIT4 ),   //reg[101e24]#6~#4 = 000b
    //reg_ts0_mode
    _RVM1(0x1e04, 0, BIT3|BIT2|BIT1|BIT0 ),   //reg[101e04]#3~#0 = 0000b
    #endif

    #if(PAD_TS0_CLK_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_CLK_OEN (PAD_TS0_CLK_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_CLK_OUT (PAD_TS0_CLK_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2538, PAD_TS0_CLK_OUT, BIT4),
    _RVM1(0x2538, PAD_TS0_CLK_OEN, BIT5),
    //chip_config != 4
    //reg_allpad_in 
    _RVM1(0x1ea0, 0, BIT15 ),   //reg[101ea0]#15 = 0b
    //reg_test_in_mode 
    _RVM1(0x1e24, 0, BIT2|BIT1|BIT0 ),   //reg[101e24]#2~#0 = 000b
    //reg_test_out_mode 
    _RVM1(0x1e24, 0, BIT6|BIT5|BIT4 ),   //reg[101e24]#6~#4 = 000b
    //reg_ts0_mode
    _RVM1(0x1e04, 0, BIT3|BIT2|BIT1|BIT0 ),   //reg[101e04]#3~#0 = 0000b
    #endif

    #if(PAD_I2CM1_SCL_IS_GPIO != GPIO_NONE)
    #define PAD_I2CM1_SCL_OEN (PAD_I2CM1_SCL_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2CM1_SCL_OUT (PAD_I2CM1_SCL_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2522, PAD_I2CM1_SCL_OUT, BIT4),
    _RVM1(0x2522, PAD_I2CM1_SCL_OEN, BIT5),
    //reg_allpad_in 
    _RVM1(0x1ea0, 0, BIT15 ),   //reg[101ea0]#15 = 0b
    //reg_test_in_mode 
    _RVM1(0x1e24, 0, BIT2|BIT1|BIT0 ),   //reg[101e24]#2~#0 = 000b
    //reg_test_out_mode 
    _RVM1(0x1e24, 0, BIT6|BIT5|BIT4 ),   //reg[101e24]#6~#4 = 000b
    //reg_i2cm0_mode
    _RVM1(0x1e12, 0, BIT1|BIT0 ),   //reg[101e12]#1~#0 = 00b
    //reg_i2cm1_mode
    _RVM1(0x1e12, 0, BIT6|BIT5|BIT4 ),   //reg[101e12]#6~#4 = 000b
    //reg_mcu_jtag_mode 
    _RVM1(0x1eaa, 0, BIT3|BIT2|BIT1|BIT0 ),   //reg[101eaa]#3~#0 = 0000b
    //reg_SecondUARTMode
    _RVM1(0x1e06, 0, BIT2|BIT1|BIT0 ),   //reg[101e06]#2~#0 = 000b
    //reg_ThirdUARTMode 
    _RVM1(0x1e06, 0, BIT6|BIT5|BIT4 ),   //reg[101e06]#6~#4 = 000b
    //reg_ForthUARTMode 
    _RVM1(0x1e06, 0, BIT10|BIT9|BIT8 ),   //reg[101e06]#10~#8 = 000b
    //reg_FastUartMode 
    _RVM1(0x1e06, 0, BIT13|BIT12|BIT11 ),   //reg[101e06]#13~#11 = 000b
    #endif

    #if(PAD_I2CM1_SDA_IS_GPIO != GPIO_NONE)
    #define PAD_I2CM1_SDA_OEN (PAD_I2CM1_SDA_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2CM1_SDA_OUT (PAD_I2CM1_SDA_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2520, PAD_I2CM1_SDA_OUT, BIT4),
    _RVM1(0x2520, PAD_I2CM1_SDA_OEN, BIT5),
    //reg_allpad_in 
    _RVM1(0x1ea0, 0, BIT15 ),   //reg[101ea0]#15 = 0b
    //reg_test_in_mode 
    _RVM1(0x1e24, 0, BIT2|BIT1|BIT0 ),   //reg[101e24]#2~#0 = 000b
    //reg_test_out_mode 
    _RVM1(0x1e24, 0, BIT6|BIT5|BIT4 ),   //reg[101e24]#6~#4 = 000b
    //reg_i2cm0_mode
    _RVM1(0x1e12, 0, BIT1|BIT0 ),   //reg[101e12]#1~#0 = 00b
    //reg_i2cm1_mode
    _RVM1(0x1e12, 0, BIT6|BIT5|BIT4 ),   //reg[101e12]#6~#4 = 000b
    //reg_mcu_jtag_mode 
    _RVM1(0x1eaa, 0, BIT3|BIT2|BIT1|BIT0 ),   //reg[101eaa]#3~#0 = 0000b
    //reg_SecondUARTMode
    _RVM1(0x1e06, 0, BIT2|BIT1|BIT0 ),   //reg[101e06]#2~#0 = 000b
    //reg_ThirdUARTMode 
    _RVM1(0x1e06, 0, BIT6|BIT5|BIT4 ),   //reg[101e06]#6~#4 = 000b
    //reg_ForthUARTMode 
    _RVM1(0x1e06, 0, BIT10|BIT9|BIT8 ),   //reg[101e06]#10~#8 = 000b
    //reg_FastUartMode 
    _RVM1(0x1e06, 0, BIT13|BIT12|BIT11 ),   //reg[101e06]#13~#11 = 000b
    #endif

    #if(PAD_I2CM0_SCL_IS_GPIO != GPIO_NONE)
    #define PAD_I2CM0_SCL_OEN (PAD_I2CM0_SCL_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2CM0_SCL_OUT (PAD_I2CM0_SCL_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x251e, PAD_I2CM0_SCL_OUT, BIT4),
    _RVM1(0x251e, PAD_I2CM0_SCL_OEN, BIT5),
    //reg_allpad_in 
    _RVM1(0x1ea0, 0, BIT15 ),   //reg[101ea0]#15 = 0b
    //reg_test_in_mode 
    _RVM1(0x1e24, 0, BIT2|BIT1|BIT0 ),   //reg[101e24]#2~#0 = 000b
    //reg_test_out_mode 
    _RVM1(0x1e24, 0, BIT6|BIT5|BIT4 ),   //reg[101e24]#6~#4 = 000b
    //reg_i2cm0_mode
    _RVM1(0x1e12, 0, BIT1|BIT0 ),   //reg[101e12]#1~#0 = 00b
    //reg_mcu_jtag_mode 
    _RVM1(0x1eaa, 0, BIT3|BIT2|BIT1|BIT0 ),   //reg[101eaa]#3~#0 = 0000b
    //reg_rf_agc_en
    _RVM1(0x1e3a, 0, BIT1 ),   //reg[101e3a]#1 = 0b
    #endif

    #if(PAD_I2CM0_SDA_IS_GPIO != GPIO_NONE)
    #define PAD_I2CM0_SDA_OEN (PAD_I2CM0_SDA_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2CM0_SDA_OUT (PAD_I2CM0_SDA_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x251c, PAD_I2CM0_SDA_OUT, BIT4),
    _RVM1(0x251c, PAD_I2CM0_SDA_OEN, BIT5),
    //reg_allpad_in 
    _RVM1(0x1ea0, 0, BIT15 ),   //reg[101ea0]#15 = 0b
    //reg_test_in_mode 
    _RVM1(0x1e24, 0, BIT2|BIT1|BIT0 ),   //reg[101e24]#2~#0 = 000b
    //reg_test_out_mode 
    _RVM1(0x1e24, 0, BIT6|BIT5|BIT4 ),   //reg[101e24]#6~#4 = 000b
    //reg_i2cm0_mode
    _RVM1(0x1e12, 0, BIT1|BIT0 ),   //reg[101e12]#1~#0 = 00b
    //reg_mcu_jtag_mode 
    _RVM1(0x1eaa, 0, BIT3|BIT2|BIT1|BIT0 ),   //reg[101eaa]#3~#0 = 0000b
    //reg_rf_agc_en
    _RVM1(0x1e3a, 0, BIT0 ),   //reg[101e3a]#0 = 0b
    #endif

    #if(PAD_ET_LINK_LED_IS_GPIO != GPIO_NONE)
    #define PAD_ET_LINK_LED_OEN (PAD_ET_LINK_LED_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_ET_LINK_LED_OUT (PAD_ET_LINK_LED_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25aa, PAD_ET_LINK_LED_OUT, BIT4),
    _RVM1(0x25aa, PAD_ET_LINK_LED_OEN, BIT5),
    //chip_config != 0
    //reg_allpad_in 
    _RVM1(0x1ea0, 0, BIT15 ),   //reg[101ea0]#15 = 0b
    //reg_SecondUARTMode
    _RVM1(0x1e06, 0, BIT2|BIT1|BIT0 ),   //reg[101e06]#2~#0 = 000b
    //reg_ThirdUARTMode 
    _RVM1(0x1e06, 0, BIT6|BIT5|BIT4 ),   //reg[101e06]#6~#4 = 000b
    //reg_ForthUARTMode 
    _RVM1(0x1e06, 0, BIT10|BIT9|BIT8 ),   //reg[101e06]#10~#8 = 000b
    //reg_FastUartMode 
    _RVM1(0x1e06, 0, BIT13|BIT12|BIT11 ),   //reg[101e06]#13~#11 = 000b
    //reg_led_mode
    _RVM1(0x1e10, 0, BIT7 ),   //reg[101e10]#7 = 0b
    #endif

    #if(PAD_ET_SPEED_LED_IS_GPIO != GPIO_NONE)
    #define PAD_ET_SPEED_LED_OEN (PAD_ET_SPEED_LED_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_ET_SPEED_LED_OUT (PAD_ET_SPEED_LED_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25ac, PAD_ET_SPEED_LED_OUT, BIT4),
    _RVM1(0x25ac, PAD_ET_SPEED_LED_OEN, BIT5),
    //chip_config != 0
    //reg_allpad_in 
    _RVM1(0x1ea0, 0, BIT15 ),   //reg[101ea0]#15 = 0b
    //reg_spdif_in_mode 
    _RVM1(0x1e0a, 0, BIT2|BIT1|BIT0 ),   //reg[101e0a]#2~#0 = 000b
    //reg_SecondUARTMode
    _RVM1(0x1e06, 0, BIT2|BIT1|BIT0 ),   //reg[101e06]#2~#0 = 000b
    //reg_ThirdUARTMode 
    _RVM1(0x1e06, 0, BIT6|BIT5|BIT4 ),   //reg[101e06]#6~#4 = 000b
    //reg_ForthUARTMode 
    _RVM1(0x1e06, 0, BIT10|BIT9|BIT8 ),   //reg[101e06]#10~#8 = 000b
    //reg_FastUartMode 
    _RVM1(0x1e06, 0, BIT13|BIT12|BIT11 ),   //reg[101e06]#13~#11 = 000b
    //reg_led_mode
    _RVM1(0x1e10, 0, BIT7 ),   //reg[101e10]#7 = 0b
    #endif

    _MEMMAP_PM_,


    #if(PAD_SPI_CZ_IS_GPIO != GPIO_NONE)
    #define PAD_SPI_CZ_OEN (PAD_SPI_CZ_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_SPI_CZ_OUT (PAD_SPI_CZ_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f30, PAD_SPI_CZ_OUT, BIT1),
    _RVM1(0x0f30, PAD_SPI_CZ_OEN, BIT0),
    //chip_config != 0
    //chip_config != 3
    //chip_config != 4
    //chip_config != 7
    //reg_spicsz0_gpio
    _RVM1(0x0e6a, BIT1 , BIT1 ),   //reg[0e6a]#1 = 1b
    #endif

    #if(PAD_SPI_DO_IS_GPIO != GPIO_NONE)
    #define PAD_SPI_DO_OEN (PAD_SPI_DO_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_SPI_DO_OUT (PAD_SPI_DO_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f36, PAD_SPI_DO_OUT, BIT1),
    _RVM1(0x0f36, PAD_SPI_DO_OEN, BIT0),
    //chip_config != 0
    //chip_config != 3
    //chip_config != 4
    //chip_config != 7
    //reg_spi_gpio
    _RVM1(0x0e6a, BIT0 , BIT0 ),   //reg[0e6a]#0 = 1b
    #endif

    #if(PAD_SPI_CK_IS_GPIO != GPIO_NONE)
    #define PAD_SPI_CK_OEN (PAD_SPI_CK_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_SPI_CK_OUT (PAD_SPI_CK_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f32, PAD_SPI_CK_OUT, BIT1),
    _RVM1(0x0f32, PAD_SPI_CK_OEN, BIT0),
    #endif

    #if(PAD_SPI_DI_IS_GPIO != GPIO_NONE)
    #define PAD_SPI_DI_OEN (PAD_SPI_DI_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_SPI_DI_OUT (PAD_SPI_DI_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f34, PAD_SPI_DI_OUT, BIT1),
    _RVM1(0x0f34, PAD_SPI_DI_OEN, BIT0),
    #endif

    #if(PAD_SPI_WPN_IS_GPIO != GPIO_NONE)
    #define PAD_SPI_WPN_OEN (PAD_SPI_WPN_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_SPI_WPN_OUT (PAD_SPI_WPN_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f02, PAD_SPI_WPN_OUT, BIT1),
    _RVM1(0x0f02, PAD_SPI_WPN_OEN, BIT0),
    //chip_config != 3
    //chip_config != 7
    //reg_spiwpn_is_gpio
    _RVM1(0x0e6a, BIT7 , BIT7 ),   //reg[0e6a]#7 = 1b
    #endif

    #if(PAD_SPI_HOLDN_IS_GPIO != GPIO_NONE)
    #define PAD_SPI_HOLDN_OEN (PAD_SPI_HOLDN_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_SPI_HOLDN_OUT (PAD_SPI_HOLDN_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f04, PAD_SPI_HOLDN_OUT, BIT1),
    _RVM1(0x0f04, PAD_SPI_HOLDN_OEN, BIT0),
    //chip_config != 3
    //chip_config != 7
    //reg_spiholdn_is_gpio
    _RVM1(0x0e6b, BIT6 , BIT6 ),   //reg[0e6b]#6 = 1b
    #endif

    #if(PAD_SPI_CZ1_IS_GPIO != GPIO_NONE)
    #define PAD_SPI_CZ1_OEN (PAD_SPI_CZ1_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_SPI_CZ1_OUT (PAD_SPI_CZ1_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f00, PAD_SPI_CZ1_OUT, BIT1),
    _RVM1(0x0f00, PAD_SPI_CZ1_OEN, BIT0),
    //chip_config != 2
    //chip_config != 3
    //chip_config != 6
    //chip_config != 7
    #endif
#if 0
    #if(PAD_UART_RX_IS_GPIO != GPIO_NONE)
    #define PAD_UART_RX_OEN (PAD_UART_RX_IS_GPIO == GPIO_IN ? BITNA: 0)
    #define PAD_UART_RX_OUT (PAD_UART_RX_IS_GPIO == GPIO_OUT_HIGH ? BITNA: 0)
    _RVM1(0x, PAD_UART_RX_OUT, BITNA),
    _RVM1(0x, PAD_UART_RX_OEN, BITNA),
    //NA
    #endif

    #if(PAD_UART_TX_IS_GPIO != GPIO_NONE)
    #define PAD_UART_TX_OEN (PAD_UART_TX_IS_GPIO == GPIO_IN ? BITNA: 0)
    #define PAD_UART_TX_OUT (PAD_UART_TX_IS_GPIO == GPIO_OUT_HIGH ? BITNA: 0)
    _RVM1(0x, PAD_UART_TX_OUT, BITNA),
    _RVM1(0x, PAD_UART_TX_OEN, BITNA),
    //NA
    #endif
#endif
    #if(PAD_PWM0_IS_GPIO != GPIO_NONE)
    #define PAD_PWM0_OEN (PAD_PWM0_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PWM0_OUT (PAD_PWM0_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f2c, PAD_PWM0_OUT, BIT1),
    _RVM1(0x0f2c, PAD_PWM0_OEN, BIT0),
    //reg_pwm0_pm_is_gpio
    _RVM1(0x0e38, BIT2 , BIT2 ),   //reg[0e38]#2 = 1b
    #endif

    #if(PAD_PWM1_IS_GPIO != GPIO_NONE)
    #define PAD_PWM1_OEN (PAD_PWM1_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PWM1_OUT (PAD_PWM1_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f2e, PAD_PWM1_OUT, BIT1),
    _RVM1(0x0f2e, PAD_PWM1_OEN, BIT0),
    //reg_pwm1_pm_is_gpio
    _RVM1(0x0e38, BIT3 , BIT3 ),   //reg[0e38]#3 = 1b
    #endif

    #if(PAD_RESET_IS_GPIO != GPIO_NONE)
    #define PAD_RESET_OEN (PAD_RESET_IS_GPIO == GPIO_IN ? BITNA: 0)
    #define PAD_RESET_OUT (PAD_RESET_IS_GPIO == GPIO_OUT_HIGH ? BITNA: 0)
    _RVM1(0x, PAD_RESET_OUT, BITNA),
    _RVM1(0x, PAD_RESET_OEN, BITNA),
    //NA
    #endif

    #if(PAD_IRIN_IS_GPIO != GPIO_NONE)
    #define PAD_IRIN_OEN (PAD_IRIN_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_IRIN_OUT (PAD_IRIN_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f28, PAD_IRIN_OUT, BIT1),
    _RVM1(0x0f28, PAD_IRIN_OEN, BIT0),
    //chip_config != 0
    //chip_config != 4
    //reg_ir_is_gpio
    _RVM1(0x0e38, BIT4 , BIT4 ),   //reg[0e38]#4 = 1b
    #endif

    #if(PAD_PWM2_IS_GPIO != GPIO_NONE)
    #define PAD_PWM2_OEN (PAD_PWM2_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PWM2_OUT (PAD_PWM2_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f26, PAD_PWM2_OUT, BIT1),
    _RVM1(0x0f26, PAD_PWM2_OEN, BIT0),
    //reg_pwm2_pm_is_gpio
    _RVM1(0x0e39, BIT1 , BIT1 ),   //reg[0e39]#1 = 1b
    #endif

    #if(PAD_HDMI_CEC_IS_GPIO != GPIO_NONE)
    #define PAD_HDMI_CEC_OEN (PAD_HDMI_CEC_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_HDMI_CEC_OUT (PAD_HDMI_CEC_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f24, PAD_HDMI_CEC_OUT, BIT1),
    _RVM1(0x0f24, PAD_HDMI_CEC_OEN, BIT0),
    //reg_cec_is_gpio
    _RVM1(0x0e38, BIT6 , BIT6 ),   //reg[0e38]#6 = 1b
    #endif

    #if(PAD_HDMI_HPD_IS_GPIO != GPIO_NONE)
    #define PAD_HDMI_HPD_OEN (PAD_HDMI_HPD_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_HDMI_HPD_OUT (PAD_HDMI_HPD_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1166, PAD_HDMI_HPD_OUT, BIT2),
    _RVM1(0x1166, PAD_HDMI_HPD_OEN, BIT3),
    //NA
    #endif

    #if(PAD_SAR3_IS_GPIO != GPIO_NONE)
    #define PAD_SAR3_OEN (PAD_SAR3_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_SAR3_OUT (PAD_SAR3_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _RVM1(0x1424, PAD_SAR3_OUT, BIT3),
    _RVM1(0x1423, PAD_SAR3_OEN, BIT3),
    //reg_sar_aisel[3]
    _RVM1(0x1422, 0, BIT3 ),   //reg[1422]#3 = 0b
    #endif

    #if(PAD_SAR2_IS_GPIO != GPIO_NONE)
    #define PAD_SAR2_OEN (PAD_SAR2_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_SAR2_OUT (PAD_SAR2_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1424, PAD_SAR2_OUT, BIT2),
    _RVM1(0x1423, PAD_SAR2_OEN, BIT2),
    //reg_sar_aisel[2]
    _RVM1(0x1422, 0, BIT2 ),   //reg[1422]#2 = 0b
    #endif

    #if(PAD_SAR1_IS_GPIO != GPIO_NONE)
    #define PAD_SAR1_OEN (PAD_SAR1_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_SAR1_OUT (PAD_SAR1_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x1424, PAD_SAR1_OUT, BIT1),
    _RVM1(0x1423, PAD_SAR1_OEN, BIT1),
    //reg_sar_aisel[1]
    _RVM1(0x1422, 0, BIT1 ),   //reg[1422]#1 = 0b
    #endif

    #if(PAD_SAR0_IS_GPIO != GPIO_NONE)
    #define PAD_SAR0_OEN (PAD_SAR0_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_SAR0_OUT (PAD_SAR0_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x1424, PAD_SAR0_OUT, BIT0),
    _RVM1(0x1423, PAD_SAR0_OEN, BIT0),
    //reg_sar_aisel[0]
    _RVM1(0x1422, 0, BIT0 ),   //reg[1422]#0 = 0b 
    #endif
//---------------------------------------------------------------------
// Pad Configuartion
//---------------------------------------------------------------------
    _MEMMAP_nonPM_,

    #define SM0_OPEN ((PAD_SM0_OPEN_MODE == PAD_MUX_MODE_1) ? (BIT0) : 0)

    _RVM1(0x1E18, SM0_OPEN, BIT0),


    #define OCP0_EN ((PAD_OCP0_EN_MODE == PAD_MUX_MODE_1) ? (BIT0) : 0)
                     
    _RVM1(0x1E1C, OCP0_EN, BIT0),


    #define SM0_EN  ((PAD_SM0_EN_MODE == PAD_MUX_MODE_1) ? (BIT0) : 0)

    _RVM1(0x1E19, SM0_EN, BIT0),


    #define I2CM0_MODE ((PAD_I2CM0_MODE == PAD_MUX_MODE_1) ? (BIT0) : \
                        (PAD_I2CM0_MODE == PAD_MUX_MODE_2) ? (BIT1) : \
                        (PAD_I2CM0_MODE == PAD_MUX_MODE_3) ? (BIT1|BIT0) : 0)

    _RVM1(0x1E12, I2CM0_MODE, BIT1|BIT0),


    #define I2CM1_MODE ((PAD_I2CM1_MODE == PAD_MUX_MODE_1) ? (BIT4) : \
                        (PAD_I2CM1_MODE == PAD_MUX_MODE_2) ? (BIT5) : \
                        (PAD_I2CM1_MODE == PAD_MUX_MODE_3) ? (BIT5|BIT4) : \
                        (PAD_I2CM1_MODE == PAD_MUX_MODE_4) ? (BIT6) : \
                        (PAD_I2CM1_MODE == PAD_MUX_MODE_5) ? (BIT6|BIT4) : 0)

    _RVM1(0x1E12, I2CM1_MODE, BITMASK(6:4)),


    #define I2CM2_MODE ((PAD_I2CM2_MODE == PAD_MUX_MODE_1) ? (BIT0) : \
                        (PAD_I2CM2_MODE == PAD_MUX_MODE_2) ? (BIT1) : \
                        (PAD_I2CM2_MODE == PAD_MUX_MODE_3) ? (BIT1|BIT0) : 0)

    _RVM1(0x1E13, I2CM2_MODE, BIT1|BIT0),

    #define TS0_MODE ((PAD_TS0_MODE == PAD_MUX_MODE_1) ? (BIT0) : \
                      (PAD_TS0_MODE == PAD_MUX_MODE_2) ? (BIT1) : \
                      (PAD_TS0_MODE == PAD_MUX_MODE_3) ? (BIT1|BIT0) : \
                      (PAD_TS0_MODE == PAD_MUX_MODE_4) ? (BIT2) : \
                      (PAD_TS0_MODE == PAD_MUX_MODE_5) ? (BIT2|BIT0) : \
                      (PAD_TS0_MODE == PAD_MUX_MODE_6) ? (BIT2|BIT1) : \
                      (PAD_TS0_MODE == PAD_MUX_MODE_7) ? (BIT2|BIT1|BIT0) : \
                      (PAD_TS0_MODE == PAD_MUX_MODE_8) ? (BIT3) : \
                      (PAD_TS0_MODE == PAD_MUX_MODE_9) ? (BIT3|BIT0) : 0)

    _RVM1(0x1E04, TS0_MODE, BITMASK(3:0)),


    #define TS1_MODE ((PAD_TS1_MODE == PAD_MUX_MODE_1) ? (BIT4) : \
                      (PAD_TS1_MODE == PAD_MUX_MODE_2) ? (BIT5) : \
                      (PAD_TS1_MODE == PAD_MUX_MODE_3) ? (BIT5|BIT4) : 0)

    _RVM1(0x1E04, TS1_MODE, BITMASK(6:4)),


    #define SPDIF_OUT_MODE ((PAD_SPDIF_OUT_MODE == PAD_MUX_MODE_1) ? (BIT4) : \
                            (PAD_SPDIF_OUT_MODE == PAD_MUX_MODE_2) ? (BIT5|BIT4) : 0)

    _RVM1(0x1E0A, SPDIF_OUT_MODE, BITMASK(6:4)),


    #define SPDIF_IN_MODE ((PAD_SPDIF_IN_MODE == PAD_MUX_MODE_1) ? (BIT0) : \
                           (PAD_SPDIF_IN_MODE == PAD_MUX_MODE_2) ? (BIT2|BIT0) : 0)

    _RVM1(0x1E0A, SPDIF_IN_MODE, BITMASK(2:0)),


    #define I2S_IN_MODE ((PAD_I2S_IN_MODE == PAD_MUX_MODE_1) ? (BIT0) : 0)

    _RVM1(0x1E0B, I2S_IN_MODE, BIT1|BIT0),


    #define I2S_OUT_MUTE_MODE ((PAD_I2S_OUT_MUTE_MODE == PAD_MUX_MODE_1) ? (BIT4) : 0)

    _RVM1(0x1E11, I2S_OUT_MUTE_MODE, BIT4),


    #define I2S_OUT_MODE ((PAD_I2S_OUT_MODE == PAD_MUX_MODE_1) ? (BIT4) : 0)

    _RVM1(0x1E0B, I2S_OUT_MODE, BIT5|BIT4),


    #define MCU_JTAG_MODE ((PAD_MCU_JTAG_MODE == PAD_MUX_MODE_1) ? (BIT4) : 0)

    _RVM1(0x1E08, MCU_JTAG_MODE, BITMASK(6:4)),


    #define SECOND_UART_MODE ((PAD_SECOND_UART_MODE == PAD_MUX_MODE_1) ? (BIT0) : \
                              (PAD_SECOND_UART_MODE == PAD_MUX_MODE_2) ? (BIT1) : \
                              (PAD_SECOND_UART_MODE == PAD_MUX_MODE_3) ? (BIT1|BIT0) : \
                              (PAD_SECOND_UART_MODE == PAD_MUX_MODE_4) ? (BIT2) : \
                              (PAD_SECOND_UART_MODE == PAD_MUX_MODE_5) ? (BIT2|BIT0) : \
                              (PAD_SECOND_UART_MODE == PAD_MUX_MODE_6) ? (BIT2|BIT1) : \
                              (PAD_SECOND_UART_MODE == PAD_MUX_MODE_7) ? (BIT2|BIT1|BIT0) : 0)

    _RVM1(0x1E06, SECOND_UART_MODE, BITMASK(2:0)),


    #define THIRD_UART_MODE ((PAD_THIRD_UART_MODE == PAD_MUX_MODE_1) ? (BIT4) : \
                             (PAD_THIRD_UART_MODE == PAD_MUX_MODE_2) ? (BIT5) : \
                             (PAD_THIRD_UART_MODE == PAD_MUX_MODE_3) ? (BIT5|BIT4) : \
                             (PAD_THIRD_UART_MODE == PAD_MUX_MODE_4) ? (BIT6) : \
                             (PAD_THIRD_UART_MODE == PAD_MUX_MODE_5) ? (BIT6|BIT4) : \
                             (PAD_THIRD_UART_MODE == PAD_MUX_MODE_6) ? (BIT6|BIT5) : \
                             (PAD_THIRD_UART_MODE == PAD_MUX_MODE_7) ? (BIT6|BIT5|BIT4) : 0)

    _RVM1(0x1E06, THIRD_UART_MODE, BITMASK(6:4)),


    #define FOURTH_UART_MODE ((PAD_FOURTH_UART_MODE == PAD_MUX_MODE_1) ? (BIT0) : \
                              (PAD_FOURTH_UART_MODE == PAD_MUX_MODE_2) ? (BIT1) : \
                              (PAD_FOURTH_UART_MODE == PAD_MUX_MODE_3) ? (BIT1|BIT0) : \
                              (PAD_FOURTH_UART_MODE == PAD_MUX_MODE_4) ? (BIT2) : \
                              (PAD_FOURTH_UART_MODE == PAD_MUX_MODE_5) ? (BIT2|BIT0) : \
                              (PAD_FOURTH_UART_MODE == PAD_MUX_MODE_6) ? (BIT2|BIT1) : \
                              (PAD_FOURTH_UART_MODE == PAD_MUX_MODE_7) ? (BIT2|BIT1|BIT0) : 0)

    _RVM1(0x1E07, FOURTH_UART_MODE, BITMASK(2:0)),


    #define FAST_UART_MODE ((PAD_FAST_UART_MODE == PAD_MUX_MODE_1) ? (BIT3) : \
                            (PAD_FAST_UART_MODE == PAD_MUX_MODE_2) ? (BIT4) : \
                            (PAD_FAST_UART_MODE == PAD_MUX_MODE_3) ? (BIT4|BIT3) : \
                            (PAD_FAST_UART_MODE == PAD_MUX_MODE_4) ? (BIT5) : \
                            (PAD_FAST_UART_MODE == PAD_MUX_MODE_5) ? (BIT5|BIT3) : \
                            (PAD_FAST_UART_MODE == PAD_MUX_MODE_6) ? (BIT5|BIT4) : \
                            (PAD_FAST_UART_MODE == PAD_MUX_MODE_7) ? (BIT5|BIT4|BIT3) : 0)

    _RVM1(0x1E07, FAST_UART_MODE, BITMASK(5:3)),


    #define HSYNC_EN ((PAD_HSYNC_EN_MODE == PAD_MUX_MODE_1) ? (BIT0) : 0)

    _RVM1(0x1E16, HSYNC_EN, BIT0),


    #define VSYNC_EN ((PAD_VSYNC_EN_MODE == PAD_MUX_MODE_1) ? (BIT4) : 0)

    _RVM1(0x1E16, VSYNC_EN, BIT4),


    #define RF_AGC_EN ((PAD_RF_AGC_EN_MODE == PAD_MUX_MODE_1) ? (BIT1) : 0)

    _RVM1(0x1E3A, RF_AGC_EN, BIT1),


    #define IF_AGC_EN ((PAD_IF_AGC_EN_MODE == PAD_MUX_MODE_1) ? (BIT0) : 0)

    _RVM1(0x1E3A, IF_AGC_EN, BIT0),


    #define LED_MODE ((PAD_LED_MODE == PAD_MUX_MODE_1) ? (BIT7) : 0)

    _RVM1(0x1E10, LED_MODE, BIT7),
	
    #define CH34_DBG_MODE ((PAD_CH34_DBG_MODE == PAD_MUX_MODE_1) ? (BIT0) : 0)

    _RVM1(0x1ED0, CH34_DBG_MODE, BIT0),

//---------------------------------------------------------------------
// Kronus U02 to fix smart card problem in 216 pin package
//---------------------------------------------------------------------
//#if (MS_BOARD_TYPE_SEL != BD_MST124E_D01A_S && MS_BOARD_TYPE_SEL != BD_MST124E_D01A_SZ)
//    _RVM1(0x1E45, 0, BIT2), // reg_156_mode set to 0 if the package is not 156 pin for SMC
//#endif
//---------------------------------------------------------------------
    // UART_SRC_SEL

    #define UART_INV    ((UART0_INV ? BIT0 : 0) | \
                         (UART1_INV ? BIT1 : 0) | \
                         (UART2_INV ? BIT2 : 0) | \
                         (UART3_INV ? BIT3 : 0) | \
                         (UART4_INV ? BIT4 : 0))

    _RVM1(0x1EAB, UART_INV, BITMASK(7:0)),

    _RVM1(0x1EA6, (UART1_SRC_SEL << 4) | (UART0_SRC_SEL << 0), BITMASK(7:0)),
    _RVM1(0x1EA7, (UART3_SRC_SEL << 4) | (UART2_SRC_SEL << 0), BITMASK(7:0)),
    _RVM1(0x1EA8, (UART4_SRC_SEL << 0), BITMASK(3:0)),


    // swtich to EJTAG3
    //_RVM1(0x1E2E, BIT1|BIT2, BITMASK(2:1)),

    _MEMMAP_PM_,

    _RVM1(0x0E13, BIT3, BIT3),   // RX0_ENABLE
    _RVM1(0x0E13, 0 , BIT4),   // UART0 -> reg_uart_sel0
#if 0
    #define SEL_CZ_ON   ((PIN_SPI_CZ1 ? BIT5 : 0) | \
                         (PIN_SPI_CZ2 ? BIT6 : 0) | \
                         (PIN_SPI_CZ3 ? BIT7 : 0))

    _RVM1(0x3C1D, ~(SEL_CZ_ON), BITMASK(7:5)),
#endif

    // VPLUG_IN_PWRGD mode setting in PAD_SAR3
    // 1. 0x1411[3:0] reg_sar_aisel:
    //      Pad GPIO/Ain switch: 1: analog input, 0: GPIO
    // 2. 0x1411[11:8] reg_oen_sar_gpio:
    //      Output enable for GPIO pad: 0: enable, 1: disable
    // 3. 0x1413[10] reg_sel_hvdetect:
    //      1'b0: VPLUG_IN_PWRGD = 1, 1'b1: VPLUG_IN_PWRGD = the power status of PAD_SAR3
    _RVM1(0x1422, BIT3, BIT3),
    _RVM1(0x1423, BIT3, BIT3),
    _RVM1(0x1427, BIT2, BIT2),

    _MEMMAP_nonPM_,

    // Clear all pad in
    _RVM1(0x1EA1, 0, BIT7),
    _END_OF_TBL2_,

//---------------------------------------------------------------------
// ISP_TOOL Write Protect (Need to check ????)
//---------------------------------------------------------------------
#if 0
#else
    //external flash #1
    _MEMMAP_PM_,
    _RVM1(0x0E6A, 0x00, BIT0), //reg_spi_gpio set to 0 (swtch to spi_do, spi_di, spi_ck)
    _RVM1(0x0E6A, 0x00, BIT1), //reg_spicsz0_gpio set to 0 (enable spi_cz0)
    // David 20130817, the address change?
    //_RVM1(0x08F4, 0x00, 0xFF), //reg_chip_select set to 0 (select embedded SPI Flash)
    //#if (PIN_FLASH_WP0)
    //_RVM1(0x0E21, BIT0, BIT0),
    //_RVM1(0x0E1F,    0, BIT0),
    //#endif
    _END_OF_TBL2_,

    //external flash #2
    _MEMMAP_PM_,
    _RVM1(0x0E6A, 0x00, BIT0), //reg_spi_gpio set to 0 (swtch to spi_do, spi_di, spi_ck)
    #if (PIN_SPI_CZ1)
    _RVM1(0x0E6A, 0x00, BIT2), //reg_spicsz1_gpio set to 0 (enable spi_cz1)
    // David 20130817, the address change?
    //_RVM1(0x08F4, 0x01, 0xFF), //reg_chip_select set to 1 (select external flash #1)
    //#if (PIN_FLASH_WP1)
    //_RVM1(0x0E21, BIT0, BIT0),
    //_RVM1(0x0E1F,    0, BIT0),
    //#endif
    _END_OF_TBL2_,
    #endif
    #endif
#endif
};
