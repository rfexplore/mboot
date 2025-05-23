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
// comply with all applicable laws and regulations
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
//    or otherwise
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

#ifndef _MVOP_H_
#define _MVOP_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
//******** ENUM DEFINITIONS ********//
typedef enum
{
    E_MVOP_POOL_ID_INTERNAL = 0, // just for test, please refine.
} E_MVOP_POOL_ID;


typedef enum
{
    //Main MVOP
    E_MVOP_CMD_INIT,                    //0x0
    E_MVOP_CMD_EXIT,                    //0x1
    E_MVOP_CMD_ENABLE,                  //0x2
    E_MVOP_CMD_SET_INPUTCFG,            //0x3
    E_MVOP_CMD_SET_OTPUTCFG,            //0x4
    E_MVOP_CMD_ENABLE_BLACK_BG,         //0x5
    E_MVOP_CMD_ENABLE_UV_SHIFT,         //0x6
    E_MVOP_CMD_SET_MONO_MODE,           //0x7

    E_MVOP_CMD_GET_HSIZE,               //0x8
    E_MVOP_CMD_GET_VSIZE,               //0x9
    E_MVOP_CMD_GET_HSTART,              //0xA
    E_MVOP_CMD_GET_VSTART,              //0xB
    E_MVOP_CMD_GET_IS_INTERLACE,        //0xC
    E_MVOP_CMD_GET_IS_HDUPLICATE,       //0xD
    E_MVOP_CMD_GET_IS_ENABLE,           //0xE
    E_MVOP_CMD_GET_OUTPUT_TIMING,       //0xF
    E_MVOP_CMD_GET_LIBVER,              //0x10
    E_MVOP_CMD_CHECK_CAPABILITY,        //0x11
    E_MVOP_CMD_GET_MAX_HOFFSET,         //0x12
    E_MVOP_CMD_GET_MAX_VOFFSET,         //0x13

    E_MVOP_CMD_SET_DBG_LEVEL,           //0x14
    E_MVOP_CMD_GET_INFO,                //0x15
    E_MVOP_CMD_GET_STATUS,              //0x16
    E_MVOP_CMD_SET_CLK,                 //0x17
    E_MVOP_CMD_SET_PATTERN,             //0x18
    E_MVOP_CMD_SET_TILE_FORMAT,         //0x19
    E_MVOP_CMD_GET_DST_INFO,            //0x1A
    E_MVOP_CMD_SET_FIXVTT,              //0x1B
    E_MVOP_CMD_SET_MMIO_MAPBASE,        //0x1C
    E_MVOP_CMD_MIU_SWITCH,              //0x1D
    E_MVOP_CMD_SET_BASEADD,             //0x1E
    E_MVOP_CMD_SEL_OP_FIELD,            //0x1F
    E_MVOP_CMD_SET_REGSIZE_FROM_MVD,    //0x20
    E_MVOP_CMD_SET_START_POS,           //0x21
    E_MVOP_CMD_SET_IMAGE_WIDTH_HIGHT,   //0x22
    E_MVOP_CMD_SET_VOP_MIRROR_MODE,     //0x23
    E_MVOP_CMD_INV_OP_VS,               //0x24
    E_MVOP_CMD_FORCE_TOP,               //0x25
    E_MVOP_CMD_ENABLE_FREERUN_MODE,     //0x26
    E_MVOP_CMD_GET_BASE_ADD,            //0x27

    //Sub MVOP
    E_MVOP_CMD_SUB_INIT,                //0x28
    E_MVOP_CMD_SUB_EXIT,                //0x29
    E_MVOP_CMD_SUB_ENABLE,              //0x2A
    E_MVOP_CMD_SUB_SET_INPUTCFG,        //0x2B
    E_MVOP_CMD_SUB_SET_OTPUTCFG,        //0x2C

    E_MVOP_CMD_SUB_ENABLE_BLACK_BG,     //0x2D
    E_MVOP_CMD_SUB_ENABLE_UV_SHIFT,     //0x2E
    E_MVOP_CMD_SUB_SET_MONO_MODE,       //0x2F
    E_MVOP_CMD_SUB_GET_HSIZE,           //0x30
    E_MVOP_CMD_SUB_GET_VSIZE,           //0x31
    E_MVOP_CMD_SUB_GET_HSTART,          //0x32
    E_MVOP_CMD_SUB_GET_VSTART,          //0x33
    E_MVOP_CMD_SUB_GET_IS_INTERLACE,    //0x34
    E_MVOP_CMD_SUB_GET_IS_HDUPLICATE,   //0x35
    E_MVOP_CMD_SUB_GET_IS_ENABLE,       //0x36
    E_MVOP_CMD_SUB_GET_OUTPUT_TIMING,   //0x37
    E_MVOP_CMD_SUB_CHECK_CAPABILITY,    //0x38
    E_MVOP_CMD_SUB_GET_MAXHOFFSET,      //0x39
    E_MVOP_CMD_SUB_GET_MAXVOFFSET,      //0x3A
    E_MVOP_CMD_SUB_SET_CLK,             //0x3B
    E_MVOP_CMD_SUB_SET_PATTERN,         //0x3C
    E_MVOP_CMD_SUB_SET_TILE_FORMAT,     //0x3D
    E_MVOP_CMD_SUB_GET_DST_INFO,        //0x3E
    E_MVOP_CMD_SUB_SET_FIXVTT,          //0x3F
    E_MVOP_CMD_SUB_SET_MMIO_MAPBASE,    //0x40
    E_MVOP_CMD_SUB_MIU_SWITCH,          //0x41
    E_MVOP_CMD_SUB_SET_BASEADD,         //0x42
    E_MVOP_CMD_SUB_SET_VOP_MIRRORMODE,  //0x43
    E_MVOP_CMD_SUB_ENABLE_FREERUN_MODE, //0x44
    E_MVOP_CMD_SUB_GET_BASEADD,         //0x45

    //Third MVOP
    E_MVOP_CMD_EX_INIT,                 //0x46
    E_MVOP_CMD_EX_EXIT,                 //0x47
    E_MVOP_CMD_EX_ENABLE,               //0x48
    E_MVOP_CMD_EX_SET_INPUTCFG,         //0x49
    E_MVOP_CMD_EX_SET_OTPUTCFG,         //0x4A

    E_MVOP_CMD_EX_SET_CLK,              //0x4B
    E_MVOP_CMD_EX_SET_PATTERN,          //0x4C
    E_MVOP_CMD_EX_SET_TILEFORMAT,       //0x4D
    E_MVOP_CMD_EX_ENABLE_UV_SHIFT,      //0x4E
    E_MVOP_CMD_EX_ENABLE_BLACK_BG,      //0x4F
    E_MVOP_CMD_EX_SET_MONO_MODE,        //0x50
    E_MVOP_CMD_EX_SET_FIXVTT,           //0x51
    E_MVOP_CMD_EX_MIU_SWITCH,           //0x52
    E_MVOP_CMD_EX_SET_VOP_MIRRORMODE,   //0x53
    E_MVOP_CMD_EX_ENABLE_FREERUN_MODE,  //0x54

    E_MVOP_CMD_EX_GET_OUTPUT_TIMING,    //0x55
    E_MVOP_CMD_EX_GET_IS_ENABLE,        //0x56
    E_MVOP_CMD_EX_GET_HSIZE,            //0x57
    E_MVOP_CMD_EX_GET_VSIZE,            //0x58
    E_MVOP_CMD_EX_GET_HSTART,           //0x59
    E_MVOP_CMD_EX_GET_VSTART,           //0x5A
    E_MVOP_CMD_EX_GET_IS_INTERLACE,     //0x5B
    E_MVOP_CMD_EX_GET_IS_HDUPLICATE,    //0x5C
    E_MVOP_CMD_EX_GET_STATUS,           //0x5D
    E_MVOP_CMD_EX_CHECK_CAPABILITY,     //0x5E
    E_MVOP_CMD_EX_GET_DST_INFO,         //0x5F

    E_MVOP_CMD_SEND_BLUE_SCREEN,        //0x60
    E_MVOP_CMD_SET_COMMAND,             //0x61
    E_MVOP_CMD_GET_COMMAND,             //0x62
    E_MVOP_CMD_SET_FREQUENCY,           //0x63

    E_MVOP_CMD_ENABLE_INTERRUPT,        //0x64
    E_MVOP_CMD_GET_INT_STATUS,          //0x65
    E_MVOP_CMD_SUB_ENABLE_INTERRUPT,    //0x66
    E_MVOP_CMD_SUB_GET_INT_STATUS,      //0x67
    E_MVOP_CMD_EX_ENABLE_INTERRUPT,     //0x68
    E_MVOP_CMD_EX_GET_INT_STATUS,       //0x69

    E_MVOP_CMD_SET_POWERSTATE,          //0x6A

    E_MVOP_CMD_NUM,
    E_MVOP_CMD_MAX = E_MVOP_CMD_NUM,
} E_MVOP_IOCTL_CMDS;

