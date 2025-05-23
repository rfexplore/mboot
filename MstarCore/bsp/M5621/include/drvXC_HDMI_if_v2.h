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

#ifndef _XC_HDMI_IF_H_
#define _XC_HDMI_IF_H_

#include "MsTypes.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
    //HDMI Rx property
    MAPI_CMD_HDMIRX_INITIAL,
    MAPI_CMD_HDMIRX_STABLE_POLLING,
    MAPI_CMD_HDMIRX_GET_SOURCE_VERSION,
    MAPI_CMD_HDMIRX_CHECK_HDCP_STATE,
    MAPI_CMD_HDMIRX_CHECK_HDCP_ENC_STATE,
    MAPI_CMD_HDMIRX_CHECK_HDCP14_KEY,
    MAPI_CMD_HDMIRX_GET_HDCP14_KEY_VAILD,
    MAPI_CMD_HDMIRX_SET_HPD_INVERT_FLAG,
    MAPI_CMD_HDMIRX_GET_PACKET_RECEIVE,
    /*************************** HDCP Repeater ***************************/
    MAPI_CMD_HDMIRX_WRITE_X74,
    MAPI_CMD_HDMIRX_READ_X74,
    MAPI_CMD_HDMIRX_SET_REPEATER,
    MAPI_CMD_HDMIRX_SET_BSTATUS,
    MAPI_CMD_HDMIRX_SET_HDMI_MODE,
    MAPI_CMD_HDMIRX_GET_INTERRUPT_STATUS,
    MAPI_CMD_HDMIRX_WRITE_KSV_LIST,
    MAPI_CMD_HDMIRX_SET_VPRIME,
    MAPI_CMD_HDMIRX_HDCP22_FILL_CIPHER_KEY,
    MAPI_CMD_HDMIRX_HDCP22_PORT_INIT,
    MAPI_CMD_HDMIRX_HDCP22_POLLING_READ_DONE,
    MAPI_CMD_HDMIRX_HDCP22_ENABLE_CIPHER,
    MAPI_CMD_HDMIRX_HDCP22_SEND_MESSAGE,
    MAPI_CMD_HDMIRX_HDCP22_HANDLER,
    MAPI_CMD_HDMIRX_HDCP22_WAIT_EVENT,
    /*************************** HDCP Repeater ***************************/
}E_HDMIRX_API_CMD_TYPE;

typedef struct
{
    stHDMI_INITIAL_TABLE stInitialTable;
} stHDMI_INITIAL_SETTING, *pstHDMI_INITIAL_SETTING;

typedef struct
{
    MS_U16 u16ReturnValue;
} stHDMI_FUNC_CAPS, *pstHDMI_FUNC_CAPS;

typedef struct
{
    HDMI_REST_t breset;
} stHDMI_PKT_RESET, *pstHDMI_PKT_RESET;

typedef struct
{
    MS_BOOL bHighLow;
    E_MUX_INPUTPORT enInputPortType;
    MS_BOOL bInverse;
} stHDMI_PULLHPD, *pstHDMI_PULLHPD;

typedef struct
{
    HDMI_GControl_INFO_t gcontrol;
    MS_U16 u16ReturnValue;
    MS_U8 ucHDMIInfoSource;
} stHDMI_GC_INFO, *pstHDMI_GC_INFO;

typedef struct
{
    MS_U32 ulPacketStatus;
    MS_BOOL bEnable;
    MS_U8 ucHDMIInfoSource;
} stHDMI_PACKET_RECEIVED, *pstHDMI_PACKET_RECEIVED;

typedef struct
{
    MS_HDMI_COLOR_FORMAT eReturnValue;
    MS_U8 ucHDMIInfoSource;
} stHDMI_GET_COLORFORMAT, *pstHDMI_GET_COLORFORMAT;

typedef struct
{
    EN_HDMI_COLOR_RANGE eReturnValue;
    MS_U8 ucHDMIInfoSource;
} stHDMI_GET_COLORRANGE, *pstHDMI_GET_COLORRANGE;

