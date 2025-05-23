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
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file    apiAUDIO.h
/// @brief  AUDIO API
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _API_AUDIO_H_
#define _API_AUDIO_H_

#include "MsCommon.h"
#include "drvAUDIO_if.h"


#ifdef __cplusplus
extern "C"
{
#endif
//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
//-----------------------------------------------------------
#define  MSAPI_AUD_SPDIF_PCM    0
#define  MSAPI_AUD_SPDIF_OFF    1
#define  MSAPI_AUD_SPDIF_NONPCM 2

#define  MSAPI_AUD_DTV_IN       0
#define  MSAPI_AUD_ATV_IN       1
#define  MSAPI_AUD_HDMI_IN      2
#define  MSAPI_AUD_ADC_IN       3
#define  MSAPI_AUD_CardRD_IN    4
#define  MSAPI_AUD_MM_IN    5

#define  MSAPI_AUD_AVsync       0
#define  MSAPI_AUD_FreeRun      1

/// A2/FM/_SOUND_MOD Report info
#define _STATUS_MOD_MONO_EXIST                  0x01
#define _STATUS_MOD_STEREO_EXIST                0x02
#define _STATUS_MOD_DUAL_EXIST                  0x04
#define _STATUS_MOD_A2_PILOT_EXIST             0x08
#define _STATUS_MOD_A2_PRIMARY_EXIST           0x10
#define _STATUS_MOD_A2_SECONDARY_EXIST         0x20
#define _STATUS_MOD_A2_DK2_EXIST                0x40
#define _STATUS_MOD_A2_DK3_EXIST                0x80

/// NICAM_SOUND_MOD Report info
#define _MASK_NICAM_STATUS_INFO                 0x0F
#define _NICAM_SOUND_MODE_LOCK_STATE            0x05
#define _MASK_NICAM_SOUND_MODE_INFO             0xF0
#define _NICAM_SOUND_MOD_MONO_NOT_EXIST         0x00
#define _NICAM_SOUND_MOD_NICAM_MONO             0x10
#define _NICAM_SOUND_MOD_NICAM_STEREO           0x20
#define _NICAM_SOUND_MOD_NICAM_DUAL             0x30
#define _NICAM_SOUND_MOD_MONO_EXIST             0x80
#define _NICAM_SOUND_MOD_MONO_n_NICAM_MONO     0x90
#define _NICAM_SOUND_MOD_MONO_n_NICAM_STEREO   0xA0
#define _NICAM_SOUND_MOD_MONO_n_NICAM_DUAL     0xB0

/********************************
*   SIF Standard
********************************/
#define _STD_SEL_INVALID    0x00
#define _STD_SEL_M_BTSC     0x01
#define _STD_SEL_M_EIAJ     0x02
#define _STD_SEL_M_A2       0x03
#define _STD_SEL_BG_A2      0x04
#define _STD_SEL_DK1_A2     0x05
#define _STD_SEL_DK2_A2     0x06
#define _STD_SEL_DK3_A2     0x07
#define _STD_SEL_BG_NICAM   0x08
#define _STD_SEL_DK_NICAM   0x09
#define _STD_SEL_I_NICAM    0x0A
#define _STD_SEL_L_NICAM    0x0B
#define _STD_SEL_AUTO       0xE0

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
///< Decoder command type
typedef enum
{
    MSAPI_AUD_DVB_DECCMD_STOP = 0x0,   ///< 1st DSP stop
    MSAPI_AUD_DVB_DECCMD_PLAY,         ///< 1st DSP play
    MSAPI_AUD_DVB_DECCMD_RESYNC,
    MSAPI_AUD_DVB_DECCMD_FREE_RUN,
    MSAPI_AUD_DVB_DECCMD_AVSYNC,
    MSAPI_AUD_DVB_DECCMD_PLAYFILE,     ///< 1st DSP play file
    MSAPI_AUD_DVB_DECCMD_PAUSE,        ///< 1st DSP pause
    MSAPI_AUD_DVB_DECCMD_PLAYFILETSP,  ///< 1st DSP play TS file
    MSAPI_AUD_DVB_DECCMD_STARTBROWSE,
    MSAPI_AUD_DVB_DECCMD_PLAYFRAME,    ///<1st DSP play MM (for uniplayer/Mstreamer)
    MSAPI_AUD_DVB_DECCMD_PLAYFRAME_STAGE, ////<1st DSP play MM (for stagecraft)
    MSAPI_AUD_DVB_DECCMD_PLAYFRAME_GS,  ///<1st DSP play MM (for uniplayer/Gstreamer ES copy)
    MSAPI_AUD_DVB_DECCMD_PLAYMM_FILE2,
    MSAPI_AUD_DVB_DECCMD_PLAY_AD,
    MSAPI_AUD_DVB_DECCMD_STOP_AD,
    MSAPI_AUD_DVB_DECCMD_SKIP,
    // SE DSP
    MSAPI_AUD_DVB2_DECCMD_STOP=0x10,   ///< 2nd DSP stop
    MSAPI_AUD_DVB2_DECCMD_PLAY,        ///< 2nd DSP play
    MSAPI_AUD_DVB2_DECCMD_RESYNC,
    MSAPI_AUD_DVB2_DECCMD_FREE_RUN,
    MSAPI_AUD_DVB2_DECCMD_AVSYNC,
    MSAPI_AUD_DVB2_DECCMD_PLAYFILE,    ///< 2nd DSP play file
    MSAPI_AUD_DVB2_DECCMD_PAUSE,       ///< 2nd DSP pause
    MSAPI_AUD_DVB2_DECCMD_PLAYFILETSP,  ///< 2nd DSP play TS file

} En_DVB_decCmdType;

///< Encoder Command
typedef enum
{
    MSAPI_AUD_DVB_ENCCMD_STOP = 0x0,   ///< stop encode
    MSAPI_AUD_DVB_ENCCMD_START         ///<  start encode
}En_DVB_EncCmdType;

///< ATV SIF Command
typedef enum
{
    MSAPI_AUD_SIF_CMD_SET_STOP = 0x00,
    MSAPI_AUD_SIF_CMD_SET_PLAY = 0x01,
    MSAPI_AUD_SIF_CMD_ENABLE_HIDEV = 0x02,
    MSAPI_AUD_SIF_CMD_SET_HIDEV_FILTER_BW_LEVEL = 0x03,
    MSAPI_AUD_SIF_CMD_RESET_FC_TRACKING = 0x04,
    MSAPI_AUD_SIF_CMD_ENABLE_FC_TRACKING = 0x05,
    MSAPI_AUD_SIF_CMD_SET_ADC_FROM_VIF_PATH = 0x06,
    MSAPI_AUD_SIF_CMD_ENABLE_AUTO_MUTE = 0x07,
    MSAPI_AUD_SIF_CMD_ENABLE_BEEPER_FUNCTION = 0x08,
    MSAPI_AUD_SIF_CMD_SET_BEEPER_TONE = 0x09,
    MSAPI_AUD_SIF_CMD_ENABLE_AGC = 0x0A,
    MSAPI_AUD_SIF_CMD_RESET_AGC = 0x0B,
    MSAPI_AUD_SIF_STANDARD_RESET = 0x0C,
    MSAPI_AUD_SIF_CMD_DETECT_MAIN_STD_ONLY = 0x0D,
    MSAPI_AUD_SIF_CMD_ENABLE_AUTO_SOUNDMODE = 0x0E,
    MSAPI_AUD_SIF_CMD_FMTX_SET_PREEMP = 0x0F,
    MSAPI_AUD_SIF_CMD_SET_ENC_CMD = 0x10,
    MSAPI_AUD_SIF_CMD_SET_ENC_Input_attenuation= 0x11,
    MSAPI_AUD_SIF_CMD_SET_ENC_Output_scaling= 0x12,
    MSAPI_AUD_SIF_CMD_SET_BTSC_Enc_M_Gain=0x13,
    MSAPI_AUD_SIF_CMD_SET_BTSC_Enc_D_Gain= 0x14,
    MSAPI_AUD_SIF_CMD_SET_BTSC_Enc_SAP_Gain= 0x15,

    MSAPI_AUD_SIF_CMD2_ADC_VIA_VIF_PIN = 0x01
} En_AUD_SIF_CmdType;

typedef enum
{
   MSAPI_AUD_SIF_42M,
   MSAPI_AUD_SIF_43M,
   MSAPI_AUD_SIF_44M,
} En_AUD_VIF_Type;

enum
{
    MSAPI_AUD_SIF_HIDEV_FILTER_BW_DEFAULT = 0x00, // equal middle filter bandwidth
    MSAPI_AUD_SIF_HIDEV_FILTER_BW_L1 = 0x10,    // narrowest filter bandwidth
    MSAPI_AUD_SIF_HIDEV_FILTER_BW_L2 = 0x20,    // middle filter bandwidth
    MSAPI_AUD_SIF_HIDEV_FILTER_BW_L3 = 0x30     // widest filter bandwidth
};

///< Decoder Number
typedef enum
{
   DVB_Audio_Decoder1,             ///<Audio decoder1
   DVB_Audio_Decoder2              ///<Audio decoder2
} En_DVB_AudioDecoder;

///< Advance sound effect type
typedef enum
{
    MSAPI_AUD_ADVSND_PL2,          ///<ID = 0
    MSAPI_AUD_ADVSND_BBE,          ///<ID = 1
    MSAPI_AUD_ADVSND_SRS,          ///<ID = 2
    MSAPI_AUD_ADVSND_VDS,          ///<ID = 3
    MSAPI_AUD_ADVSND_VSPK,         ///<ID = 4
    MSAPI_AUD_ADVSND_SUPV,         ///<ID = 5
    MSAPI_AUD_ADVSND_TSHD,         ///<ID = 6
    MSAPI_AUD_ADVSND_DTS_ULTRATV,  ///<ID = 7
    MSAPI_AUD_ADVSND_TSHDVIQ,      ///<ID = 8
    MSAPI_AUD_ADVSND_AUDYSSEY,     ///<ID = 9
    MSAPI_AUD_ADVSND_DBX,      ///<ID = 10
    MSAPI_AUD_ADVSND_SSHD_TSHD,         ///<ID = 55
    MSAPI_AUD_ADVSND_PURESND,         ///<ID = 66

} En_DVB_advsndType;

///< Select audio output mode type
typedef enum
{
    MSAPI_AUD_DVB_SOUNDMODE_STEREO,
    MSAPI_AUD_DVB_SOUNDMODE_LEFT,
    MSAPI_AUD_DVB_SOUNDMODE_RIGHT,
    MSAPI_AUD_DVB_SOUNDMODE_NONE,
} En_DVB_soundModeType;

///< Melody play command type
typedef enum
{
    MSAPI_AUD_MELODY_START,
    MSAPI_AUD_MELODY_STOP,
    MSAPI_AUD_MELODY_RESUME,
    MSAPI_AUD_MELODY_CHK_STATUS
}En_DVB_melodyPlayCmdType;

typedef struct
{
    MS_U32 u32BufStartAddr;
    MS_U32 u32BufEndAddr;
    MS_U32 u32BufWrPtr;
    MS_U32 u32BufTotalSize;
    MS_U32 u32BufferedPCMSize;
}audioBufMonitor;

typedef struct
{
    audioBufMonitor _stBufInfo;
    MS_U32 u32ReqAddr;
    MS_U32 u32ReqSize;
    MS_U32 u32RetAddr1;
    MS_U32 u32RetSize1;
    MS_U32 u32RetAddr2;
    MS_U32 u32RetSize2;
    MS_U8 u8RingStatus;
}audioRingTask;


///< Melody play status
typedef enum
{
    MSAPI_AUD_MELODY_NEED_MORE_DATA,
    MSAPI_AUD_MELODY_PLAY_INIT,
    MSAPI_AUD_MELODY_PLAY_ING,
    MSAPI_AUD_MELODY_PLAY_DONE,
    MSAPI_AUD_MELODY_PLAY_STOP
}En_DVB_melodyPlayStatus;

///< Select AD output type
enum
{
    AD_OUT_SPEAKER, ///< AD out in speaker only
    AD_OUT_HP,      ///< AD out in HP only
    AD_OUT_BOTH,    ///< AD out both in speaker & HP
    AD_OUT_NONE     ///< no AD out
};

///< Audio System Type
typedef enum
{
    E_AUDIO_SYSTEM_BTSC,
    E_AUDIO_SYSTEM_A2,
    E_AUDIO_SYSTEM_EIAJ
}AUDIO_SYSTEM_TYPE;

///< Audio SIF Standard Type
typedef enum
{
    E_AUDIOSTANDARD_BG          = 0x00,         ///< Audio standard BG
    E_AUDIOSTANDARD_BG_A2       = 0x01,         ///< Audio standard BG A2
    E_AUDIOSTANDARD_BG_NICAM    = 0x02,         ///< Audio standard BG NICAM
    E_AUDIOSTANDARD_I           = 0x03,         ///< Audio standard I
    E_AUDIOSTANDARD_DK          = 0x04,         ///< Audio standard DK
    E_AUDIOSTANDARD_DK1_A2      = 0x05,         ///< Audio standard DK1 A2
    E_AUDIOSTANDARD_DK2_A2      = 0x06,         ///< Audio standard DK2 A2
    E_AUDIOSTANDARD_DK3_A2      = 0x07,         ///< Audio standard DK3 A2
    E_AUDIOSTANDARD_DK_NICAM    = 0x08,         ///< Audio standard DK NICAM
    E_AUDIOSTANDARD_L           = 0x09,         ///< Audio standard L
    E_AUDIOSTANDARD_M           = 0x0A,         ///< Audio standard M
    E_AUDIOSTANDARD_M_BTSC      = 0x0B,         ///< Audio standard M BTSC
    E_AUDIOSTANDARD_M_A2        = 0x0C,         ///< Audio standard M A2
    E_AUDIOSTANDARD_M_EIA_J     = 0x0D,         ///< Audio standard M EIA J
    E_AUDIOSTANDARD_NOTSTANDARD = 0x0F          ///< Not Audio standard
} AUDIOSTANDARD_TYPE;

///< Audio Stream type
typedef enum
{
    E_AUDIOSTREAM_INVALID       = 0x00,
    E_AUDIOSTREAM_MPEG          = 0x01,
    E_AUDIOSTREAM_AC3           = 0x02,
    E_AUDIOSTREAM_MPEG_AD       = 0x03,
    E_AUDIOSTREAM_MPEG4         = 0x04,
    E_AUDIOSTREAM_AAC           = 0x05,
    E_AUDIOSTREAM_AC3P          = 0x06,
    E_AUDIOSTREAM_AC3_AD        = 0x07,
    E_AUDIOSTREAM_AC3P_AD       = 0x08,
    E_AUDIOSTREAM_AACP_AD       = 0x09,
    E_AUDIOSTREAM_DRA           = 0x0A
} AUDIOSTREAM_TYPE;

///< Audio mode type
typedef enum
{
    E_AUDIOMODE_INVALID      = 0x00,            ///< Audio Mode Invalid
    E_AUDIOMODE_MONO         = 0x01,           ///< Audio Mode MONO
    E_AUDIOMODE_FORCED_MONO  = 0x02,           ///< Audio Mode Forced MONO
    E_AUDIOMODE_G_STEREO     = 0x03,           ///< Audio Mode G Stereo
    E_AUDIOMODE_K_STEREO     = 0x04,           ///< Audio Mode  K Stereo
    E_AUDIOMODE_MONO_SAP     = 0x05,           ///< Audio Mode MONO SAP
    E_AUDIOMODE_STEREO_SAP   = 0x06,           ///< Audio Mode Stereo SAP
    E_AUDIOMODE_DUAL_A       = 0x07,           ///< Audio Mode Dual A
    E_AUDIOMODE_DUAL_B       = 0x08,           ///< Audio Mode Dual B
    E_AUDIOMODE_DUAL_AB      = 0x09,           ///< Audio Mode Dual AB
    E_AUDIOMODE_NICAM_MONO   = 0x0A,           ///< Audio Mode NICAM MONO
    E_AUDIOMODE_NICAM_STEREO = 0x0B,           ///< Audio Mode NICAM Stereo
    E_AUDIOMODE_NICAM_DUAL_A = 0x0C,           ///< Audio Mode NICAM DUAL A
    E_AUDIOMODE_NICAM_DUAL_B = 0x0D,           ///< Audio Mode NICAM DUAL B
    E_AUDIOMODE_NICAM_DUAL_AB = 0x0E,           ///< Audio Mode NICAM DUAL AB
    E_AUDIOMODE_HIDEV_MONO   = 0x0F,           ///< Audio Mode HIDEV MONO
    E_AUDIOMODE_LEFT_LEFT    = 0x10,           ///< Audio Mode Left left
    E_AUDIOMODE_RIGHT_RIGHT  = 0x11,           ///< Audio Mode right right
    E_AUDIOMODE_LEFT_RIGHT   = 0x12           ///< Audio Mode left right
} AUDIOMODE_TYPE;

///< sif carrier status define
typedef enum
{ // Never change the value of enumeration. Because each bit has weight.
    E_STATE_AUDIO_NO_CARRIER           = 0x00, ///< No carrier detect
    E_STATE_AUDIO_PRIMARY_CARRIER      = 0x01, ///< Carrier 1 exist
    E_STATE_AUDIO_SECONDARY_CARRIER    = 0x02, ///< Carrier 2 exist
    E_STATE_AUDIO_NICAM                = 0x04, ///< Nicam lock state
    E_STATE_AUDIO_STEREO               = 0x08, ///< A2 Stereo exist
    E_STATE_AUDIO_BILINGUAL            = 0x10, ///< A2 Dual exist
    E_STATE_AUDIO_PILOT                = 0x20, ///< A2 Pilot exist
    E_STATE_AUDIO_DK2                  = 0x40, ///< Sound standard is DK2
    E_STATE_AUDIO_DK3                  = 0x80 ///< Sound standard is DK3
} AUDIOSTATUS;

///< EQ band define
typedef enum
{
    E_EQUALIZER_BAND_1    = 0x00, ///< EQ Band1
    E_EQUALIZER_BAND_2    = 0x01, ///< EQ Band2
    E_EQUALIZER_BAND_3    = 0x02, ///< EQ Band3
    E_EQUALIZER_BAND_4    = 0x03, ///< EQ Band4
    E_EQUALIZER_BAND_5    = 0x04, ///< EQ Band5
    E_EQUALIZER_BAND_6    = 0x05, ///< EQ Band6
    E_EQUALIZER_BAND_7    = 0x06  ///< EQ Band7
} EQUALIZER_BAND;

///< Audio command type
typedef enum
{
    E_AUDIO_COM_PLAY,
    E_AUDIO_COM_STOP,
    E_AUDIO_COM_RESYNC,
    E_AUDIO_COM_FREE_RUN,
    E_AUDIO_COM_LOCK_AVSYNC,
    E_AUDIO_COM_FILE_PLAY,      // start playing file based audio in MHEG5
    E_AUDIO_COM_FILE_PAUSE,      // pause playing file based audio in MHEG5
    E_AUDIO_DVB2_COM_PLAY,
    E_AUDIO_DVB2_COM_STOP,
    E_AUDIO_DVB2_COM_RESYNC,
    E_AUDIO_DVB2_COM_FREE_RUN,
    E_AUDIO_DVB2_COM_LOCK_AVSYNC,
    E_AUDIO_DVB2_COM_FILE_PLAY,      // start playing file based audio in MHEG5
    E_AUDIO_DVB2_COM_FILE_PAUSE      // pause playing file based audio in MHEG5
} DTV_AUDIO_COMMAND;

enum
{
    MSAPI_AUD_MODE_STEREO,  ///< 0:STEREO
    MSAPI_AUD_MODE_LL,      ///< 1: LL
    MSAPI_AUD_MODE_RR,      ///< 2: RR
    MSAPI_AUD_MODE_MUTE     ///< 3:MUTE
};

/// TSXT mode
enum
{
    MSAPI_TSXT_OFF     = 0,
    MSAPI_TSXT_STEREO,
    MSAPI_TSXT_3D_MONO,
    MSAPI_TSXT_MAX,
    MSAPI_TSXT_INVALID = -1
};

typedef enum
{
    E_TSXT_MOD_STEREO,         ///< 0:STEREO
    E_TSXT_MOD_3D_MONO,        ///< 0:MONO
    E_TSXT_MOD_MAX             ///< 0:PASSIVE MATRIX
} TSXT_MODETYPE;

///< TSXT parameter
typedef enum
{
    E_TSXT_PARA_TS_GAIN,
    E_TSXT_PARA_DC_GAIN,
    E_TSXT_PARA_TB_GAIN,
    E_TSXT_PARA_SPK,
    E_TSXT_PARA_INPUT_MOD,
    E_TSXT_PARA_OUTPUT_GAIN
} TSXT_PARATYPE;

///< Audio DSP system type
typedef enum
{
    E_AUDIO_DSP_MPEG,                       ///< AUDIO DSP = MPEG
    E_AUDIO_DSP_AC3,                        ///< AUDIO DSP = AC3
    E_AUDIO_DSP_AC3_AD,                     ///< AUDIO DSP = AC3 AD
    E_AUDIO_DSP_SIF,                        ///< AUDIO DSP = SIF
    E_AUDIO_DSP_AACP,                       ///< AUDIO DSP = HE-AAC
    E_AUDIO_DSP_MPEG_AD,                    ///< AUDIO DSP = MPEG AD
    E_AUDIO_DSP_AC3P,                       ///< AUDIO DSP = AC3+
    E_AUDIO_DSP_AC3P_AD,                    ///< AUDIO DSP = AC3P AD
    E_AUDIO_DSP_AACP_AD,                    ///< AUDIO DSP = HE-AAC AD
    E_AUDIO_DSP_DTS,                        ///< AUDIO DSP = DTS
    E_AUDIO_DSP_MS10_DDT,                   ///< AUDIO DSP = MS10 DDT
    E_AUDIO_DSP_MS10_DDC,                   ///< AUDIO DSP = MS10 DDC
    E_AUDIO_DSP_FLAC,                       ///< AUDIO DSP = FLAC
    E_AUDIO_DSP_FM_RADIO,                   ///< AUDIO DSP = FM RADIO
    E_AUDIO_DSP_KTV,
    E_AUDIO_DSP_RESERVE3,
    E_AUDIO_DSP_RESERVE2,
    E_AUDIO_DSP_RESERVE1,
    E_AUDIO_DSP_MAX,
    E_AUDIO_DSP_INVALID=0xFF                ///< AUDIO DSP = Invalid
} AUDIO_DSP_SYSTEM;

///<Audio source switch group
typedef enum
{
    E_AUDIO_GROUP_MAIN,      ///< Main speaker, HP, Line-out , S/PDIF PCM mode
    E_AUDIO_GROUP_SCART,    ///< SCART
    E_AUDIO_GROUP_SPDIF,    ///< SPDIF non-pcm
    E_AUDIO_GROUP_SUB,       ///< SUB source for PIP application
    E_AUDIO_GROUP_INVALID
} AUDIO_SWITCH_GROUP;

#define TBL_INDEX  MS_U8
#define TBL_LENGTH MS_U8

#define FM_RADIO_TBL_LENGTH    6   ///< FM Radio Threshold table length
#define BTSC_TBL_LENGTH       10   ///< BTSC Threshold table length
#define A2_M_TBL_LENGTH       10   ///< A2_M Threshold table length
#define A2_BG_TBL_LENGTH      10   ///< A2_BG Threshold table length
#define A2_DK_TBL_LENGTH      10   ///< A2_DK Threshold table length
#define FM_I_TBL_LENGTH        4   ///< FM_I Threshold table length
#define AM_TBL_LENGTH          2   ///< AM Threshold table length
#define NICAM_BG_TBL_LENGTH    2   ///< NICAM_BG Threshold table length
#define NICAM_I_TBL_LENGTH     2   ///< NICAM_I Threshold table length
#define HIDEV_M_TBL_LENGTH     4   ///< HIDEV_M Threshold table length
#define HIDEV_BG_TBL_LENGTH    4   ///< HIDEV_BG Threshold table length
#define HIDEV_DK_TBL_LENGTH    4   ///< HIDEV_DK Threshold table length
#define HIDEV_I_TBL_LENGTH     4   ///< HIDEV_I Threshold table length

#define FM_RADIO_TBL_INDEX      0
#define BTSC_TBL_INDEX          0
#define A2_M_TBL_INDEX          0
#define A2_BG_TBL_INDEX      A2_M_TBL_INDEX + A2_M_TBL_LENGTH
#define A2_DK_TBL_INDEX      A2_BG_TBL_INDEX + A2_BG_TBL_LENGTH
#define FM_I_TBL_INDEX       A2_DK_TBL_INDEX + A2_DK_TBL_LENGTH
#define AM_TBL_INDEX         FM_I_TBL_INDEX + FM_I_TBL_LENGTH
#define NICAM_BG_TBL_INDEX  AM_TBL_INDEX + AM_TBL_LENGTH
#define NICAM_I_TBL_INDEX   NICAM_BG_TBL_INDEX + NICAM_BG_TBL_LENGTH
#define HIDEV_M_TBL_INDEX   NICAM_I_TBL_INDEX + NICAM_I_TBL_LENGTH
#define HIDEV_BG_TBL_INDEX  HIDEV_M_TBL_INDEX + HIDEV_M_TBL_LENGTH
#define HIDEV_DK_TBL_INDEX  HIDEV_BG_TBL_INDEX + HIDEV_BG_TBL_LENGTH
#define HIDEV_I_TBL_INDEX   HIDEV_DK_TBL_INDEX + HIDEV_DK_TBL_LENGTH

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

//============================================================
// AUDIO_SYSTEM RELATIONAL API FUNCTION
//============================================================
 void    MApi_AUDIO_SetPowerOn(MS_BOOL bFlag);
 void    MApi_AUDIO_Initialize(void);
 void    MApi_AUDIO_SetSystemInfo(AUDIO_INIT_INFO *pau_info);
 void    MApi_Audio_Monitor(void);

 void    MApi_AUDIO_SetOutConnectivity(void);
 void    MApi_AUDIO_SetPathInfo(AUDIO_PATH_INFO *path_info);
 void    MApi_AUDIO_SetOutputInfo(AUDIO_OUT_INFO *pout_info);
 void    MApi_AUDIO_SetSourceInfo(AUDIO_SOURCE_INFO_TYPE eSourceType);
 void    MApi_AUDIO_InputSwitch(AUDIO_INPUT_TYPE enSource, AUDIO_SWITCH_GROUP enGroup);
 MS_BOOL MApi_AUDIO_SetDataCaptureSource(AUDIO_DEVICE_TYPE eID, AUDIO_CAPTURE_SOURCE_TYPE eSource);
 void   MApi_AUDIO_SetOutputSourceInfo(OUTPUT_SOURCE_INFO *pSourceInfo);
 void   MApi_AUDIO_GetOutputSourceInfo(OUTPUT_SOURCE_INFO *pSourceInfo);

 MS_U16  MApi_AUDIO_ReadMailBox(MS_BOOL bDspType, MS_U8 u8ParamNum);
 void    MApi_AUDIO_WriteMailBox(MS_BOOL bDspType, MS_U8 u8ParamNum, MS_U16 u16Data);

 void    MApi_AUDIO_FwTriggerDSP(MS_U16 u16Cmd);
 void    MApi_AUDIO_SetFwStatus(MS_BOOL en);
 void    MApi_AUDIO_ExitAudioSystem(void);

 void    MApi_AUDIO_RebootDsp(MS_BOOL bDspType);
 void    MApi_AUDIO_SendIntrupt(MS_BOOL bDspType,MS_U8 u8Cmd);

 void    MApi_AUDIO_I2S_SetMode(AUDIO_I2S_MODE_TYPE u8Mode, AUDIO_I2S_MODE_VALUE  u8Val);

 void    MApi_AUDIO_WritePreInitTable(void);            //Refine power on sequence for earphone & DAC pop noise issue
 void    MApi_AUDIO_EnaEarphone_LowPower_Stage(void);   //Refine power on sequence for earphone & DAC pop noise issue
 void    MApi_AUDIO_EnaEarphone_HighDriving_Stage(void);//Refine power on sequence for earphone & DAC pop noise issue

//=============================================================
// AUDIO_SOUND RELATIONAL API FUNCTION
//=============================================================
 MS_BOOL MApi_SND_ProcessEnable(Sound_ENABLE_Type Type, MS_BOOL enable);
 MS_BOOL MApi_SND_SetParam1( Sound_SET_PARAM_Type Type, MS_U16 param1, MS_U16 param2);
 MS_U16  MApi_SND_GetParam1( Sound_GET_PARAM_Type Type, MS_U16 param1);
 MS_BOOL MApi_AUDIO_SOUND_SetParam( Sound_SET_Type Type, MS_U32 *p_param );

 void    MApi_AUDIO_SetAbsoluteVolume( MS_U8 u8Path, MS_U8 u8Vol1, MS_U8 u8Vol2 );
 void    MApi_AUDIO_SetPreScale ( MS_U8 u8Path, MS_U8 u8PreScale );
 void    MApi_AUDIO_SetMute( MS_U8 u8Path, MS_BOOL EnMute);
 void    MApi_AUDIO_SourceConnect(AUDIO_OUTPORT_SOURCE_TYPE eConnectSourceType, MS_BOOL bIsConnect );
 void    MApi_AUDIO_SetBalance ( MS_U8 u8Balance );
 void    MApi_AUDIO_EnableEQ ( MS_BOOL EnEQ );
 void    MApi_AUDIO_EnableTone ( MS_BOOL EnTone );
 void    MApi_AUDIO_EnableAutoVolume ( MS_BOOL EnAVL );
 void    MApi_AUDIO_EnableSurround ( MS_BOOL EnSurround );
 void    MApi_AUDIO_EnableBalance( MS_U8 EnBalance );
 void    MApi_AUDIO_SetDynamicBass( MS_U8 u8DynamicBass );
 void    MApi_AUDIO_SetBass( MS_U8 u8Bass );
 void    MApi_AUDIO_AbsoluteBass( MS_U8 u8Bass );
 void    MApi_AUDIO_SetTreble( MS_U8 u8Treble );
 void    MApi_AUDIO_AbsoluteTreble( MS_U8 u8Treble );
 void    MApi_AUDIO_SetEq(MS_U8 u8band, MS_U8 u8level);
 void    MApi_AUDIO_SetEq_7band(MS_U8 u8band, MS_U8 u8level);
 void    MApi_AUDIO_SetAvcThreshold(MS_U8 Level);
 void    MApi_AUDIO_SetAvcMode( MS_U8 AvcMode );
 void    MApi_AUDIO_SetAvcAT( MS_U8 AvcAT );
 void    MApi_AUDIO_SetAvcRT( MS_U8 AvcRT );
 void    MApi_AUDIO_SetBufferProcess(MS_U8 DelayTime);
 void    MApi_AUDIO_SetSurroundXA( MS_U8 mode );
 void    MApi_AUDIO_SetSurroundXB( MS_U8 mode );
 void    MApi_AUDIO_SetSurroundXK( MS_U8 mode );
 void    MApi_AUDIO_SetSurroundLPFGain( MS_U8 mode );
 MS_U8   MApi_AUDIO_ConvertVolumeUnit(MS_U8 UiVolume);
// void  MApi_AUDIO_SetMaxClipping( MS_U8 MaxClippingValue, MS_U16 Volume );
 void    MApi_AUDIO_SetPEQCoef(AUDIO_PEQ_COEF *peq_coef);
 void    MApi_AUDIO_EnablePEQ(MS_BOOL Enpeq);
 MS_BOOL MApi_AUDIO_EnableDcRemove(MS_BOOL EnDcRemove);

//============================================================
// AUDIO_DTV RELATIONAL API FUNCTION
//============================================================

 // PIP Multiplayer
 MS_BOOL MApi_AUDIO_ReleaseDecodeSystem(AUDIO_DEC_ID DecId);
 MS_BOOL MApi_AUDIO_SetDecodeSystem(AUDIO_DEC_ID DecId, AudioDecStatus_t * p_AudioDecStatus);
 MS_BOOL MApi_AUDIO_GetDecodeSystem(AUDIO_DEC_ID DecId, AudioDecStatus_t * p_AudioDecStatus);
 MS_BOOL MApi_AUDIO_SetDecodeCmd(AUDIO_DEC_ID DecId, En_DVB_decCmdType enDecComamnd);
 AUDIO_DEC_ID MApi_AUDIO_OpenDecodeSystem(AudioDecStatus_t * p_AudioDecStatus);

 MS_BOOL MApi_AUDIO_SetSystem(En_DVB_decSystemType enDecSystem);
 MS_BOOL MApi_AUDIO_GetDecSysSupportStatus(En_DVB_decSystemType enDecSystem);
 void    MApi_AUDIO_SetH264StreamID_Mod(MS_U8 stream_id_mod);
 MS_BOOL MApi_AUDIO_IsMadLock(void);
 En_DVB_decCmdType MApi_AUDIO_GetDecStatus ( void );

//Play command
 AU_DVB_DECCMD MApi_AUDIO_GetCommand(En_DVB_AudioDecoder AdoDecoderNo);
 void    MApi_AUDIO_SetCommand(En_DVB_decCmdType enDecComamnd);

//Decoder Lock information
 MS_BOOL MApi_AUDIO_GetMAD_LOCK(void);

// Audio Description
 void    MApi_AUDIO_SetADOutputMode (MS_U8 out_mode);
 void    MApi_AUDIO_SetADAbsoluteVolume (MS_U8 AbsVolume);
 void    MApi_AUDIO_ADSetMute(MS_BOOL EnMute);

//============================================================
// AUDIO_PVR RELATED FUNCTIONS
//============================================================
 void    MApi_AUDIO_SetEncInit(AU_DVB_ENCBR BitRate, MS_U8 u8EncFrameNum);
 void    MApi_AUDIO_SetEncCommand(AU_DVB_ENCCMD enEncComamnd);
 void    MApi_AUDIO_GetEncodeFrameInfo(AU_DVB_ENC_FRAME_INFO *EncFrameInfo);
 MS_BOOL MApi_AUDIO_GetEncodeDoneFlag(void);

//============================================================
// AUDIO_ATV RELATIONAL API FUNCTION
//============================================================
 void    MApi_AUDIO_SIF_SetStandard(AUDIOSTANDARD_TYPE standard_type);
 MS_U8   MApi_AUDIO_SIF_GetSoundMode(void);
 MS_BOOL MApi_AUDIO_SIF_SetSoundMode(MS_U8 u8SifSoundMode);
 MS_BOOL MApi_AUDIO_SIF_GetAudioStatus(AUDIOSTATUS * eAudioStatus);
 void    MApi_AUDIO_SIF_StartAutoStandardDetection(void);
 AUDIOSTANDARD_TYPE MApi_AUDIO_SIF_GetResultOfAutoStandardDetection(void);
 AUDIOSTANDARD_TYPE MApi_AUDIO_SIF_ConvertToBasicAudioStandard(AUDIOSTANDARD_TYPE eStandard);
 void    MApi_AUDIO_SIF_SetThreshold(THR_TBL_TYPE code *ThrTbl);
 void    MApi_AUDIO_SIF_SetPrescale(MS_U8, int);
 MS_BOOL MApi_AUDIO_SIF_IsPALType(AUDIO_SIF_PAL_TYPE pal_type);
 void    MApi_AUDIO_SIF_SetPALType(AUDIO_SIF_PAL_TYPE pal_type);
 void    MApi_AUDIO_SIF_SendCmd(En_AUD_SIF_CmdType enAudSifCommand, MS_U8 comm_arg1, MS_U8 comm_arg2);
 void    MApi_AUDIO_SIF_Shift(En_AUD_VIF_Type type);

//============================================================
// AUDIO_FM_RADIO RELATIONAL API FUNCTION
//============================================================
 MS_U8 MApi_AUDIO_FM_RADIO_GetSoundMode(void);
 void MApi_AUDIO_FM_RADIO_SetSoundMode(MS_U8 u8FMRadioSoundMode);
 void MApi_AUDIO_FM_RADIO_DeEmphassisOption(MS_BOOL Is_Europe);
 MS_S8 MApi_AUDIO_FM_RADIO_GET_DC_AMP(void);
 MS_U8 MApi_AUDIO_FM_RADIO_GET_NSR(void);
 void MApi_AUDIO_FM_RADIO_RESET(void);

//=============================================================
// AUDIO_SPDIF RELATIONAL API FUNCTION
//=============================================================
 void    MApi_AUDIO_SPDIF_HWEN(MS_BOOL spdif_hwen);
 void    MApi_AUDIO_SPDIF_SetMute(MS_BOOL mute_en);
 void    MApi_AUDIO_SPDIF_SetMode(MS_U8 spdif_mode);
 MS_U8   MApi_AUDIO_SPDIF_GetMode(void);
 void    MApi_AUDIO_SPDIF_SetSCMS(MS_U8 C_bit_en, MS_U8 L_bit_en);
 MS_U8   MApi_AUDIO_SPDIF_GetSCMS(void);
 void    MApi_AUDIO_SPDIF_SetOutputType(AUDIO_SPDIF_OUTPUT_TYPE eType); // New API for S/PDIF output auto/PCM mode switching
 void    MApi_Audio_SPDIF_Monitor(void);
 void    MApi_Audio_Monitor_SPDIF_NONPCM_SmpRate(void);
 MS_BOOL MApi_AUDIO_SPDIF_ChannelStatus_CTRL(AUDIO_SPDIF_CS_TYPE cs_mode, AUDIO_SPDIF_CS_TYPE_STATUS status);
MS_BOOL MApi_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_OUTPUT_TYPE ePath, Digital_Out_Channel_Status_t *stChannelStatus);
MS_BOOL MApi_AUDIO_DigitalOut_SetDeviceCapability(DIGITAL_OUTPUT_TYPE eoutType, AUDIO_FORMAT_CODE ecodecType, Digital_Out_Device_Capability_t *p_codecCapability);
//=============================================================
// AUDIO_HDMI RELATIONAL API FUNCTION
//=============================================================
 void    MApi_AUDIO_HDMI_Tx_SetMute(MS_BOOL mute_en);
 void    MApi_AUDIO_HDMI_Tx_GetStatus(MS_BOOL *onOff, AUDIO_FS_TYPE *hdmi_SmpFreq, HDMI_TX_OUTPUT_TYPE *outType );
 MS_U8   MApi_AUDIO_HDMI_Monitor(void);
 MS_BOOL MApi_AUDIO_HDMI_GetNonpcmFlag(void);
 void    MApi_AUDIO_HDMI_SetNonpcm(MS_U8 nonPCM_en);
 AUDIO_HDMI_RX_TYPE MApi_AUDIO_HDMI_RX_SetNonpcm(MS_U8 nonPCM_en);
 AUDIO_HDMI_RX_TYPE MApi_AUDIO_HDMI_RX_GetNonPCM(void);
 void    MApi_AUDIO_DTV_HDMI_CFG(MS_U8 ctrl);
 MS_U16  MApi_AUDIO_HDMI_GetSynthFreq(void);
 void    MApi_AUDIO_HDMI_SetDownSample(MS_U8 ratio);
 void    MApi_AUDIO_HDMI_TX_SetMode(HDMI_TX_OUTPUT_TYPE outType);