// Main MVOP
typedef struct DLL_PACKED
{
    MS_BOOL bEnable;
} stMVOP_ENABLE,*pstMVOP_ENABLE;

typedef struct DLL_PACKED
{
    MVOP_InputSel in;
    MVOP_InputCfg* pCfg;
    MVOP_Result eRet;
} stMVOP_SET_INPUTCFG,*pstMVOP_SET_INPUTCFG;

typedef struct DLL_PACKED
{
    MVOP_VidStat *pstVideoStatus;
    MS_BOOL bEnHDup;
    MVOP_Result eRet;
} stMVOP_SET_OTPUTCFG,*pstMVOP_SET_OTPUTCFG;

typedef struct DLL_PACKED
{
    MS_BOOL bEnable;
} stMVOP_ENABLE_UV_SHIFT,*pstMVOP_ENABLE_UV_SHIFT;

typedef struct DLL_PACKED
{
    MS_BOOL bEnable;
} stMVOP_SET_MONO_MODE,*pstMVOP_SET_MONO_MODE;

typedef struct DLL_PACKED
{
    MS_U16 u16Ret;
} stMVOP_GET_HSIZE,*pstMVOP_GET_HSIZE;

typedef struct DLL_PACKED
{
    MS_U16 u16Ret;
} stMVOP_GET_VSIZE,*pstMVOP_GET_VSIZE;

