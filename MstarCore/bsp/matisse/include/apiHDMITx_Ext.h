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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   apiHDMITx.h
/// @brief  HDMITx Interface
/// @author MStar Semiconductor Inc.
///
///  CL351033++:
///   Add CEC function for STB
///  CL310477++:
///   Open analog setting for the different board condition
///  CL309397++:
///   Modify apiHDMITx prototype for NDS
///  CL308729++:
///   Fix AVMUTE problem while HDCP is on
///  CL299817++:
///  i. Add I2C timeout mechanism in EDID and HDCP
///  ii. Add SET_AVMUTE API to avoid transition garbage noise while timing changed ]]>
///  CL288415++:
///   Add SRM DSA Signature Checking function
///  CL283331++:
///   Fix HDMI v1.3 deep color mode output unstable problem
///  CL282607++:
///   i. Fix YUV422 / YUV444 bugs
///   ii. Add MApi_HDMITx_GetHdcpKey() to get HDCP key from external storage.
///  CL276751++:
///   Modify HDMI / HDCP state mechine for NDS
///  CL275230++:
///   i. MApi_HDMITx_GetRxDCInfoFromEDID() to get Rx's deep color information from EDID
///   ii. MApi_HDMITx_SetHDMITxMode_CD() to set output mode and deep color setting
///  CL266666++:
///   Add event report for NDS
///  CL263961++:
///   Add CEC init and checkbuffer for NDS
///  CL260934++:
///   Add some customized APIs for NDS
///  CL259645++:
///   i. Remove EDID header check. If header is wrong, force to DVI output
///   ii. Add force output mode "MApi_HDMITx_ForceHDMIOutputMode()"
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _API_HDMITX_H_
#define _API_HDMITX_H_

#include "MsTypes.h"
//#include "halHDMITx.h"
//#include "drvHDMITx.h"
//#include "regHDMITx.h"



