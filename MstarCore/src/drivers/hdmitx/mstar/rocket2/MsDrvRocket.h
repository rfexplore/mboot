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

#ifndef _MS_DRV_ROCKET2_
#define _MS_DRV_ROCKET2_
#include <common.h>
#include <MsTypes.h>

typedef enum
{
    E_MAPI_HDMITX_TIMING_480_60I,
    E_MAPI_HDMITX_TIMING_480_60P,
    E_MAPI_HDMITX_TIMING_576_50I,
    E_MAPI_HDMITX_TIMING_576_50P,
    E_MAPI_HDMITX_TIMING_720_50P,
    E_MAPI_HDMITX_TIMING_720_60P,
    E_MAPI_HDMITX_TIMING_1080_50I,
    E_MAPI_HDMITX_TIMING_1080_50P,
    E_MAPI_HDMITX_TIMING_1080_60I,
    E_MAPI_HDMITX_TIMING_1080_60P,
    E_MAPI_HDMITX_TIMING_1080_30P,
    E_MAPI_HDMITX_TIMING_1080_25P,
    E_MAPI_HDMITX_TIMING_1080_24P,
    E_MAPI_HDMITX_TIMING_4K2K_30P,
    E_MAPI_HDMITX_TIMING_1440_50P,
    E_MAPI_HDMITX_TIMING_1440_60P,
    E_MAPI_HDMITX_TIMING_1440_24P,
    E_MAPI_HDMITX_TIMING_1440_30P,
    E_MAPI_HDMITX_TIMING_1470_50P,
    E_MAPI_HDMITX_TIMING_1470_60P,
    E_MAPI_HDMITX_TIMING_1470_24P,
    E_MAPI_HDMITX_TIMING_1470_30P,
    E_MAPI_HDMITX_TIMING_2160_24P,
    E_MAPI_HDMITX_TIMING_2160_30P,
    E_MAPI_HDMITX_TIMING_4K2K_25P,
    E_MAPI_HDMITX_TIMING_4K1K_60P,
    E_MAPI_HDMITX_TIMING_2K2K_60P,
    E_MAPI_HDMITX_TIMING_2560x1440_60P,
    E_MAPI_HDMITX_TIMING_4K2K_24P,
    E_MAPI_HDMITX_TIMING_4K2K_50P,
    E_MAPI_HDMITX_TIMING_4K2K_60P,
    E_MAPI_HDMITX_TIMING_4096x2160_24P,
    E_MAPI_HDMITX_TIMING_MAX,
}EN_MAPI_HDMITX_TIMING_TYPE;

typedef enum
{
    E_MAPI_ROCKY_RES_640x480p        =0,
    E_MAPI_ROCKY_RES_720x480i         = 1,
    E_MAPI_ROCKY_RES_720x576i         = 2,
    E_MAPI_ROCKY_RES_720x480p         = 3,
    E_MAPI_ROCKY_RES_720x576p         = 4,
    E_MAPI_ROCKY_RES_1280x720p_50Hz   = 5,
    E_MAPI_ROCKY_RES_1280x720p_60Hz   = 6,
    E_MAPI_ROCKY_RES_1920x1080i_50Hz  = 7,
    E_MAPI_ROCKY_RES_1920x1080i_60Hz  = 8,
    E_MAPI_ROCKY_RES_1920x1080p_24Hz  = 9,
    E_MAPI_ROCKY_RES_1920x1080p_25Hz  = 10,
    E_MAPI_ROCKY_RES_1920x1080p_30Hz  = 11,
    E_MAPI_ROCKY_RES_1920x1080p_50Hz  = 12,
    E_MAPI_ROCKY_RES_1920x1080p_60Hz  = 13,
    E_MAPI_ROCKY_RES_4K2Kp_30Hz       = 14,
    E_MAPI_ROCKY_RES_1280x1470p_50Hz  = 15,
    E_MAPI_ROCKY_RES_1280x1470p_60Hz  = 16,
    E_MAPI_ROCKY_RES_1280x1470p_24Hz  = 17,
    E_MAPI_ROCKY_RES_1280x1470p_30Hz  = 18,
    E_MAPI_ROCKY_RES_1920x2205p_24Hz  = 19,
    E_MAPI_ROCKY_RES_1920x2205p_30Hz  = 20,
    E_MAPI_ROCKY_RES_4K2Kp_25Hz       = 21,
    E_MAPI_ROCKY_RES_4K2Kp_24Hz       = 22,
    E_MAPI_ROCKY_RES_4K2Kp_50Hz       = 23,
    E_MAPI_ROCKY_RES_4K2Kp_60Hz       = 24,
    E_MAPI_ROCKY_RES_4K1Kp_60Hz       = 25,
    E_MAPI_ROCKY_RES_2K2Kp_60Hz       = 26,
    E_MAPI_ROCKY_RES_2560x1440p_60Hz  = 27,
    E_MAPI_ROCKY_RES_4096x2160p_24Hz  = 28,
    E_MAPI_ROCKY_RES_MAX              = 29,
} EN_MAPI_DEVICE_ROCKY_VIDEO_TIMING;//sync to apiHDMITx.h in device rocket

typedef enum
{
    E_MAPI_RAPTORS_GET_EDID_FAILED = 1,
    E_MAPI_RAPTORS_RESOLUTION_NOT_SUPPORT = 2,
    E_MAPI_RAPTORS_RESOLUTION_SUPPORT_SUCCESS = 3,
    E_MAPI_RAPTORS_RESOLUTION_BESTSUPPORT_SUCCESS = 4,
}E_MAPI_RAPTORS_SUPPORT_FORMAT;

MS_BOOL msHdmitx_Rocket_Init(EN_MAPI_DEVICE_ROCKY_VIDEO_TIMING outPutType );



#endif

