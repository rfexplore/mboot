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
/// @file   drvAUDIO.h
/// @author MStar Semiconductor Inc.
/// @brief   Audio common driver
//
///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#ifndef _DRVAUDIO_H_
#define _DRVAUDIO_H_

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#include "MsTypes.h"
#include "drvAUDIO_if.h"

#ifdef __cplusplus
extern "C"
{
#endif

////////////////////////////////////////////////////////////////////////////////
// Define & data type
////////////////////////////////////////////////////////////////////////////////
/// Version string.
#define MSIF_AUDIO_LIB_CODE             {'A','U','D','-'}    //Lib code
#define MSIF_AUDIO_LIBVER               {'0','0'}            //LIB version
#define MSIF_AUDIO_BUILDNUM             {'5','B'}            //Build Number
#define MSIF_AUDIO_CHANGELIST           {'0','0','0','0','0','0','0','0'} //P4 ChangeList Number

#define AUD_DRV_VERSION                 /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_AUDIO_LIB_CODE,                /* IP__                                             */  \
    MSIF_AUDIO_LIBVER,                  /* 0.0 ~ Z.Z                                        */  \
    MSIF_AUDIO_BUILDNUM,                /* 00 ~ 99                                          */  \
    MSIF_AUDIO_CHANGELIST,              /* CL#                                              */  \
    MSIF_OS

#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)

#define AU_memcpy(  pDstAddr, pSrcAddr, u32Size) \
    do { \
        register unsigned long u32I=0; \
        register unsigned long u32Dst = (unsigned long)pDstAddr; \
        void * pSrc = (void *)pSrcAddr; \
        if( (u32Dst % 4) || ((unsigned long)pSrc % 4) ) \
        { \
            for( u32I=0; u32I< (unsigned long)(u32Size); u32I++) \
            { \
                ((volatile unsigned char *)u32Dst)[u32I] = ((volatile unsigned char *)pSrc)[u32I]; \
            } \
        } \
        else \
        { \
            for( u32I=0; u32I < ((unsigned long)(u32Size)/4); u32I++) \
            { \
                ((volatile unsigned long *)u32Dst)[u32I] = ((volatile unsigned long *)pSrc)[u32I]; \
            } \
            u32Dst += u32I*4; \
            pSrc = (void *)((unsigned long)pSrc + u32I*4); \
            if((u32Size)%4) \
            { \
                for( u32I=0; u32I<((unsigned long)(u32Size)%4); u32I++) \
                { \
                    ((volatile unsigned char *)u32Dst)[u32I] = ((volatile unsigned char *)pSrc)[u32I]; \
                } \
            } \
        } \
    }while(0)

#else

#define AU_memcpy(  pDstAddr, pSrcAddr, u32Size) \
    do { \
        register MS_U32 u32I=0; \
        register MS_VIRT virtDst = (MS_VIRT)pDstAddr; \
        void * pSrc = (void *)pSrcAddr; \
        if( (virtDst % 4) || ((MS_VIRT)pSrc % 4) ) \
        { \
            for( u32I=0; u32I< (MS_U32)(u32Size); u32I++) \
            { \
                ((volatile MS_U8 *)virtDst)[u32I] = ((volatile MS_U8 *)pSrc)[u32I]; \
            } \
        } \
        else \
        { \
            for( u32I=0; u32I < ((MS_U32)(u32Size)/4); u32I++) \
            { \
                ((volatile MS_U32 *)virtDst)[u32I] = ((volatile MS_U32 *)pSrc)[u32I]; \
            } \
            virtDst += u32I*4; \
            pSrc = (void *)((MS_VIRT)pSrc + u32I*4); \
            if((u32Size)%4) \
            { \
                for( u32I=0; u32I<((MS_U32)(u32Size)%4); u32I++) \
                { \
                    ((volatile MS_U8 *)virtDst)[u32I] = ((volatile MS_U8 *)pSrc)[u32I]; \
                } \
            } \
        } \
    }while(0)

#endif

#define AU_Memset(pDstAddr, u8value, u32Size)                   \
    do {                                                        \
        MS_U32 i = 0;                                           \
        volatile MS_U8 *dst = (volatile MS_U8 *)(pDstAddr);     \
        for (i = 0; i < (u32Size); i++)                         \
        {                                                       \
            dst[i] = (u8value);                                 \
        }                                                       \
    } while (0)

#define HDMI_NonPCM_Mask         0x01
#define HDMI_Input_Path_Mask       0x10

typedef enum
{
  Hdmi_Input_Main = 0x00,
  Hdmi_Input_Sub = 0x10,

} Hdmi_Input_Path;

//================================================================
//  AUDIO version informtion functions
//================================================================
 extern MS_BOOL MDrv_AUD_GetLibVer(const MSIF_Version **ppVersion);

//================================================================
//  Basic Register read/write function
//================================================================
 MS_U16 MDrv_AUDIO_ReadReg(MS_U32 u32RegAddr);
 void   MDrv_AUDIO_WriteReg(MS_U32 u32RegAddr, MS_U16  u16Val);
 void   MDrv_AUDIO_WriteMaskReg(MS_U32 u32RegAddr, MS_U16  u16Mask, MS_U16  u16Val);

 MS_U8  MDrv_AUDIO_ReadByte(MS_U32 addr);
 MS_U8  MDrv_AUDIO_DecReadByte(MS_U32 addr);
 MS_U8  MDrv_AUDIO_SeReadByte(MS_U32 addr);

 void   MDrv_AUDIO_WriteByte(MS_U32 addr, MS_U8 value);
 void   MDrv_AUDIO_DecWriteByte(MS_U32 addr, MS_U8 value);
 void   MDrv_AUDIO_SeWriteByte(MS_U32 addr, MS_U8 value);

 void   MDrv_AUDIO_WriteMaskByte(MS_U32 u32RegAddr, MS_U8  u8Mask, MS_U8  u8Val);
 void   MDrv_AUDIO_DecWriteMaskByte(MS_U32 u32RegAddr, MS_U8  u8Mask, MS_U8  u8Val);
 void   MDrv_AUDIO_SeWriteMaskByte(MS_U32 u32RegAddr, MS_U8  u8Mask, MS_U8  u8Val);

 MS_U16 MDrv_AUDIO_ReadDecMailBox(MS_U8 paramNum);
 void   MDrv_AUDIO_WriteDecMailBox(MS_U8 paramNum, MS_U16 data1);
 MS_U16 MDrv_AUDIO_ReadSeMailBox(MS_U8 paramNum);
 void   MDrv_AUDIO_WriteSeMailBox(MS_U8 paramNum, MS_U16 data1);

//================================================================
//  For Audio initialize
//================================================================
void    MDrv_AUDIO_InitMMIO(void);
MS_BOOL MDrv_AUDIO_AttachInterrupt(MS_BOOL bEnable);
MS_BOOL MDrv_AUDIO_Mutex_Init(void);
MS_BOOL MDrv_AUDIO_SHM_Init(void);
MS_BOOL MDrv_AUDIO_Init(void);
MS_BOOL MDrv_AUDIO_Close(void);
MS_BOOL MDrv_AUDIO_Open(void);
void    MDrv_AUDIO_WriteInitTable(void);
void    MDrv_AUDIO_WritePreInitTable(void);                //Refine power on sequence for earphone & DAC pop noise issue
void    MDrv_AUDIO_EnaEarphone_LowPower_Stage(void);    //Refine power on sequence for earphone & DAC pop noise issue
void    MDrv_AUDIO_EnaEarphone_HighDriving_Stage(void);    //Refine power on sequence for earphone & DAC pop noise issue
void    MDrv_AUDIO_SwResetMAD(void);
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
void    MDrv_AUDIO_SetDspBaseAddr(MS_U8 u8Index, MS_U32 u32Bin_Base_Address, MS_U32 u32Mad_Base_Buffer_Adr);
#else
void    MDrv_AUDIO_SetDspBaseAddr(MS_U8 u8Index, MS_PHY phyBin_Base_Address, MS_PHY phyMad_Base_Buffer_Adr);
#endif
MS_PHY  MDrv_AUDIO_GetDspBinBaseAddr(MS_U8 u8Iindex);
MS_PHY  MDrv_AUDIO_GetDspMadBaseAddr(MS_U8 u8Index) ;
void    MDrv_AUDIO_DspBootOnDDR(MS_BOOL bEnable);
MS_BOOL MDrv_AUDIO_DecoderLoadCode(void);
MS_BOOL MDrv_AUDIO_SeSystemLoadCode(void);
void    MDrv_AUDIO_DspReboot(MS_U8 param);
void    MDrv_AUDIO_RebootDecDSP(void);
void    MDrv_AUDIO_TriggerSifPLL(void);
void    MDrv_AUDIO_SetPlayFileFlag(MS_BOOL bDspType, MS_BOOL bSet);
MS_BOOL MDrv_AUDIO_GetPlayFileFlag(MS_BOOL bDspType);
MS_BOOL MDrv_AUDIO_InitialVars(void);
void MDrv_AUDIO_SET_INIT_FLAG(MS_BOOL bSet);
MS_BOOL MDrv_AUDIO_GET_INIT_FLAG(void);
void MDrv_AUDIO_ResetDspCodeType(void);

//================================================================
// Audio I/O Config. function
//================================================================
AUDIO_PATH_GROUP_TYPE  MDrv_AUDIO_GetPathGroup(void);
void    MDrv_AUDIO_SetNormalPath(AUDIO_PATH_TYPE u8Path, AUDIO_INPUT_TYPE u8Input, AUDIO_OUTPUT_TYPE output);
void    MDrv_AUDIO_SetInputPath( AUDIO_INPUT_TYPE u8Input, AUDIO_PATH_TYPE u8Path);
void    MDrv_AUDIO_SetOutputPath( AUDIO_PATH_TYPE u8Path, AUDIO_OUTPUT_TYPE u8Output);
void    MDrv_AUDIO_SetInternalPath(AUDIO_INTERNAL_PATH_TYPE u8Path,  AUDIO_OUTPUT_TYPE u8Output);

MS_U8   MDrv_AUDIO_ReadDspCodeType(void);

void    MDrv_AUDIO_BT_SetUploadRate(MS_BOOL bEnable, MS_U8 u8Samprate);
void    MDrv_AUDIO_BT_SetBufferCounter(MS_U32 u32Counter);

MS_U8   MDrv_AUDIO_ReadDspCounter(AUDIO_DSP_COUNTER_TYPE CounterType);
void    MDrv_AUDIO_SetPowerDownWait(MS_BOOL bEnable);
MS_BOOL MDrv_AUDIO_IsDTV(void);
void    MDrv_AUDIO_SetFading(MS_U32 u32VolFading);

//=====================================================
// AUDIO SPDIF Relational Hal Function
//=====================================================
void    MDrv_AUDIO_SPDIF_SetMute(MS_U8 mute_en);
void    MDrv_AUDIO_SPDIF_SetMode(MS_U8 spdif_mode, MS_U8 input_src);
void    MDrv_AUDIO_SPDIF_ByPassChannel(MS_BOOL u8Enable);
void    MDrv_AUDIO_SPDIF_SetChannelStatus(SPDIF_CS_MODE_TYPE eType, SPDIF_CS_MODE_VALUE eValue);
void    MDrv_AUDIO_SPDIF_Monitor_SamplingRate(void);
//=====================================================
// AUDIO HDMI Relational Hal Function
//=====================================================
void    MDrv_AUDIO_HDMI_Tx_GetStatus(MS_BOOL *onOff, AUDIO_FS_TYPE *hdmi_SmpFreq, HDMI_TX_OUTPUT_TYPE *outType );
MS_U8   MDrv_AUDIO_HDMI_DolbyMonitor(void);
void    MDrv_AUDIO_HDMI_AC3_PathCFG(MS_U8 u8Ctrl);
void    MDrv_AUDIO_HDMI_TX_SetMode(HDMI_TX_OUTPUT_TYPE outType, AUDIO_SOURCE_INFO_TYPE eSource);
//================================================================
// for audio driver internal use function
//================================================================
void    MDrv_AUDIO_AdcInit(void);
MS_U8   MDrv_AUDIO_GetDecoderStatus(void);

MS_U16  MDrv_AUDIO_GetUniDecodeDoneTag(void);

void    MDrv_AUDIO_update_DspUsage(AUDIO_SOURCE_INFO_TYPE eSourceType);
//================================================================
// Audio ALSA Interface Relational Function
//================================================================
MS_BOOL MDrv_AUDIO_ALSA_Enable(MS_BOOL bEnable);

void MDrv_AUDIO_DumpDspInfo(void);

MS_U32 MDrv_AUDIO_GetCaps(AUDIO_DEC_ID DecId, MS_U32 *pCaps);

#ifdef __cplusplus
}
#endif

#endif

