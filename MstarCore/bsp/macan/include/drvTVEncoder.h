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
/// @file    drvTVEncoder.h
/// @brief  TVEncoder interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

/* !\defgroup VE VE interface (drvTVEncoder.h)
    \ingroup CVBS_OUT

    VE is used to covernt input video into standard definition resolution(pal/nstc)
    and pass through the DAC to scart out.

    <b> VE Block Diagram: </b> \n
    \image html drvVE_pic01.png

 *! \defgroup VE_INFO VE Information pool
 *  \ingroup VE

 *! \defgroup VE_INIT VE init control
 *  \ingroup VE

 *! \defgroup VE_FEATURE VE feature operation
 *  \ingroup VE

 *! \defgroup VE_ToBeModified VE api to be modified
 *  \ingroup VE

 *! \defgroup VE_ToBeRemove VE api to be removed
 *  \ingroup VE
*/
#ifndef _DRVTVENCODER_H
#define _DRVTVENCODER_H

#include "MsDevice.h"

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define MSIF_VE_TAG                 {'M','S','I','F'}    //Version ID header
#define MSIF_VE_CLASS               {'0','0'}            //info Class code
#define MSIF_VE_CUS                 0x6666               //Customer ID
#define MSIF_VE_MOD                 0x6666               //Module ID
#define MSIF_VE_CHIP                0Xffff               //CHIP ID
#define MSIF_VE_CPU                 '1'                  //CPU
#define MSIF_VE_LIB_CODE            {'V','E','_','_'}    //Lib code
#define MSIF_VE_LIBVER              {'0','1'}            //LIB version
#define MSIF_VE_BUILDNUM            {'1','4'}            //Build Number
#define MSIF_VE_CHANGELIST          {'0','0','6','8','8','5','4','7'} //P4 ChangeList Number
//#define MSIF_OS                     '0'                  //OS

#define VE_DRV_VERSION                  /* Character String for DRV/API version             */  \
    MSIF_VE_TAG,                        /* 'MSIF'                                           */  \
    MSIF_VE_CLASS,                      /* '00'                                             */  \
    MSIF_VE_CUS,                        /* 0x0000                                           */  \
    MSIF_VE_MOD,                        /* 0x0000                                           */  \
    MSIF_VE_CHIP,                                                                               \
    MSIF_VE_CPU,                                                                                \
    MSIF_VE_LIB_CODE,                   /* IP__                                             */  \
    MSIF_VE_LIBVER,                     /* 0.0 ~ Z.Z                                        */  \
    MSIF_VE_BUILDNUM,                   /* 00 ~ 99                                          */  \
    MSIF_VE_CHANGELIST,                 /* CL#                                              */  \
    MSIF_OS

/// version of structure 'MS_VE_VECAPTURESTATE' of current VE lib
#define VE_VECAPTURESTATE_VERSION                          0 //Version number for compatibility with AP and VE driver
#define VE_INPUTSRC_INFO_VERSION                           0

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
/// Result of VE function
typedef enum _VE_Result
{
    E_VE_NOT_SUPPORT = -1,
    E_VE_FAIL = 0,
    E_VE_OK = 1,
    E_VE_TIMEOUT,
    E_VE_QUEUE_FULL,
    E_VE_BUSY
} VE_Result;

/// definition of video system
typedef enum
{
    MS_VE_NTSC,     ///< NTSC
    MS_VE_NTSC_443, ///< NTSC443
    MS_VE_NTSC_J,   ///< NTSC_J
    MS_VE_PAL_M,    ///< PAL_M
    MS_VE_PAL_N,    ///< PAL_N
    MS_VE_PAL_NC,   ///< PAL_Nc
    MS_VE_PAL,      ///< PAL_B
    MS_VE_SECAM,    ///< SECAM
    MS_VE_VIDEOSYS_NUM,
} MS_VE_VIDEOSYS;

typedef enum
{
    MS_VE_LAYER_NONE = 0,                   ///<0: VE LAYER NONE
    MS_VE_LAYER_FRAME_VIDEO_MUX1_MUX2,      ///<1: FrameColor->Video->osd  ->osd1
    MS_VE_LAYER_FRAME_VIDEO_MUX2_MUX1,      ///<2: FrameColor->Video->osd1 ->osd
    MS_VE_LAYER_FRAME_MUX1_VIDEO_MUX2,      ///<3: FrameColor->osd ->Video->osd1
    MS_VE_LAYER_FRAME_MUX1_MUX2_VIDEO,      ///<4: FrameColor->osd ->osd1 ->Video
    MS_VE_LAYER_FRAME_MUX2_VIDEO_MUX1,      ///<5: FrameColor>osd1 ->Video->osd
    MS_VE_LAYER_FRAME_MUX2_MUX1_VIDEO,      ///<6: FrameColor->osd1 ->osd->Video
    MS_VE_LAYER_RESERVED,
} MS_VE_OSD_LAYER_SEL;

/// VE cap's capability
typedef struct
{
    MS_BOOL bNTSC;
    MS_BOOL bPAL;
    MS_BOOL bVESupported;        ///< supported VE or not
} VE_Caps;

