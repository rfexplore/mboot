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
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
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
////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   apiGPD_v2.h
/// @brief  GPD Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _API_GPD_V2_H_
#define _API_GPD_V2_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
//-------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------
// below for utopia20 structure

typedef enum {
    MApi_CMD_GPD_Init,
    MApi_CMD_GPD_InputSource,
    MApi_CMD_GPD_OutputDecode,
    MApi_CMD_GPD_OutputDecodeROI,
    MApi_CMD_GPD_OutputDecodeMGIF,
    MApi_CMD_GPD_GetDuration,
    MApi_CMD_GPD_SetGIFMode,
    MApi_CMD_GPD_ScalingEnable,
    MApi_CMD_GPD_ScalingDisable,
    MApi_CMD_GPD_SetControl,
    MApi_CMD_GPD_GetControl,
    MApi_CMD_GPD_GetCRCResult
} E_GPD_V2_IOCTL_CMD;


typedef struct _GPD_INIT_PARAM
{
    MS_PHY SrcBuf;
    void* retVal;
}GPD_INIT_PARAM, *PGPD_INIT_PARAM;

typedef struct _GPD_INPUTSOURCE_PARAM
{
    gpd_pic_info* param;
    MS_PHY InputBuf;
    MS_U32 Size;
    MS_S32 retVal;
}GPD_INPUTSOURCE_PARAM, *PGPD_INPUTSOURCE_PARAM;

typedef struct _GPD_OUTPUTDECODE_PARAM
{
    MS_PHY DecodeBuf;
    MS_U32 ocolor;
    MS_U32 Size;
    MS_S32 retVal;
}GPD_OUTPUTDECODE_PARAM, *PGPD_OUTPUTDECODE_PARAM;

typedef struct _GPD_OUTPUTDECODEROI_PARAM
{
    MS_PHY DecodeBuf;
    MS_U32 ocolor;
    MS_U32 Size;
    gpd_roi_info *roi;
    MS_S32 retVal;
}GPD_OUTPUTDECODEROI_PARAM, *PGPD_OUTPUTDECODEROI_PARAM;

typedef struct _GPD_GETDURATION_PARAM
{
    MS_S32 retVal;
}GPD_GETDURATION_PARAM, *PGPD_GETDURATION_PARAM;

typedef struct _GPD_SETGIFMODE_PARAM
{
   MS_U32 mode;
}GPD_SETGIFMODE_PARAM, *PGPD_SETGIFMODE_PARAM;

typedef struct _GPD_SCALINGENABLE_PARAM
{
    MS_U32 mode;
}GPD_SCALINGENABLE_PARAM, *PGPD_SCALINGENABLE_PARAM;


typedef struct _GPD_CONTROL_PARAM
{
    gpd_user_cmd cmd_id;
    MS_VIRT param;
    MS_S32 retVal;
}GPD_CONTROL_PARAM, *PGPD_CONTROL_PARAM;

typedef struct _GPD_GET_CONTROL_PARAM
{
    gpd_get_cmd cmd_id;
    MS_VIRT param;
    MS_S32 retVal;
}GPD_GET_CONTROL_PARAM, *PGPD_GET_CONTROL_PARAM;

typedef struct _GPD_GETCRCRESULT_PARAM
{
    const MS_U8 *pu8Data;
    MS_U32 u32Size;
    MS_U32 u32Width;
    MS_U32 ocolor;
    MS_U32 retVal;
}GPD_GETCRCRESULT_PARAM, *PGPD_GETCRCRESULT_PARAM;


void GPDRegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 GPD_Open(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 GPD_Close(void* pInstance);
MS_U32 GPD_Ioctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);


#ifdef __cplusplus
}
#endif

#endif // _API_GPD_V2_H_


