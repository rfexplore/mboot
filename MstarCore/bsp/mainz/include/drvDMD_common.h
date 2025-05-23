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
/// @file   drvDMD_common.h
/// @brief  DVBC Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

/*! \defgroup Demod Demod interface

   
    Peripheral functions are controlled by each DEMOD standard
    
    <b>Features</b>
    
    - Registers read/write.
    - I2C, RF/IF, TS and SSPI control.  
 
    <b>ATSC Interface brief</b>
    ATSC is the demodulator of supporting 8VSB and J83B standard. 
    Receive RF signal from Tuner and transmit demodulated data to TS module.
    
    <b>Features</b>
    
    - Support 8VSB and J83B standard.
    - Pass A74 SPEC.
    - Pass the field stream.    
    
    <b> ATSC Block Diagram: </b> \n
    \image html drvDMD_ATSC_pic01.png
    
    <b> Operation Code Flow: </b> \n
    -# Initialize ATSC parameters and load DEMOD FW
    -# Enable DEMOD state machine
    -# Monitor lock status
    
    <b>DTMB interface brief</b>    
    DTMB is the demodulator of supporting DTMB(6/7/8/9M) standard. 
    Receive RF signal from Tuner and transmit demodulated data to TS module.
    
    <b>Features</b>
    
    - Support DTMB(6/7/8/9M) standard.
    - Pass CHINA SPEC.
    - Pass the field stream.    
    
    <b> DTMB Block Diagram: </b> \n
    \image html drvDMD_DTMB_pic01.png
    
    <b> Operation Code Flow: </b> \n
    -# Initialize DTMB parameters and load DEMOD FW
    -# Enable DEMOD state machine
    -# Monitor lock status

 *! \defgroup COMMON COMMON interface (drvDMD_common.h)
 *  \ingroup Demod
    
 *! \defgroup COMMON_BASIC COMMON basic control
 *  \ingroup COMMON
 */

#ifndef _DRV_DMD_COMMON_H_
#define _DRV_DMD_COMMON_H_

