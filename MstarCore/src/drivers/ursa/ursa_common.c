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
#ifndef __DRV_URSA_COMMON_C__
#define __DRV_URSA_COMMON_C__
#include "ursa_common.h"
#include <command.h>
#include <common.h>
#include <MsSystem.h>
#include <MsDebug.h>
#include <MsSysUtility.h>
/*
 * creating the global URSA command table here.
 */

URSA_COMMON_STATUS MDrv_URSA_COM_Write_URSA_HandshakeType(void)
{
    UBOOT_TRACE("IN\n");

    URSA_COMMON_TYPE ursa_type = MDrv_URSA_COM_Get_URSA_Type();

    switch (ursa_type)
    {
        case URSA_COMMON_U6:
            Write2Byte(URSA_COMMON_HANDSHAKE_TYPE_DUMMY_REG, URSA_COMMON_URSA6_HANDSHAKE);
            UBOOT_TRACE("OK\n");
            return URSA_SUCCED;

        case URSA_COMMON_U7:
            Write2Byte(URSA_COMMON_HANDSHAKE_TYPE_DUMMY_REG, URSA_COMMON_URSA7_HANDSHAKE);
            UBOOT_TRACE("OK\n");
            return URSA_SUCCED;

        case URSA_COMMON_U8:        // it don't need handshake
            UBOOT_TRACE("OK\n");
            return URSA_SUCCED;

        case URSA_COMMON_U9:
            Write2Byte(URSA_COMMON_HANDSHAKE_TYPE_DUMMY_REG, URSA_COMMON_URSA7_HANDSHAKE);
            UBOOT_TRACE("OK\n");
            return URSA_SUCCED;

        case URSA_COMMON_KS2:
            Write2Byte(URSA_COMMON_HANDSHAKE_TYPE_DUMMY_REG, URSA_COMMON_URSA6_HANDSHAKE);
            UBOOT_TRACE("OK\n");
            return URSA_SUCCED;

        case URSA_COMMON_FRC:
            Write2Byte(URSA_COMMON_HANDSHAKE_TYPE_DUMMY_REG, URSA_COMMON_URSA6_HANDSHAKE);
            UBOOT_TRACE("OK\n");
            return URSA_SUCCED;

        case URSA_COMMON_UNKNOWN:
            UBOOT_ERROR("[URSA_COMMON] Unknown URSA type !\n");
            UBOOT_TRACE("OK\n");
            return URSA_FAILED;

        default:
            UBOOT_ERROR("[URSA_COMMON] ursa_type is a illeagal value !\n");
            UBOOT_TRACE("OK\n");
            return URSA_FAILED;
    }
}

URSA_COMMON_TYPE MDrv_URSA_COM_Get_URSA_Type(void)
{
    UBOOT_TRACE("IN\n");

    int ursa_type_num = URSA_COMMON_UNKNOWN;
    st_sys_misc_setting misc_setting;
    char *ursa_type = getenv("ursa_type");

    if (NULL != ursa_type)
    {
        ursa_type_num = simple_strtoul(ursa_type, NULL, 10);
    }
    else
    {
        Read_MiscSetting_ToFlash(&misc_setting);
        ursa_type_num = misc_setting.m_u32ursa_type;
    }

    switch (ursa_type_num)
    {
        case URSA_COMMON_U6:
            UBOOT_DEBUG("[URSA_COMMON] ursa type is U6.\n");
            UBOOT_TRACE("OK\n");
            return URSA_COMMON_U6;
    
        case URSA_COMMON_U7:
            UBOOT_DEBUG("[URSA_COMMON] ursa type is U7.\n");
            UBOOT_TRACE("OK\n");
            return URSA_COMMON_U7;
    
        case URSA_COMMON_U8:        // it don't need handshake
            UBOOT_DEBUG("[URSA_COMMON] ursa type is U8.\n");
            UBOOT_TRACE("OK\n");
            return URSA_COMMON_U8;
    
        case URSA_COMMON_U9:
            UBOOT_DEBUG("[URSA_COMMON] ursa type is U9.\n");
            UBOOT_TRACE("OK\n");
            return URSA_COMMON_U9;
    
        case URSA_COMMON_KS2:
            UBOOT_DEBUG("[URSA_COMMON] ursa type is KS2.\n");
            UBOOT_TRACE("OK\n");
            return URSA_COMMON_KS2;
            
        case URSA_COMMON_FRC:
            UBOOT_DEBUG("[URSA_COMMON] ursa type is FRC.\n");
            UBOOT_TRACE("OK\n");
            return URSA_COMMON_FRC;

        case URSA_COMMON_U11:
            UBOOT_DEBUG("[URSA_COMMON] ursa type is FRC.\n");
            UBOOT_TRACE("OK\n");
            return URSA_COMMON_U11;

        case URSA_COMMON_UNKNOWN:
            UBOOT_ERROR("[URSA_COMMON] ursa_type is an illeagal value !\n");
            UBOOT_TRACE("OK\n");
            return URSA_COMMON_UNKNOWN;
    
        default:
            UBOOT_ERROR("[URSA_COMMON] ursa_type is a illeagal value !\n");
            return URSA_FAILED;
    }

}
#endif //__DRV_URSA_COMMON_C__