/// VE extented capability: for STB chips, not all chips support up-scaling or
/// crop
typedef struct
{
    MS_BOOL bSupport_UpScale;
    MS_BOOL bSupport_CropMode;
} MS_VE_Ext_Cap;

/// VE driver info.
typedef struct
{
    VE_Caps stCaps;
} VE_DrvInfo;

/// VE DRV status
typedef struct
{
    MS_VE_VIDEOSYS         VideoSystem;         // video std of output signal
    MS_U16  u16H_CapStart;
    MS_U16  u16H_CapSize;
    MS_U16  u16V_CapStart;
    MS_U16  u16V_CapSize;

} VE_DrvStatus;

typedef struct
{
    MS_U8*     pVE_TBL;
    MS_U8*     pVE_Coef_TBL;
    MS_U8*     pVBI_TBL;
    MS_BOOL    bvtotal_525;
    MS_BOOL    bPALSwitch;
    MS_BOOL    bPALout;
}MS_VE_Out_VideoSYS, *PMS_VE_Out_VideoSYS;

/// type of output destination for TV encoder
typedef enum
{
    MS_VE_DEST_NONE = 0,
    MS_VE_DEST_SCART,     ///< output destination is SCART
    MS_VE_DEST_CVBS,      ///< output destination is CVBS
    MS_VE_DEST_SVIDEO,    ///< output destination is S-Video
    MS_VE_DEST_YPBPR,     ///< output destination is YPbPr
    MS_VE_DEST_NUM,
} MS_VE_OUTPUT_DEST_TYPE;

/// type of input source for TV encoder
typedef enum
{
    MS_VE_SRC_DTV       = 0x00, ///< input source is DTV
    MS_VE_SRC_ATV       = 0x01, ///< input source is ATV
    MS_VE_SRC_CVBS0     = 0x02, ///< input source is CVBS0
    MS_VE_SRC_CVBS1     = 0x03, ///< input source is CVBS1
    MS_VE_SRC_CVBS2     = 0x04, ///< input source is CVBS2
    MS_VE_SRC_CVBS3     = 0x05, ///< input source is CVBS3
    MS_VE_SRC_SVIDEO    = 0x06, ///< input source is SVIDEO
    MS_VE_SRC_DSUB      = 0x07, ///< input source is DSUB
    MS_VE_SRC_COMP      = 0x08, ///< input source is COMP
    MS_VE_SRC_HDMI_A    = 0x09, ///< input source is COMP
    MS_VE_SRC_HDMI_B    = 0x0A, ///< input source is COMP
    MS_VE_SRC_HDMI_C    = 0x0B, ///< input source is COMP
    MS_VE_SRC_MAIN      = 0x0C, ///< input source is from main window
    MS_VE_SRC_SUB       = 0x0D, ///< input source is from sub window
    MS_VE_SRC_SCALER    = 0x0E, ///< input source is scaler op2
    MS_VE_SRC_DI        = 0x0F, ///< input source is DI
    MS_VE_SRC_BT656     = 0x10, ///< input source is BT656
    MS_VE_SRC_DTV_FROM_MVOP = 0x11,  ///< input source is DTV but from MVOP
    MS_VE_SRC_DNR       = 0x12,  ///< input source is DNR
    MS_VE_SRC_NONE      = 0xFF,
}MS_VE_INPUT_SRC_TYPE;

typedef struct
{
    MS_U32 PreHScalingRatio;
    MS_U32 PreVScalingRatio;
    MS_U32 PostHScalingRatio;
    MS_U32 PostVScalingRatio;
}MS_VE_Backup_Reg, *PMS_VE_Backup_Reg;

/// status of switchinging output destination
typedef enum
{
    MS_VE_SWITCH_DST_SUCCESS,               ///< success
    MS_VE_SWITCH_DST_INVALID_COMBINATION,   ///< invalid combination
    MS_VE_SWITCH_DST_INVALID_PARAM,         ///< invalid parameter
    MS_VE_SWITCH_DST_FAIL,                  ///< fail
}MS_SWITCH_VE_DST_STATUS;


/// VE output Type
typedef enum
{
    MS_VE_OUT_CCIR656,  ///< output signal is CCIR656
    MS_VE_OUT_TVENCODER,///< output signal from TVEncoder
    MS_VE_OUT_CAPTURE,///<   output signal from VE Capture
}MS_VE_OUT_TYPE;

typedef struct
{
    MS_U16  u16Width;
    MS_U16  u16height;
    MS_PHY  u32MiuBaseAddr;
    MS_U32  u32MemSize;
    MS_BOOL bVECapture; ///< TRUE/FALSE, enable/disable VE capture
}MS_VE_Output_CAPTURE, *PMS_VE_Output_CAPTURE;

/// the information of switching ouput destination for TV encoder
typedef struct
{
    MS_VE_OUTPUT_DEST_TYPE OutputDstType; ///< type of output destination
    MS_SWITCH_VE_DST_STATUS Status;     ///< the returning status of switching output destination
}MS_Switch_VE_Dest_Info, *PMS_Switch_VE_Dest_Info;