typedef struct DLL_PACKED
{
    MS_U16 u16Ret;
} stMVOP_GET_HSTART,*pstMVOP_GET_HSTART;

typedef struct DLL_PACKED
{
    MS_U16 u16Ret;
} stMVOP_GET_VSTART,*pstMVOP_GET_VSTART;

typedef struct DLL_PACKED
{
    MS_BOOL bRet;
} stMVOP_GET_IS_INTERLACE, *pstMVOP_GET_IS_INTERLACE;

typedef struct DLL_PACKED
{
    MS_BOOL bRet;
} stMVOP_GET_IS_HDUPLICATE, *pstMVOP_GET_IS_HDUPLICATE;

typedef struct DLL_PACKED
{
    MS_BOOL* pbEnable;
    MVOP_Result eRet;
} stMVOP_GET_IS_ENABLE, *pstMVOP_GET_IS_ENABLE;

typedef struct DLL_PACKED
{
    MVOP_Timing* pMVOPTiming;
    MVOP_Result eRet;
} stMVOP_GET_OUTPUT_TIMING, *pstMVOP_GET_OUTPUT_TIMING;

typedef struct DLL_PACKED
{
    const MSIF_Version **ppVersion;
    MVOP_Result eRet;
} stMVOP_GET_LIBVER,*pstMVOP_GET_LIBVER;

typedef struct DLL_PACKED
{
    MS_U16 u16HSize;
    MS_U16 u16VSize;
    MS_U16 u16Fps;
    MVOP_Result bRet;
} stMVOP_CHECK_CAPABILITY,*pstMVOP_CHECK_CAPABILITY;

typedef struct DLL_PACKED
{
    MS_U16 u16HSize;
    MS_U16 u16VSize;
    MS_U16 u16Fps;
    MS_U16 u16Ret;
} stMVOP_GET_MAX_HOFFSET, *pstMVOP_GET_MAX_HOFFSET;

