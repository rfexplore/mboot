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
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (!��MStar Confidential Information!�L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvDMX_TTX.h
/// @brief  MStar DVT TTX Driver Interface
/// @author MStar Semiconductor Inc.
/// @version 1.0
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_DMX_TTX_H
#define _DRV_DMX_TTX_H

#include "MsDevice.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define VBI_TTX_UTOPIA20 (1)

#define VBI_TTX_DATA_LENGTH 48

typedef struct
{
    MS_BOOL _bTTXInit;
    MS_BOOL _bSuspend;
    MS_PHY bufferAddr;
    MS_U16 packetCount;
    MS_PHY _TTXBufAddr;
    MS_U16 _TTXBufLen;
    MS_U8 _FilterID;
} DMX_TTX_StoreInfo;

typedef enum
{
    DMX_TTX_INIT,
    DMX_TTX_EXIT,
    DMX_TTX_SUSPEND,
    DMX_TTX_RESUME,
} EN_DMX_TTX_CMD;

typedef MS_U32(*DMX_CB_FN)(MS_U8 SecFltNo);
//-------------------------------------------------------------------------------------------------
/// Reset TTX ring buffer
/// @ingroup TTX_Task
/// @return None
//-------------------------------------------------------------------------------------------------
void MDrv_DMX_TTX_RingBuffer_Reset(void);
//-------------------------------------------------------------------------------------------------
/// Initialize DMX TTX module
/// @ingroup TTX_General
/// @param  bufferAddr       \b IN: Set TTX ring buffer address
/// @param  packetCount    \b IN: Set TTX ring buffer size
/// @return TRUE  : Succeed
/// @return FALSE : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DMX_TTX_Init(MS_PHY bufferAddr, MS_U16 packetCount);
//-------------------------------------------------------------------------------------------------
/// Finalize DMX TTX module
/// @ingroup TTX_General
/// @return TRUE  : Succeed
/// @return FALSE : Fail
//-------------------------------------------------------------------------------------------------
// MS_BOOL MDrv_DMX_TTX_Exit(void);

//-----------------
// TTX_Task
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set TTX callback function.
/// @ingroup TTX_Task
/// @param  fn         \b IN: callback sunction
/// @return None
//-------------------------------------------------------------------------------------------------
void MDrv_DMX_TTX_SetCB(DMX_CB_FN fn);
//-------------------------------------------------------------------------------------------------
/// Set TTX PID filter index to use callback function get the write address of section buffer.
/// @ingroup TTX_Task
/// @param  fid         \b IN: PID filter index
/// @return None
//-------------------------------------------------------------------------------------------------
void MDrv_DMX_TTX_SetFilterID(MS_U8 fid);
//-------------------------------------------------------------------------------------------------
/// Check TTX buffer is empty or not.
/// @ingroup TTX_Task
/// @return TRUE  : Buffer is empty.
/// @return FALSE : Buffer is not empty.
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DMX_TTX_PacketBufferIsEmpty(void);
//-------------------------------------------------------------------------------------------------
/// Get TTX data (Get all of the packets in the TTX buffer (if the input buffer is big enough))
/// @ingroup TTX_Task
/// @param  dataAddr       \b IN: Set TTX buffer address
/// @param  length           \b OUT: Get total packt length
/// @return TRUE  : Succeed
/// @return FALSE : Fail
//-------------------------------------------------------------------------------------------------
// MS_BOOL MDrv_DMX_TTX_GetPackets(MS_PHY dataAddr, MS_U32 *length);
//-------------------------------------------------------------------------------------------------
/// Get TTX data (Get 1 of the packets in the TTX buffer)
/// @ingroup TTX_Task
/// @param  packetAddress \b OUT: Get TTX buffer address
/// @return TRUE  : Succeed
/// @return FALSE : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DMX_TTX_GetPacket(MS_PHY *packetAddress);

//-------------------------------------------------------------------------------------------------
/// Check if there is a packet buffer overflow. If there is an overflow,
/// the the packet buffer should be cleared from the reading task.
/// @ingroup TTX_Task
/// @return TRUE  : If there is packet buffer overflow
/// @return FALSE : Otherwise
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DMX_TTX_PacketBufferIsOverflow( void );

//-------------------------------------------------------------------------------------------------
/// Return the nomber of packet buffer overflows since the last reset or creation.
/// @ingroup TTX_Task
/// @return : The number of packet buffer overflows.
//-------------------------------------------------------------------------------------------------
MS_U16 MDrv_DMX_TTX_PacketBufferGetNoOfOverflows( void );

//-----------------
// TTX_STR
//-------------------------------------------------------------------------------------------------
/// Save DMX_TTX driver states to DRAM.
/// @ingroup TTX_STR
/// @return TRUE  : Succeed
/// @return FALSE : Fail
//-------------------------------------------------------------------------------------------------
// MS_BOOL MDrv_DMX_TTX_Suspend(void);

//-------------------------------------------------------------------------------------------------
/// Restore DMX_TTX driver states from DRAM
/// @ingroup TTX_STR
/// @return TRUE  : Succeed
/// @return FALSE : Fail
//-------------------------------------------------------------------------------------------------
// MS_BOOL MDrv_DMX_TTX_Resume(void);

//-------------------------------------------------------------------------------------------------
/// Control TTX STR functions.
/// @ingroup TTX_STR
/// @param  u16PowerState  \b IN: STR power mode. (to control STR suspend & resume case.)
/// @return : Utopia status - UTOPIA_STATUS_SUCCESS (Succeed) or UTOPIA_STATUS_FAIL (Fail)
//-------------------------------------------------------------------------------------------------
// MS_U32  MDrv_DMX_TTX_SetPowerState(EN_POWER_MODE u16PowerState);

#undef INTERFACE

#ifdef __cplusplus
}
#endif

#endif

