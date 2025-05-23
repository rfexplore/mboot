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

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------
#include <common.h>
#include <drvCPU.h>
#include <MsTypes.h>
#include <MsDebug.h>
#include <MsSetupTee.h>
#include <MsSysUtility.h>
#include <uboot_mmap.h>

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#if defined (CONFIG_R2_BRINGUP)
#define BIT2		0x04
#define BIT11       0x0800
#define BIT12       0x1000
#define BIT13       0x2000

#if (CONFIG_TV_CHIP==0)

#define UART_DBG_SECR2          0x00
#else
#define UART_DBG_SECR2          0x0D
#endif

#if (CONFIG_TV_CHIP==0)
#define REG_R2_BASE             0x103500
#else
#define REG_R2_BASE             0x122A00
#endif
//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Extern Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Private Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Public Functions
//-------------------------------------------------------------------------------------------------
void MApi_Disable_R2(void)
{
    UBOOT_TRACE("IN\n");

    WriteByte(REG_R2_BASE+0x40*2, 0x00); //disable R2

    UBOOT_TRACE("OK\n");
}

void MApi_R2_StartFrDRAM(MS_U32 addr)
{
    MS_U32 AeonBufAdr=addr;
    MS_U16 value;
    char *pEnv=NULL;
    UBOOT_TRACE("IN\n");
    printf("R2 Boot from 0x%08x\n", (unsigned int)AeonBufAdr);

    // select UART source
    //WriteByte(0x101EA6, UART_DBG_SRC);

    pEnv=getenv("uart2R2");
    if(pEnv!=NULL)
    {
        if(strcmp(pEnv,"1")==0)
        {
            printf("switch uart message to uart\n");
            WriteByte(0x101EA6, UART_DBG_SECR2);
        }
    }

//switch UART to dig_mux (instead of MCU51)
    value = Read2Byte(0x0E12);
#if 1//HK message
    value &= (~BIT12); //0: dig_mux (reg_uart_sel0)
    value |= BIT11; //1: enable uart rx via  PAD_DDCA for dig_mux (reg_uart_sel0)
#else //PM message
    value |= (BIT12); //1: HK MCU51 uart0
    value |= BIT11; //1: enable uart rx via  PAD_DDCA for dig_mux (reg_uart_sel0)
#endif
    Write2Byte(0x0E12,value);

    #if defined (CONFIG_NUTTX_BRINGUP_TEST)

    MDrv_COPRO_GetBase();
    MDrv_COPRO_Init_Front();
    MDrv_COPRO_Init_End(0xa200000);
    #else
    MDrv_COPRO_GetBase();
    MDrv_COPRO_Init_Front();
    MDrv_COPRO_Init_End(addr);
    #endif

    UBOOT_TRACE("OK\n");
}
#endif //CONFIG_R2_BRINGUP