typedef struct DLL_PACKED
{
    MS_U16 u16HSize;
    MS_U16 u16VSize;
    MS_U16 u16Fps;
    MS_U16 u16Ret;
} stMVOP_GET_MAX_VOFFSET, *pstMVOP_GET_MAX_VOFFSET;

typedef struct DLL_PACKED
{
    MS_U8 level;
} stMVOP_SET_DBG_LEVEL,*pstMVOP_SET_DBG_LEVEL;

typedef struct DLL_PACKED
{
    const MVOP_DrvInfo* pRet;
} stMVOP_GET_INFO, *pstMVOP_GET_INFO;

typedef struct DLL_PACKED
{
    MVOP_DrvStatus *pMVOPStat;
    MS_BOOL bRet;
} stMVOP_GET_STATUS, *pstMVOP_GET_STATUS;

typedef struct DLL_PACKED
{
    MS_BOOL bEnable;
} stMVOP_SET_CLK, *pstMVOP_SET_CLK;

typedef struct DLL_PACKED
{
    MVOP_Pattern enMVOPPattern;
} stMVOP_SET_PATTERN, *pstMVOP_SET_PATTERN;

typedef struct DLL_PACKED
{
    MVOP_TileFormat eTileFmt;
    MS_BOOL bRet;
} stMVOP_SET_TILE_FORMAT, *pstMVOP_SET_TILE_FORMAT;

typedef struct DLL_PACKED
{
    MVOP_DST_DispInfo *pDstInfo;
    MS_U32 u32SizeofDstInfo;
    MS_BOOL bRet;
} stMVOP_GET_DST_INFO, *pstMVOP_GET_DST_INFO;

typedef struct DLL_PACKED
{
    MS_U16 u16FixVtt;
    MS_BOOL bRet;
} stMVOP_SET_FIXVTT, *pstMVOP_SET_FIXVTT;

typedef struct DLL_PACKED
{
    MS_BOOL bRet;
} stMVOP_SET_MMIO_MAPBASE, *pstMVOP_SET_MMIO_MAPBASE;

typedef struct DLL_PACKED
{
    MS_U8 u8Miu;
    MVOP_Result eRet;
} stMVOP_MIU_SWITCH, *pstMVOP_MIU_SWITCH;

typedef struct DLL_PACKED
{
    MS_PHY u32YOffset;
    MS_PHY u32UVOffset;
    MS_BOOL bProgressive;
    MS_BOOL b422pack;
    MS_BOOL bRet;
} stMVOP_SET_BASEADD, *pstMVOP_SET_BASEADD;

typedef struct DLL_PACKED
{
    MS_BOOL bEnable;
} stMVOP_SEL_OP_FIELD, *pstMVOP_SEL_OP_FIELD;

typedef struct DLL_PACKED
{
    MS_BOOL bEnable;
} stMVOP_SET_REGSIZE_FROM_MVD, *pstMVOP_SET_REGSIZE_FROM_MVD;

typedef struct DLL_PACKED
{
    MS_U16 u16Xpos;
    MS_U16 u16Ypos;
} stMVOP_SET_START_POS, *pstMVOP_SET_START_POS;

typedef struct DLL_PACKED
{
    MS_U16 u16Width;
    MS_U16 u16Height;
    MS_BOOL bRet;
} stMVOP_SET_IMAGE_WIDTH_HIGHT, *pstMVOP_SET_IMAGE_WIDTH_HIGHT;

typedef struct DLL_PACKED
{
    MS_BOOL bEnable;
    MVOP_DrvMirror eMirrorMode;
} stMVOP_SET_VOP_MIRROR_MODE, *pstMVOP_SET_VOP_MIRROR_MODE;

typedef struct DLL_PACKED
{
    MS_BOOL bEnable;
} stMVOP_INV_OP_VS, *pstMVOP_INV_OP_VS;

typedef struct DLL_PACKED
{
    MS_BOOL bEnable;
} stMVOP_FORCE_TOP, *pstMVOP_FORCE_TOP;