MS_BOOL MApi_AUDIO_HDMI_RX_GetHdmiInAudioStatus( ST_HDMI_AUDIO_STATUS *p_hdmiAudioSts);
//=============================================================
// AUDIO_ADVANCE_SOUND RELATIONAL API FUNCTION
//=============================================================
 MS_BOOL MApi_AUDIO_ADVSOUND_ProcessEnable(ADVSND_TYPE type);
 MS_BOOL MApi_AUDIO_ADVSOUND_SubProcessEnable(ADVFUNC proc, MS_BOOL enable);
 MS_BOOL MApi_AUDIO_ADVSOUND_SetParam(ADVSND_PARAM param, MS_U16 u16value1, MS_U16 u16value2);
 MS_BOOL MApi_AUDIO_ADVSND_SetParam(ADVSND_PARAM param, MS_U32 u32value1, MS_U16 u16value2);
 MS_U32  MApi_AUDIO_ADVSOUND_GetInfo( AdvSound_GET_Type Type);
void MApi_DBXTV_SetMode(EN_DBXTV_TotSonMode totsonmode, EN_DBXTV_TotVolMode totvolmode, EN_DBXTV_TotSurMode totsurmode,MS_U32 enable);

 void    MApi_AUDIO_SeInit(void);
 void    MApi_AUDIO_SetAdvSndSys(MS_U8 u8AdvSurrMod);
 void    MApi_AUDIO_SetVDS(MS_U8 u8enVDS);
 void    MApi_AUDIO_SetVSPK(MS_U8 u8enVSPK);
 void    MApi_AUDIO_SetSRS(MS_U8 u8enSRS);
 void    MApi_AUDIO_SetBBE(MS_U8 u8enBBE,MS_U8 u8BBEMode);
 void    MApi_AUDIO_VSPK_WMod(MS_U8 u8VSPK_WMode);
 void    MApi_AUDIO_VSPK_SMod(MS_U8 u8VSPK_SMode);
 void    MApi_AUDIO_SRS_DC(MS_U8 u8SRSenDC);
 void    MApi_AUDIO_SRS_TruBass(MS_U8 u8SRSenTruBass);
 void    MApi_AUDIO_SRS_SetTsxtPara(MS_U8 u8mode, MS_U8 u8value);
 void    MApi_AUDIO_SetSRSTSHD(MS_U8 u8enTSHD);
 void    Mapi_SOUND_TSHD_TruBass(MS_U8 u8TSHDenTruBass);
 void    MApi_AUDIO_TSHD_Definition(MS_U8 u8TSHDenDefinition);
 void    Mapi_SOUND_TSHD_Definition(MS_U8 u8TSHDenDefinition);
 void    MApi_AUDIO_SRS_SetTshdPara(MS_U8 u8mode, MS_U8 u8value);
 void    MApi_AUDIO_COPY_Parameter(ADVSND_PARAM type, void* Parameter_ptr, MS_U32 size);

