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
/// @file   drvPWM.h
/// @brief  Pulse Width Modulation driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

/*! \defgroup G_PWM PWM interface
    \ingroup  G_PERIPHERAL

    \brief 
 
    Pulse-width modulation (PWM), or pulse-duration modulation (PDM), is a technique used to encode 
	a message into a pulsing signal. It is a type of modulation. Although this modulation technique 
	can be used to encode information for transmission, its main use is to allow the control of the 
	power supplied to electrical devices, especially to inertial loads such as motors.
    
    <b>Features</b>
    - PWM Initialize & De-initialize function
    - PWM Duty Cycle function
    - PWM Periodic function

    <b> Operation Code Flow: </b> \n
    -# PWM initialize with configuration.
    -# PWM channel enable.
    -# Setup duty cycle and periodic.

    \defgroup G_PWM_INIT Initialization Task relative
    \ingroup  G_PWM
    \defgroup G_PWM_COMMON Common Task relative
    \ingroup  G_PWM
    \defgroup G_PWM_CONTROL Control relative
    \ingroup  G_PWM
    \defgroup G_PWM_OTHER  other relative
    \ingroup  G_PWM
    \defgroup G_PWM_ToBeModified PWM api to be modified
    \ingroup  G_PWM
    \defgroup G_PWM_ToBeRemove PWM api to be removed
    \ingroup  G_PWM
*/

#ifndef _DRV_TEMP_H_
#define _DRV_TEMP_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "UFO.h"
////////////////////////////////////////////////////////////////////////////////
// Define & data type
////////////////////////////////////////////////////////////////////////////////
#define MSIF_PWM_LIB_CODE                     {'P','W','M','_'}    //Lib code
#define MSIF_PWM_LIBVER                       {'0','1'}            //LIB version
#define MSIF_PWM_BUILDNUM                     {'0','6'}            //Build Number
#define MSIF_PWM_CHANGELIST                   {'0','0','3','4','3','8','2','1'} //P4 ChangeList Number
#define MSIF_PWM_OS                           '0'                  //OS

#define          PWM_DRV_VERSION                  /* Character String for DRV/API version             */  \
            MSIF_TAG,                         /* 'MSIF'                                           */  \
            MSIF_CLASS,                       /* '00'                                             */  \
            MSIF_CUS,                         /* 0x0000                                           */  \
            MSIF_MOD,                         /* 0x0000                                           */  \
            MSIF_CHIP,                                                                              \
            MSIF_CPU,                                                                               \
            MSIF_PWM_LIB_CODE,                    /* IP__                                             */  \
            MSIF_PWM_LIBVER,                      /* 0.0 ~ Z.Z                                        */  \
            MSIF_PWM_BUILDNUM,                    /* 00 ~ 99                                          */  \
            MSIF_PWM_CHANGELIST,                  /* CL#                                              */  \
            MSIF_OS

#define PWM_DIV (4)

typedef enum _PWM_DbgLv
{
    E_PWM_DBGLV_NONE,          // no debug message
    E_PWM_DBGLV_ERR_ONLY,      // show error only
    E_PWM_DBGLV_INFO,          // show error & informaiton
    E_PWM_DBGLV_ALL            // show error, information & funciton name
}PWM_DbgLv;

typedef enum _PWM_ChNum
{
    E_PWM_CH0,
    E_PWM_CH1,
    E_PWM_CH2,
    E_PWM_CH3,
    E_PWM_CH4,
    E_PWM_CH5,
    E_PWM_CH6,
    E_PWM_CH7,
    E_PWM_CH8,
    E_PWM_CH9
}PWM_ChNum;

typedef enum _PWM_Property
{
    E_PWM_GetPeriod,
    E_PWM_GetDutyCycle,
    E_PWM_GetShift,
    E_PWM_GetDiv,
    E_PWM_GetPolarity,
    E_PWM_GetVdben,
    E_PWM_GetRsten,
    E_PWM_GetDben,
    E_PWM_GetOen
}PWM_Property;

typedef enum _PWM_CntMode
{
    E_PWM_Normal = 0x00,
    E_PWM_01Mode = 0x10,
    E_PWM_10Mode = 0x11
}PWM_CntMode;

typedef enum _PWM_Result
{
    E_PWM_OK,
    E_PWM_FAIL
} PWM_Result;

typedef struct _PWM_Info
{
    MS_U8   u8ChNum;           // number of this channel
    MS_U32  u32IOMap;          // base of this channel's registers
    MS_U32  mck;               // base clock rate from mcu
}PWM_Info;