typedef struct DLL_PACKED
{
    MS_BOOL bEnable;
    MS_BOOL bRet;
} stMVOP_ENABLE_FREERUN_MODE, *pstMVOP_ENABLE_FREERUN_MODE;

typedef struct DLL_PACKED
{
    MS_PHY* u32YOffset;
    MS_PHY* u32UVOffset;
} stMVOP_GET_BASE_ADD, *pstMVOP_GET_BASE_ADD;

// Sub MVOP
typedef struct DLL_PACKED
{
    MS_BOOL bEnable;
} stMVOP_SUB_ENABLE,*pstMVOP_SUB_ENABLE;

typedef struct DLL_PACKED
{
    MVOP_InputSel in;
    MVOP_InputCfg* pCfg;
    MVOP_Result eRet;
} stMVOP_SUB_SET_INPUTCFG,*pstMVOP_SUB_SET_INPUTCFG;

typedef struct DLL_PACKED
{
    MVOP_VidStat *pstVideoStatus;
    MS_BOOL bEnHDup;
    MVOP_Result eRet;
} stMVOP_SUB_SET_OTPUTCFG,*pstMVOP_SUB_SET_OTPUTCFG;

typedef struct DLL_PACKED
{
    MS_BOOL bEnable;
} stMVOP_SUB_ENABLE_UV_SHIFT,*pstMVOP_SUB_ENABLE_UV_SHIFT;

typedef struct DLL_PACKED
{
    MS_BOOL bEnable;
} stMVOP_SUB_SET_MONO_MODE,*pstMVOP_SUB_SET_MONO_MODE;

typedef struct DLL_PACKED
{
    MVOP_DrvStatus *pMVOPStat;
    MS_BOOL bRet;
} stMVOP_SUB_GET_STATUS, *pstMVOP_SUB_GET_STATUS;

typedef struct DLL_PACKED
{
    MS_U16 u16Ret;
} stMVOP_SUB_GET_HSIZE,*pstMVOP_SUB_GET_HSIZE;

typedef struct DLL_PACKED
{
    MS_U16 u16Ret;
} stMVOP_SUB_GET_VSIZE,*pstMVOP_SUB_GET_VSIZE;

typedef struct DLL_PACKED
{
    MS_U16 u16Ret;
} stMVOP_SUB_GET_HSTART,*pstMVOP_SUB_GET_HSTART;

typedef struct DLL_PACKED
{
    MS_U16 u16Ret;
} stMVOP_SUB_GET_VSTART,*pstMVOP_SUB_GET_VSTART;

typedef struct DLL_PACKED
{
    MS_BOOL bRet;
} stMVOP_SUB_GET_IS_INTERLACE, *pstMVOP_SUB_GET_IS_INTERLACE;

typedef struct DLL_PACKED
{
    MS_BOOL bRet;
} stMVOP_SUB_GET_IS_HDUPLICATE, *pstMVOP_SUB_GET_IS_HDUPLICATE;

typedef struct DLL_PACKED
{
    MS_BOOL* pbEnable;
    MVOP_Result eRet;
} stMVOP_SUB_GET_IS_ENABLE, *pstMVOP_SUB_GET_IS_ENABLE;

typedef struct DLL_PACKED
{
    MVOP_Timing* pMVOPTiming;
    MVOP_Result eRet;
} stMVOP_SUB_GET_OUTPUT_TIMING, *pstMVOP_SUB_GET_OUTPUT_TIMING;

typedef struct DLL_PACKED
{
    MS_U16 u16HSize;
    MS_U16 u16VSize;
    MS_U16 u16Fps;
    MS_BOOL bRet;
} stMVOP_SUB_CHECK_CAPABILITY,*pstMVOP_SUB_CHECK_CAPABILITY;

typedef struct DLL_PACKED
{
    MS_U16 u16HSize;
    MS_U16 u16VSize;
    MS_U16 u16Fps;
    MS_U16 u16Ret;
} stMVOP_SUB_GET_MAX_HOFFSET, *pstMVOP_SUB_GET_MAX_HOFFSET;