//=============================================================
// AUDIO_KTV RELATIONAL API FUNCTION
//=============================================================
 MS_BOOL MApi_AUDIO_SetKTVInfo (AUDIO_KTV_MODEL modelType, AUDIO_KTV_PARAMTYPE paramType, MS_U32 param1, MS_U32 param2);
 MS_U32  MApi_AUDIO_GetKTVInfo (AUDIO_KTV_MODEL modelType, AUDIO_KTV_INFOTYPE infoType);
 void    MApi_AUDIO_SetMixModeVolume(AUDIO_SOURCE_INFO_TYPE eSourceType, AUDIO_MIX_VOL_TYPE VolType, MS_U8 u8Vol1, MS_U8 u8Vol2);
 void    MApi_AUDIO_SetMixModeMute(AUDIO_SOURCE_INFO_TYPE eSourceType, AUDIO_MIX_VOL_TYPE VolType, MS_BOOL EnMute);

 void    MApi_AUDIO_PlayMenuSound(void);

//=============================================================
// AUDIO_OTHERS FUNCTION
//=============================================================
//Cathy0511 void MApi_AUDIO_SetAudioSource(AUDIOSOURCE_TYPE eSource);
//Certification
 MS_BOOL MApi_AUDIO_SetCertMode( Audio_CertMode CertType,Audio_CertEnable  enable);

