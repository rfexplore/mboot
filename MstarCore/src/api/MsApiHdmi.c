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
#include <command.h>
#include <common.h>
#include <MsDebug.h>
#if(CONFIG_HDMITX_MSTAR_ROCKET ==1)
#include <apiHDMITx.h>
#elif(CONFIG_HDMITX_MSTAR_ROCKET2 ==1)
#include <hdmitx/mstar/rocket2/MsDrvRocket.h>
#endif

#if (ENABLE_HDMITX_MSTAR==1)
extern int msHdmitx_Disp_Init(void);
#elif (CONFIG_HDMITX_MSTAR_ROCKET==1)
extern MS_BOOL msHdmitx_Rocket_Init(HDMITX_VIDEO_TIMING outPutType );
#elif (CONFIG_TV_HDMITX_MSTAR==1)
extern int MsDrvTvHdmiTxInit(void);
#endif

int do_hdmi(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int ret = 0;    
    UBOOT_TRACE("IN\n");
    if (argc < 2)
    {
        cmd_usage(cmdtp);
    }   
    else
    {
       if(strncmp(argv[1], "init", 5) == 0)
       {
        #if(CONFIG_HDMITX_MSTAR_ROCKET ==1 ||CONFIG_HDMITX_MSTAR_ROCKET2 ==1)
       
            char * p_str = NULL;
        #if (CONFIG_HDMITX_MSTAR_ROCKET==1)
            HDMITX_VIDEO_TIMING u8ResolutionEnv = HDMITX_RES_1920x1080p_60Hz;
        #else
            EN_MAPI_DEVICE_ROCKY_VIDEO_TIMING u8ResolutionEnv = E_MAPI_ROCKY_RES_1920x1080p_60Hz;
        #endif
        
            p_str = getenv ("resolution");
            if(NULL != p_str)
            {
                u8ResolutionEnv = (int)simple_strtol(p_str, NULL, 10);
            }
            UBOOT_DEBUG("ROCKET resolution Type=%d \n",u8ResolutionEnv);
            ret =msHdmitx_Rocket_Init(u8ResolutionEnv);
            if(ret == TRUE)
                ret = 0;
            else
                ret = -1;
        #elif (CONFIG_TV_HDMITX_MSTAR==1)
            ret = MsDrvTvHdmiTxInit();
        #else
        
            ret = msHdmitx_Disp_Init();
        #endif
       }

       else
       {
           cmd_usage(cmdtp);
       }     
    }

    UBOOT_TRACE("OK\n");
    return ret; 
}