typedef struct DLL_PACKED
{
    MS_U16 u16HSize;
    MS_U16 u16VSize;
    MS_U16 u16Fps;
    MS_U16 u16Ret;
} stMVOP_SUB_GET_MAX_VOFFSET, *pstMVOP_SUB_GET_MAX_VOFFSET;

typedef struct DLL_PACKED
{
    MS_BOOL bEnable;
} stMVOP_SUB_SET_CLK, *pstMVOP_SUB_SET_CLK;

typedef struct DLL_PACKED
{
    MVOP_Pattern enMVOPPattern;
} stMVOP_SUB_SET_PATTERN, *pstMVOP_SUB_SET_PATTERN;

typedef struct DLL_PACKED
{
    MVOP_TileFormat eTileFmt;
    MS_BOOL bRet;
} stMVOP_SUB_SET_TILE_FORMAT, *pstMVOP_SUB_SET_TILE_FORMAT;

typedef struct DLL_PACKED
{
    MVOP_DST_DispInfo *pDstInfo;
    MS_U32 u32SizeofDstInfo;
    MS_BOOL bRet;
} stMVOP_SUB_GET_DST_INFO, *pstMVOP_SUB_GET_DST_INFO;

typedef struct DLL_PACKED
{
    MS_U16 u16FixVtt;
    MS_BOOL bRet;
} stMVOP_SUB_SET_FIXVTT, *pstMVOP_SUB_SET_FIXVTT;

typedef struct DLL_PACKED
{
    MS_BOOL bRet;
} stMVOP_SUB_SET_MMIO_MAPBASE, *pstMVOP_SUB_SET_MMIO_MAPBASE;

typedef struct DLL_PACKED
{
    MS_U8 u8Miu;
    MVOP_Result eRet;
} stMVOP_SUB_MIU_SWITCH, *pstMVOP_SUB_MIU_SWITCH;

typedef struct DLL_PACKED
{
    MS_PHY u32YOffset;
    MS_PHY u32UVOffset;
    MS_BOOL bProgressive;
    MS_BOOL b422pack;
    MS_BOOL bRet;
} stMVOP_SUB_SET_BASEADD, *pstMVOP_SUB_SET_BASEADD;

typedef struct DLL_PACKED
{
    MS_BOOL bEnable;
    MVOP_DrvMirror eMirrorMode;
} stMVOP_SUB_SET_VOP_MIRROR_MODE, *pstMVOP_SUB_SET_VOP_MIRROR_MODE;

typedef struct DLL_PACKED
{
    MS_BOOL bEnable;
    MS_BOOL bRet;
} stMVOP_SUB_ENABLE_FREERUN_MODE, *pstMVOP_SUB_ENABLE_FREERUN_MODE;

typedef struct DLL_PACKED
{
    MS_PHY* u32YOffset;
    MS_PHY* u32UVOffset;
} stMVOP_SUB_GET_BASE_ADD, *pstMVOP_SUB_GET_BASE_ADD;

// 3rd MVOP
typedef struct DLL_PACKED
{
    MVOP_DevID eID;
    MS_U32 u32InitParam;
    MVOP_Result eRet;
} stMVOP_EX_INIT,*pstMVOP_EX_INIT;

typedef struct DLL_PACKED
{
    MVOP_DevID eID;
    MS_U32 u32ExitParam;
    MVOP_Result eRet;
} stMVOP_EX_EXIT,*pstMVOP_EX_EXIT;

typedef struct DLL_PACKED
{
    MVOP_DevID eID;
    MS_BOOL bEnable;
    MVOP_Result eRet;
} stMVOP_EX_ENABLE,*pstMVOP_EX_ENABLE;

typedef struct DLL_PACKED
{
    MVOP_DevID eID;
    MVOP_InputSel in;
    MVOP_InputCfg* pCfg;
    MVOP_Result eRet;
} stMVOP_EX_SET_INPUTCFG,*pstMVOP_EX_SET_INPUTCFG;

typedef struct DLL_PACKED
{
    MVOP_DevID eID;
    MVOP_VidStat *pstVideoStatus;
    MS_BOOL bEnHDup;
    MVOP_Result eRet;
} stMVOP_EX_SET_OTPUTCFG,*pstMVOP_EX_SET_OTPUTCFG;