typedef struct
{
    MS_HDMI_EQ enEq;
    MS_U8 u8EQValue;
} stHDMI_SET_EQ, *pstHDMI_SET_EQ;

typedef struct
{
    MS_HDMI_EQ enEq;
    MS_U8 u8EQValue;
    E_MUX_INPUTPORT enInputPortType;
    MS_BOOL bReturnValue;
} stHDMI_SET_EQ_TO_PORT, *pstHDMI_SET_EQ_TO_PORT;

typedef struct
{
    MS_U16 u16MuteEvent;
    MS_U16 u16MuteMask;
} stHDMI_AUDIO_MUTE_ENABLE, *pstHDMI_AUDIO_MUTE_ENABLE;

typedef struct
{
    MS_BOOL bEn_PAR;
    MS_BOOL bEn_AFAR;
    MS_HDMI_AR_TYPE eReturnValue;
    MS_U8 ucHDMIInfoSource;
} stHDMI_GET_ASPECTRATIO, *pstHDMI_GET_ASPECTRATIO;

typedef struct
{
    MS_BOOL bEn_AFAR;
    MS_HDMI_AR_TYPE eReturnValue;
    MS_U8 ucHDMIInfoSource;
} stHDMI_GET_ACTIVEFORMAT_ASPECTRATIO, *pstHDMI_GET_ACTIVEFORMAT_ASPECTRATIO;

typedef struct
{
    EN_AVI_INFOFRAME_VERSION eReturnValue;
    MS_U8 ucHDMIInfoSource;
} stHDMI_GET_AVIINFOFRAMEVER, *pstHDMI_GET_AVIINFOFRAMEVER;

typedef struct
{
    MS_U8 u8value;
    MS_BOOL bread;
    MS_U8 u8ReturnValue;
    MS_U8 ucHDMIInfoSource;
} stHDMI_ERR_STATUS_UPDATE, *pstHDMI_ERR_STATUS_UPDATE;

typedef struct
{
    MS_U16 u8ReturnValue;
} stHDMI_GET_TMDS_FREQ, *pstHDMI_GET_TMDS_FREQ;

typedef struct
{
    HDMI_POLLING_STATUS_t **pStatus;
} stHDMI_GET_POLLINGSTATUS, *pstHDMI_GET_POLLINGSTATUS;

typedef struct
{
    MS_BOOL bReturnValue;
} stHDMI_ISHDMI_MODE, *pstHDMI_ISHDMI_MODE;

typedef struct DLL_PACKED
{
    MS_U8 * pu8ManufactorData;
    MS_U8 ucHDMIInfoSource;
} stHDMI_GET_MID_INFO, *pstHDMI_GET_MID_INFO;

typedef struct DLL_PACKED
{
    MS_HDMI_PACKET_STATE_t u8state;
    MS_U8 u8byte_idx;
    MS_U8 *pValue;
    MS_BOOL bReturnValue;
    MS_U8 ucHDMIInfoSource;
} stHDMI_GET_PACKET_VALUE, *pstHDMI_GET_PACKET_VALUE;

typedef struct
{
    MS_DVI_CHANNEL_TYPE u8Channel;
    MS_U16 u16ReturnValue;
} stDVI_CHANNELPHASESTATUS, *pstDVI_CHANNELPHASESTATUS;

typedef struct
{
    MS_U16 u16Reset;
} stDVI_SOFTWARERESET, *pstDVI_SOFTWARERESET;

typedef struct
{
    MS_BOOL bReturnValue;
    MS_U8 ucHDMIInfoSource;
} stHDMI_GET_AVIIINFO_ACTIVEINFOPRESENT, *pstHDMI_GET_AVIIINFO_ACTIVEINFOPRESENT;

typedef struct
{
    MS_BOOL bPullLow;
    E_MUX_INPUTPORT enInputPortType;
} stDVI_CLKPULLLOW, *pstDVI_CLKPULLLOW;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_U8 ucHDMIInfoSource;
} stDVI_SWITCHSRC, *pstDVI_SWITCHSRC;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_BOOL bReturnValue;
} stHDMI_SETFORCLOCKLESSTHAN70MHZ, *pstHDMI_SETFORCLOCKLESSTHAN70MHZ;