/* This structure is full the same with hal for now. */
typedef struct _PWM_Arg
{
    MS_U16  index;
    MS_U8   div;
    MS_U16  period;
    MS_U16  duty;
    MS_BOOL polarity;
    MS_BOOL vdben;
    MS_BOOL rsten;
    MS_BOOL dben;
} PWM_Arg;

typedef struct _PWM_DrvStatus
{
    MS_BOOL PWM0;
    MS_BOOL PWM1;
    MS_BOOL PWM2;
    MS_BOOL PWM3;
    MS_BOOL PWM4;
    MS_BOOL PWM5;
    MS_BOOL PWM6;
    MS_BOOL PWM7;
    MS_BOOL PWM8;
} PWM_DrvStatus;

typedef struct _PWM_3D_Arg
{
	MS_U32 Shift;
	MS_U32 Duty;
} PWM_3D_Arg;

typedef enum _N_LR_SYNC_SEL
{
    E_VSYNC             = 0,
    E_LR_RISING         = 1,
    E_LR_FALLING        = 2,
    E_LR_RISING_FALLING = 3
} N_LR_SYNC_SEL;


typedef enum _PM_PWM_ChNum
{
    E_PM_PWM_CH0,
    E_PM_PWM_CH1,
    E_PM_PWM_CH2,
}PM_PWM_ChNum;

typedef enum _PM_PWM_PAD_CFG
{
    PAD_PM_VID0,
    PAD_PM_GPIO_1,
    PAD_PM_VID1,
    PAD_PM_GPIO_3,
    PAD_GPIO_2,
    PAD_GPIO_4,
}PM_PWM_PAD_CFG;

#define NON_INVERSE     0
#define INVERSE         1

////////////////////////////////////////////////////////////////////////////////
// include utopia v2  header files here
////////////////////////////////////////////////////////////////////////////////
#include "drvPWM_v2.h"
#include "MsDevice.h"
////////////////////////////////////////////////////////////////////////////////
// Extern Function
////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PWM_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
#if defined(UFO_PUBLIC_HEADER_500_3)
void MDrv_PM_PWM_Enable(PM_PWM_ChNum index, PM_PWM_PAD_CFG Pad_Sel, MS_BOOL  b_en);
#else
void MDrv_PM_PWM_Enable(void);
#endif
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PWM_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
#if defined(UFO_PUBLIC_HEADER_500_3)
void MDrv_PM_PWM_Period(PM_PWM_ChNum index,MS_U16 u16PeriodPWM);
#else
void MDrv_PM_PWM_Period(MS_U16 u16PeriodPWM);
#endif
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PWM_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
#if defined(UFO_PUBLIC_HEADER_500_3)
void MDrv_PM_PWM_DutyCycle(PM_PWM_ChNum index,MS_U16 u16DutyPWM);
#else
void MDrv_PM_PWM_DutyCycle(MS_U16 u16DutyPWM);
#endif
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PWM_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
#if defined(UFO_PUBLIC_HEADER_500_3)
void MDrv_PM_PWM_Div(PM_PWM_ChNum index,MS_U8 u8DivPWM);
#else
void MDrv_PM_PWM_Div(MS_U8 u8DivPWM);
#endif
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PWM_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
#if defined(UFO_PUBLIC_HEADER_500_3)
void MDrv_PM_PWM_Polarity(PM_PWM_ChNum index,MS_BOOL bPolPWM);
#else
void MDrv_PM_PWM_Polarity(MS_BOOL bPolPWM);
#endif
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PWM_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
#if defined(UFO_PUBLIC_HEADER_500_3)
void MDrv_PM_PWM_Dben(PM_PWM_ChNum index,MS_BOOL bdbenPWM);
#else
void MDrv_PM_PWM_Dben(MS_BOOL bDbenPWM);
#endif
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PWM_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U16 MDrv_PWM_SetPowerState(EN_POWER_MODE u16PowerState);

//------------------------------------------------------------------------------
/// Description : Show the PWM driver version
/// @ingroup G_PWM_COMMON
/// @param  ppVersion \b OUT: output PWM driver version
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//------------------------------------------------------------------------------
PWM_Result MDrv_PWM_GetLibVer(const MSIF_Version **ppVersion);

//------------------------------------------------------------------------------
/// Description : Show the PWM_PAD is PWM(True) or GPIO(false)
/// @ingroup G_PWM_COMMON
/// @param  pStatus \b OUT: output PWM driver status
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//------------------------------------------------------------------------------
PWM_Result MDrv_PWM_GetStatus(PWM_DrvStatus *pStatus);