typedef struct DLL_PACKED
{
    MVOP_DevID eID;
    MS_BOOL bEnable;
} stMVOP_EX_SET_CLK, *pstMVOP_EX_SET_CLK;

typedef struct DLL_PACKED
{
    MVOP_DevID eID;
    MVOP_Pattern enMVOPPattern;
    MVOP_Result eRet;
} stMVOP_EX_SET_PATTERN, *pstMVOP_EX_SET_PATTERN;

typedef struct DLL_PACKED
{
    MVOP_DevID eID;
    MVOP_TileFormat eTileFmt;
    MVOP_Result eRet;
} stMVOP_EX_SET_TILE_FORMAT, *pstMVOP_EX_SET_TILE_FORMAT;

typedef struct DLL_PACKED
{
    MVOP_DevID eID;
    MS_BOOL bEnable;
    MVOP_Result eRet;
} stMVOP_EX_ENABLE_UV_SHIFT,*pstMVOP_EX_ENABLE_UV_SHIFT;

typedef struct DLL_PACKED
{
    MVOP_DevID eID;
    MVOP_Result eRet;
} stMVOP_EX_ENABLE_BLACK_BG,*pstMVOP_EX_ENABLE_BLACK_BG;

typedef struct DLL_PACKED
{
    MVOP_DevID eID;
    MS_BOOL bEnable;
    MVOP_Result eRet;
} stMVOP_EX_SET_MONO_MODE,*pstMVOP_EX_SET_MONO_MODE;

typedef struct DLL_PACKED
{
    MVOP_DevID eID;
    MS_U16 u16FixVtt;
    MVOP_Result eRet;
} stMVOP_EX_SET_FIXVTT, *pstMVOP_EX_SET_FIXVTT;

typedef struct DLL_PACKED
{
    MVOP_DevID eID;
    MS_U8 u8Miu;
    MVOP_Result eRet;
} stMVOP_EX_MIU_SWITCH, *pstMVOP_EX_MIU_SWITCH;

typedef struct DLL_PACKED
{
    MVOP_DevID eID;
    MS_BOOL bEnable;
    MVOP_DrvMirror eMirrorMode;
    MVOP_Result eRet;
} stMVOP_EX_SET_VOP_MIRROR_MODE, *pstMVOP_EX_SET_VOP_MIRROR_MODE;

typedef struct DLL_PACKED
{
    MVOP_DevID eID;
    MS_BOOL bEnable;
    MVOP_Result eRet;
} stMVOP_EX_ENABLE_FREERUN_MODE, *pstMVOP_EX_ENABLE_FREERUN_MODE;

typedef struct DLL_PACKED
{
    MVOP_DevID eID;
    MVOP_Timing* pMVOPTiming;
    MVOP_Result eRet;
} stMVOP_EX_GET_OUTPUT_TIMING, *pstMVOP_EX_GET_OUTPUT_TIMING;

typedef struct DLL_PACKED
{
    MVOP_DevID eID;
    MS_BOOL* pbEnable;
    MVOP_Result eRet;
} stMVOP_EX_GET_IS_ENABLE, *pstMVOP_EX_GET_IS_ENABLE;

typedef struct DLL_PACKED
{
    MVOP_DevID eID;
    MS_U16 u16Ret;
} stMVOP_EX_GET_HSIZE,*pstMVOP_EX_GET_HSIZE;

typedef struct DLL_PACKED
{
    MVOP_DevID eID;
    MS_U16 u16Ret;
} stMVOP_EX_GET_VSIZE,*pstMVOP_EX_GET_VSIZE;

typedef struct DLL_PACKED
{
    MVOP_DevID eID;
    MS_U16 u16Ret;
} stMVOP_EX_GET_HSTART,*pstMVOP_EX_GET_HSTART;

typedef struct DLL_PACKED
{
    MVOP_DevID eID;
    MS_U16 u16Ret;
} stMVOP_EX_GET_VSTART,*pstMVOP_EX_GET_VSTART;