typedef struct
{
    MS_BOOL bClockLessThan70MHz;
} stHDMI_DVI_ADJUST, *pstHDMI_DVI_ADJUST;

typedef struct
{
    INPUT_SOURCE_TYPE_t enInputSourceType;
} stHDMI_SETUPDATEPHASELINECOUNT, *pstHDMI_SETUPDATEPHASELINECOUNT;

typedef struct
{
    MS_BOOL bEnable;
} stHDCP_ENABLE, *pstHDCP_ENABLE;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_BOOL bEnable;
    MS_BOOL bReturnValue;
} stHDMI_SET_HDCPENABLE, *pstHDMI_SET_HDCPENABLE;

typedef struct DLL_PACKED
{
    MS_U8 * pu8HdcpKeyData;
} stHDCP_INITPRODUCTIONKEY, *pstHDCP_INITPRODUCTIONKEY;

typedef struct DLL_PACKED
{
    MS_U16 usHDCPStatus;
} stHDCP_GETSTATUS, *pstHDCP_GETSTATUS;

typedef struct
{
    MS_BOOL bStable;
} stHDCP_VSYNC_END_EN, *pstHDCP_VSYNC_END_EN;

typedef struct
{
    MS_BOOL bEnable;
} stHDMI_AUDIO_OUTPUT, *pstHDMI_AUDIO_OUTPUT;

typedef struct
{
    MS_U8 u8ReturnValue;
    MS_U8 ucHDMIInfoSource;
} stHDMI_AUDIO_CP_HDR_INFO, *pstHDMI_AUDIO_CP_HDR_INFO;

typedef struct
{
    MS_U8 u8byte;
    MS_U8 u8ReturnValue;
    MS_U8 ucHDMIInfoSource;
} stHDMI_AUDIO_CHANNEL_STATUS, *pstHDMI_AUDIO_CHANNEL_STATUS;

typedef struct DLL_PACKED
{
    const MSIF_Version **ppVersion;
    HDMI_Ret_Value eReturnValue;
} stHDMI_GETLIBVER, *pstHDMI_GETLIBVER;

typedef struct DLL_PACKED
{
    MS_HDMI_INFO *pInfo;
    HDMI_Ret_Value eReturnValue;
} stHDMI_GETINFO, *pstHDMI_GETINFO;

typedef struct DLL_PACKED
{
    MS_HDMI_Status *pStatus;
    HDMI_Ret_Value eReturnValue;
} stHDMI_GETSTATUS, *pstHDMI_GETSTATUS;

typedef struct DLL_PACKED
{
    XC_DDCRAM_PROG_INFO *pstDDCRam_Info;
    MS_U32 u32SizeOfInfo;
} stHDMI_READ_DDCRAM, *pstHDMI_READ_DDCRAM;

typedef struct DLL_PACKED
{
    XC_DDCRAM_PROG_INFO *pstDDCRam_Info;
    MS_U32 u32SizeOfInfo;
} stHDMI_PROG_DDCRAM, *pstHDMI_PROG_DDCRAM;

typedef struct
{
    MS_HDMI_CONTENT_TYPE enReturnValue;
    MS_U8 ucHDMIInfoSource;
} stHDMI_GET_CONTENT_TYPE, *pstHDMI_GET_CONTENT_TYPE;

typedef struct
{
    MS_HDMI_EXT_COLORIMETRY_FORMAT enReturnValue;
    MS_U8 ucHDMIInfoSource;
} stHDMI_GET_EXT_COLORIMETRY, *pstHDMI_GET_EXT_COLORIMETRY;

typedef struct
{
    MS_U8 u8ReturnValue;
    MS_U8 ucHDMIInfoSource;
} stHDMI_GET_PIXEL_REPETITION, *pstHDMI_GET_PIXEL_REPETITION;

typedef struct
{
    MS_BOOL bReturnValue;
    MS_U8 ucHDMIInfoSource;
} stHDMI_CHECK_4K2K, *pstHDMI_CHECK_4K2K;