//=============================================================
// AUDIO_MM RELATIONAL API FUNCTION
//=============================================================
// Each Decoder functions
// Set Info
 MS_BOOL MApi_AUDIO_SetCommAudioInfo( Audio_COMM_infoType infoType, MS_U32 param1, MS_U32 param2 );
 MS_BOOL MApi_AUDIO_SetMpegInfo( Audio_MPEG_infoType infoType, MS_U32 param1, MS_U32 param2 );
 MS_BOOL MApi_AUDIO_SetAC3Info( Audio_AC3_infoType infoType, MS_U32 param1, MS_U32 param2 );
 MS_BOOL MApi_AUDIO_SetAC3PInfo( Audio_AC3P_infoType infoType, MS_U32 param1, MS_U32 param2 );
 MS_BOOL MApi_AUDIO_SetAACInfo( Audio_AAC_infoType infoType, MS_U32 param1, MS_U32 param2 );
 MS_BOOL MApi_AUDIO_SetWmaInfo( Audio_WMA_infoType infoType, MS_U32 param1, MS_U32 param2 );
 MS_BOOL MApi_AUDIO_SetDTSCommonCtrl( Audio_DTS_infoType infoType, Audio_DTS_CtrlType ctrl_type );

// Get Info
 long long MApi_AUDIO_GetCommAudioInfo( Audio_COMM_infoType infoType );
 MS_U32  MApi_AUDIO_GetMpegInfo( Audio_MPEG_infoType infoType );
 MS_U32  MApi_AUDIO_GetAC3Info( Audio_AC3_infoType infoType );
 MS_U32  MApi_AUDIO_GetAC3PInfo( Audio_AC3P_infoType infoType );
 MS_U32  MApi_AUDIO_GetAACInfo( Audio_AAC_infoType infoType );
 MS_U32  MApi_AUDIO_GetWmaInfo( Audio_WMA_infoType infoType );
 MS_U32  MApi_AUDIO_GetDTSInfo( Audio_DTS_infoType infoType );

 MS_U8   MApi_AUDIO_XPCM_Param(XPCM_TYPE audioType, MS_U8  channels, MS_U16 sampleRate,
                                MS_U8  bitsPerSample, MS_U16 blockSize, MS_U16 samplePerBlock);
 void    MApi_AUDIO_XPCM2_Param(XPCM_TYPE audioType, MS_U8  channels, MS_U16 sampleRate,
                                MS_U8  bitsPerSample, MS_U16 blockSize, MS_U16 samplePerBlock);
 MS_U16  MApi_AUDIO_XPCM2_CheckIntStatus(void);