#ifdef __cplusplus
extern "C"
{
#endif


//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define USE_UTOPIA2P0

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef enum
{
    _QPSK        = 0x0,
    _16QAM        = 0x1,
    _64QAM        = 0x2,
    _UNKNOW_QAM   = 0xff,    
}DMD_CONSTEL;

typedef enum
{
    _CR1Y2        = 0x0,
    _CR2Y3        = 0x1,
    _CR3Y4        = 0x2,
    _CR5Y6        = 0x3,
    _CR7Y8        = 0x4,
    _UNKNOW_CR   = 0xff,    
}DMD_CODERATE;

typedef struct
{
    float   power_db;
    MS_U8   sar3_val;
}DMD_RFAGC_SSI;

typedef struct
{
    float   power_db;
    MS_U8   agc_val;
}DMD_IFAGC_SSI;

typedef struct
{
    float   attn_db;
    MS_U8   agc_err;
}DMD_IFAGC_ERR;

typedef struct
{
    DMD_CONSTEL         constel;
    DMD_CODERATE        code_rate;
    float               p_ref;
}DMD_SSI_DBM_NORDIGP1;

typedef struct
{
    DMD_CONSTEL   constel;
    DMD_CODERATE   code_rate;
    float   cn_ref;
}DMD_SQI_CN_NORDIGP1;

typedef struct
{
    DMD_RFAGC_SSI *pRfagcSsi;
    MS_U16 u16RfagcSsi_Size;
    DMD_IFAGC_SSI *pIfagcSsi_LoRef; 
    MS_U16 u16IfagcSsi_LoRef_Size;    
    DMD_IFAGC_SSI *pIfagcSsi_HiRef;   
    MS_U16 u16IfagcSsi_HiRef_Size;    
    DMD_IFAGC_ERR *pIfagcErr_LoRef;  
    MS_U16 u16IfagcErr_LoRef_Size;    
    DMD_IFAGC_ERR *pIfagcErr_HiRef;    
    MS_U16 u16IfagcErr_HiRef_Size;    
}DMD_SSI_TABLE;

typedef struct _s_I2C_Interface_func
{
    MS_BOOL (*I2C_WriteBytes)(MS_U16 u16BusNumSlaveID, MS_U8 u8addrcount, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data);
    MS_BOOL (*I2C_ReadBytes)(MS_U16 u16BusNumSlaveID, MS_U8 u8AddrNum, MS_U8* paddr, MS_U16 u16size, MS_U8* pu8data);
}s_I2C_Interface_func;


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
/// Register DEMOD bank address
/// @ingroup COMMON_BASIC
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DMD_PreInit(void);
//-------------------------------------------------------------------------------------------------
/// Enable RF AGC Tristate 
/// @ingroup COMMON_BASIC
/// @param bEnable    \b IN: Enable as TRUE
/// @return : NULL
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC void MDrv_DMD_RFAGC_Tristate(MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// Enable IF AGC Tristate 
/// @ingroup COMMON_BASIC
/// @param bEnable    \b IN: Enable as TRUE
/// @return : NULL
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC void MDrv_DMD_IFAGC_Tristate(MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// Get TS clock rate 
/// @ingroup COMMON_BASIC
/// @param fTS_CLK    \b OUT: the pointer to TS clock rate
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DMD_TS_GetClockRate(float *fTS_CLK);
//-------------------------------------------------------------------------------------------------
/// ts output clock frequency and phase configure 
/// @ingroup COMMON_BASIC
/// @param u8cmd_array    \b IN: the pointer to TS clock configuration
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DMD_TSO_Clk_Control(MS_U8 *u8cmd_array);
//-------------------------------------------------------------------------------------------------
/// read register data 
/// @ingroup COMMON_BASIC
/// @param u32Reg    \b IN: register address
/// @param u8Value    \b OUT: the pointer to register data
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DMD_ReadReg(MS_U32 u32Reg, MS_U8 *u8Value);
//-------------------------------------------------------------------------------------------------
/// write register data 
/// @ingroup COMMON_BASIC
/// @param u32Reg    \b IN: register address
/// @param u8Value    \b IN: register data
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DMD_WriteReg(MS_U32 u32Reg, MS_U8 u8Value);
//-------------------------------------------------------------------------------------------------
/// write register data for more than one byte 
/// @ingroup COMMON_BASIC
/// @param u32Reg    \b IN: register address
/// @param u8Value    \b IN: the pointer to data
/// @param u8Length    \b IN: length of data
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DMD_WriteRegs(MS_U32 u32Reg, MS_U8 *u8Value, MS_U8 u8Length);
//-------------------------------------------------------------------------------------------------
/// Change I2C channel 
/// @ingroup COMMON_BASIC
/// @param ch_num    \b IN: I2C channel number
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DMD_I2C_Channel_Change(MS_U8 ch_num);
//-------------------------------------------------------------------------------------------------
/// Set I2C channel 
/// @ingroup COMMON_BASIC
/// @param ch_num    \b IN: I2C channel number
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DMD_I2C_Channel_Set(MS_U8  ch_num);
//-------------------------------------------------------------------------------------------------
/// Initialize SSPI interface 
/// @ingroup COMMON_BASIC
/// @param u8DeviceNum    \b IN: SSPI device number
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DMD_SSPI_Init(MS_U8  u8DeviceNum);
//-------------------------------------------------------------------------------------------------
/// MIU write data more than one byte per SSPI interface 
/// @ingroup COMMON_BASIC
/// @param u32Addr    \b IN: register address
/// @param pdata    \b IN: the pointer to data
/// @param u16Size    \b IN: length of data
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DMD_SSPI_MIU_Writes(MS_U32 u32Addr, MS_U8 *pdata, MS_U16 u16Size);
//-------------------------------------------------------------------------------------------------
/// MIU read data more than one byte per SSPI interface 
/// @ingroup COMMON_BASIC
/// @param u32Addr    \b IN: register address
/// @param pdata    \b IN: the pointer to data
/// @param u16Size    \b IN: length of data
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DMD_SSPI_MIU_Reads(MS_U32 u32Addr, MS_U8 *pdata, MS_U16 u16Size);
//-------------------------------------------------------------------------------------------------
/// MIU write data per SSPI interface 
/// @ingroup COMMON_BASIC
/// @param u16Addr    \b IN: register address
/// @param data    \b IN: register data
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DMD_SSPI_RIU_Write8(MS_U16 u16Addr, MS_U8 data);
//-------------------------------------------------------------------------------------------------
/// MIU read data per SSPI interface 
/// @ingroup COMMON_BASIC
/// @param u16Addr    \b IN: register address
/// @param pdata    \b out: register data
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DMD_SSPI_RIU_Read8(MS_U16 u16Addr, MS_U8 *pdata);

#ifdef __cplusplus
}
#endif


#endif // _DRV_DVBC_H_

