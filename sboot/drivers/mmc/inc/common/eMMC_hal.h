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

#ifndef __eMMC_HAL_H__
#define __eMMC_HAL_H__

#include "eMMC.h"

#define U32BEND2LEND(X) ( ((X&0x000000FF)<<24) + ((X&0x0000FF00)<<8) + ((X&0x00FF0000)>>8) + ((X&0xFF000000)>>24) )
#define U16BEND2LEND(X) ( ((X&0x00FF)<<8) + ((X&0xFF00)>>8) )

#define _START_TXMIT                0x40 // transmission bit

#define eMMC_GO_IDLE_STATE		(_START_TXMIT+0)
#define eMMC_SEND_OP_COND		(_START_TXMIT+1)
#define eMMC_ALL_SEND_CID		(_START_TXMIT+2)
#define eMMC_SET_RLT_ADDR		(_START_TXMIT+3)
#define eMMC_SEL_DESEL_CARD		(_START_TXMIT+7)
#define eMMC_SEND_EXT_CSD		(_START_TXMIT+8)
#define eMMC_SEND_CSD			(_START_TXMIT+9)
#define eMMC_SWITCH				(_START_TXMIT+6)
#define eMMC_ERASE_GROUP_S		(_START_TXMIT+35)
#define eMMC_ERASE_GROUP_E		(_START_TXMIT+36)
#define eMMC_ERASE				(_START_TXMIT+38)
#define eMMC_SEND_STATUS		(_START_TXMIT+13)
#define eMMC_R_SINGLE_BLOCK		(_START_TXMIT+17)
#define eMMC_R_MULTIP_BLOCK		(_START_TXMIT+18)
#define eMMC_STOP_TRANSMIT		(_START_TXMIT+12)
#define eMMC_W_SINGLE_BLOCK		(_START_TXMIT+24)
#define eMMC_W_MULTIP_BLOCK		(_START_TXMIT+25)
#define eMMC_SEND_TUNING_BLK	(_START_TXMIT+21)

#define STUFF_BITS			0x00000000

#define eMMC_FCIE_CMD_RSP_ERR_RETRY_CNT       5
#define eMMC_CMD_API_ERR_RETRY_CNT            5
#define eMMC_CMD_API_WAIT_FIFOCLK_RETRY_CNT   5

extern  U32 eMMC_FCIE_WaitEvents(U32 u32_RegAddr, U16 u16_Events, U32 u32_MicroSec);
extern  U32 eMMC_FCIE_PollingEvents(U32 u32_RegAddr, U16 u16_Events, U32 u32_MicroSec);
extern  U32 eMMC_FCIE_FifoClkRdy(U8 u8_Dir);
extern void eMMC_FCIE_DumpDebugBus(void);
extern void eMMC_FCIE_DumpRegisters(void);
extern void eMMC_FCIE_CheckResetDone(void);
extern  U32 eMMC_FCIE_Reset(void);
extern  U32 eMMC_FCIE_Init(void);
extern void eMMC_FCIE_ErrHandler_Stop(void);
extern  U32 eMMC_FCIE_ErrHandler_Retry(void);
extern void eMMC_FCIE_ErrHandler_RestoreClk(void);
extern void eMMC_FCIE_ErrHandler_ReInit(void);
extern  U32 eMMC_FCIE_SendCmd(U16 u16_Mode, U16 u16_Ctrl, U32 u32_Arg, U8 u8_CmdIdx, U8 u8_RspByteCnt);
extern void eMMC_FCIE_ClearEvents(void);
extern  U32 eMMC_FCIE_WaitD0High_Ex(U32 u32_us);
extern  U32 eMMC_FCIE_WaitD0High(U32 u32_us);
extern void eMMC_FCIE_GetCIFC(U16 u16_WordPos, U16 u16_WordCnt, U16 *pu16_Buf);
extern void eMMC_FCIE_GetCIFD(U16 u16_WordPos, U16 u16_WordCnt, U16 *pu16_Buf);
extern   U8 eMMC_FCIE_CmdRspBufGet(U8 u8addr);
extern   U8 eMMC_FCIE_DataFifoGet(U8 u8addr);

// eMMC_hal_speed.c
extern  U32 eMMC_FCIE_EnableSDRMode(void);

//----------------------------------------
extern  U32 eMMC_Identify(void);
extern  U32 eMMC_CMD0(U32 u32_Arg);
extern  U32 eMMC_CMD1(void);
extern  U32 eMMC_CMD2(void);
extern  U32 eMMC_CMD3_CMD7(U16 u16_RCA, U8 u8_CmdIdx);
extern  U32 eMMC_CMD9(U16 u16_RCA);
extern  U32 eMMC_CSD_Config(void);
extern  U32 eMMC_ExtCSD_Config(void);
extern  U32 eMMC_CMD8(U8 *pu8_DataBuf);
extern  U32 eMMC_CMD8_MIU(U8 *pu8_DataBuf);
extern  U32 eMMC_CMD8_CIFD(U8 *pu8_DataBuf);
extern  U32 eMMC_SetBusSpeed(U8 u8_BusSpeed);
extern  U32 eMMC_SetDrivingStrength(U8 u8Driving);
extern  U32 eMMC_SetBusWidth(U8 u8_BusWidth, U8 u8_IfDDR);
extern  U32 eMMC_ModifyExtCSD(U8 u8_AccessMode, U8 u8_ByteIdx, U8 u8_Value);
extern  U32 eMMC_CMD6(U32 u32_Arg);
extern  U32 eMMC_Dump_eMMCStatus(void);
extern  U32 eMMC_CMD13(U16 u16_RCA);
extern  U32 eMMC_CMD17(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf);
extern  U32 eMMC_CMD17_MIU(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf);
extern  U32 eMMC_CMD17_CIFD(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf);
extern  U32 eMMC_CMD12_NoCheck(U16 u16_RCA);
extern  U32 eMMC_CMD12(U16 u16_RCA);
extern  U32 eMMC_CMD18(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf, U16 u16_BlkCnt);
extern  U32 eMMC_CMD18_MIU(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf, U16 u16_BlkCnt);
extern  U32 eMMC_CMD24(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf);
extern  U32 eMMC_CMD24_MIU(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf);
extern  U32 eMMC_CMD24_CIFD(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf);
extern  U32 eMMC_CMD25(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf, U16 u16_BlkCnt);
extern  U32 eMMC_CMD25_MIU(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf, U16 u16_BlkCnt);
extern  U32 eMMC_GetR1(void);
extern  U32 eMMC_CheckR1Error(void);

//----------------------------------------
extern  U32 eMMC_FCIE_PollingFifoClkReady(void);
extern  U32 eMMC_FCIE_PollingMIULastDone(void);

#endif // __eMMC_HAL_H__
#else

#endif