/// status of switchinging input source
typedef enum
{
    MS_VE_SWITCH_SRC_SUCCESS,           ///< success
    MS_VE_SWITCH_SRC_INVALID_PARAM,     ///< invalid parameter
    MS_VE_SWITCH_SRC_FAIL,              ///< fail
}MS_SWITCH_VE_SRC_STATUS;

typedef enum
{
    MS_VE_HMirrorMode,      //H mirror
    MS_VE_VMirrorMode,      //V mirror
    MS_VE_HVBothMirrorMode, //HV mirror
    MS_VE_NoneMirror,
}MS_VE_MirrorStatus;

/// the information of switching output destination for TV encoder
typedef struct
{
    MS_VE_INPUT_SRC_TYPE InputSrcType; ///< type of input source
    MS_SWITCH_VE_SRC_STATUS Status; ///< the returning status of switching input source
}MS_Switch_VE_Src_Info, *PMS_Switch_VE_Src_Info;

typedef struct
{
    MS_U16  u16Version;   //< Version number for this structure
    MS_VE_INPUT_SRC_TYPE eInputSrcType; ///< type of input source for Video encoder
    MS_VE_INPUT_SRC_TYPE eInputSrcOfMixedSrc;  // The real input source for mixed "InputSrcType" of VE,
                                              // eg. Real InputSrc of "MS_VE_SRC_MAIN", "MS_VE_SRC_SUB", "MS_VE_SRC_SCALER" etc.
}MS_VE_InputSrc_Info, *PMS_VE_InputSrc_Info;

/// output control for VE
typedef struct
{
    MS_BOOL bEnable; ///< TRUE/FALSE, enable/disable VE
    MS_VE_OUT_TYPE OutputType; ///< VE output type
}MS_VE_Output_Ctrl, *PMS_VE_Output_Ctrl;

/// VE set mode type
typedef struct
{
    MS_U16 u16H_CapStart;
    MS_U16 u16H_CapSize;
    MS_U16 u16V_CapStart;
    MS_U16 u16V_CapSize;
    MS_U16 u16H_SC_CapStart;
    MS_U16 u16H_SC_CapSize;
    MS_U16 u16V_SC_CapStart;
    MS_U16 u16V_SC_CapSize;
    MS_U16 u16InputVFreq;
    MS_BOOL bHDuplicate;
    MS_BOOL bSrcInterlace;
}MS_VE_Set_Mode_Type;

//VE_ApiStatus
typedef struct
{
    PMS_VE_Output_Ctrl      output_ctrl;
    MS_Switch_VE_Src_Info   src_info;
    MS_Switch_VE_Dest_Info  dst_info;
}MS_VE_ApiStatus;

/// Define Window position and size attribute
typedef struct
{
    MS_U16 x;    ///<start x of the window
    MS_U16 y;    ///<start y of the window
    MS_U16 width;    ///<width of the window
    MS_U16 height;    ///<height of the window
} MS_VE_WINDOW_TYPE;

//Display information
typedef struct
{
    MS_U32 VDTOT; //Output vertical total
    MS_U32 DEVST; //Output DE vertical start
    MS_U32 DEVEND;//Output DE Vertical end
    MS_U32 HDTOT;// Output horizontal total
    MS_U32 DEHST; //Output DE horizontal start
    MS_U32 DEHEND;// Output DE horizontal end
    MS_BOOL bInterlaceMode;
} MS_VE_DST_DispInfo;

//VE register bank
typedef enum
{
    MS_VE_REG_BANK_3B,
    MS_VE_REG_BANK_3E,
    MS_VE_REG_BANK_3F,
} MS_VE_REG_BANK;

//External Info
typedef struct
{
    //op to ve timing
    MS_U16 u16VttIn;
    MS_U16 u16HttIn;
    MS_U16 u16HdeIn;
    MS_U16 u16VdeIn;

    MS_U16 u16Vde_St;
    MS_U16 u16Hde_St;

}MS_VE_PLL_InCfg;

//VE customer scaling setting
typedef struct
{
    MS_U16   u16HScalingsrc; ///< H Source size for VE scaling
    MS_U16   u16HScalingdst;///< H Dest size for VE scaling
    MS_U16   u16VScalingsrc;///< V Source size for VE scaling
    MS_U16   u16VScalingdst;///< V Dest size for VE scaling
    MS_BOOL  bHCusScalingEnable; ///< Enable/Disable H customer scaling
    MS_BOOL  bVCusScalingEnable; ///< Enable/Disable V customer scaling
}MS_VE_CusScalingInfo, *PMS_VE_CusScalingInfo;

//Structure AP use
typedef struct
{
    MS_U16  u16Version;   //< Version number for this structure
    MS_U16  u16Length;    //< Length of this structure, unit: byte
    MS_BOOL bEnable;      //< InOut, VE capture enable state
    MS_U8   u8FrameCount; //< Out, Current captured frame number,value range: 0~3
    MS_U8   u8Result;     //< Out, Function return status
}MS_VE_VECAPTURESTATE, *PMS_VE_VECAPTURESTATE;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
/// Description : Show the VE driver version
/// @ingroup VE_INFO
/// @param ppVersion \b Out: Library version string
/// @return             @ref  VE_Result
//-------------------------------------------------------------------------------------------------
// VE_Result MDrv_VE_GetLibVer(const MSIF_Version **ppVersion);