typedef struct DLL_PACKED
{
    MVOP_DevID eID;
    MS_BOOL bRet;
} stMVOP_EX_GET_IS_INTERLACE, *pstMVOP_EX_GET_IS_INTERLACE;

typedef struct DLL_PACKED
{
    MVOP_DevID eID;
    MS_BOOL bRet;
} stMVOP_EX_GET_IS_HDUPLICATE, *pstMVOP_EX_GET_IS_HDUPLICATE;

typedef struct DLL_PACKED
{
    MVOP_DevID eID;
    MVOP_DrvStatus *pMVOPStat;
    MS_BOOL bRet;
} stMVOP_EX_GET_STATUS, *pstMVOP_EX_GET_STATUS;

typedef struct DLL_PACKED
{
    MVOP_DevID eID;
    MS_U32 u32InParam;
    MS_BOOL bRet;
} stMVOP_EX_CHECK_CAPABILITY,*pstMVOP_EX_CHECK_CAPABILITY;

typedef struct DLL_PACKED
{
    MVOP_DevID eID;
    MVOP_DST_DispInfo *pDstInfo;
    MS_U32 u32SizeofDstInfo;
    MS_BOOL bRet;
} stMVOP_EX_GET_DST_INFO, *pstMVOP_EX_GET_DST_INFO;

typedef struct DLL_PACKED
{
    MS_U16 u16Width;
    MS_U16 u16Height;
    MVOP_Result eRet;
} stMVOP_SEND_BLUE_SCREEN, *pstMVOP_SEND_BLUE_SCREEN;

typedef struct DLL_PACKED
{    
    MVOP_Command eCmd;
    MVOP_Result eRet;
    MVOP_Handle* stHd;
    void* pPara;
} stMVOP_SET_COMMAND, *pstMVOP_SET_COMMAND;

typedef struct DLL_PACKED
{    
    MVOP_Command eCmd;
    MS_U32 u32ParaSize;
    MVOP_Result eRet;
    MVOP_Handle* stHd;    
    void* pPara; 
} stMVOP_GET_COMMAND, *pstMVOP_GET_COMMAND;

typedef struct DLL_PACKED
{
    MVOP_FREQUENCY eFreq;
} stMVOP_SET_FREQUENCY, *pstMVOP_SET_FREQUENCY;

typedef struct DLL_PACKED
{
    MS_U8 eIntType;
    MS_BOOL bRet;
} stMVOP_ENABLE_INTERRUPT, *pstMVOP_ENABLE_INTERRUPT;

typedef struct DLL_PACKED
{
    MS_U8 u8Ret;
} stMVOP_GET_INT_STATUS, *pstMVOP_GET_INT_STATUS;

typedef struct DLL_PACKED
{
    MS_U8 eIntType;
    MS_BOOL bRet;
} stMVOP_SUB_ENABLE_INTERRUPT, *pstMVOP_SUB_ENABLE_INTERRUPT;

typedef struct DLL_PACKED
{
    MS_U8 u8Ret;
} stMVOP_SUB_GET_INT_STATUS, *pstMVOP_SUB_GET_INT_STATUS;

typedef struct DLL_PACKED
{
    MVOP_DevID eID;
    MS_U8 eIntType;
    MS_BOOL bRet;
} stMVOP_EX_ENABLE_INTERRUPT, *pstMVOP_EX_ENABLE_INTERRUPT;

typedef struct DLL_PACKED
{
    MVOP_DevID eID;
    MS_U8 u8Ret;
} stMVOP_EX_GET_INT_STATUS, *pstMVOP_EX_GET_INT_STATUS;

#if 1
typedef struct DLL_PACKED
{
    EN_POWER_MODE u16PowerState;
    MS_U32 u32Ret;
} stMVOP_SET_POWER_STATE, *pstMVOP_SET_POWER_STATE;
#endif

// OBSOLETE STRUCT **END**, please avoid using these

void MVOPRegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 MVOPOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 MVOPClose(void* pInstance);
MS_U32 MVOPIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);
MS_U32 MVOPStr(MS_U32 u32PowerState, void* pModule);

#ifdef __cplusplus
}
#endif

#undef _MVOP_H_
#endif //_mvop_H_