typedef struct
{
    MS_U8 ucHDMIInfoSource;
} stHDMI_3D_4K2K_PROCESS, *pstHDMI_3D_4K2K_PROCESS;

typedef struct
{
    MS_BOOL bTrue;
    MS_U8 ucHDMIInfoSource;
} stHDMI_AVG_SCALERINGDOWN, *pstHDMI_AVG_SCALERINGDOWN;

typedef struct
{
    E_HDMI_ADDITIONAL_VIDEO_FORMAT eReturnValue;
    MS_U8 ucHDMIInfoSource;
} stHDMI_CHECK_ADDITIONAL_FORMAT, *pstHDMI_CHECK_ADDITIONAL_FORMAT;

typedef struct
{
    E_XC_3D_INPUT_MODE eReturnValue;
    MS_U8 ucHDMIInfoSource;
} stHDMI_GET_3D_STRUCTURE, *pstHDMI_GET_3D_STRUCTURE;

typedef struct
{
    E_HDMI_3D_EXT_DATA_T eReturnValue;
    MS_U8 ucHDMIInfoSource;
} stHDMI_GET_3D_EXT_DATA, *pstHDMI_GET_3D_EXT_DATA;

typedef struct DLL_PACKED
{
    sHDMI_3D_META_FIELD *pdata;
    MS_U8 ucHDMIInfoSource;
} stHDMI_GET_3D_META_FIELD, *pstHDMI_GET_3D_META_FIELD;

typedef struct
{
    MS_U8 u8ReturnValue;
    MS_U8 ucHDMIInfoSource;
} stHDMI_GET_VIC_CODE, *pstHDMI_GET_VIC_CODE;

typedef struct
{
    E_HDMI_VIC_4Kx2K_CODE eReturnValue;
    MS_U8 ucHDMIInfoSource;
} stHDMI_GET_4KX2K_VIC_CODE, *pstHDMI_GET_4KX2K_VIC_CODE;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_BOOL bEnable;
    MS_BOOL bDrivingHigh;
} stHDMI_ARC_PINCONTROL, *pstHDMI_ARC_PINCONTROL;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_U16 u16Reset;
} stDVI_SOFTWARE_RESET, *pstDVI_SOFTWARE_RESET;

typedef struct
{
    EN_POWER_MODE enPowerState;
    MS_U32 u32ReturnValue;
} stHDMI_SET_POWERSTATE, *pstHDMI_SET_POWERSTATE;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_BOOL bReturnValue;
} stHDMI_CHECK_HDMI20_SETTING, *pstHDMI_CHECK_HDMI20_SETTING;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_U8 ucSourceVersion;
} stHDMI_GET_SOURCE_VERSION, *pstHDMI_GET_SOURCE_VERSION;

typedef struct
{
    MS_U32 ul5VDetectGPIOSelect;
} stHDMI_SET_5V_DETECT_GPIO_SELECT, *pstHDMI_SET_5V_DETECT_GPIO_SELECT;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_BOOL bStatus;
} stHDMI_GET_DE_STABLE_STATUS, *pstHDMI_GET_DE_STABLE_STATUS;

typedef struct
{
    MS_BOOL bHDCP14KeyVaildFlag;
} stHDMI_GET_HDCP14_KEY_VAILD, *pstHDMI_GET_HDCP14_KEY_VAILD;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_U8 ucHDCPState;
} stHDMI_CHECK_HDCP_STATE, *pstHDMI_CHECK_HDCP_STATE;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_U8 ucHDCPENCState;
} stHDMI_CHECK_HDCP_ENC_STATE, *pstHDMI_CHECK_HDCP_ENC_STATE;

typedef struct
{
    MS_BOOL bInverse;
    E_MUX_INPUTPORT enInputPortType;
} stHDMI_SET_HPD_INVERT_FLAG, *pstHDMI_SET_HPD_INVERT_FLAG;

typedef struct
{
    MS_U16 usReturnValue;
    E_HDMI_GET_DATA_INFO enInfo;
    MS_U8 ucHDMIInfoSource;
} stHDMI_GET_DATA_INFO, *pstHDMI_GET_DATA_INFO;