//-------------------------------------------------------------------------------------------------
/// Description : Get VE Information
/// @ingroup VE_INFO
/// @return @ref VE_DrvInfo return the VE information in this member
//-------------------------------------------------------------------------------------------------
const VE_DrvInfo * MDrv_VE_GetInfo(void);

//-------------------------------------------------------------------------------------------------
/// Description : Get VE Status
/// @ingroup VE_INFO
/// @param  pDrvStatus                  \b OUT: store the status
/// @return @ref VE_Result
//-------------------------------------------------------------------------------------------------
VE_Result MDrv_VE_GetStatus(VE_DrvStatus *pDrvStatus);

//-------------------------------------------------------------------------------------------------
/// Description : Set VE debug memesage level
/// @ingroup VE_INFO
/// @param  level \b IN: the debug meseage level
/// @return None
//-------------------------------------------------------------------------------------------------
void MDrv_VE_SetDbgLevel(MS_U16 level);

//-------------------------------------------------------------------------------------------------
/// Description : Get VE capability that support which standard
/// @ingroup VE_INFO
/// @param  pCaps \b OUT: the standard that VE support
/// @return None
//-------------------------------------------------------------------------------------------------
// void MDrv_VE_GetCaps(VE_Caps* pCaps);

//------------------------------------------------------------------------------
/// Description : Set the input source of video encoder
/// @ingroup VE_ToBeModified
/// @param  -pSwitchInfo \b IN/OUT: the information of switching input destination of TV encodeer
/// @return None
//------------------------------------------------------------------------------
void MDrv_VE_SwitchInputSource(PMS_Switch_VE_Src_Info pSwitchInfo);

//------------------------------------------------------------------------------
/// Description: Extended API for set the input source of video encoder(To Expand&Replace MDrv_VE_SwitchInputSource)
/// @ingroup VE_EEATURE
/// @param  -pSwitchInfo \b IN/OUT: the information of switching input destination of TV encodeer
/// @return @ref MS_SWITCH_VE_SRC_STATUS
//------------------------------------------------------------------------------
MS_SWITCH_VE_SRC_STATUS MDrv_VE_SetInputSource(PMS_VE_InputSrc_Info pInputSrcInfo);

//------------------------------------------------------------------------------
/// Description : Set the output destination of video encoder
/// @ingroup VE_FEATURE
///              None SCART CVBS SVIDEO YPbPr
///       None    O     O     O     O     O
///       SCART O      -     X     X     X
///        CVBS  O     X      -     O     O
///     S_VIDE  O     O     X      O     -     X
///       YPbPr   O     X     O      X     -
///
/// @param  -pSwitchInfo \b IN/OUT: the information of switching output destination of TV encodeer
/// @return None
//------------------------------------------------------------------------------
void MDrv_VE_SwitchOuputDest(PMS_Switch_VE_Dest_Info pSwitchInfo);

//------------------------------------------------------------------------------
/// Description : Control the output of video encoder
/// @ingroup VE_FEATURE
/// @param  -pOutputCtrl \b IN: the control information of VE
/// @return none
//------------------------------------------------------------------------------
void MDrv_VE_SetOutputCtrl(PMS_VE_Output_Ctrl pOutputCtrl);

//------------------------------------------------------------------------------
/// Description : Set the output video standard of video encoder
/// @ingroup VE_FEATURE
/// @param  -VideoSystem \b IN: the video standard
/// @return TRUE: supported and success,  FALSE: unsupported or unsuccess
//------------------------------------------------------------------------------
MS_BOOL MDrv_VE_SetOutputVideoStd(MS_VE_VIDEOSYS VideoSystem);

//------------------------------------------------------------------------------
/// Description : Load customized setting table for VE
/// User can load customized table in initialize function, then enable it.
/// Driver will apply customized table setting when MDrv_VE_SetOutputVideoStd
/// Table using sample:
/// MS_U8 pPALTable[12] =
///        {0xAA,0xBB,0xCC,0xDD,  => add one customized setting. format: {bank_H, bank_L, 8-bit address, value,}
///          0xFF,0xFF,0xFF,0xFF} => must set 0xFF,0xFF,0xFF,0xFF for end of table command setting
/// Note: Users set value which want to be changed only.
/// @ingroup VE_FEATURE
/// @param  -VideoSystem \b IN: the video standard
/// @param  -pTbl \b IN: pointer to the table
/// @return TRUE: supported and success,  FALSE: unsupported or unsuccess
//------------------------------------------------------------------------------
// MS_BOOL MDrv_VE_SetCusTable(MS_VE_VIDEOSYS VideoSystem, MS_U8* pTbl);

//------------------------------------------------------------------------------
/// Description : Enable/Disable customized table
/// @ingroup VE_FEATURE
/// @param  -bEnable \b IN: Enable/Disable customized table
//------------------------------------------------------------------------------
// void MDrv_VE_EnableCusTable(MS_BOOL bEnable);