//-------------------------------------------------------------------------------------------------
/// Description: Initial PWM driver
/// @ingroup G_PWM_INIT
/// @param eLevel    \b debug level for PWM driver
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM_Init(PWM_DbgLv eLevel);
//-------------------------------------------------------------------------------------------------
/// Description: IOutput enable_bar of PWM pads
/// @ingroup G_PWM_CONTROL
/// @param u8IndexPWM    \b which pwm is setting
/// @param bOenPWM    \b True/False for enable/disable
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM_Oen(PWM_ChNum u8IndexPWM, MS_BOOL bOenPWM);
//-------------------------------------------------------------------------------------------------
/// Description: Set the period of the specific pwm
/// @ingroup G_PWM_ToBeRemove
/// @param u8IndexPWM    \b which pwm is setting
/// @param u16PeriodPWM    \b the 18-bit period value
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM_Period(PWM_ChNum u8IndexPWM, MS_U32 u32PeriodPWM);
//-------------------------------------------------------------------------------------------------
/// Description: Set the period of the specific pwm
/// @ingroup G_PWM_CONTROL
/// @param u8IndexPWM    \b which pwm is setting
/// @return u16PeriodPWM
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM_GetProperty(PWM_Property eProperty, PWM_ChNum u8IndexPWM, MS_U32 *u32PWMVar);
//-------------------------------------------------------------------------------------------------
/// Description: Set the Duty of the specific pwm
/// @ingroup G_PWM_ToBeRemove
/// @param u8IndexPWM    \b which pwm is setting
/// @param u16DutyPWM    \b the 18-bit Duty value
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM_DutyCycle(PWM_ChNum u8IndexPWM, MS_U32 u32DutyPWM);
//-------------------------------------------------------------------------------------------------
/// Description: Set the Unit_Div of the pwm
/// @ingroup G_PWM_ToBeRemove
/// @param u16UnitDivPWM    \b the Unit Div value
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM_UnitDiv(MS_U16 u16UnitDivPWM);
//-------------------------------------------------------------------------------------------------
/// Description: Set the Div of the specific pwm
/// @ingroup G_PWM_ToBeRemove
/// @param u8IndexPWM    \b which pwm is setting
/// @param u16DivPWM    \b the 16-bit div value
//-------------------------------------------------------------------------------------------------
void MDrv_PWM_Div(PWM_ChNum u8IndexPWM, MS_U16 u16DivPWM);
//-------------------------------------------------------------------------------------------------
/// Description:  Set the Polarity of the specific pwm
/// @param u8IndexPWM    \b which pwm is setting
/// @param bPolPWM    \b True/False for Inverse/Non-inverse
//-------------------------------------------------------------------------------------------------
void MDrv_PWM_Polarity(PWM_ChNum u8IndexPWM, MS_BOOL bPolPWM);
//-------------------------------------------------------------------------------------------------
/// Description: Set the Vsync Double buffer of the specific pwm
/// @ingroup G_PWM_ToBeRemove
/// @param u8IndexPWM    \b which pwm is setting
/// @param bVdbenPWM    \b True/False for Enable/Disable
//-------------------------------------------------------------------------------------------------
void MDrv_PWM_Vdben(PWM_ChNum u8IndexPWM, MS_BOOL bVdbenPWM);
//-------------------------------------------------------------------------------------------------
/// Description: Set the Hsync reset of the specific pwm
/// @param u8IndexPWM    \b which pwm is setting
/// @param bRstPWM    \b True/False for Enable/Disable
//-------------------------------------------------------------------------------------------------
void MDrv_PWM_ResetEn(PWM_ChNum u8IndexPWM, MS_BOOL bRstPWM);
//-------------------------------------------------------------------------------------------------
/// Description:  Set the Double buffer of the specific pwm
/// @ingroup G_PWM_ToBeRemove
/// @param u8IndexPWM    \b which pwm is setting
/// @param bDbenPWM    \b True/False for Enable/Disable
//-------------------------------------------------------------------------------------------------
void MDrv_PWM_Dben(PWM_ChNum u8IndexPWM, MS_BOOL bDbenPWM);
//-------------------------------------------------------------------------------------------------
/// Description: IMPULSE ENABLE
/// @ingroup G_PWM_CONTROL
/// @param u8IndexPWM    \b which pwm is setting
/// @param bOenPWM    \b True/False for enable/disable
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
void MDrv_PWM_IMPULSE_EN(PWM_ChNum index, MS_BOOL bdbenPWM);
//-------------------------------------------------------------------------------------------------
/// Description: ODDEVEN_SYNC setting
/// @ingroup G_PWM_CONTROL
/// @param u8IndexPWM    \b which pwm is setting
/// @param bOenPWM    \b True/False for enable/disable
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
void MDrv_PWM_ODDEVEN_SYNC(PWM_ChNum index, MS_BOOL bdbenPWM);
//-------------------------------------------------------------------------------------------------
/// Description:  Set the Rst Mux of the specific pwm
/// @ingroup G_PWM_CONTROL
/// @param u8IndexPWM    \b which pwm is setting
/// @param bMuxPWM    \b True/False for Enable/Disable
//-------------------------------------------------------------------------------------------------
void MDrv_PWM_RstMux(PWM_ChNum u8IndexPWM, MS_BOOL bMuxPWM);
//-------------------------------------------------------------------------------------------------
/// Description: Set the Rst_Cnt of the specific pwm
/// @ingroup G_PWM_CONTROL
/// @param u8IndexPWM    \b which pwm is setting
/// @param u8RstCntPWM    \b the Rst_Cnt value
//-------------------------------------------------------------------------------------------------
void MDrv_PWM_RstCnt(PWM_ChNum u8IndexPWM, MS_U8 u8RstCntPWM);
//-------------------------------------------------------------------------------------------------
/// Description: Set the Bypass Unit of the specific pwm
/// @ingroup G_PWM_CONTROL
/// @param u8IndexPWM    \b which pwm is setting
/// @param bBypassPWM    \b True/False for Enable/Disable
//-------------------------------------------------------------------------------------------------
void MDrv_PWM_BypassUnit(PWM_ChNum u8IndexPWM, MS_BOOL bBypassPWM);
//-------------------------------------------------------------------------------------------------
/// Description: Counter mode for PWM0 and PWM1
/// @ingroup G_PWM_CONTROL
/// @param u8CntModePWM    \b Cnt Mode
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
/// @note    \n
///     11: PWM1 donate internal divider to PWM0    \n
///     10: PWM0 donate internal divider to PWM1    \n
///     0x: Normal mode    \n
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM01_CntMode(PWM_CntMode u8CntModePWM);
//-------------------------------------------------------------------------------------------------
/// Description: Counter mode for PWM2 and PWM3
/// @ingroup G_PWM_ToBeRemove
/// @param u8CntModePWM    \b Cnt Mode
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
/// @note    \n
///     11: PWM3 donate internal divider to PWM2    \n
///     10: PWM2 donate internal divider to PWM3    \n
///     0x: Normal mode    \n
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM23_CntMode(PWM_CntMode u8CntModePWM);
//-------------------------------------------------------------------------------------------------
/// Description: Counter mode for PWM6 and PWM7
/// @ingroup G_PWM_ToBeRemove
/// @param u8CntModePWM    \b Cnt Mode
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
/// @note    \n
///     11: PWM7 donate internal divider to PWM6    \n
///     10: PWM6 donate internal divider to PWM7    \n
///     0x: Normal mode    \n
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM67_CntMode(PWM_CntMode u8CntModePWM);
//-------------------------------------------------------------------------------------------------
/// Description: Set the Shift of the specific pwm
/// @ingroup G_PWM_ToBeRemove
/// @param u8IndexPWM    \b which pwm is setting
/// @param u16DutyPWM    \b the 18-bit Shift value
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM_Shift(PWM_ChNum u8IndexPWM, MS_U32 u32DutyPWM);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PWM_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_PWM_Nvsync(PWM_ChNum u8IndexPWM, MS_BOOL bNvsPWM);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PWM_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_PWM_Align(PWM_ChNum u8IndexPWM, MS_BOOL bAliPWM);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PWM_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PWM_Set3DMode(PWM_ChNum u8IndexPWM, PWM_3D_Arg *pArraySeting, MS_U8 u8ArraySize);

//-------------------------------------------------------------------------------------------------
/// Description: Set debug level for debug message
/// @param eLevel    \b debug level for PWM driver
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM_SetDbgLevel(PWM_DbgLv eLevel);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_PWM_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM_INV_3D_Flag(MS_BOOL bInvPWM);

void MDrv_PWM_3D_LR_Sync(PWM_ChNum u8IndexPWM, MS_BOOL bNvsPWM, N_LR_SYNC_SEL LrSyncSel, MS_U8 u8RstCnt);

#ifdef __cplusplus
}
#endif

#endif // _DRV_TEMP_H_