#ifdef __cplusplus
extern "C"
{
#endif


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
#define MSIF_HDMITX_LIB_CODE               {'H','D','M','I'}
#define MSIF_HDMITX_LIBVER                 {'0','0'}
#define MSIF_HDMITX_BUILDNUM               {'2','0'}
#define MSIF_HDMITX_CHANGELIST             {'0','0','6','7','7','7','7','2'}
#define HDMITX_API_VERSION              /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_HDMITX_LIB_CODE,                  /* IP__                                             */  \
    MSIF_HDMITX_LIBVER,                    /* 0.0 ~ Z.Z                                        */  \
    MSIF_HDMITX_BUILDNUM,                  /* 00 ~ 99                                          */  \
    MSIF_HDMITX_CHANGELIST,                /* CL#                                              */  \
    MSIF_OS

typedef enum
{
    HDMITX_DVI            = 0,  // DVI without HDCP
    HDMITX_DVI_HDCP       = 1,  // DVI with HDCP
    HDMITX_HDMI           = 2,  // HDMI without HDCP
    HDMITX_HDMI_HDCP      = 3,  // HDMI with HDCP
} HDMITX_OUTPUT_MODE;

typedef enum
{
    HDMITX_SEND_PACKET        = 0x00,   // send packet
    HDMITX_CYCLIC_PACKET      = 0x04,   // cyclic packet by frame count
    HDMITX_STOP_PACKET        = 0x80,   // stop packet
} HDMITX_PACKET_PROCESS;

typedef enum
{
    HDMITX_NULL_PACKET        = 0x00,
    HDMITX_ACR_PACKET         = 0x01,
    HDMITX_AS_PACKET          = 0x02,
    HDMITX_GC_PACKET          = 0x03,
    HDMITX_ACP_PACKET         = 0x04,
    HDMITX_ISRC1_PACKET       = 0x05,
    HDMITX_ISRC2_PACKET       = 0x06,
    HDMITX_DSD_PACKET         = 0x07,
    HDMITX_HBR_PACKET         = 0x09,
    HDMITX_GM_PACKET          = 0x0A,

    HDMITX_VS_INFOFRAME       = 0x81,
    HDMITX_AVI_INFOFRAME      = 0x82,
    HDMITX_SPD_INFOFRAME      = 0x83,
    HDMITX_AUDIO_INFOFRAME    = 0x84,
    HDMITX_MPEG_INFOFRAME     = 0x85,
} HDMITX_PACKET_TYPE;

typedef enum
{
    HDMITX_VIDEO_CD_NoID     = 0, // DVI mode
    HDMITX_VIDEO_CD_24Bits     = 4, // HDMI 8 bits
    HDMITX_VIDEO_CD_30Bits     = 5, // HDMI 10 bits
    HDMITX_VIDEO_CD_36Bits     = 6, // HDMI 12 bits
    HDMITX_VIDEO_CD_48Bits     = 7, // HDMI 16 bits
} HDMITX_VIDEO_COLORDEPTH_VAL;

typedef enum
{
    HDMITX_VIDEO_COLOR_RGB444     = 0,
    HDMITX_VIDEO_COLOR_YUV422     = 1,
    HDMITX_VIDEO_COLOR_YUV444     = 2,
} HDMITX_VIDEO_COLOR_FORMAT;

typedef enum
{
    HDMITX_RES_640x480p        =0,
    HDMITX_RES_720x480i         = 1,
    HDMITX_RES_720x576i         = 2,
    HDMITX_RES_720x480p         = 3,
    HDMITX_RES_720x576p         = 4,
    HDMITX_RES_1280x720p_50Hz   = 5,
    HDMITX_RES_1280x720p_60Hz   = 6,
    HDMITX_RES_1920x1080i_50Hz  = 7,
    HDMITX_RES_1920x1080i_60Hz  = 8,
    HDMITX_RES_1920x1080p_24Hz  = 9,
    HDMITX_RES_1920x1080p_25Hz  = 10,
    HDMITX_RES_1920x1080p_30Hz  = 11,
    HDMITX_RES_1920x1080p_50Hz  = 12,
    HDMITX_RES_1920x1080p_60Hz  = 13,
    HDMITX_RES_4K2Kp_30Hz       = 14,
    HDMITX_RES_1280x1470p_50Hz  = 15,
    HDMITX_RES_1280x1470p_60Hz  = 16,
    HDMITX_RES_1280x1470p_24Hz  = 17,
    HDMITX_RES_1280x1470p_30Hz  = 18,
    HDMITX_RES_1920x2205p_24Hz  = 19,
    HDMITX_RES_1920x2205p_30Hz  = 20,
    HDMITX_RES_4K2Kp_25Hz       = 21,
    HDMITX_RES_1024x768p_60Hz   = 22,
    HDMITX_RES_1280x800p_60Hz   = 23,
    HDMITX_RES_1600x1200p_60Hz  = 24,
    HDMITX_RES_1920x1200p_60Hz  = 25,
    HDMITX_RES_4K1Kp_60Hz       = 26,
    HDMITX_RES_2K2Kp_60Hz       = 27,
    HDMITX_RES_2560x1440p_60Hz  = 28,
    HDMITX_RES_4K2Kp_24Hz       = 29,
    HDMITX_RES_2560x1600p_60Hz  = 30,
    HDMITX_RES_800x600p_60Hz    = 31,
    HDMITX_RES_848x480p_60Hz    = 32,
    HDMITX_RES_1280x768Rp_60Hz  = 33,
    HDMITX_RES_1280x960p_60Hz   = 34,
    HDMITX_RES_1280x1024p_60Hz  = 35,
    HDMITX_RES_1360x768p_60Hz   = 36,
    HDMITX_RES_1366x768p_60Hz   = 37,
    HDMITX_RES_1440x900Rp_60Hz  = 38,
    HDMITX_RES_1600X900Rp_60Hz  = 39,
    HDMITX_RES_1600x1200Rp_60Hz = 40,
    HDMITX_RES_1680x1050Rp_60Hz = 41,
    HDMITX_RES_2048x1152Rp_60Hz = 42,
    HDMITX_RES_4096x2160p_24Hz  = 43,
    HDMITX_RES_MAX              = 44,

} HDMITX_VIDEO_TIMING;

typedef enum
{
    HDMITX_VIC_NOT_AVAILABLE        = 0,
    HDMITX_VIC_640x480p_60_4_3      = 1,
    HDMITX_VIC_720x480p_60_4_3      = 2,
    HDMITX_VIC_720x480p_60_16_9     = 3,
    HDMITX_VIC_1280x720p_60_16_9    = 4,
    HDMITX_VIC_1920x1080i_60_16_9   = 5,
    HDMITX_VIC_720x480i_60_4_3      = 6,
    HDMITX_VIC_720x480i_60_16_9     = 7,
    HDMITX_VIC_720x240p_60_4_3      = 8,
    HDMITX_VIC_720x240p_60_16_9     = 9,
    HDMITX_VIC_2880x480i_60_4_3     = 10,
    HDMITX_VIC_2880x480i_60_16_9    = 11,
    HDMITX_VIC_2880x240p_60_4_3     = 12,
    HDMITX_VIC_2880x240p_60_16_9    = 13,
    HDMITX_VIC_1440x480p_60_4_3     = 14,
    HDMITX_VIC_1440x480p_60_16_9    = 15,
    HDMITX_VIC_1920x1080p_60_16_9   = 16,
    HDMITX_VIC_720x576p_50_4_3      = 17,
    HDMITX_VIC_720x576p_50_16_9     = 18,
    HDMITX_VIC_1280x720p_50_16_9    = 19,
    HDMITX_VIC_1920x1080i_50_16_9   = 20,
    HDMITX_VIC_720x576i_50_4_3      = 21,
    HDMITX_VIC_720x576i_50_16_9     = 22,
    HDMITX_VIC_720x288p_50_4_3      = 23,
    HDMITX_VIC_720x288p_50_16_9     = 24,
    HDMITX_VIC_2880x576i_50_4_3     = 25,
    HDMITX_VIC_2880x576i_50_16_9    = 26,
    HDMITX_VIC_2880x288p_50_4_3     = 27,
    HDMITX_VIC_2880x288p_50_16_9    = 28,
    HDMITX_VIC_1440x576p_50_4_3     = 29,
    HDMITX_VIC_1440x576p_50_16_9    = 30,
    HDMITX_VIC_1920x1080p_50_16_9   = 31,
    HDMITX_VIC_1920x1080p_24_16_9   = 32,
    HDMITX_VIC_1920x1080p_25_16_9   = 33,
    HDMITX_VIC_1920x1080p_30_16_9   = 34,
    HDMITX_VIC_2880x480p_60_4_3     = 35,
    HDMITX_VIC_2880x480p_60_16_9    = 36,
    HDMITX_VIC_2880x576p_50_4_3     = 37,
    HDMITX_VIC_2880x576p_50_16_9    = 38,
    HDMITX_VIC_1920x1080i_50_16_9_1250_total    = 39,
    HDMITX_VIC_1920x1080i_100_16_9  = 40,
    HDMITX_VIC_1280x720p_100_16_9   = 41,
    HDMITX_VIC_720x576p_100_4_3     = 42,
    HDMITX_VIC_720x576p_100_16_9    = 43,
    HDMITX_VIC_720x576i_100_4_3     = 44,
    HDMITX_VIC_720x576i_100_16_9    = 45,
    HDMITX_VIC_1920x1080i_120_16_9  = 46,
    HDMITX_VIC_1280x720p_120_16_9   = 47,
    HDMITX_VIC_720x480p_120_4_3     = 48,
    HDMITX_VIC_720x480p_120_16_9    = 49,
    HDMITX_VIC_720x480i_120_4_3     = 50,
    HDMITX_VIC_720x480i_120_16_9    = 51,
    HDMITX_VIC_720x576p_200_4_3     = 52,
    HDMITX_VIC_720x576p_200_16_9    = 53,
    HDMITX_VIC_720x576i_200_4_3     = 54,
    HDMITX_VIC_720x576i_200_16_9    = 55,
    HDMITX_VIC_720x480p_240_4_3     = 56,
    HDMITX_VIC_720x480p_240_16_9    = 57,
    HDMITX_VIC_720x480i_240_4_3     = 58,
    HDMITX_VIC_720x480i_240_16_9    = 59,
    HDMITX_VIC_1280x720p_24_16_9    = 60,
    HDMITX_VIC_1280x720p_25_16_9    = 61,
    HDMITX_VIC_1280x720p_30_16_9    = 62,
    HDMITX_VIC_1920x1080p_120_16_9  = 63,
    HDMITX_VIC_1920x1080p_100_16_9  = 64,
} HDMITX_AVI_VIC;

typedef enum
{
    HDMITX_VIDEO_AR_Reserved    = 0,
    HDMITX_VIDEO_AR_4_3         = 1,
    HDMITX_VIDEO_AR_16_9        = 2,
} HDMITX_VIDEO_ASPECT_RATIO;

typedef enum
{
    HDMITX_VIDEO_SI_NoData    = 0,
    HDMITX_VIDEO_SI_Overscanned         = 1,
    HDMITX_VIDEO_SI_Underscanned        = 2,
    HDMITX_VIDEO_SI_Reserved    = 3,
} HDMITX_VIDEO_SCAN_INFO;

typedef enum
{
    HDMITX_VIDEO_AFD_SameAsPictureAR    = 8, // 1000
    HDMITX_VIDEO_AFD_4_3_Center         = 9, // 1001
    HDMITX_VIDEO_AFD_16_9_Center        = 10, // 1010
    HDMITX_VIDEO_AFD_14_9_Center        = 11, // 1011
    HDMITx_VIDEO_AFD_Others = 15, // 0000~ 0111, 1100 ~ 1111
} HDMITX_VIDEO_AFD_RATIO;


typedef enum
{
    HDMITX_VIDEO_VS_No_Addition    = 0, // 000
    HDMITX_VIDEO_VS_4k_2k          = 1, // 001
    HDMITX_VIDEO_VS_3D             = 2, // 010
    HDMITx_VIDEO_VS_Reserved       = 7, // 011~ 111
} HDMITX_VIDEO_VS_FORMAT;


typedef enum
{
    HDMITX_VIDEO_3D_FramePacking     = 0, // 0000
    HDMITX_VIDEO_3D_FieldAlternative = 1, // 0001
    HDMITX_VIDEO_3D_LineAlternative  = 2, // 0010
    HDMITX_VIDEO_3D_SidebySide_FULL  = 3, // 0011
    HDMITX_VIDEO_3D_L_Dep            = 4, // 0100
    HDMITX_VIDEO_3D_L_Dep_Graphic_Dep= 5, // 0101
    HDMITX_VIDEO_3D_TopandBottom     = 6, // 0110
    HDMITX_VIDEO_3D_SidebySide_Half  = 8, // 1000
    HDMITx_VIDEO_3D_Not_in_Use       = 15, // 1111
} HDMITX_VIDEO_3D_STRUCTURE;

typedef enum
{
    HDMITX_EDID_3D_FramePacking               = 1, // 3D_STRUCTURE_ALL_0
    HDMITX_EDID_3D_FieldAlternative           = 2, // 3D_STRUCTURE_ALL_1
    HDMITX_EDID_3D_LineAlternative            = 4, // 3D_STRUCTURE_ALL_2
    HDMITX_EDID_3D_SidebySide_FULL            = 8, // 3D_STRUCTURE_ALL_3
    HDMITX_EDID_3D_L_Dep                      = 16, // 3D_STRUCTURE_ALL_4
    HDMITX_EDID_3D_L_Dep_Graphic_Dep          = 32, // 3D_STRUCTURE_ALL_5
    HDMITX_EDID_3D_TopandBottom               = 64, // 3D_STRUCTURE_ALL_6
    HDMITX_EDID_3D_SidebySide_Half_horizontal = 256, // 3D_STRUCTURE_ALL_8
    HDMITX_EDID_3D_SidebySide_Half_quincunx   = 32768, // 3D_STRUCTURE_ALL_15
} HDMITX_EDID_3D_STRUCTURE_ALL;


typedef enum
{
    HDMITx_VIDEO_4k2k_Reserved    = 0, // 0x00
    HDMITX_VIDEO_4k2k_30Hz        = 1, // 0x01
    HDMITX_VIDEO_4k2k_25Hz        = 2, // 0x02
    HDMITX_VIDEO_4k2k_24Hz        = 3, // 0x03
    HDMITx_VIDEO_4k2k_24Hz_SMPTE  = 4, // 0x04
} HDMITX_VIDEO_4k2k_VIC;


typedef enum
{
    HDMITX_AUDIO_FREQ_NO_SIG  = 0,
    HDMITX_AUDIO_32K          = 1,
    HDMITX_AUDIO_44K          = 2,
    HDMITX_AUDIO_48K          = 3,
    HDMITX_AUDIO_88K          = 4,
    HDMITX_AUDIO_96K          = 5,
    HDMITX_AUDIO_176K         = 6,
    HDMITX_AUDIO_192K         = 7,
    HDMITX_AUDIO_FREQ_MAX_NUM = 8,
} HDMITX_AUDIO_FREQUENCY;

typedef enum
{
    HDMITX_AUDIO_FORMAT_PCM   = 0,
    HDMITX_AUDIO_FORMAT_DSD   = 1,
    HDMITX_AUDIO_FORMAT_HBR   = 2,
    HDMITX_AUDIO_FORMAT_NA    = 3,
} HDMITX_AUDIO_SOURCE_FORMAT;

typedef enum
{
    HDMITX_AUDIO_SOURCE_SPDIF   = 0,
    HDMITX_AUDIO_SOURCE_I2S     = 1,
}HDMITX_AUDIO_SOURCE_TYPE;

typedef enum
{
    HDMITX_AUDIO_CH_2  = 2, // 2 channels
    HDMITX_AUDIO_CH_8  = 8, // 8 channels
} HDMITX_AUDIO_CHANNEL_COUNT;

typedef enum
{
    HDMITX_AUDIO_PCM        = 0, // PCM
    HDMITX_AUDIO_NONPCM     = 1, // non-PCM
} HDMITX_AUDIO_CODING_TYPE;

//HDMITx Capability
typedef enum
{
    E_HDMITX_CAP_SUPPORT_DVI  =0, ///< return true if H/W support scaler device1
}EN_HDMITX_CAPS;

typedef struct
{
    MS_U8 Reserved;
}HDMI_TX_INFO;

typedef struct
{
    MS_BOOL bIsInitialized;
    MS_BOOL bIsRunning;
}HDMI_TX_Status;

typedef struct
{
    // HDMI Tx Current, Pre-emphasis and Double termination
    MS_U8    tm_txcurrent; // TX current control(U4: 0x11302B[13:12], K1: 0x11302B[13:11])
    MS_U8    tm_pren2; // pre-emphasis mode control, 0x11302D[5]
    MS_U8    tm_precon; // TM_PRECON, 0x11302E[7:4]
    MS_U8    tm_pren; // pre-emphasis enable, 0x11302E[11:8]
    MS_U8    tm_tenpre; // Double termination pre-emphasis enable, 0x11302F[3:0]
    MS_U8    tm_ten; // Double termination enable, 0x11302F[7:4]
    MS_U8    tm_pw_ten;// Set bank30_2B bit0
} HDMITX_ANALOG_TUNING;

typedef enum
{
    E_HDCP_DISABLE      = 0, // HDCP disable
    E_HDCP_FAIL = 1, // HDCP fail
    E_HDCP_PASS = 2, // HDCP pass
} HDMITX_HDCP_STATUS;

typedef enum
{
    CHECK_NOT_READY = 0,
    CHECK_REVOKED = 1,
    CHECK_NOT_REVOKED = 2,
}HDMITX_REVOCATION_STATE;

typedef enum
{
    HDMITX_INT_HDCP_DISABLE      = 0, // HDCP disable
    HDMITX_INT_HDCP_FAIL         = 1, // HDCP fail
    HDMITX_INT_HDCP_PASS         = 2, // HDCP pass
    HDMITX_INT_HDCP_PROCESS      = 3, // HDCP processing
} HDMITX_INT_HDCP_STATUS;

typedef enum
{
    E_UNHDCPRX_NORMAL_OUTPUT      = 0, // still display normally
    E_UNHDCPRX_HDCP_ENCRYPTION = 1, // HDCP encryption to show snow screen
    E_UNHDCPRX_BLUE_SCREEN = 2, // blue screen
} HDMITX_UNHDCPRX_CONTROL;

typedef enum
{
    E_HDCPRXFail_NORMAL_OUTPUT      = 0, // still display normally
    E_HDCPRXFail_HDCP_ENCRYPTION = 1, // HDCP encryption to show snow screen
    E_HDCPRXFail_BLUE_SCREEN = 2, // blue screen
} HDMITX_HDCPRXFail_CONTROL;


typedef enum
{
    HDMITX_INPUT_LESS_60MHZ  =0,
    HDMITX_INPUT_60_to_160MHZ  =1,
    HDMITX_INPUT_OVER_160MHZ  =2,
} HDMITX_INPUT_FREQ;

typedef struct
{
    MS_BOOL (*HPD_CB)(void);
}HDMITX_HPD_CB_CONFIG;

typedef struct
{
    MS_BOOL (*HWIIC_ReadBytes)(MS_U16 u16SlaveCfg, MS_U32 uAddrCnt, MS_U8 *pRegAddr, MS_U32 uSize, MS_U8 *pData);
    MS_BOOL (*HWIIC_WriteBytes)(MS_U16 u16SlaveCfg, MS_U32 uAddrCnt, MS_U8 *pRegAddr, MS_U32 uSize, MS_U8 *pData);
}HDMITX_HWIIC_CB_CONFIG;
//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------


//*********************//
//        DVI / HDMI           //
//*********************//

DLL_PUBLIC MS_BOOL MApi_HDMITx_Init(void);

DLL_PUBLIC MS_BOOL MApi_HDMITx_Exit(void);


// HDMI Tx module On/Off
/*
    Before turn on HDMI TX module, video and audio source should be prepared ready and set the following APIs first.
        {
            ...
            MApi_HDMITx_TurnOnOff(TRUE);
            MApi_HDMITx_SetRBChannelSwap(TRUE);
            MApi_HDMITx_SetColorFormat(HDMITX_VIDEO_COLOR_YUV444, HDMITX_VIDEO_COLOR_RGB444);
            MApi_HDMITx_SetVideoOnOff(TRUE);
            MApi_HDMITx_SetHDMITxMode_CD(HDMITX_HDMI, HDMITX_VIDEO_CD_24Bits);
            MApi_HDMITx_SetVideoOutputTiming(HDMITX_RES_1920x1080p_60Hz);
            MApi_HDMITx_Exhibit();
            ...
        }

*/
DLL_PUBLIC void MApi_HDMITx_TurnOnOff(MS_BOOL state);

DLL_PUBLIC void MApi_HDMITx_TurnOnOff_Ex(MS_BOOL state);

// HDMI packet enable or not
DLL_PUBLIC void MApi_HDMITx_EnablePacketGen(MS_BOOL bflag);

// HDMI Tx output is DVI / HDMI mode
DLL_PUBLIC void MApi_HDMITx_SetHDMITxMode(HDMITX_OUTPUT_MODE mode);

// HDMI Tx output is DVI / HDMI mode and color depth
DLL_PUBLIC void MApi_HDMITx_SetHDMITxMode_CD(HDMITX_OUTPUT_MODE mode, HDMITX_VIDEO_COLORDEPTH_VAL val);

// HDMI Tx control color depth
DLL_PUBLIC void MApi_HDMITx_SetHDMITxMode_CD_User(HDMITX_VIDEO_COLORDEPTH_VAL val);

// HDMI Tx TMDS signal On/Off
DLL_PUBLIC void MApi_HDMITx_SetTMDSOnOff(MS_BOOL state);

// HDMI Tx TMDS control disable/enable
DLL_PUBLIC void MApi_HDMITx_DisableTMDSCtrl(MS_BOOL bFlag);

// HDMI Tx R/B channel swap
DLL_PUBLIC void MApi_HDMITx_SetRBChannelSwap(MS_BOOL state);

// HDMI Tx Exhibit funtcion
DLL_PUBLIC void MApi_HDMITx_Exhibit(void);

// HDMI Tx force output mode
DLL_PUBLIC void MApi_HDMITx_ForceHDMIOutputMode(MS_BOOL bflag, HDMITX_OUTPUT_MODE output_mode);

// HDMI Tx force output color format
DLL_PUBLIC MS_BOOL MApi_HDMITx_ForceHDMIOutputColorFormat(MS_BOOL bflag, HDMITX_VIDEO_COLOR_FORMAT output_color);

// HDMI Tx force vide on/off
DLL_PUBLIC void MApi_HDMITx_ForceHDMIVideoOnOff(MS_BOOL bflag, MS_BOOL state);

// Get the connected HDMI Rx status
DLL_PUBLIC MS_BOOL MApi_HDMITx_GetRxStatus(void);

// Get Rx's deep color definition from EDID
DLL_PUBLIC MS_BOOL MApi_HDMITx_GetRxDCInfoFromEDID(HDMITX_VIDEO_COLORDEPTH_VAL *val);

// Get Rx's support video format from EDID
DLL_PUBLIC MS_BOOL MApi_HDMITx_GetRxVideoFormatFromEDID(MS_U8 *pu8Buffer, MS_U8 u8BufSize);

// Get Rx's data block length
DLL_PUBLIC MS_BOOL MApi_HDMITx_GetDataBlockLengthFromEDID(MS_U8 *pu8Length, MS_U8 u8TagCode);

// Get Rx's support audio format from EDID
DLL_PUBLIC MS_BOOL MApi_HDMITx_GetRxAudioFormatFromEDID(MS_U8 *pu8Buffer, MS_U8 u8BufSize);

// Get Rx's support mode from EDID
DLL_PUBLIC MS_BOOL MApi_HDMITx_EDID_HDMISupport(MS_BOOL *HDMI_Support);

// Get Rx's ID Manufacturer Name from EDID
DLL_PUBLIC MS_BOOL MApi_HDMITx_GetRxIDManufacturerName(MS_U8 *pu8Buffer);

DLL_PUBLIC MS_BOOL MApi_HDMITx_GetBksv(MS_U8 *pdata);

DLL_PUBLIC MS_BOOL MApi_HDMITx_GetAksv(MS_U8 *pdata);


// Get Rx's EDID data
DLL_PUBLIC MS_BOOL MApi_HDMITx_GetEDIDData(MS_U8 *pu8Buffer, MS_BOOL BlockIdx);

// Get  Rx's supported 3D structures of specific timing from EDID
DLL_PUBLIC MS_BOOL MApi_HDMITx_GetRx3DStructureFromEDID(HDMITX_VIDEO_TIMING timing, HDMITX_EDID_3D_STRUCTURE_ALL *p3DStructure);

//Get Rx's supported 4k2k timing from EDID
DLL_PUBLIC MS_BOOL  MApi_HDMITx_GetRx4k2kVicFromEDID(MS_U8 u8VicNum, HDMITX_VIDEO_4k2k_VIC *p4k2kVic);

// This function clear settings of user defined packet
DLL_PUBLIC void MApi_HDMITx_PKT_User_Define_Clear(void);

// This function set user defined hdmi packet
DLL_PUBLIC void MApi_HDMITx_PKT_User_Define(HDMITX_PACKET_TYPE packet_type, MS_BOOL def_flag,
HDMITX_PACKET_PROCESS def_process, MS_U8 def_fcnt);

// This function let user define hdmi packet content
DLL_PUBLIC MS_BOOL MApi_HDMITx_PKT_Content_Define(HDMITX_PACKET_TYPE packet_type, MS_U8 *data, MS_U8 length);


//*********************//
//             Video                //
//*********************//

// HDMI Tx video output On/Off
DLL_PUBLIC void MApi_HDMITx_SetVideoOnOff(MS_BOOL state);
// HDMI Tx video color format
DLL_PUBLIC void MApi_HDMITx_SetColorFormat(HDMITX_VIDEO_COLOR_FORMAT in_color, HDMITX_VIDEO_COLOR_FORMAT out_color);
// HDMI Tx video output timing
DLL_PUBLIC void MApi_HDMITx_SetVideoOutputTiming(HDMITX_VIDEO_TIMING mode);
// HDMI Tx video output aspect ratio
DLL_PUBLIC void MApi_HDMITx_SetVideoOutputAsepctRatio(HDMITX_VIDEO_ASPECT_RATIO out_ar);
// HDMI Tx video output Overscan and AFD ratio
DLL_PUBLIC void MApi_HDMITx_SetVideoOutputOverscan_AFD(MS_BOOL bflag, HDMITX_VIDEO_SCAN_INFO out_scaninfo, MS_U8 out_afd);
DLL_PUBLIC void MApi_HDMITx_SetVideoOutputOverscan_AFD_II(MS_BOOL bflag, HDMITX_VIDEO_SCAN_INFO out_scaninfo, MS_U8 out_afd, MS_U8 A0 );
DLL_PUBLIC void MApi_HDMITx_Set_VS_InfoFrame(HDMITX_VIDEO_VS_FORMAT vs_format, HDMITX_VIDEO_3D_STRUCTURE vs_3d, HDMITX_VIDEO_4k2k_VIC vs_vic);

//*********************//
//             Audio                //
//*********************//

// HDMI Tx audio output On/Off
DLL_PUBLIC void MApi_HDMITx_SetAudioOnOff(MS_BOOL state);
// HDMI Tx audio output sampling frequency
// For Uranus
DLL_PUBLIC void MApi_HDMITx_SetAudioFrequency(HDMITX_AUDIO_FREQUENCY freq);
// HDMI Tx Module audio output: sampling frequency, channel count and coding type
// For Oberon
DLL_PUBLIC void MApi_HDMITx_SetAudioConfiguration(HDMITX_AUDIO_FREQUENCY freq, HDMITX_AUDIO_CHANNEL_COUNT ch, HDMITX_AUDIO_CODING_TYPE type);
// HDMI Tx get audio CTS value.
DLL_PUBLIC MS_U32 MApi_HDMITx_GetAudioCTS(void);
// HDMI Tx mute/unmute audio FIFO.
DLL_PUBLIC void MApi_HDMITx_MuteAudioFIFO(MS_BOOL bflag);
// Set HDMI audio source format
DLL_PUBLIC void MApi_HDMITx_SetAudioSourceFormat(HDMITX_AUDIO_SOURCE_FORMAT fmt);
// Set HDMI audio source type
DLL_PUBLIC void MApi_HDMITx_SetAudioSourceType(HDMITX_AUDIO_SOURCE_TYPE src);

//void MApi_HDMITx_SetAudioFrequencyFromMad(void);
//*********************//
//             HDCP                //
//*********************//

// HDMI Tx Get HDCP key (set internal/external HDCP key)
// @param[in] useinternalkey: TRUE -> from internal, FALSE -> from external, like SPI flash
DLL_PUBLIC void MApi_HDMITx_GetHdcpKey(MS_BOOL useinternalkey, MS_U8 *data);
// HDMI Tx HDCP encryption On/Off
DLL_PUBLIC void MApi_HDMITx_SetHDCPOnOff(MS_BOOL state);
// This routine set HDMI Tx AVMUTE
DLL_PUBLIC void MApi_HDMITx_SetAVMUTE(MS_BOOL bflag);
// This routine get HDMI Tx AVMUTE status
DLL_PUBLIC MS_BOOL MApi_HDMITx_GetAVMUTEStatus(void);
// HDMI Tx HDCP status
DLL_PUBLIC HDMITX_HDCP_STATUS MApi_HDMITx_GetHDCPStatus(void);
DLL_PUBLIC HDMITX_HDCP_STATUS MApi_HDMITx_GetHDCPStatus_Ex(void);
// HDCP start Authentication
DLL_PUBLIC void MApi_HDMITx_HDCP_StartAuth(MS_BOOL bFlag);
// HDMI Tx Internal HDCP status
DLL_PUBLIC HDMITX_INT_HDCP_STATUS MApi_HDMITx_GetINTHDCPStatus(void);
// HDMI video output or blank or encryption while connected with unsupport HDCP Rx
DLL_PUBLIC void MApi_HDMITx_UnHDCPRxControl(HDMITX_UNHDCPRX_CONTROL state);
// HDMI video output or blank or encryption while HDCP authentication fail
DLL_PUBLIC void MApi_HDMITx_HDCPRxFailControl(HDMITX_HDCPRXFail_CONTROL state);
// This routine to set the time interval from sent aksv to R0.
DLL_PUBLIC MS_BOOL MApi_HDMITx_SetAksv2R0Interval(MS_U32 u32Interval);
// This API to get active Rx status.
DLL_PUBLIC MS_BOOL MApi_HDMITx_IsHDCPRxValid(void);
// This API return revocation check state
DLL_PUBLIC HDMITX_REVOCATION_STATE MApi_HDMITx_HDCP_RevocationKey_Check(void);
// This API will update revocation list (note : size 1 = 5 bytes !!!)
DLL_PUBLIC void MApi_HDMITx_HDCP_RevocationKey_List(MS_U8 *data, MS_U16 size);
// This API will update the times of Ri checking fail
DLL_PUBLIC MS_BOOL MApi_HDMITx_HDCP_Set_Ri_RetryTimes(MS_U8 u8Times);

// Debug
DLL_PUBLIC MS_BOOL MApi_HDMITx_GetLibVer(const MSIF_Version **ppVersion);

DLL_PUBLIC MS_BOOL MApi_HDMITx_GetInfo(HDMI_TX_INFO *pInfo);

DLL_PUBLIC MS_BOOL MApi_HDMITx_GetStatus(HDMI_TX_Status *pStatus);

DLL_PUBLIC MS_BOOL MApi_HDMITx_HDCP_IsSRMSignatureValid(MS_U8 *data, MS_U32 size);

/**
* @brief set debug mask
* @param[in] u16DbgSwitch DEBUG MASK,
*   0x01: Debug HDMITX, 0x02: Debug HDCP
*/
DLL_PUBLIC MS_BOOL MApi_HDMITx_SetDbgLevel(MS_U16 u16DbgSwitch);

DLL_PUBLIC void MApi_HDMITx_SetHPDGpioPin(MS_U8 u8pin);

// Adjust HDMITx analog setting for HDMI test or compliant issue
DLL_PUBLIC void MApi_HDMITx_AnalogTuning(HDMITX_ANALOG_TUNING *pInfo);

DLL_PUBLIC void MApi_HDMITx_DisableRegWrite(MS_BOOL bFlag);

//*********************//
//             CEC                 //
//*********************//

/// This routine get EDID physical address
DLL_PUBLIC void MApi_HDMITx_GetEDIDPhyAdr(MS_U8 *pdata);
// This routine turn on/off HDMI Tx CEC
DLL_PUBLIC void MApi_HDMITx_SetCECOnOff(MS_BOOL bflag);
// This routine get HDMI Tx CEC On/Off status
DLL_PUBLIC MS_BOOL MApi_HDMITx_GetCECStatus(void);
// This routine force get EDID from reciver
DLL_PUBLIC MS_BOOL MApi_HDMITx_EdidChecking(void);

//*********************//
//      RxBypassMode         //
//*********************//
DLL_PUBLIC MS_BOOL MApi_HDMITx_RxBypass_Mode(HDMITX_INPUT_FREQ freq, MS_BOOL bflag);

DLL_PUBLIC MS_BOOL MApi_HDMITx_Disable_RxBypass(void);


//*************************//
//      CHIP Capaibility   //
//*************************//
DLL_PUBLIC MS_BOOL MApi_HDMITx_GetChipCaps(EN_HDMITX_CAPS eCapType, MS_U32* pRet, MS_U32 ret_size);

DLL_PUBLIC MS_BOOL MApi_HDMITx_Set_HPD_CB(HDMITX_HPD_CB_CONFIG stCfg);

DLL_PUBLIC MS_BOOL MApi_HDMITx_Set_HWIIC_CB(HDMITX_HWIIC_CB_CONFIG stCfg);

#ifdef __cplusplus
}
#endif


#endif // _API_HDMITX_H_