//------------------------------------------------------------------------------
/// Description : Power on VE
/// @ingroup VE_ToBeRemove
/// @return none
//------------------------------------------------------------------------------
// void MDrv_VE_PowerOn(void);
//------------------------------------------------------------------------------
/// Description : Power off VE
/// @ingroup VE_ToBeRemove
/// @return none
//------------------------------------------------------------------------------
// void MDrv_VE_PowerOff(void);

//------------------------------------------------------------------------------
/// Description : Set RIU Base
/// @ingroup VE_FEATURE
/// @return TRUE: success, FALSE: unsuccess
//------------------------------------------------------------------------------
MS_BOOL MDrv_VE_SetIOMapBase(void);

//------------------------------------------------------------------------------
/// Description : Initiate VE
/// @ingroup VE_INIT
/// @param[in]  u32MIUAddress  \b IN: The address reserved for VE
/// @return none
//------------------------------------------------------------------------------
void MDrv_VE_Init(MS_PHYADDR u32MIUAddress);

//------------------------------------------------------------------------------
/// Description : Exit VE
/// @ingroup VE_FEATURE
/// @return TRUE: success, FALSE: unsuccess
//------------------------------------------------------------------------------
MS_BOOL MDrv_VE_Exit(void);

//------------------------------------------------------------------------------
/// Description : Set Wild-screen signal
/// @ingroup VE_FEATURE
/// @param  -bEn \b IN: Enable/Disable WSS
/// @param  -u16WSSData \b IN: The WSS data
/// @return none
//------------------------------------------------------------------------------
void MDrv_VE_SetWSSData(MS_BOOL bEn, MS_U16 u16WSSData);

//------------------------------------------------------------------------------
/// Description : Get Wild-screen signal data
/// @ingroup VE_FEATURE
/// @return @ref MS_U16 retun the data of WSS
//------------------------------------------------------------------------------
MS_U16 MDrv_VE_GetWSSData(void);

//------------------------------------------------------------------------------
/// Description : VE Setmode. Configurate VE for CVBS out.
/// @ingroup VE_FEATURE
/// @param[in]  pVESetMode  \b IN: The parameter for VE
/// @return none
//------------------------------------------------------------------------------
void MDrv_VE_SetMode(MS_VE_Set_Mode_Type * pVESetMode);

//------------------------------------------------------------------------------
/// Description : Output black screen from VE. This function is not for disabling VE.
/// @ingroup VE_FEATURE
/// @param[in]  pVESetMode  \b IN: enable or disable the black screen
/// @return none
//------------------------------------------------------------------------------
void MDrv_VE_SetBlackScreen(MS_BOOL bEn);

//------------------------------------------------------------------------------
/// Description : Return true if VE output black video right now.
/// @ingroup VE_FEATURE
/// @return @ref MS_BOOL
//------------------------------------------------------------------------------
MS_BOOL MDrv_VE_IsBlackScreenEnabled(void);

//------------------------------------------------------------------------------
/// Description : VE restart TVE to read data from TT buffer
/// @ingroup VE_FEATURE
/// @param[in]  bEnable  \b IN: enable or disable the TTX
/// @return none
//------------------------------------------------------------------------------
void MDrv_VE_EnableTtx(MS_BOOL bEnable);

//------------------------------------------------------------------------------
/// Description : VE set TTx buffer address
/// @ingroup VE_FEATURE
/// @param[in]  u32StartAddr  \b IN: Start of the Memory address that store TTX
/// @param[in]  u32Size       \b IN: the size of store the TTX data
/// @return none
//------------------------------------------------------------------------------
void MDrv_VE_SetTtxBuffer(MS_U32 u32StartAddr, MS_U32 u32Size);

//------------------------------------------------------------------------------
/// Description : VE clear TT buffer read done status
/// @ingroup VE_FEATURE
/// @return none
//------------------------------------------------------------------------------
void MDrv_VE_ClearTtxReadDoneStatus(void);

//------------------------------------------------------------------------------
/// Description : VE TT buffer read done status
/// @ingroup VE_FEATURE
/// @return TRUE/FALSE
//------------------------------------------------------------------------------
MS_BOOL MDrv_VE_GetTtxReadDoneStatus(void);

//------------------------------------------------------------------------------
/// Description : VE Set VBI TT active Lines
/// @ingroup VE_FEATURE
/// @param[in]  u8LinePerField       \b IN: TTX active line
/// @return none
//------------------------------------------------------------------------------
void MDrv_VE_SetVbiTtxActiveLines(MS_U8 u8LinePerField);

//------------------------------------------------------------------------------
/// Description : VE Set VBI TT active Lines by the given bitmap
/// @ingroup VE_FEATURE
/// @param <IN>\b u32Bitmap: a bitmap that defines whick physical lines the teletext lines are to be inserted.
/// @return none
//------------------------------------------------------------------------------
// void MDrv_VE_SetVbiTtxActiveLinesBitmap(MS_U32 u32Bitmap);

