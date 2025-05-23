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
#ifndef _DRVUSB_H
#define _DRVUSB_H

#include "drvUsbcommon.h"

//
// Function support
//
#define USBC_IP_SUPPORT 1 // over current detection support

#define USB_IF_EHSET_SUPPORT 1 // embeded host hisg speed electrical test procedure

#define USB_CDC_SUPPORT 0 // 1 to turn on cdc support

#define USB_MASS_STORAGE_SUPPORT 1

#define USB_HID_SUPPORT                     0

//
// Extended Function support
//

#define USB_SYSTEM_STR_SUPPORT

//#define USB_NOT_SUPPORT_EX_HUB

//
// Parameter definition
//
#define USBC_OVER_CURRENT 1
#define USBC_NON_OVER_CURRENT 2

#define  USB_PLUG_IN	1
#define  USB_PLUG_OUT	2

#define USB_EVENT_DEV_TYPE_UNKNOW    0
#define USB_EVENT_DEV_TYPE_STOR         1
#define USB_EVENT_DEV_TYPE_HID            2
#define USB_EVENT_DEV_TYPE_CDC          3
#define USB_EVENT_DEV_NO_RESPONSE          4

#define USB_NOT_RESPONSE		 0xff
#define USB_INTERFACE_CLASS_NONE 0
#define USB_INTERFACE_CLASS_MSD  0x08
#define USB_INTERFACE_CLASS_HUB  0x09
#define USB_INTERFACE_CLASS_HID  0x03

typedef void ( *USBCallback ) (MS_U8 u8Event, MS_U32 u32Para, char *s);
// for MDrv_USB_RegisterCallBack()
//u8Event: 1=plug, 2=unplug�K can extend later);
//u32Para: bit 0 ~ 3, device type (0: unknow, 1: Mass Storage, 2: HID)
//             bit 4 ~ 31, reserved
// for MDrv_OverCurrentDetect_RegisterCallBack()
//u8Event: 1=over current,�K can extend later);
//u32Para: port number

extern USBCallback _DrvUSB_CBFun;

void MDrv_OverCurrentDetect_RegisterCallBack (USBCallback pCallbackFn, MS_U8 port_mask);
void MDrv_USB_RegisterCallBack (USBCallback pCallbackFn);

void MDrv_Usb_Init(
    mem_Alloc     pfn_Cachedmem_Alloc,
    mem_Free      pfn_Cachedmem_Free,
    mem_Alloc     pfn_NonCachedmem_Alloc,
    mem_Free      pfn_NonCachedmem_Free,
    mem_VA2PA     pfn_mem_VA2PA,
    mem_PA2VA     pfn_mem_PA2VA,
    mem_Cached2Noncached pfn_mem_Cached2Noncached,
    mem_NonCached2Cached pfn_mem_NonCached2Cached
);
MS_BOOL MDrv_USB_Port_Init(MS_U8 u8Hostid);
void MDrv_UsbClose(void);
int MDrv_UsbDeviceConnectBitEx(MS_U8 u8Hostid);
MS_BOOL MDrv_USB_Port_Close(MS_U8 u8Hostid);
#ifdef USB_SYSTEM_STR_SUPPORT
void MDrv_Usb_STR_Off(MS_U8 u8Hostid);
void MDrv_Usb_STR_On(MS_U8 u8Hostid);
#endif

/* Mass Storage class API */
MS_BOOL MDrv_UsbBlockReadToMIU(MS_U8 lun, MS_U32 u32BlockAddr, MS_U32 u32BlockNum, MS_U32 u32MIUAddr);
MS_BOOL MDrv_UsbBlockWriteFromMIU(MS_U8 lun, MS_U32 u32BlockAddr, MS_U32 u32BlockNum, MS_U32 u32MIUAddr);
MS_BOOL MDrv_UsbBlockReadToMIUEx(MS_U8 uPort,MS_U8 lun, MS_U32 u32BlockAddr, MS_U32 u32BlockNum, MS_U32 u32MIUAddr);
MS_BOOL MDrv_UsbBlockWriteFromMIUEx(MS_U8 uPort,MS_U8 lun, MS_U32 u32BlockAddr, MS_U32 u32BlockNum, MS_U32 u32MIUAddr);
MS_U32 MDrv_GetUsbBlockSize(MS_U8 lun);
MS_U32 MDrv_GetUsbBlockSizeEx(MS_U8 uPort, MS_U8 lun);
MS_U32 MDrv_GetUsbBlockNumEx(MS_U8 uPort, MS_U8 lun);
MS_U32 MDrv_GetUsbBlockNum(MS_U8 lun);
MS_U8 MDrv_UsbGetMaxLUNCount(void);
MS_U8 MDrv_UsbGetMaxLUNCountEx(MS_U8 uPort);
MS_BOOL MDrv_UsbIsLunConnected(MS_U8 uPort, MS_U8 lun);

MS_BOOL ChkUsbReady(void); // Obsolete
MS_BOOL ChkUsbReadyEx(MS_U8 uPort); // Obsolete

/* Lookup USB information by disk path */
MS_U8 MDrv_USB_MscLookupHostID(char *str);
char *MDrv_USB_MscLookupManufacturerString(char *str);
char *MDrv_USB_MscLookupProductString(char *str);
char *MDrv_USB_MscLookupSerialNumberString(char *str);
MS_U32 MDrv_USB_MscLookupVidPid(char *str);
#endif

