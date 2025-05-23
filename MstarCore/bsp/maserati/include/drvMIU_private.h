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
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
////////////////////////////////////////////////////////////////////////////////

#ifndef _DRVMIU_PRIV_H_
#define _DRVMIU_PRIV_H_

////////////////////////////////////////////////////////////////////////////////
/// @file drvBDMA_private.h
/// @author MStar Semiconductor Inc.
/// @brief Byte DMA control driver
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"

typedef MS_BOOL (*IOCTL_MIU_INIT)(void);
typedef void (*IOCTL_MIU_MASK_IP)(MS_U8 , MS_U8);
typedef void (*IOCTL_MIU_MASK_REQ)(MS_U8 , eMIUClientID);
typedef MS_BOOL (*IOCTL_MIU_SET_SSC)(MS_U16, MS_U16, MS_BOOL);
typedef MS_BOOL (*IOCTL_MIU_SET_SSCVALUE)(MS_U8, MS_U16, MS_U16, MS_BOOL);
typedef MS_BOOL (*IOCTL_MIU_PROTECT)(MS_U8, MS_U8*, MS_PHY, MS_PHY, MS_BOOL);
typedef MS_BOOL (*IOCTL_MIU_PROTECTEx)(MS_U8, MS_U32*, MS_PHY, MS_PHY, MS_BOOL);
typedef MS_BOOL (*IOCTL_MIU_SELMIU)(eMIUClientID, eMIU_SelType);
typedef MS_BOOL (*IOCTL_MIU_GETPROTECTINFO)(MS_U8, MIU_PortectInfo*);
typedef MS_BOOL (*IOCTL_MIU_SETGROUPPRIORITY)(MS_U8, MIU_GroupPriority);
typedef MS_BOOL (*IOCTL_MIU_SETHIGHPRIORITY)(MS_U8, eMIUClientID, MS_BOOL);
typedef void (*IOCTL_MIU_PRINTPROTECTINFO)(void);
typedef MS_BOOL (*IOCTL_MIU_DRAM_SIZE)(MS_U8, MS_U8);
typedef MS_BOOL (*IOCTL_MIU_PROTECTALIGN)(MS_U32 *);
typedef MS_BOOL (*IOCTL_MIU_GETDRAMTYPE)(MIU_ID eMiu, MIU_DDR_TYPE* pType);

typedef struct _MIU_RESOURCE_PRIVATE
{
    MS_U32 Dummy;
}MIU_RESOURCE_PRIVATE;

typedef struct _MIU_INSTANT_PRIVATE
{
    IOCTL_MIU_INIT fpMIU_Init;
    IOCTL_MIU_MASK_IP fpMIU_Mask_Req_OPM_R;
    IOCTL_MIU_MASK_IP fpMIU_Mask_Req_DNRB_R;
    IOCTL_MIU_MASK_IP fpMIU_Mask_Req_DNRB_W;
    IOCTL_MIU_MASK_IP fpMIU_Mask_Req_DNRB_RW;
    IOCTL_MIU_MASK_IP fpMIU_Mask_Req_SC_RW;
    IOCTL_MIU_MASK_IP fpMIU_Mask_Req_MVOP_R;
    IOCTL_MIU_MASK_IP fpMIU_Mask_Req_MVD_R;
    IOCTL_MIU_MASK_IP fpMIU_Mask_Req_MVD_W;
    IOCTL_MIU_MASK_IP fpMIU_Mask_Req_MVD_RW;
    IOCTL_MIU_MASK_IP fpMIU_Mask_Req_AUDIO_RW;
    IOCTL_MIU_MASK_REQ fpMIU_Mask_Req;
    IOCTL_MIU_MASK_REQ fpMIU_UnMask_Req;
    IOCTL_MIU_SET_SSC fpMIU_SetSsc;
    IOCTL_MIU_SET_SSCVALUE fpMIU_SetSscValue;
    IOCTL_MIU_PROTECT fpMIU_Protect;
    IOCTL_MIU_PROTECTEx fpMIU_ProtectEx;
    IOCTL_MIU_SELMIU fpMIU_SelMIU;
    IOCTL_MIU_GETPROTECTINFO fpMIU_GetProtectInfo;
    IOCTL_MIU_SETGROUPPRIORITY fpMIU_SetGroupPriority;
    IOCTL_MIU_SETHIGHPRIORITY fpMIU_SetHighPriority;
    IOCTL_MIU_PRINTPROTECTINFO fpMIU_PrintProtectInfo;
    IOCTL_MIU_DRAM_SIZE fpMIU_Dram_Size;
    IOCTL_MIU_PROTECTALIGN fpMIU_ProtectAlign;
    IOCTL_MIU_GETDRAMTYPE fpMIU_GetDramType;
}MIU_INSTANT_PRIVATE;