//------------------------------------------------------------------------------
/// Description : VE Set VBI ttx active line
/// @ingroup VE_FEATURE
/// @param <IN>\b odd_start: odd page start line
/// @param <IN>\b odd_end: odd page end line
/// @param <IN>\b even_start: even page start line
/// @param <IN>\b even_end: even page end line
/// @return none
//------------------------------------------------------------------------------
// void MDrv_VE_SetVbiTtxRange(MS_U16 odd_start, MS_U16 odd_end,
//                             MS_U16 even_start, MS_U16 enen_end);
//------------------------------------------------------------------------------
/// Description : VE Set VBI CC
/// @ingroup VE_FEATURE
/// @param[in]  bEnable  \b IN: enable or disable the CC
/// @return none
//------------------------------------------------------------------------------
// void MDrv_VE_EnableCcSw(MS_BOOL bEnable);

//------------------------------------------------------------------------------
/// Description : VE Set VBI CC active line
/// @ingroup VE_FEATURE
/// @param <IN>\b odd_start: odd page start line
/// @param <IN>\b odd_end: odd page end line
/// @param <IN>\b even_start: even page start line
/// @param <IN>\b even_end: even page end line
/// @return none
//------------------------------------------------------------------------------
// void MDrv_VE_SetCcRange(MS_U16 odd_start, MS_U16 odd_end, MS_U16 even_start, MS_U16 enen_end);

//------------------------------------------------------------------------------
/// Description : VE Set VBI CC data
/// @ingroup VE_FEATURE
/// @param <IN>\b bIsOdd: odd field use
/// @param <IN>\b data: the CC data
/// @return none
//------------------------------------------------------------------------------
// void MDrv_VE_SendCcData(MS_BOOL bIsOdd, MS_U16 data);

//------------------------------------------------------------------------------
/// Description : VE Set display windows
/// @ingroup VE_FEATURE
/// @param <IN>\b stDispWin: the info of the display window
/// @return none
//------------------------------------------------------------------------------
void MDrv_VE_set_display_window(MS_VE_WINDOW_TYPE stDispWin);

//------------------------------------------------------------------------------
/// Description : VE Set output with Frame Color
/// @ingroup VE_FEATURE
/// @param <IN>\b u32aRGB: the value of frame color
/// @return none
//------------------------------------------------------------------------------
// VE_Result MDrv_VE_SetFrameColor(MS_U32 u32aRGB);

//------------------------------------------------------------------------------
/// Description : VE Set output with OSD
/// @ingroup VE_FEATURE
/// @param <IN>\b bEnable: enable or disable the OSD
/// @return none
//------------------------------------------------------------------------------
void MDrv_VE_SetOSD(MS_BOOL bEnable);

//------------------------------------------------------------------------------
/// Description : Set VE OSD Layer
/// @ingroup VE_FEATURE
/// @param <IN>\b eVideoOSDLayer: set the OSD layer
/// @return none
//------------------------------------------------------------------------------
// VE_Result MDrv_VE_Set_OSDLayer(MS_VE_OSD_LAYER_SEL eVideoOSDLayer);

//------------------------------------------------------------------------------
/// Description: Get VE OSD Layer
/// @ingroup VE_FEATURE
/// @return @ref MS_VE_OSD_LAYER_SEL
//------------------------------------------------------------------------------
// MS_VE_OSD_LAYER_SEL MDrv_VE_Get_OSDLayer(void);

//------------------------------------------------------------------------------
/// Description: Set VE Video Alpha
/// @ingroup VE_FEATURE
/// @param <IN>\b u8Val: set the video alpha value
/// @return @ref VE_Result
//------------------------------------------------------------------------------
// VE_Result MDrv_VE_Set_VideoAlpha(MS_U8 u8Val);

//------------------------------------------------------------------------------
/// Description: Get VE Video Alpha
/// @ingroup VE_FEATURE
/// @param <OUT>\b pu8Val: pointer to the value
/// @return @ref VE_Result
//------------------------------------------------------------------------------
// VE_Result MDrv_VE_Get_VideoAlpha(MS_U8 *pu8Val);

//------------------------------------------------------------------------------
/// Description: VE Set RGB In
/// @ingroup VE_FEATURE
/// @param <IN>\b bEnable: set the input is RGB or not
/// @return none
//------------------------------------------------------------------------------
void MDrv_VE_SetRGBIn(MS_BOOL bEnable);

//------------------------------------------------------------------------------
/// Description: VE Get output video standard
/// @ingroup VE_FEATURE
/// @return @ref MS_VE_VIDEOSYS
//------------------------------------------------------------------------------
MS_VE_VIDEOSYS MDrv_VE_Get_Output_Video_Std(void);

//------------------------------------------------------------------------------
/// Description: VE Set Capture Mode
/// @ingroup VE_FEATURE
/// @param <IN>\b bEnable: enable or disable the capture mode
/// @return none
//------------------------------------------------------------------------------
void MDrv_VE_SetCaptureMode(MS_BOOL bEnable);