/*************************** HDCP22 ***************************/
typedef struct DLL_PACKED
{
    MS_U8 ucPortIdx;
    MS_U8* pucRiv;
    MS_U8* pucSessionKey;
} stHDCP22_FILL_CIPHER_KEY, *pstHDCP22_FILL_CIPHER_KEY;

#if 0 //wilson for u2k
typedef struct DLL_PACKED
{
    HDCP22_Recv_CBF pCBFunc;
    void* pContext;
} stHDCP22_InitCBFunc, *pstHDCP22_InitCBFunc;
#endif

typedef struct
{
    MS_U8    ucPortIdx;
} stHDCP22_PORT_INIT, *pstHDCP22_PORT_INIT;

typedef struct
{
    MS_U8    ucPortIdx;
    MS_BOOL bReturnValue;
} stHDCP22_POLLING_READ_DONE, *pstHDCP22_POLLING_READ_DONE;

typedef struct
{
    MS_U8    ucPortType;
    MS_U8    ucPortIdx;
    MS_BOOL bIsEnable;
} stHDCP22_ENABLE_CIPHER, *pstHDCP22_ENABLE_CIPHER;

typedef struct DLL_PACKED
{
    MS_U8    ucPortType;
    MS_U8    ucPortIdx;
    MS_U8*    pucData;
    MS_U32    dwDataLen;
    void*    pDummy;
} stHDCP22_SEND_MESSAGE, *pstHDCP22_SEND_MESSAGE;

typedef struct
{
    MS_BOOL bHdcp22RecvMsgFlag;
    MS_BOOL bIRQModeFlag;
    MS_U8 ucPortIdx;
    MS_U8 ucHdcp22PortType;
    MS_U8 pucHdcp22MsgData[129];
    MS_U32 ucHdcp22MsgLen;
} stHDCP22_HANDLER, *pstHDCP22_HANDLER;

typedef struct
{
    MS_U8 ucHDCPWriteDoneIndex;
} stHDCP22_WAIT_EVENT, *pstHDCP22_WAIT_EVENT;

/*************************** HDCP22 ***************************/
typedef struct DLL_PACKED
{
    MS_U32 u32Cmd;
    void *pBuf;
    MS_U32 u32BufSize;
    MS_BOOL bRet;
    MS_U8 ucHDMIInfoSource;
} stHDMI_CONTROL, *pstHDMI_CONTROL;

/************************ HDCP Repeater ************************/
typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_U8 ucOffset;
    MS_U8 ucData;
} stHDCP_WRITE_X74, *pstHDCP_WRITE_X74;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_U8 ucOffset;
    MS_U8 ucRetData;
} stHDCP_READ_X74, *pstHDCP_READ_X74;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_BOOL bIsRepeater;
} stHDCP_SET_REPEATER, *pstHDCP_SET_REPEATER;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_U16 usBstatus;
} stHDCP_SET_BSTATUS, *pstHDCP_SET_BSTATUS;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_BOOL bHDMIMode;
} stHDCP_SET_HDMI_MODE, *pstHDCP_SET_HDMI_MODE;

typedef struct
{
    E_MUX_INPUTPORT enInputPortType;
    MS_U8 ucRetIntStatus;
} stHDCP_GET_INTERRUPT_STATUS, *pstHDCP_GET_INTERRUPT_STATUS;

typedef struct DLL_PACKED
{
    E_MUX_INPUTPORT enInputPortType;
    MS_U8* pucKSV;
    MS_U32 ulLen;
} stHDCP_WRITE_KSV_LIST, *pstHDCP_WRITE_KSV_LIST;

typedef struct DLL_PACKED
{
    E_MUX_INPUTPORT enInputPortType;
    MS_U8* pucVPrime;
} stHDCP_SET_VPRIME, *pstHDCP_SET_VPRIME;
/************************ HDCP Repeater ************************/

#ifdef __cplusplus
}
#endif

#endif //_XC_HDMI_IF_H_
