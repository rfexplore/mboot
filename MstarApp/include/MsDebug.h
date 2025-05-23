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

#ifndef _MSTAR_DEBUG_H_
#define _MSTAR_DEBUG_H_

typedef enum
{
    EN_DEBUG_LEVEL_DISABLE=0,    
    EN_DEBUG_LEVEL_ERROR=0x01,
    EN_DEBUG_LEVEL_INFO=0x02,
    EN_DEBUG_LEVEL_TRACE=0x04,
    EN_DEBUG_LEVEL_DEBUG=0x08,
    EN_DEBUG_LEVEL_BOOTTIME=0x10,
    EN_DEBUG_LEVEL_INVALID=0x1000,    
    EN_DEBUG_LEVEL_MAX=EN_DEBUG_LEVEL_INVALID      
}EN_DEBUG_LEVEL;

typedef enum
{
    EN_DEBUG_MODULE_DISABLE=0,
    EN_DEBUG_MODULE_UPGRADE=(0x01<<0),    
    EN_DEBUG_MODULE_FAT=(0x01<<1),
    EN_DEBUG_MODULE_ALL=0xFFFFFFFF
}EN_DEBUG_MODULE;

extern EN_DEBUG_LEVEL dbgLevel;
extern EN_DEBUG_MODULE dbgModule;

#define DEFAULT_DEBUG_LEVEL (EN_DEBUG_LEVEL_INFO+EN_DEBUG_LEVEL_ERROR)
#define DEFAULT_DEBUG_MODULE (EN_DEBUG_MODULE_ALL)

#if CONFIG_MINIUBOOT
#define UBOOT_ERRDUMP(msg...) 
#define UBOOT_ERROR(msg...)  \
    do{\
        printf("\033[0;31m[ERROR] %s:%d: \033[0m",__FUNCTION__,__LINE__);\
        printf(msg);\
    }while(0)
#define UBOOT_INFO(msg...)  printf(msg)
#define UBOOT_TRACE(msg...) 
#define UBOOT_DEBUG(msg...) 
#define UBOOT_DUMP(addr, size) 
#define UBOOT_BOOTTIME(msg...)
#else
#define UBOOT_ERROR(msg...)\
    do{\
        if(dbgLevel&EN_DEBUG_LEVEL_ERROR) \
        { \
            printf("\033[0;31m[ERROR] %s:%d: \033[0m",__FUNCTION__,__LINE__);\
            printf(msg);\
        } \
    }while(0)
    
#define UBOOT_INFO(msg...)\
    do{\
        if(dbgLevel&EN_DEBUG_LEVEL_INFO) \
        { \
                printf(msg);\
        } \
    }while(0)

#define UBOOT_BOOTTIME(msg...)\
    do{\
        if(dbgLevel&EN_DEBUG_LEVEL_BOOTTIME) \
        { \
                printf(msg);\
        } \
    }while(0)



            
#if 1
#define UBOOT_TRACE(msg...)\
    do{\
        if(dbgLevel&EN_DEBUG_LEVEL_TRACE) \
        { \
                printf("\033[0;32m[TRACE] %s \033[0m",__FUNCTION__);\
                printf(msg);\
        } \
    }while(0)
    
#else
    do{\
        if(dbgLevel&EN_DEBUG_LEVEL_TRACE) \
        { \
            if(dbgModule&module) \
            {               \
                printf("\033[0;32m[TRACE]%s\033[0m",__FUNCTION__);\
                printf(msg);\
            }\
        } \
    }while(0)
    
#define UBOOT_TRACE(msg...)   _UBOOT_TRACE(EN_DEBUG_MODULE_ALL,msg)  

#endif

#if 1
#define UBOOT_DEBUG(msg...)\
    do{\
        if(dbgLevel&EN_DEBUG_LEVEL_DEBUG) \
        { \
                printf("\033[0;34m[DEBUG] %s:%d: \033[0m",__FUNCTION__,__LINE__);\
                printf(msg);\
        } \
    }while(0)

#else
#define _UBOOT_DEBUG(module, msg...)\
    do{\
        if(dbgLevel&EN_DEBUG_LEVEL_DEBUG) \
        { \
            if(dbgModule&module) \
            {               \
                printf("\033[0;34m[DEBUG]%s:%d:\033[0m",__FUNCTION__,__LINE__);\
                printf(msg);\
            } \
        } \
    }while(0)

#define UBOOT_DEBUG(msg...)   _UBOOT_DEBUG(EN_DEBUG_MODULE_ALL,msg)      
#endif

#define UBOOT_DUMP(addr, size)\
    do{\
        if(dbgLevel&EN_DEBUG_LEVEL_DEBUG) \
        { \
                printf("\033[0;34m[DUMP] %s:%d:\033[0m\n",__FUNCTION__,__LINE__);\
                _dump(addr,size);\
        } \
    }while(0)
            
#define UBOOT_ERRDUMP(addr, size)\
            do{\
                if(dbgLevel&EN_DEBUG_LEVEL_ERROR) \
                { \
                        printf("\033[0;31m[DUMP] %s:%d:\033[0m\n",__FUNCTION__,__LINE__);\
                        _dump(addr,size);\
                } \
            }while(0)


int do_set_debugging_message_level(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_initDebugLevel(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);        
void _dump(unsigned int addr, unsigned int size);

#endif
#endif