//------------------------------------------------------------------------------
/// Description: VE Get Destination Information for GOP mixer
/// @param <OUT>\b pDstInfo: output DST display info
/// @param <IN>\b u32SizeofDstInfo: The size of DST display info
/// @return @ref MS_BOOL
//------------------------------------------------------------------------------
MS_BOOL MApi_VE_GetDstInfo(MS_VE_DST_DispInfo *pDstInfo, MS_U32 u32SizeofDstInfo);

//------------------------------------------------------------------------------
/// Description: VE Set Test pattern
/// @ingroup VE_FEATURE
/// @param <IN>\b bEnable: enable or disable the test pattern
/// @return none
//------------------------------------------------------------------------------
// void MDrv_VE_Set_TestPattern(MS_BOOL bEn);

//------------------------------------------------------------------------------
/// Description: Write the VE register
/// @ingroup VE_FEATURE
/// @param <IN>\b VE_BK: select the VE bank
/// @param <IN>\b u32Reg: the register to be set
/// @param <IN>\b u16Val: set value
/// @param <IN>\b u16Mask: the mask value
/// @return none
//------------------------------------------------------------------------------
void MApi_VE_W2BYTE_MSK(MS_VE_REG_BANK VE_BK, MS_U32 u32Reg, MS_U16 u16Val, MS_U16 u16Mask);

//------------------------------------------------------------------------------
/// Description: Write the VE register
/// @ingroup VE_FEATURE
/// @param <IN>\b VE_BK: select the VE bank
/// @param <IN>\b u32Reg: the register to be read
/// @param <IN>\b u16Mask: the mask value
/// @return @ref MS_U16
//------------------------------------------------------------------------------
// MS_U16 MApi_VE_R2BYTE_MSK(MS_VE_REG_BANK VE_BK, MS_U32 u32Reg, MS_U16 u16Mask);

//------------------------------------------------------------------------------
/// Description: VE Dump Table Interface
/// @ingroup VE_FEATURE
/// @param <IN>\b pVETable: pointer to the table
/// @param <IN>\b u8TableType: the type of table
/// @return none
//------------------------------------------------------------------------------
// void MDrv_VE_DumpTable(MS_U8 *pVETable, MS_U8 u8TableType);

//------------------------------------------------------------------------------
/// Description: This routine set flag to mask register write for special case \n
///            e.g. MBoot to APP with logo display
/// @ingroup VE_FEATURE
/// @param <IN>\b bFlag: TRUE: Mask register write, FALSE: not Mask
/// @return @ref VE_Result
//------------------------------------------------------------------------------
VE_Result MDrv_VE_DisableRegWrite(MS_BOOL bFlag);

//------------------------------------------------------------------------------
/// Description: VE show internal color bar (test pattern)
/// @ingroup VE_FEATURE
/// @param <IN>\b bEnable: TRUE to enable color; false to disable color bar
/// @return None
//------------------------------------------------------------------------------
// void MDrv_VE_ShowColorBar(MS_BOOL bEnable);

//------------------------------------------------------------------------------
/// Description: This routine set read/write addjustment to centralize VE display window.\n
///        Set the adjustment and it works after MDrv_VE_set_display_window() API is invoked. \n
/// @ingroup VE_FEATURE
/// @param <IN>\b s32WAddrAdjustment: the pixel units to adjust on write address
/// @param <IN>\b s32RAddrAdjustment: the pixel units to adjust on read address
/// @return @ref VE_Result
//------------------------------------------------------------------------------
VE_Result MDrv_VE_AdjustPositionBase(MS_S32 s32WAddrAdjustment, MS_S32 s32RAddrAdjustment);

//------------------------------------------------------------------------------
/// Description: In STB Chip, VE needs to set vepll to lock frame rate with HD Path.\n
/// @ingroup VE_FEATURE
/// @param <IN>\b pInCfg: Information of HD path
/// @param <IN>\b bEnable: enable/disable frame lock
/// @return @ref VE_Result
//------------------------------------------------------------------------------
// VE_Result MDrv_VE_SetFrameLock(MS_VE_PLL_InCfg *pInCfg, MS_BOOL bEnable);