// RM
 MS_U8   MApi_AUDIO_RA8_Param(MS_U16 mNumCodecs, MS_U16 mSamples, MS_U16 mSampleRate,
                                MS_U16* Channels, MS_U16* Regions, MS_U16* cplStart, MS_U16* cplQbits,
                                MS_U16* FrameSize);

 En_DVB_melodyPlayStatus MApi_AUDIO_PlayMelody(En_DVB_melodyPlayCmdType eMelodyCmd, MS_U32* u32FileAddr,MS_U32 u32FileSize, MS_U32 u32DspReqAddr);
 void    MApi_AUDIO_Init(En_DVB_decSystemType enDecSystem);
 void    MApi_AUDIO_StartDecode(void);
 void    MApi_AUDIO_StartBrowse(void);
 void    MApi_AUDIO_StopDecode(void);
 void    MApi_AUDIO_PauseDecode(void);
 MS_U8   MApi_AUDIO_CheckPlayDone(void);

 MS_U16  MApi_AUDIO_GetResidualBufferSize(void);
 MS_U16  MApi_AUDIO_GetPCMBufferSize(MS_U16 u16BitRate);
 MS_U16  MApi_AUDIO_GetPCMBufferSize2(MS_U16 u16BitRate,MS_U16 u16SampleRate);
 MS_U32  MApi_AUDIO_GetCurrentFrameNumber(void);
 MS_U16  MApi_AUDIO_GetSampleRate(void);
 MS_U32  MApi_AUDIO_GetBitRate(void);

 Audio_MP3_LAYER MApi_AUDIO_GetLayer(void);
 MS_U8   MApi_AUDIO_CheckInputRequest(MS_U32 *pU32WrtAddr, MS_U32 *pU32WrtBytes);
 void    MApi_AUDIO_SetInput(void);
 void    MApi_AUDIO_SetSampleRateIndex(MS_U16 u16Index);
 void    MApi_AUDIO_SetXPCMParam(XPCM_TYPE audioType, MS_U8 channels, MS_U16 sampleRate, MS_U8 bitsPerSample, MS_U16 blockSize, MS_U16 samplePerBlock);
 void    MApi_AUDIO_FileEndNotification(void);
 void    MApi_AUDIO_FileEndDataHandle(MS_U32 u32DataLeft);
 MS_U32  MApi_AUDIO_GetPlayTick(void);
 MS_U16  MApi_AUDIO_GetEsMEMCnt(void);

 void    MApi_AUDIO_SetASFParm(WMA_ASF_PARMTYPE parm_type, MS_U32 value);

 void    MApi_AUDIO_MM_SetInput (En_DVB_AudioDecoder AUDDecoderNo, MS_U8 u8IntTag);
 MS_U32  MApi_AUDIO_MM_CheckPlayDone (En_DVB_AudioDecoder AUDDecoderNo);
 MS_U8   MApi_AUDIO_MM_CheckInputRequest(En_DVB_AudioDecoder AUDDecoderNo, MS_U32 *pU32WrtAddr, MS_U32 *pU32WrtBytes);
 void    MApi_AUDIO_DmaReader_Init(SAMPLE_RATE sampleRate);
 void    MApi_AUDIO_DmaReader_AllInput_Init(void);
 MS_BOOL MApi_AUDIO_DmaReader_WritePCM(void* buffer, MS_U32 bytes);
 void    MApi_AUDIO_DmaWriter_Init(void);

 void    MApi_AUDIO_USBPCM_Enable(MS_BOOL bEnable);
 void    MApi_AUDIO_USBPCM_SetFlag(MS_BOOL bEnable);
 MS_BOOL MApi_AUDIO_USBPCM_GetFlag(void);
 void    MApi_AUDIO_USBPCM_GetMemInfo(AUDIO_UPLOAD_INFO *uploadInfo);
 MS_BOOL MApi_AUDIO_PCMCapture_Init(const AUDIO_DEVICE_TYPE eID, const AUDIO_CAPTURE_SOURCE_TYPE eSource);
 MS_BOOL MApi_AUDIO_PCMCapture_Start(const AUDIO_DEVICE_TYPE eID);
 MS_BOOL MApi_AUDIO_PCMCapture_Stop(const AUDIO_DEVICE_TYPE eID);
 MS_BOOL MApi_AUDIO_PCMCapture_Read(const AUDIO_DEVICE_TYPE eID, void *buffer, const MS_U32 bytes);

 MS_BOOL MApi_AUDIO_VoIP_Config(AUDIO_VoIP_CONFIG_TYPE configType, MS_U32 param1, MS_U32 param2);

 MS_BOOL MApi_AUDIO_ALSA_Check(void);
 MS_BOOL MApi_AUDIO_ALSA_Enable(MS_BOOL bEnable);

 MS_BOOL MApi_AUDIO_UNI_CheckDecodeDone(MS_U32 *pU32WrtAddr, MS_U32 *pU32WrtBytes);
 void    MApi_AUDIO_UNI_SetOutput (MS_U32 PCMOutCnt);
 void    MApi_AUDIO_UNI_Set_PCMInputWriteAddr (MS_U32 PCMIn_Wptr);
 MS_U32  MApi_AUDIO_UNI_Get_OutPCMLevel (void);
 void    MApi_AUDIO_RingTask(audioRingTask* auRingTask) ;
 void    MApi_AUDIO_Ring_DataTransfer(audioRingTask* auRingTask, audioRingTask* auRingTask_1);
 MS_BOOL MApi_AUDIO_MM2_initAesInfo( AUDIO_DEC_ID dec_id );
 MS_BOOL MApi_AUDIO_MM2_checkAesInfo( AUDIO_DEC_ID  dec_id, AES_INFO *aes_info );
 MS_BOOL MApi_AUDIO_MM2_inputAesFinished( AUDIO_DEC_ID dec_id, MS_U32 es_size, MS_BOOL ptsExist, MS_U64 pts );

 MS_S32   MApi_AUDIO_PCM_Open(void *pData);
 MS_S32   MApi_AUDIO_PCM_Close(MS_S32 s32DeviceId);
 MS_S32   MApi_AUDIO_PCM_Start(MS_S32 s32DeviceId);
 MS_S32   MApi_AUDIO_PCM_Stop(MS_S32 s32DeviceId);
 MS_S32   MApi_AUDIO_PCM_Set(MS_S32 s32DeviceId, MS_U32 u32Cmd, const void *pData);
 MS_S32   MApi_AUDIO_PCM_Get(MS_S32 s32DeviceId, MS_U32 u32Cmd, void *pData);
 MS_U32   MApi_AUDIO_PCM_Read(MS_S32 s32DeviceId, void *pBuf, MS_U32 u32Size);
 MS_U32   MApi_AUDIO_PCM_Write(MS_S32 s32DeviceId, const void *pBuf, MS_U32 u32Size);
 MS_S32   MApi_AUDIO_PCM_Flush(MS_S32 s32DeviceId);

//====== STAOS PIP START 2012/02/01 ========

 MS_BOOL MApi_AUDIO_SetAudioParam2( AUDIO_DEC_ID DecId, Audio_ParamType paramType, MS_U32 Param );
 MS_BOOL MApi_AUDIO_GetAudioInfo2( AUDIO_DEC_ID DecId, Audio_InfoType infoType, void * pInfo );

//====== STAOS PIP END 2012/02/01 ========

MS_U32 MApi_AUDIO_GetDDRInfo(AUDIO_DEC_ID DecId, EN_AUDIO_DDRINFO DDRInfo);

#ifdef __cplusplus
}
#endif

#endif // _API_AUDIO_H_