// void MIURegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 MIUOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
// MS_U32 MIUClose(void* pInstance);
// MS_U32 MIUIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);
// MS_BOOL _MDrv_MIU_Init(void);
// void _MDrv_MIU_Mask_Req_OPM_R(MS_U8 u8Mask, MS_U8 u8Miu);
// void _MDrv_MIU_Mask_Req_DNRB_R(MS_U8 u8Mask, MS_U8 u8Miu);
// void _MDrv_MIU_Mask_Req_DNRB_W(MS_U8 u8Mask, MS_U8 u8Miu);
// void _MDrv_MIU_Mask_Req_DNRB_RW(MS_U8 u8Mask, MS_U8 u8Miu);
// void _MDrv_MIU_Mask_Req_SC_RW(MS_U8 u8Mask, MS_U8 u8Miu);
// void _MDrv_MIU_Mask_Req_MVOP_R(MS_U8 u8Mask, MS_U8 u8Miu);
// void _MDrv_MIU_Mask_Req_MVD_R(MS_U8 u8Mask, MS_U8 u8Miu);
// void _MDrv_MIU_Mask_Req_MVD_W(MS_U8 u8Mask, MS_U8 u8Miu);
// void _MDrv_MIU_Mask_Req_MVD_RW(MS_U8 u8Mask, MS_U8 u8Miu);
// void _MDrv_MIU_Mask_Req_AUDIO_RW(MS_U8 u8Mask, MS_U8 u8Miu);
// void _MDrv_MIU_MaskReq(MS_U8 u8Miu, eMIUClientID eClientID);
// void _MDrv_MIU_UnMaskReq(MS_U8 u8Miu, eMIUClientID eClientID);
// MS_BOOL _MDrv_MIU_SetSsc(MS_U16 u16Fmodulation, MS_U16 u16FDeviation, MS_BOOL bEnable);
MS_BOOL _MDrv_MIU_SetSscValue(MS_U8 u8MiuDev, MS_U16 u16Fmodulation, MS_U16 u16FDeviation, MS_BOOL bEnable);
MS_BOOL _MDrv_MIU_Protect(MS_U8 u8Blockx, MS_U8 *pu8ProtectId, MS_PHY u32Start, MS_PHY u32End, MS_BOOL  bSetFlag);
MS_BOOL _MDrv_MIU_ProtectEx(MS_U8 u8Blockx, MS_U32 *pu32ProtectId, MS_PHY u32Start, MS_PHY u32End, MS_BOOL  bSetFlag);
// MS_BOOL _MDrv_MIU_SelMIU(eMIUClientID eClientID, eMIU_SelType eType);
MS_BOOL _MDrv_MIU_GetProtectInfo(MS_U8 u8MiuDev, MIU_PortectInfo *pInfo);
// MS_BOOL _MDrv_MIU_SetGroupPriority(MS_U8 u8MiuDev, MIU_GroupPriority sPriority);
// MS_BOOL _MDrv_MIU_SetHPriorityMask(MS_U8 u8MiuDev, eMIUClientID eClientID, MS_BOOL bMask);
void _MDrv_MIU_PrintProtectInfo(void);
MS_BOOL _MDrv_MIU_Dram_Size(MS_U8 MiuID, MS_U8 DramSize);
MS_BOOL _MDrv_MIU_ProtectAlign(MS_U32 *u32PageShift);
MS_BOOL _MDrv_MIU_GetDramType(MIU_ID eMiu, MIU_DDR_TYPE* pType);

#ifdef __cplusplus
}
#endif
#endif // _DRVMIU_PRIV_H_