//------------------------------------------------------------------------------
/// Description: Provide the capability of Customer scaling for VE.\n
/// @ingroup VE_FEATURE
/// @param <IN>\b pstVECusScalingInfo : Src and Dest info for customer scaling, @ref MS_VE_CusScalingInfo
/// @return @ref VE_Result
//------------------------------------------------------------------------------
VE_Result  MDrv_VE_Set_Customer_Scaling(MS_VE_CusScalingInfo *pstVECusScalingInfo);
//------------------------------------------------------------------------------
/// Description: MDrv_VE_set_crop_window, set up crop window (crop video frame in MIU)
/// @ingroup VE_FEATURE
/// @param <IN>\b stCropWin: crop window
/// @return @ref VE_Result
//------------------------------------------------------------------------------
// VE_Result MDrv_VE_set_crop_window(MS_VE_WINDOW_TYPE stCropWin);
//------------------------------------------------------------------------------
/// Description: VE be set display window (backend), after MIU
/// @ingroup VE_FEATURE
/// @param <IN>\b stDispWin: display window
/// @return @ref VE_Result
//------------------------------------------------------------------------------
// VE_Result MDrv_VE_set_be_display_window(MS_VE_WINDOW_TYPE stDispWin);
//------------------------------------------------------------------------------
/// Description: MDrv_VE_Get_Ext_Caps: get VE extented capability
/// @ingroup VE_FEATURE
/// @param <OUT>\b cap: Get capbility
/// @return @ref VE_Result
//------------------------------------------------------------------------------
// VE_Result MDrv_VE_Get_Ext_Caps(MS_VE_Ext_Cap *cap);
//------------------------------------------------------------------------------
/// Description: MDrv_VE_SetWindow, set crop window and display window in one API.
/// @ingroup VE_FEATURE
/// @argument:
/// @param <IN>\b stSrcWin: a pointer to MS_VE_WINDOW_TYPE - source window, depends on input size
/// @param <IN>\b stCropWin: a pointer to MS_VE_WINDOW_TYPE - crop window, depends on source window size
/// @param <IN>\b stDispWin: a pointer to MS_VE_WINDOW_TYPE - display window, depends on output size
/// @return @ref VE_Result
//------------------------------------------------------------------------------
// VE_Result MDrv_VE_SetWindow(MS_VE_WINDOW_TYPE *stSrcWin, MS_VE_WINDOW_TYPE *stCropWin,
//         MS_VE_WINDOW_TYPE *stDispWin);

//------------------------------------------------------------------------------
/// Description: Init and config the VE capture
/// @ingroup VE_FEATURE
/// @param pVECapture \b IN : @ref PMS_VE_Output_CAPTURE
/// @return none
//------------------------------------------------------------------------------
void MDrv_VE_InitVECapture(PMS_VE_Output_CAPTURE pVECapture);

//------------------------------------------------------------------------------
/// Description: Enable VE capture
/// @ingroup VE_FEATURE
/// @param <IN>\b pstVECapState: a pointer to caputre state
/// @return @ref MS_BOOL
//------------------------------------------------------------------------------
MS_BOOL MDrv_VE_EnaVECapture(PMS_VE_VECAPTURESTATE pstVECapState);

//------------------------------------------------------------------------------
/// Description: Get VE capture state
/// @ingroup VE_FEATURE
/// @param <IN>\b pstVECapState: a pointer to get caputre state
/// @return @ref MS_BOOL
//------------------------------------------------------------------------------
// MS_BOOL MDrv_VE_GetVECaptureState(PMS_VE_VECAPTURESTATE pstVECapState);

//-------------------------------------------------------------------------------------------------
/// Description: Wait on the finish of specified frame: Check if the frame is captured,
/// if Yes, return TRUE, otherwise sleep until the next Vsync ISR
/// @ingroup VE_FEATURE
/// @param <IN>\b pstVECapState: a pointer to caputre state
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_VE_VECaptureWaitOnFrame(PMS_VE_VECAPTURESTATE pstVECapState);

////////////////////////////////////////////////////////////////////////////////
/// Description : the frame start is used to adjust output video (in pixel)
/// @ingroup VE_FEATURE
/// @param <IN>        \b stSrcWin :source window, depends on input size
/// @param <IN>\b pixel_offset : set pixel offset value
/// @return @ref VE_Result
////////////////////////////////////////////////////////////////////////////////
VE_Result MDrv_VE_Adjust_FrameStart(MS_VE_WINDOW_TYPE *stSrcWin,MS_S16 pixel_offset);

//-------------------------------------------------------------------------------------------------
/// Description : Start or Stop to send WSS data on 525i system
/// @ingroup VE_FEATURE
/// @param <IN>\b  bEn: enable/disable wss525
/// @param <IN>\b  u32WSSData: 20-bit wss data includes 14 bits of data and 6 bits of CRC
/// @return             @ref  VE_Result
//-------------------------------------------------------------------------------------------------
VE_Result MDrv_VE_SetWSS525Data(MS_BOOL bEn, MS_U32 u32WSSData);

//-------------------------------------------------------------------------------------------------
/// Description : Get current wss data
/// @ingroup VE_FEATURE
/// @return   32-bit wss data, it includes 14 bits of data and 6 bits of CRC in lower 20 bits
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_VE_GetWSS525Data(void);

//-------------------------------------------------------------------------------------------------
/// Description : Set the state for supesend
/// @ingroup VE_FEATURE
/// @param <IN>\b  u16PowerState: The state for suspend
/// @return  @ref MS_U32 , TRUE for success, false for fail
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_TVE_SetPowerState(EN_POWER_MODE u16PowerState);

//-------------------------------------------------------------------------------------------------
/// Description : Set the VE mirror mode.
/// @ingroup VE_FEATURE
/// @param <IN>\b  eMirrorMode: MS_VE_HMirrorMode, MS_VE_VMirrorMode, MS_VE_HVBothMirrorMode, MS_VE_NoneMirror
/// @return  void
//-------------------------------------------------------------------------------------------------
void MDrv_VE_SetMirrorMode(MS_VE_MirrorStatus eMirrorMode);

#ifdef __cplusplus
}
#endif

#endif  // _DRVTVENCODER_H
